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
    KEYMAP(
		KC_ESC, MI_CH1_Click, MI_CH2_Click, MI_CH3_Click, MI_CH4_Click, MI_CH5_Click, MI_CH6_Click, MI_CH7_Click, MI_CH8_Click, MI_CH9_Click, MI_CH10_Click, MI_CH11_Click, MI_CH12_Click, MO(1), MO(2), 
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, 
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, 
		MO(1), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGDN, KC_PGUP, 
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END, 
		KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RGUI, MO(3), KC_RCTL, KC_LEFT, KC_DOWN, KC_RIGHT),


    KEYMAP(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC__MUTE, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    
    KEYMAP(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC__MUTE, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    
    KEYMAP(
		KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, TG(1), TG(2), 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RESET, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, BL_DEC, BL_INC, BL_TOGG, BL_STEP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END),

};



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
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
        case MI_CH4_Click:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                midi_send_cc(&midi_device, 1, 4, 127);
            } else {
                // when keycode QMKBEST is released
                midi_send_cc(&midi_device, 1, 4, 0);
            }
            break;
        case MI_CH5_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 5, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 5, 0);
            }
            break;
        case MI_CH6_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 6, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 6, 0);
            }
            break;
        case MI_CH7_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 7, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 7, 0);
            }
            break;
        case MI_CH8_Click:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                midi_send_cc(&midi_device, 1, 8, 127);
            } else {
                // when keycode QMKBEST is released
                midi_send_cc(&midi_device, 1, 8, 0);
            }
            break;
        case MI_CH9_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 9, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 9, 0);
            }
            break;
        case MI_CH10_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 10, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 10, 0);
            }
            break;
        case MI_CH11_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 11, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 11, 0);
            }
            break;
        case MI_CH12_Click:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                midi_send_cc(&midi_device, 1, 12, 127);
            } else {
                // when keycode QMKURL is released
                midi_send_cc(&midi_device, 1, 12, 0);
            }
            break;

        
    }
    return true;
}

const int encoderKeysL2[24] = {
    KC_A,KC_B,\
    KC_C,KC_D,\
    KC_E,KC_F,\
    KC_G,KC_H,\
    KC_I,KC_J,\
    KC_K,KC_L,\
    KC_M,KC_N,\
    KC_O,KC_P,\
    KC_Q,KC_R,\
    KC_S,KC_T,\
    KC_U,KC_V,\
    KC_W,KC_X\
    };

const int encoderKeysL1[24] = {
    KC_MS_WH_DOWN,KC_MS_WH_UP,\
    KC_LEFT,KC_RIGHT,\
    KC_UP,KC_DOWN,\
    KC_G,KC_H,\
    KC_I,KC_J,\
    KC_K,KC_L,\
    KC_M,KC_N,\
    KC_O,KC_P,\
    KC_Q,KC_R,\
    BL_DEC,BL_INC,\
    BL_DEC,BL_INC,\
    KC__VOLDOWN,KC__VOLUP\
    };

int midiVals[12] = {
    64,\
    64,\
    64,\
    64,\
    64,\
    64,\
    64,\
    64,\
    64,\
    64,\
    64,\
    64\
    };

//void encoder_update_user(uint8_t index, bool clockwise) {
//  tap_code(encoderKeys[(index * 2) + clockwise]);
//}


// void encoder_update_user(uint8_t index, bool clockwise) {
//     if (clockwise) {
//       midi_send_cc(&midi_device, 1, index+1, 1);
//     } else {
//       midi_send_cc(&midi_device, 1, index+1, 65);
//     }
//   wait_ms(10);
// }

void tap_code_catcher(int input) {
    if ( input == BL_INC ) {
        backlight_increase();
        return;
    } else if  ( input == BL_DEC ) {
        backlight_decrease();
        return;
    } else {
        tap_code(input);
    }
}

void encoder_update_user(uint8_t index, bool clockwise) {
// left encoder
    
    switch(biton32(layer_state)){
        case 2:
            if (clockwise) {
                //backlight_increase();
                tap_code_catcher(encoderKeysL1[(index*2)+1]);
            } else {
                //backlight_decrease();
                tap_code_catcher(encoderKeysL1[(index*2)]);
            }
            break;
        case 1:
            if (clockwise) {
                if (midiVals[index] < 127) {
                    midiVals[index] = midiVals[index]+1;
                }
            midi_send_cc(&midi_device, 1, index+1, midiVals[index]);
            } else {
                if (midiVals[index] > 0) {
                midiVals[index] = midiVals[index]-1;
                }
            midi_send_cc(&midi_device, 1, index+1, midiVals[index]);
            }
            break;
        default:
            if (clockwise) {
            midi_send_cc(&midi_device, 1, index+1, 63);
            } else {
            midi_send_cc(&midi_device, 1, index+1, 65);
            }
            //wait_ms(10);
            break;
    
    }
}



// void encoder_update_user(uint8_t index, bool clockwise) {
//   if (index == 0) { /* First encoder */
//     if (clockwise) {
//       midi_send_cc(&midi_device, 1, 0, 1);
//     } else {
//       midi_send_cc(&midi_device, 1, 0, 65);
//     }
//   } else if (index == 1) { /* Second encoder */  
//     if (clockwise) {
//       midi_send_cc(&midi_device, 1, 1, 1);
//     } else {
//       midi_send_cc(&midi_device, 1, 1, 65);
//     }
//       } else if (index == 2) { /* Second encoder */  
//     if (clockwise) {
//       midi_send_cc(&midi_device, 1, 2, 1);
//     } else {
//       midi_send_cc(&midi_device, 1, 2, 65);
//     }
//     } else if (index == 3) { /* Second encoder */  
//     if (clockwise) {
//       midi_send_cc(&midi_device, 1, 3, 1);
//     } else {
//       midi_send_cc(&midi_device, 1, 3, 65);
//     }
//   }
//   wait_ms(10);
// }

/*
void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool led_update_user(led_t led_state) {
    return true;
}
*/
