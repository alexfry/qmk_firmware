# FnCoder24

Custom keypad with **4 encoders**, RGB underglow, backlight, and MIDI.

* Keyboard Maintainer: [Alex Fry](https://github.com/alexfry/)
* Hardware: FnCoder24 (ATmega32U4, Atmel DFU)
* More: https://alexfry.com/fncoder

## Build / flash

```bash
qmk compile -kb fncoder24 -km default
qmk flash   -kb fncoder24 -km default

qmk compile -kb fncoder24 -km via
qmk flash   -kb fncoder24 -km via
```

**MCU:** `atmega32u4` · DFU USB ID typically `03EB:2FF4`  
**USB:** VID `0xAF84` · PID `0x0024`

LTO is enabled in `keyboard.json` so VIA builds fit in flash.

## Related

Full update / VIA notes for the FnCoder line (focused on 84, covers 24 briefly):

→ [../fncoder84/docs/UPDATE-AND-VIA.md](../fncoder84/docs/UPDATE-AND-VIA.md)
