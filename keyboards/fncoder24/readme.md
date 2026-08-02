# FnCoder24

Custom **number pad** with **4 encoders**, RGB status LEDs, backlight, MIDI, and Raw HID host I/O (same protocol family as FnCoder84).

* Keyboard Maintainer: [Alex Fry](https://github.com/alexfry/)
* Hardware: FnCoder24 (ATmega32U4, Atmel DFU)
* More: https://alexfry.com/fncoder

## Build / flash

```bash
export PATH="$HOME/Library/Python/3.9/bin:/opt/homebrew/opt/avr-gcc@14/bin:/opt/homebrew/opt/avr-binutils/bin:/opt/homebrew/bin:$PATH"

qmk compile -kb fncoder24 -km via
# or: qmk compile -kb fncoder24 -km default
```

**MCU:** `atmega32u4` · DFU ID typically `03EB:2FF4`  
**USB:** VID `0xAF84` · PID `0x0024`

LTO is enabled so VIA builds fit in flash (~90% used).

```bash
dfu-programmer atmega32u4 erase --force
dfu-programmer atmega32u4 flash .build/fncoder24_via.hex
dfu-programmer atmega32u4 reset
```

### Enter DFU (bootloader)

| Method | Action |
|--------|--------|
| Key combo | Hold **MO(1)** (second row, far left), tap **`.`** (bottom row, right) |
| Bootmagic | Hold **top-left** key while plugging USB |

## Encoders

| Action | Behaviour |
|--------|-----------|
| **Push** (base) | MIDI CC click **0–3** (127/0) + Raw HID **BUTTON** (same idea as FnCoder84) |
| **Turn** (base) | Relative MIDI CC **0–3** (CW=**65** / CCW=**63**) + Raw HID **TURN** |
| **Turn** + **MO(1)** | That encoder’s status LED **hue** + TURN |
| **Push** + **MO(1)** | Lighting: `BL_TOGG` / `BL_STEP` / `UG_TOGG` / `UG_NEXT` |
| **VIA** | Knobs **`e0`–`e3`**, remappable CCW/CW per layer |

MIDI turn polarity is **historical for the 24** (opposite of the 84’s 63/65 mapping). HID TURN is still **+1 CW / −1 CCW** on both boards.

## Boot LEDs

Same as FnCoder84 (only 4 LEDs instead of 12):

1. Orange chase per LED (HSV 17/180/255, 50 ms)
2. Settle warm white **HSV 27 / 110 / 100**

## Raw HID (PCoIP-friendly)

Namespaces: `0xFE` LEDs · `0xFC` STREAM/PING · `0xFD` TURN/BUTTON/PONG.  
Stream defaults **off** (so VIA is not desynced); apps send `FC 01 01` first.

See [HOST_RAW_HID_PROTOCOL.md](HOST_RAW_HID_PROTOCOL.md) and the shared [84 protocol doc](../fncoder84/HOST_RAW_HID_PROTOCOL.md).

```bash
python3 -m pip uninstall -y hid hidapi && python3 -m pip install --user hidapi
python3 ../fncoder84/tools/raw_hid_encoder_monitor.py --pid 0x0024
```

## VIA

1. Settings → show **Design** tab; **V2 definitions OFF**
2. Load draft: `keyboards/fncoder24/via.json`  
   (Not in the online VIA repo — “Fetching v3 definition failed” is expected until sideloaded.)
3. Layout: numpad with vertical **2u** `+` and **Enter**, horizontal **2u** `0`, four encoder knobs on top

### VIA EEPROM reseeding

Firmware bumps an internal seed version when the flash keymap / encoder map changes and rewrites VIA EEPROM (`dynamic_keymap_reset`) once on boot. If layer 0 still shows old lighting after a feature change, reflash the latest build (seed runs automatically) or use VIA “reset keymap”.

## Documentation

| Doc | Topic |
|-----|--------|
| [HOST_RAW_HID_PROTOCOL.md](HOST_RAW_HID_PROTOCOL.md) | 24-specific HID / MIDI notes |
| [../fncoder84/HOST_RAW_HID_PROTOCOL.md](../fncoder84/HOST_RAW_HID_PROTOCOL.md) | Full bidirectional protocol |
| [../fncoder84/docs/UPDATE-AND-VIA.md](../fncoder84/docs/UPDATE-AND-VIA.md) | Line-wide QMK/VIA update notes |
