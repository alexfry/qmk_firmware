# MCU name

# MCU = STM32F303
MCU = at90usb1286

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = atmel-dfu


KEYBOARD_SHARED_EP = yes

## Features
# CONSOLE_ENABLE = yes
ENCODER_ENABLE = yes
# JOYSTICK_ENABLE = yes
MIDI_ENABLE = yes
BACKLIGHT_ENABLE = yes
RAW_ENABLE = yes


# Build Options
#

#BOOTMAGIC_ENABLE = yes # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no  # Mouse keys
EXTRAKEY_ENABLE = no  # Audio control and System control
CONSOLE_ENABLE = no    # Console for debug
COMMAND_ENABLE = no    # Commands for debug and configuration
SERIAL_ENABLE = no
SLEEP_LED_ENABLE = no  # Breathing sleep LED during USB suspend
NKRO_ENABLE = no      # USB Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
AUDIO_ENABLE = no
RGBLIGHT_ENABLE = yes

# OLED_DRIVER_ENABLE = yes
