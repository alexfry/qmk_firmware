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

#include "raw_hid.h"
// #include "ws2812.h"

extern MidiDevice midi_device;
extern LED_TYPE rgb_matrix_ws2812_array[12];


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
    MI_CH12_Click,
    RGB_MAXSAT,
    RGB_SETD65
};

#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom(); // Enables RGB, without saving settings
  rgblight_sethsv_noeeprom(27,67,100);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}
#endif


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
		KC_TRNS, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_M_X, RGB_M_G, RGB_M_T, RGB_M_TW, KC_TRNS, KC_TRNS, KC_TRNS, MO(4), 
		KC_TRNS, RGB_SETD65, RGB_MAXSAT, KC_TRNS, RESET, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, RGB_SPI, RGB_SPD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, BL_DEC, BL_INC, BL_TOGG, BL_STEP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END),

    KEYMAP(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

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
        case RGB_MAXSAT:
            if (record->event.pressed) {
                // Do something when pressed
                rgblight_sethsv_noeeprom(237,255,255);
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key
            break;
        case RGB_SETD65:
            if (record->event.pressed) {
                // Do something when pressed
                rgblight_sethsv_noeeprom(27,67,100);
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key
            break;

        
    }
    return true;
}

const int encoderKeysL2[24] = {
    KC_A,KC_Q,\
    KC_S,KC_W,\
    KC_D,KC_E,\
    KC_F,KC_R,\
    KC_G,KC_T,\
    KC_H,KC_Y,\
    KC_J,KC_U,\
    KC_K,KC_I,\
    KC_L,KC_O,\
    KC_SCLN,KC_P,\
    KC_QUOT,KC_LBRC,\
    KC_BSLS,KC_RBRC\
    };

const int encoderKeysL1[24] = {
    KC_MS_WH_DOWN,KC_MS_WH_UP,\
    KC_LEFT,KC_RIGHT,\
    KC_UP,KC_DOWN,\
    RGB_HUI,RGB_HUD,\
    RGB_SAD,RGB_SAI,\
    RGB_VAD,RGB_VAI,\
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

int encoderhues[12] = {
    237,\
    237,\
    237,\
    237,\
    237,\
    237,\
    237,\
    237,\
    237,\
    237,\
    237,\
    237\
    };

int encodersats[12] = {
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255\
    };

int encodervals[12] = {
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255,\
    255\
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

bool encoder_update_user(uint8_t index, bool clockwise) {
// left encoder
    
    switch(biton32(layer_state)){
        case 3:
            if (clockwise) {
                if (get_mods() & MOD_BIT(KC_LSFT) || get_mods() & MOD_BIT(KC_RSFT)){
                    encodersats[index] = encodersats[index] + 4;
                    if (encodersats[index] > 255){
                        encodersats[index] = 255;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else if (get_mods() & MOD_BIT(KC_LGUI) || get_mods() & MOD_BIT(KC_RGUI)){
                    encodervals[index] += 16;
                    if (encodervals[index] >= 255){
                        encodervals[index] = 255;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else {
                    encoderhues[index] += 4;
                    
                    // if (encoderhues[index] > 255){
                    //     encoderhues[index] = 255;
                    // }
                    // int hues = rgblight_get_hue();
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                }
            } else {

                if (get_mods() & MOD_BIT(KC_LSFT) || get_mods() & MOD_BIT(KC_RSFT)){
                    encodersats[index] = encodersats[index] - 4;
                    if (encodersats[index] < 0){
                        encodersats[index] = 0;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else if (get_mods() & MOD_BIT(KC_LGUI) || get_mods() & MOD_BIT(KC_RGUI)){
                    encodervals[index] = encodervals[index] - 16;
                    if (encodervals[index] < 0){
                        encodervals[index] = 0;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else {
                    encoderhues[index] -= 4;
                    // if (encoderhues[index] < 0){
                    //     encoderhues[index] = 0;
                    // }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                }

            }
            break;
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
            // if (clockwise) {
            //     //backlight_increase();
            //     tap_code_catcher(encoderKeysL2[(index*2)+1]);
            // } else {
            //     //backlight_decrease();
            //     tap_code_catcher(encoderKeysL2[(index*2)]);
            // }
            break;
            //wait_ms(10);
            break;
    
    }
    return true; 
}


//////    LED HID STUFF




void set_led_range(int start, int stop, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = start; i <= stop; i++) {
        //rgb_led_set_color(i, r, g, b);
        rgblight_setrgb_at(r, g, b, i);
    }
}



#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch (data[0]) {
        // Set RGB state
        case 1:
            switch (data[1]) {
                case 2:
                    rgblight_step_noeeprom();
                    // rgb_matrix_enable_noeeprom();
                    // rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    // rgb_matrix_set_color_all(0, 0, 0);
                    // break;
                case 3:
                    rgblight_setrgb(RGB_WHITE);
                    // rgb_matrix_enable_noeeprom();
                    // rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    // rgb_matrix_set_color_all(0, 0, 0);
                    // break;
                case 4:
                    rgblight_disable_noeeprom();
                    // rgb_matrix_set_flags(LED_FLAG_NONE);
                    // rgb_matrix_disable_noeeprom();
                    // break;
                case 5:
                    rgblight_enable_noeeprom();
                    // rgb_matrix_set_flags(LED_FLAG_ALL);
                    // rgb_matrix_step();
                    // break;
                default:
                    // rgb_matrix_set_flags(LED_FLAG_ALL);
                    // rgb_matrix_enable_noeeprom();
                    break;
            }
            break;
        // Notifications
        // Note: caller is expected to reset RGB to previous state
        // TODO: set flags on single LEDs so RGB animation isn't interrupted on underglow
        case 2:
            switch (data[1]) {
                // Bottom underglow
                case 1: {
                    // uint8_t r = data[2];
                    // uint8_t g = data[3];
                    // uint8_t b = data[4];
                    // switch (rgb_matrix_get_flags()) {
                    //     case LED_FLAG_ALL:
                    //         rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    //         break;
                    //     case LED_FLAG_UNDERGLOW:
                    //         rgb_matrix_set_flags(LED_FLAG_NONE);
                    //         break;
                    //     case LED_FLAG_NONE:
                    //         rgb_matrix_enable_noeeprom();
                    //         break;
                    // }
                    // for (int i = 67; i <= 81; i++) {
                    //     rgb_matrix_set_color(i, r, g, b);
                    // }
                    // break;
                }
                // Full color change
                case 2: {
                    // uint8_t r = data[2];
                    // uint8_t g = data[3];
                    // uint8_t b = data[4];
                    // rgb_matrix_set_flags(LED_FLAG_NONE);
                    // rgb_matrix_set_color_all(r, g, b);
                    // break;
                }
                // Full underglow
                case 3: {
                    // uint8_t r = data[2];
                    // uint8_t g = data[3];
                    // uint8_t b = data[4];
                    // switch (rgb_matrix_get_flags()) {
                    //     case LED_FLAG_ALL:
                    //         rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    //         break;
                    //     case LED_FLAG_UNDERGLOW:
                    //         rgb_matrix_set_flags(LED_FLAG_NONE);
                    //         break;
                    //     case LED_FLAG_NONE:
                    //         rgb_matrix_enable_noeeprom();
                    //         break;
                    // }
                    // for (int i = 67; i <= 105; i++) {
                    //     rgb_matrix_set_color(i, r, g, b);
                    // }
                    // break;
                }
            }
            break;
        // Get RGB state
        case 3:
            // switch (rgb_matrix_get_flags()) {
            //     case LED_FLAG_ALL:
            //         data[0] = 1;
            //         break;
            //     case LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR:
            //         data[0] = 2;
            //         break;
            //     case LED_FLAG_UNDERGLOW:
            //         data[0] = 3;
            //         break;
            //     case LED_FLAG_NONE:
            //         data[0] = 4;
            //         break;
            //     default:
            //         data[0] = 0;
            //         break;
            // }
            // raw_hid_send(data, RAW_EPSIZE);
            // break;
        // Single LED control
        case 4: {
            // TODO: set only the specific LED(s) flag
            // rgb_matrix_set_flags(LED_FLAG_NONE);
            uint8_t index = data[2];
            uint8_t r     = data[3];
            uint8_t g     = data[4];
            uint8_t b     = data[5];
            switch (data[1]) {
                // Set one led to color
                case 1:
                    // rgb_matrix_set_color(index, r, g, b);
                    rgblight_setrgb_at(r, g, b, index);
                    break;
                // Set one row to color
                case 2:
                    switch (index) {
                        case 1:  // First row
                            set_led_range(0, 14, r, g, b);
                            break;
                        case 2:  // Second row
                            set_led_range(15, 29, r, g, b);
                            break;
                        case 3:  // Third row
                            set_led_range(30, 43, r, g, b);
                            break;
                        case 4:  // Fourth row
                            set_led_range(44, 57, r, g, b);
                            break;
                        case 5:  // Fifth row
                            set_led_range(58, 66, r, g, b);
                            break;
                        case 6:  // Bottom underglow
                            set_led_range(67, 81, r, g, b);
                            break;
                        case 7:  // Right underglow
                            set_led_range(82, 86, r, g, b);
                            break;
                        case 8:  // Top underglow
                            set_led_range(87, 99, r, g, b);
                            break;
                        case 9:  // Left underglow
                            set_led_range(100, 104, r, g, b);
                            break;
                    }
                    break;
            }
        }
    }
}
#endif




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
