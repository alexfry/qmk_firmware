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

extern MidiDevice midi_device;

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

	KEYMAP(
		BL_TOGG, BL_STEP, RGB_TOG, RGB_MODE_FORWARD, 
		MO(1), KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, 
		KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS, 
		KC_KP_4, KC_KP_5, KC_KP_6, 
		KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER, 
		KC_KP_0, KC_KP_DOT),

	KEYMAP(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, RESET),

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
    KC_KP_1,MI_Cs_3,\
    MI_D_4,MI_D_3,\
    MI_Ds_4,MI_Ds_3,\
    MI_E_4,MI_E_3\
    };

//void encoder_update_user(uint8_t index, bool clockwise) {
//  tap_code(encoderKeys[(index * 2) + clockwise]);
//}


void encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { /* First encoder */
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 0, 65);
    } else {
      midi_send_cc(&midi_device, 1, 0, 63);
    }
  } else if (index == 1) { /* Second encoder */  
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 1, 65);
    } else {
      midi_send_cc(&midi_device, 1, 1, 63);
    }
      } else if (index == 2) { /* Second encoder */  
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 2, 65);
    } else {
      midi_send_cc(&midi_device, 1, 2, 63);
    }
    } else if (index == 3) { /* Second encoder */  
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 3, 65);
    } else {
      midi_send_cc(&midi_device, 1, 3, 63);
    }
  }
  wait_ms(40);
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
