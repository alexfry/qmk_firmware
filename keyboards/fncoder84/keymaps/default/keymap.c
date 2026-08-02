// Copyright 2019 Alex Fry
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#ifdef RAW_ENABLE
#    include "raw_hid.h"
#endif
#ifdef VIA_ENABLE
#    include "via.h"
#endif
#include "qmk_midi.h"

/*
 * Host → keyboard LED status protocol (desktop app feedback for encoder-driven software).
 *
 * This is NOT aesthetic config — it mirrors app state onto the encoder LEDs.
 *
 * Packet formats (32-byte Raw HID):
 *
 *   Prefixed (works with BOTH default and VIA builds — preferred):
 *     data[0] = 0xFE              // FNCODER_HOST_CMD, avoids VIA command IDs
 *     data[1] = command           // same as legacy command below
 *     data[2...] = payload
 *
 *   Legacy unprefixed (default / non-VIA builds only — IDs collide with VIA):
 *     data[0] = command
 *     data[1...] = payload
 *
 * Commands:
 *   1 / sub 2: rgblight_step_noeeprom
 *   1 / sub 3: solid white
 *   1 / sub 4: disable
 *   1 / sub 5: enable
 *   4 / sub 1: set LED data[2] to RGB data[3..5]
 *   4 / sub 2: set LED range group data[2] to RGB data[3..5]
 *              (groups 1-9; only indices < RGBLIGHT_LED_COUNT actually light)
 */
#define FNCODER_HOST_CMD 0xFE

/*
 * Warm white for encoder status LEDs — original post-boot settle (506c716).
 * Hue 27 / sat 110 / val 100: warm amber-white at moderate brightness.
 * (Earlier wrong settle of sat 67 / val 255 read as bright cool white.)
 */
#define FNCODER_LED_HUE 27
#define FNCODER_LED_SAT 110
#define FNCODER_LED_VAL 100

/* Startup chase flash (original 506c716 animation): warm-orange pulse per LED */
#define FNCODER_BOOT_FLASH_HUE 17
#define FNCODER_BOOT_FLASH_SAT 180
#define FNCODER_BOOT_FLASH_VAL 255
#define FNCODER_BOOT_FLASH_MS  50

enum layer_names {
    _BASE,
    _FN1,
    _FN2,
    _FN3,
    _FN4
};

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

    /* Start dark, then chase one LED at a time (original boot anim). */
    rgblight_sethsv_noeeprom(FNCODER_LED_HUE, FNCODER_LED_SAT, 0);
    for (uint8_t i = 0; i < RGBLIGHT_LED_COUNT; i++) {
        rgblight_sethsv_at(FNCODER_BOOT_FLASH_HUE, FNCODER_BOOT_FLASH_SAT, FNCODER_BOOT_FLASH_VAL, i);
        wait_ms(FNCODER_BOOT_FLASH_MS);
        rgblight_sethsv_at(FNCODER_LED_HUE, FNCODER_LED_SAT, 0, i);
    }
    /* Settle on original warm white (not full-bright cool white). */
    rgblight_sethsv_noeeprom(FNCODER_LED_HUE, FNCODER_LED_SAT, FNCODER_LED_VAL);
}

enum custom_keycodes {
    MI_CH1_Click = SAFE_RANGE,
    MI_CH2_Click,
    MI_CH3_Click,
    MI_CH4_Click,
    MI_CH5_Click,
    MI_CH6_Click,
    MI_CH7_Click,
    MI_CH8_Click,
    MI_CH9_Click,
    MI_CH10_Click,
    MI_CH11_Click,
    MI_CH12_Click
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,  MI_CH1_Click, MI_CH2_Click, MI_CH3_Click, MI_CH4_Click, MI_CH5_Click, MI_CH6_Click, MI_CH7_Click, MI_CH8_Click, MI_CH9_Click, MI_CH10_Click, MI_CH11_Click, MI_CH12_Click, MO(1),   MO(2),
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
        MO(1),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_PGUP, KC_PGDN,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LALT, KC_LGUI, KC_SPC,  KC_RGUI, MO(3),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_FN1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MUTE, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_FN2] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MUTE, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_FN3] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TG(1),   TG(2),
        _______, KC_F13,  KC_F24,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MO(4),
        _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, UG_HUEU, UG_HUED, UG_SATU, UG_SATD, UG_VALU, UG_VALD, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, BL_DOWN, BL_UP,   BL_TOGG, BL_STEP, _______, _______, _______, _______, _______, KC_PGUP, _______,
        _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_END
    ),

    [_FN4] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

static void midi_click(uint8_t cc, bool pressed) {
    midi_send_cc(&midi_device, 1, cc, pressed ? 127 : 0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MI_CH1_Click:
            midi_click(1, record->event.pressed);
            return false;
        case MI_CH2_Click:
            midi_click(2, record->event.pressed);
            return false;
        case MI_CH3_Click:
            midi_click(3, record->event.pressed);
            return false;
        case MI_CH4_Click:
            midi_click(4, record->event.pressed);
            return false;
        case MI_CH5_Click:
            midi_click(5, record->event.pressed);
            return false;
        case MI_CH6_Click:
            midi_click(6, record->event.pressed);
            return false;
        case MI_CH7_Click:
            midi_click(7, record->event.pressed);
            return false;
        case MI_CH8_Click:
            midi_click(8, record->event.pressed);
            return false;
        case MI_CH9_Click:
            midi_click(9, record->event.pressed);
            return false;
        case MI_CH10_Click:
            midi_click(10, record->event.pressed);
            return false;
        case MI_CH11_Click:
            midi_click(11, record->event.pressed);
            return false;
        case MI_CH12_Click:
            midi_click(12, record->event.pressed);
            return false;
    }
    return true;
}

