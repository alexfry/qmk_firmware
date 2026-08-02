# FnCoder84 Raw HID protocol (bidirectional)

Transport: **USB Raw HID**, **32-byte** reports (QMK default).  
USB IDs: **VID `0xAF84` / PID `0x0084`**.

Used for:

1. **Host → keyboard** — status LED colours (app state feedback)
2. **Keyboard → host** — relative encoder turns + push buttons (for apps over PCoIP when MIDI forwarding fails)
3. **Host → keyboard** — stream enable / path ping

VIA also uses Raw HID. FnCoder namespaces stay **outside** VIA command IDs `0x01`–`0x15`.

---

## Byte 0 namespaces

| `data[0]` | Direction | Role |
|-----------|-----------|------|
| **`0xFE`** | Host → KB | LED / RGB status commands |
| **`0xFC`** | Host → KB | Stream control (STREAM, PING) |
| **`0xFD`** | KB → Host | Encoder events (TURN, BUTTON, PONG) |
| `0x01`–`0x15` | VIA | Leave alone |

Pad unused bytes with `0`. Always send/receive full **32** bytes.

---

## Index conventions

| Concept | Range | Notes |
|---------|-------|--------|
| Encoder index | `0`–`11` | Physical left→right |
| Status LED index | `0`–`11` | Same as encoder index |
| Legacy MIDI CC | `1`–`12` | `cc = index + 1` |

Turns are **relative only** (no absolute position on the wire). Host maintains continuous values.

---

## Host → keyboard: LEDs (`0xFE`)

```
data[0]    = 0xFE
data[1]    = command
data[2...] = payload
```

### Command `1` — global RGB helper

| `data[2]` | Action |
|-----------|--------|
| `2` | `rgblight_step_noeeprom()` |
| `3` | solid white |
| `4` | disable RGB |
| `5` | enable RGB |

### Command `4` — set LED(s)

| `data[2]` | `data[3]` | `data[4..6]` | Action |
|-----------|-----------|--------------|--------|
| `1` | LED index | R, G, B | One LED |
| `2` | range 1–9 | R, G, B | Legacy range group |

Example — encoder 3 LED red:

```
FE 04 01 03 FF 00 00 00 ...
```

No reply is sent for LED commands.

Full historical detail also in [HOST_LED_PROTOCOL.md](HOST_LED_PROTOCOL.md) (subset of this doc).

---

## Host → keyboard: control (`0xFC`)

```
data[0] = 0xFC
data[1] = command
data[2...] = payload
```

| `command` | Name | Payload | Behaviour |
|-----------|------|---------|-----------|
| `0x01` | STREAM | `data[2]` = `0` off / `1` on | Enable/disable keyboard→host TURN/BUTTON reports |
| `0x02` | PING | `data[2]` = cookie (any byte) | Keyboard replies with PONG (see below) |

**Defaults:** stream is **on** after boot (easy PCoIP bring-up). Use STREAM off if you want silence when no app is listening.

### PING example

```
FC 02 A5 00 00 ...
```

---

## Keyboard → host: events (`0xFD`)

Unsolicited **IN** reports via `raw_hid_send`. Host must **read** the Raw HID input stream continuously.

```
data[0] = 0xFD
data[1] = msg_type
data[2] = encoder_index   // 0..11 (0 for PONG)
data[3] = payload
data[4] = seq             // uint8, increments every event (detect drops)
data[5] = flags           // reserved, 0
data[6..31] = 0
```

| `msg_type` | Name | `payload` |
|------------|------|-----------|
| `0x01` | **TURN** | **signed int8** steps: `+1` CW, `-1` CCW (may be `±N` if coalesced later) |
| `0x02` | **BUTTON** | `1` = pressed, `0` = released |
| `0x7F` | **PONG** | cookie from PING (`data[2]` of PING) |

### Examples

```
# Encoder 3 CW one detent, seq 0x2A
FD 01 03 01 2A 00 ...

# Encoder 3 CCW (payload 0xFF = -1 as int8)
FD 01 03 FF 2B 00 ...

# Encoder 0 pressed
FD 02 00 01 2C 00 ...

# Encoder 0 released
FD 02 00 00 2D 00 ...

# PONG for cookie 0xA5
FD 7F 00 A5 2E 00 ...
```

### Host continuous model

```text
on TURN:   value[index] += signed(payload)
on BUTTON: button[index] = payload   # or edge-trigger on 1 / 0
```

`seq` should increase by 1 (mod 256) each event; gaps mean dropped reports over the tunnel.

---

## Dual-emit with MIDI

Firmware still sends **MIDI** (relative CC / click) for local use. Raw HID events are **additional** (dual-emit) when stream is on.

| Input | MIDI (existing) | Raw HID `0xFD` |
|-------|-----------------|----------------|
| Turn (base / FN1) | relative CC 63/65 | TURN ±1 |
| Push | CC click 127/0 | BUTTON 1/0 |
| Turn on FN3 (hue) | — | TURN ±1 + LED hue change |

On **`via`** builds, base/FN1 turns go through encoder map custom keycodes (HID + MIDI). On **`default`**, `encoder_update_user` dual-emits.

---

## App connect sequence (PCoIP)

1. Open Raw HID interface for `0xAF84:0x0084` (may need usage-page filter for the Raw HID collection).
2. Send PING (`FC 02 <cookie>`); wait for `FD 7F` with same cookie — proves HID path through the tunnel.
3. Ensure STREAM on (`FC 01 01`) if you previously disabled it.
4. Read loop: handle `0xFD` TURN/BUTTON; send `0xFE` LED updates as needed.
5. Prefer **your app alone** on the remote session Raw HID (VIA may contend for the same interface).

Test script: [`tools/raw_hid_encoder_monitor.py`](tools/raw_hid_encoder_monitor.py).

### Host tooling install (macOS)

There are two PyPI packages that both try to provide `import hid`:

| Package | Notes |
|---------|--------|
| **`hidapi`** (Cython) | **Use this.** Works out of the box on arm64 macOS. |
| **`hid`** (ctypes) | Often pulled in by QMK’s Python deps; needs system `libhidapi` and easily shadows the working package. |

```bash
brew install hidapi   # optional native library
python3 -m pip uninstall -y hid hidapi
python3 -m pip install --user hidapi
python3 keyboards/fncoder84/tools/raw_hid_encoder_monitor.py
```

Open the interface with **usage_page `0xFF60`** (QMK Raw HID). The monitor script prefers that automatically.

Verified: PING→PONG and TURN/BUTTON events on local USB after flashing `fncoder84:via` with this protocol.

---

## Firmware builds

| Keymap | `0xFE` LEDs | `0xFC` / `0xFD` | VIA |
|--------|-------------|-----------------|-----|
| `fncoder84:via` | yes | yes | yes |
| `fncoder84:default` | yes (+ legacy unprefixed LEDs) | yes | no |

---

## Reserved / future

| Item | Notes |
|------|--------|
| Multi-event batch (`msg_type 0x10`) | If single-event rate saturates under PCoIP |
| `SET_MODE` bitmask | Mute MIDI, HID-only, etc. |
| Absolute positions | Not planned; host owns continuous state |
