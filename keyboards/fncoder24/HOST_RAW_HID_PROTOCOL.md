# FnCoder24 Raw HID protocol (bidirectional)

Same wire format as **FnCoder84**. Differences for this board only:

| | FnCoder24 |
|--|-----------|
| USB | VID **`0xAF84`** / PID **`0x0024`** |
| Encoders / LEDs | **4** (indices `0`–`3`) |
| MCU | ATmega32U4 |
| MIDI (dual-emit) | Channel **1**, CC **`index` 0–3**, CW=**65** / CCW=**63** (historical polarity) |

Full framing, commands, and host notes:

→ **[../fncoder84/HOST_RAW_HID_PROTOCOL.md](../fncoder84/HOST_RAW_HID_PROTOCOL.md)**

## Quick reference

| `data[0]` | Direction | Role |
|-----------|-----------|------|
| `0xFE` | Host → KB | LED status (`04 01 <i> R G B` for LED `i` 0–3) |
| `0xFC` | Host → KB | STREAM / PING |
| `0xFD` | KB → Host | TURN (`±1` int8), BUTTON, PONG |

## Behaviour on this board

| Input | MIDI | Raw HID `0xFD` |
|-------|------|----------------|
| **Push** (base) | CC 0–3 click 127/0 | BUTTON 1/0 |
| Turn (base) | relative CC 0–3 | TURN |
| Turn while **MO(1)** | — | TURN + that LED’s hue |
| Push while **MO(1)** | lighting keycodes | (no BUTTON — different keycodes) |

Stream defaults **off** (avoids desyncing VIA on the shared Raw HID pipe). Host apps must send **`FC 01 01`** before events. The monitor script does this automatically. VIA build uses `encoder_map` (`e0`–`e3` in `via.json`).

### VIA EEPROM note

Custom keycodes for turns/hue/clicks live in a single enum. Adding or reordering them **invalidates** keycodes already stored in VIA EEPROM. Firmware reseeds **matrix + encoder map** from flash when `FNCODER_ENCODER_MAP_VER` increases (see `keymaps/default/keymap.c`). Bump that version after any custom-keycode or default-layer layout change.

## Test

```bash
python3 -m pip uninstall -y hid hidapi && python3 -m pip install --user hidapi
python3 ../fncoder84/tools/raw_hid_encoder_monitor.py --vid 0xAF84 --pid 0x0024
```
