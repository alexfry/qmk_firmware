# FnCoder84

Custom **84% / 75%-style** keyboard with **12 encoders**, RGB underglow, backlight, MIDI, and Raw HID host LED control.

![FnCoder84](docs/board-photo.png)

* Keyboard Maintainer: [Alex Fry](https://github.com/alexfry/)
* Hardware: FnCoder84 (AT90USB1286, Atmel DFU)
* More: https://alexfry.com/fncoder

## Quick build / flash

```bash
# default keymap (no VIA)
qmk compile -kb fncoder84 -km default
qmk flash   -kb fncoder84 -km default

# VIA keymap
qmk compile -kb fncoder84 -km via
qmk flash   -kb fncoder84 -km via
```

**Bootloader:** hold **MO(3)** (bottom row, right of RGUI) and tap **R**, or use the hardware reset.

**MCU for Toolbox / dfu-programmer:** `at90usb1286`  
**Note:** QMK Toolbox on macOS may reject MCU index 0 (`AT90USB1286`); use CLI if needed:

```bash
dfu-programmer at90usb1286 erase --force
dfu-programmer at90usb1286 flash .build/fncoder84_via.hex
dfu-programmer at90usb1286 reset
```

## Documentation

| Doc | Topic |
|-----|--------|
| [docs/UPDATE-AND-VIA.md](docs/UPDATE-AND-VIA.md) | Full write-up of the QMK update, VIA setup, IDs, encoders, flashing |
| [HOST_LED_PROTOCOL.md](HOST_LED_PROTOCOL.md) | Desktop app Raw HID LED status protocol (`0xFE`) |

## USB IDs

| | Value |
|--|--------|
| VID | `0xAF84` |
| PID | `0x0084` |

(`0xFEED` is rejected by modern VIA as a placeholder VID.)
