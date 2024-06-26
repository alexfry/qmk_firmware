/* Copyright 2019 Alex Fry
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H


// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
      KC_NLCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, KC_F1,\
      KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS, KC_F2,\
      KC_KP_4, KC_KP_5, KC_KP_6,_______, KC_F3,\
      KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,KC_F4,\
      KC_KP_0, KC_KP_DOT,_______,_______\
    ),
};



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
            } else {
                // when keycode QMKURL is released
            }
            break;
    }
    return true;
}

const int encoderKeys[8] = {
    KC_A,KC_B,\
    KC_C,KC_D,\
    KC_E,KC_F,\
    KC_G,KC_H\
    };

void encoder_update_user(uint8_t index, bool clockwise) {
  tap_code(encoderKeys[(index * 2) + clockwise]);
}


/*
void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool led_update_user(led_t led_state) {
    return true;
}
*/
