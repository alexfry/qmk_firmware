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
    MI_CH0_Click,
    MI_CH1_Click,
    MI_CH2_Click,
    MI_CH3_Click
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
      MI_CH0_Click, MI_CH1_Click, MI_CH2_Click, MI_CH3_Click,\
      KC_NLCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,\
      KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS,\
      KC_KP_4, KC_KP_5, KC_KP_6,_______,\
      KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,\
      KC_KP_0, KC_KP_DOT,_______,_______\
    ),
};



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MI_CH0_Click:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                midi_send_cc(&midi_device, 1, 0, 127);
            } else {
                // when keycode QMKBEST is released
                midi_send_cc(&midi_device, 1, 0, 0);
            }
            break;
        case MI_CH1_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 1, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 1, 0);
            }
            break;
        case MI_CH2_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 2, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 2, 0);
            }
            break;
        case MI_CH3_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 3, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 3, 0);
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
      midi_send_cc(&midi_device, 1, 0, 1);
    } else {
      midi_send_cc(&midi_device, 1, 0, 65);
    }
  } else if (index == 1) { /* Second encoder */  
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 1, 1);
    } else {
      midi_send_cc(&midi_device, 1, 1, 65);
    }
      } else if (index == 2) { /* Second encoder */  
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 2, 1);
    } else {
      midi_send_cc(&midi_device, 1, 2, 65);
    }
    } else if (index == 3) { /* Second encoder */  
    if (clockwise) {
      midi_send_cc(&midi_device, 1, 3, 1);
    } else {
      midi_send_cc(&midi_device, 1, 3, 65);
    }
  }
  wait_ms(10);
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
