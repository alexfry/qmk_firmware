# FnCoder84 host LED status protocol

Used by a **desktop app** to show software state on the keyboard LEDs
(e.g. status of tools driven by the 12 encoders). This is functional
feedback, not VIA aesthetic lighting config.

Transport: **USB Raw HID**, 32-byte reports (QMK default).

USB IDs (current): **VID `0xAF84` / PID `0x0084`** (VIA rejects the old QMK placeholder `0xFEED`).

## Why the `0xFE` prefix?

VIA also uses Raw HID. Its command IDs include `0x01` and `0x04`, which
were the same bytes as this board’s older LED commands. If the firmware
intercepted those, VIA would break.

So status packets use a reserved first byte:

| Byte | Meaning |
|------|---------|
| `0xFE` | FnCoder host status command (safe with VIA) |
| `0x01`–`0x15` | Reserved for VIA — do not use for LEDs when VIA firmware is flashed |

## Packet format (preferred — works with `default` and `via` firmware)

```
data[0]    = 0xFE
data[1]    = command
data[2...] = payload
(remaining bytes ignored / zero-padded to 32)
```

### Command `1` — global RGB helper

| data[2] (sub) | Action |
|---------------|--------|
| `2` | `rgblight_step_noeeprom()` |
| `3` | solid white |
| `4` | disable RGB |
| `5` | enable RGB |

Example (enable):

```
FE 01 05 00 00 ...
```

### Command `4` — set LED(s)

| data[2] (sub) | data[3] | data[4..6] | Action |
|---------------|---------|------------|--------|
| `1` | LED index | R, G, B | Set one LED |
| `2` | range id 1–9 | R, G, B | Set a predefined index range |

Example (set encoder/LED index 3 to red):

```
FE 04 01 03 FF 00 00 00 ...
```

LED indices that are ≥ `RGBLIGHT_LED_COUNT` (currently **12**) are ignored
safely. Prefer indices `0`–`11` for the physical encoder LEDs.

## Legacy format (non-VIA / `default` firmware only)

Same as above but **without** the `0xFE` prefix (`command` in `data[0]`).
Do **not** use legacy packets against a VIA build.

## Firmware builds

| Keymap | Host LED protocol | VIA app |
|--------|-------------------|---------|
| `fncoder84:default` | `0xFE` **and** legacy | No |
| `fncoder84:via` | `0xFE` only | Yes |

## Desktop app notes

1. Open the keyboard’s **Raw HID** interface (same device as the keyboard HID; usage page/id as QMK Raw HID).
2. Prefer **`0xFE`-prefixed** packets always so one app works for both firmwares.
3. No response packet is sent for LED commands (fire-and-forget).