const uint16_t encoderKeysL1[24] = {
    MS_WHLD, MS_WHLU,
    KC_LEFT, KC_RGHT,
    KC_UP,   KC_DOWN,
    KC_G,    KC_H,
    KC_I,    KC_J,
    KC_K,    KC_L,
    KC_M,    KC_N,
    KC_O,    KC_P,
    KC_Q,    KC_R,
    BL_DOWN, BL_UP,
    BL_DOWN, BL_UP,
    KC_VOLD, KC_VOLU
};

static int midiVals[12] = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
static int encoderhues[12] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

static void tap_code_catcher(uint16_t input) {
    if (input == BL_UP) {
        backlight_increase();
    } else if (input == BL_DOWN) {
        backlight_decrease();
    } else {
        tap_code(input);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index >= 12) {
        /* false = we handled it; do not fall through to QMK's default volume keys */
        return false;
    }

    switch (get_highest_layer(layer_state)) {
        case _FN3:
            if (clockwise) {
                encoderhues[index] = (encoderhues[index] + 4) & 0xFF;
            } else {
                encoderhues[index] = (encoderhues[index] - 4) & 0xFF;
            }
            rgblight_sethsv_at(encoderhues[index], 255, 255, index);
            break;
        case _FN2:
            if (clockwise) {
                tap_code_catcher(encoderKeysL1[(index * 2) + 1]);
            } else {
                tap_code_catcher(encoderKeysL1[(index * 2)]);
            }
            break;
        case _FN1:
            if (clockwise) {
                if (midiVals[index] < 127) {
                    midiVals[index]++;
                }
            } else {
                if (midiVals[index] > 0) {
                    midiVals[index]--;
                }
            }
            midi_send_cc(&midi_device, 1, index + 1, midiVals[index]);
            break;
        default:
            /* Relative MIDI CC on the base layer */
            midi_send_cc(&midi_device, 1, index + 1, clockwise ? 63 : 65);
            break;
    }
    /* Must return false so encoder_update_kb does not also send KC_VOLU/KC_VOLD. */
    return false;
}

#ifdef RAW_ENABLE
static void set_led_range(int start, int stop, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = start; i <= stop; i++) {
        rgblight_setrgb_at(r, g, b, i);
    }
}

/* command + payload: command at [0], same layout as the legacy unprefixed packets. */
static void fncoder_handle_led_command(const uint8_t *cmd, uint8_t length) {
    if (length < 2) {
        return;
    }

    switch (cmd[0]) {
        case 1:
            switch (cmd[1]) {
                case 2:
                    rgblight_step_noeeprom();
                    break;
                case 3:
                    rgblight_setrgb(RGB_WHITE);
                    break;
                case 4:
                    rgblight_disable_noeeprom();
                    break;
                case 5:
                    rgblight_enable_noeeprom();
                    break;
                default:
                    break;
            }
            break;
        case 4:
            if (length < 6) {
                return;
            }
            {
                uint8_t led_index = cmd[2];
                uint8_t r         = cmd[3];
                uint8_t g         = cmd[4];
                uint8_t b         = cmd[5];
                switch (cmd[1]) {
                    case 1:
                        /* Per-encoder / per-LED status (indices 0 .. RGBLIGHT_LED_COUNT-1). */
                        rgblight_setrgb_at(r, g, b, led_index);
                        break;
                    case 2:
                        switch (led_index) {
                            case 1:
                                set_led_range(0, 14, r, g, b);
                                break;
                            case 2:
                                set_led_range(15, 29, r, g, b);
                                break;
                            case 3:
                                set_led_range(30, 43, r, g, b);
                                break;
                            case 4:
                                set_led_range(44, 57, r, g, b);
                                break;
                            case 5:
                                set_led_range(58, 66, r, g, b);
                                break;
                            case 6:
                                set_led_range(67, 81, r, g, b);
                                break;
                            case 7:
                                set_led_range(82, 86, r, g, b);
                                break;
                            case 8:
                                set_led_range(87, 99, r, g, b);
                                break;
                            case 9:
                                set_led_range(100, 104, r, g, b);
                                break;
                        }
                        break;
                }
            }
            break;
        default:
            break;
    }
}

#    ifdef VIA_ENABLE
/*
 * VIA owns raw_hid_receive. Claim only our host-status packets (0xFE)
 * and leave all real VIA command IDs alone.
 */
bool via_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] != FNCODER_HOST_CMD) {
        return false;
    }
    if (length > 1) {
        fncoder_handle_led_command(&data[1], length - 1);
    }
    /* No reply required; returning true means we fully handled the packet. */
    return true;
}
#    else
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (data[0] == FNCODER_HOST_CMD) {
        if (length > 1) {
            fncoder_handle_led_command(&data[1], length - 1);
        }
        return;
    }
    /* Legacy desktop app: unprefixed commands (not safe under VIA). */
    fncoder_handle_led_command(data, length);
}
#    endif
#endif
