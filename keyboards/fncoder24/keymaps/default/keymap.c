// Copyright 2019 Alex Fry
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#include "qmk_midi.h"

enum layer_names {
    _BASE,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        BL_TOGG, BL_STEP, UG_TOGG, UG_NEXT,
        MO(1),   KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_P4,   KC_P5,   KC_P6,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_P0,   KC_PDOT
    ),
    [_FN] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
        _______, QK_BOOT
    ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    /* Four encoders send relative MIDI CC on channels 0-3.
     * Return false so QMK does not also apply default volume keys. */
    if (index > 3) {
        return false;
    }
    if (clockwise) {
        midi_send_cc(&midi_device, 1, index, 65);
    } else {
        midi_send_cc(&midi_device, 1, index, 63);
    }
    wait_ms(40);
    return false;
}
