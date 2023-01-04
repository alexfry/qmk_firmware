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

#if defined(MIDI_ENABLE)
    extern MidiDevice midi_device;
#endif
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



// #if RGBLIGHT_ENABLE
// void keyboard_post_init_rgb(void) {
//     /*if (userspace_config.rgb_layer_change) { rgblight_enable_noeeprom(); }*/
//     /*if (rgblight_config.enable) {*/
//         /*layer_state_set_user(layer_state);*/
//         /*uint16_t old_hue = rgblight_config.hue;*/
//         uint16_t old_hue = 170;
//         rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
//         for (uint16_t i = 255; i > 0; i--) {
//             rgblight_sethsv_noeeprom( ( i + old_hue) % 255, 255, 255);
//             matrix_scan();
//             wait_ms(10);
//         }
//     /*}*/
// }
// #endif



#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {

    // uint16_t old_hue = 170;
    // rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    // for (uint16_t i = 255; i > 0; i--) {
    //     rgblight_sethsv_noeeprom( ( i + old_hue) % 255, 255, 255);
    //     matrix_scan();
    //     wait_ms(10);
    // }

  rgblight_enable_noeeprom(); // Enables RGB, without saving settings


  rgblight_sethsv_noeeprom(27,67,0);
  for (int i = 0; i < RGBLED_NUM; i++) {
        rgblight_sethsv_at(17, 180, 255, i);
        wait_ms(50);
        rgblight_sethsv_at(27, 110, 0, i);
  }
  rgblight_sethsv_noeeprom(27,110,100);
}
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        #if defined(MIDI_ENABLE)
    KEYMAP(
		KC_ESC, MI_CH1_Click, MI_CH2_Click, MI_CH3_Click, MI_CH4_Click, MI_CH5_Click, MI_CH6_Click, MI_CH7_Click, MI_CH8_Click, MI_CH9_Click, MI_CH10_Click, MI_CH11_Click, MI_CH12_Click, MO(1), MO(2),
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
		MO(1), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGDN, KC_PGUP,
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END,
		KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RGUI, MO(3), KC_RCTL, KC_LEFT, KC_DOWN, KC_RIGHT),
        #endif
        #if defined(JOYSTICK_ENABLE)
    KEYMAP(
        KC_ESC, JS_2, JS_5, JS_8, JS_11, JS_14, JS_17, JS_20, JS_23, JS_26, JS_29, KC_TRNS, KC_MUTE, MO(1), MO(2),
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
		MO(1), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGDN, KC_PGUP,
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END,
		KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RGUI, MO(3), KC_RCTL, KC_LEFT, KC_DOWN, KC_RIGHT),
        #endif




    KEYMAP(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    KEYMAP(
		KC_TRNS, G(A(KC_F13)), G(A(KC_F14)), G(A(KC_F15)), G(A(KC_F16)), G(A(KC_F17)), G(A(KC_F18)), G(A(KC_F19)), G(A(KC_F20)), G(A(KC_F21)), G(A(KC_F22)), G(A(KC_F23)), G(A(KC_F24)), KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    KEYMAP(
		KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, TG(1), TG(2),
		KC_TRNS, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_M_X, RGB_M_G, RGB_M_T, RGB_M_TW, KC_TRNS, KC_TRNS, KC_TRNS, MO(4),
		KC_TRNS, RGB_SETD65, RGB_MAXSAT, KC_TRNS, QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
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


const int encoderKeysL2[24] = {
    A(KC_F13) ,G(KC_F13),\
    A(KC_F14) ,G(KC_F14),\
    A(KC_F15) ,G(KC_F15),\
    A(KC_F16) ,G(KC_F16),\
    A(KC_F17) ,G(KC_F17),\
    A(KC_F18) ,G(KC_F18),\
    A(KC_F19) ,G(KC_F19),\
    A(KC_F20) ,G(KC_F20),\
    A(KC_F21) ,G(KC_F21),\
    A(KC_F22) ,G(KC_F22),\
    A(KC_F23) ,G(KC_F23),\
    A(KC_F24) ,G(KC_F24)\
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
    KC_VOLD,KC_VOLU\
    };

const int gameHidKeys[24] = {
    JS_0,JS_1,\
    JS_3,JS_4,\
    JS_6,JS_7,\
    JS_9,JS_10,\
    JS_12,JS_13,\
    JS_15,JS_16,\
    JS_18,JS_19,\
    JS_21,JS_22,\
    JS_24,JS_25,\
    JS_27,JS_28,\
    JS_30,JS_31,\
    KC_VOLD,KC_VOLU\
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
    27,\
    27,\
    27,\
    27,\
    27,\
    27,\
    27,\
    27,\
    27,\
    27,\
    27,\
    27\
    };

int encodersats[12] = {
    110,\
    110,\
    110,\
    110,\
    110,\
    110,\
    110,\
    110,\
    110,\
    110,\
    110,\
    110\
    };

int encodervals[12] = {
    100,\
    100,\
    100,\
    100,\
    100,\
    100,\
    100,\
    100,\
    100,\
    100,\
    100,\
    100\
    };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        #if defined(MIDI_ENABLE)
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
        #endif
        case RGB_MAXSAT:
            if (record->event.pressed) {
                // Do something when pressed
                int h = 237;
                int s = 255;
                int v = 255;
                rgblight_sethsv_noeeprom(h,s,v);
                int i;
                for (i = 0; i < RGBLED_NUM; ++i)
                {
                    encoderhues[i] = h;
                    encodersats[i] = s;
                    encodervals[i] = v;
                }
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key
            break;
        case RGB_SETD65:
            if (record->event.pressed) {
                // Do something when pressed
                int h = 27;
                int s = 110;
                int v = 100;

                if (encoderhues[1] == 27 && encodersats[1] == 110 && encodervals[1] == 100) {
                    h = 17;
                    s = 180;
                    v = 255;
                }

                rgblight_sethsv_noeeprom(h,s,v);
                int i;
                for (i = 0; i < RGBLED_NUM; ++i)
                {
                    encoderhues[i] = h;
                    encodersats[i] = s;
                    encodervals[i] = v;
                }
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key
            break;


    }
    return true;
}

// void d65_to_hsv(int input) {
//     int lowVals[3] = {21,61,34};
//     int midVals[3] = {21,67,100};
//     int highVals[3] {21,160,255};
// }



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
    #if defined(JOYSTICK_ENABLE)
    } else if  ( input == JS_0 ) {
        register_joystick_button(0);
        unregister_joystick_button(0);
        return;
    } else if  ( input == JS_1 ) {
        register_joystick_button(1);
        unregister_joystick_button(1);
        return;
    } else if  ( input == JS_2 ) {
        register_joystick_button(2);
        unregister_joystick_button(2);
        return;
    } else if  ( input == JS_3 ) {
        register_joystick_button(3);
        unregister_joystick_button(3);
        return;
    } else if  ( input == JS_4 ) {
        register_joystick_button(4);
        unregister_joystick_button(4);
        return;
    } else if  ( input == JS_5 ) {
        register_joystick_button(5);
        unregister_joystick_button(5);
        return;
    } else if  ( input == JS_6 ) {
        register_joystick_button(6);
        unregister_joystick_button(6);
        return;
    } else if  ( input == JS_7 ) {
        register_joystick_button(7);
        unregister_joystick_button(7);
        return;
    } else if  ( input == JS_8 ) {
        register_joystick_button(8);
        unregister_joystick_button(8);
        return;
    } else if  ( input == JS_9 ) {
        register_joystick_button(9);
        unregister_joystick_button(9);
        return;
    } else if  ( input == JS_10 ) {
        register_joystick_button(10);
        unregister_joystick_button(10);
        return;
    } else if  ( input == JS_11 ) {
        register_joystick_button(11);
        unregister_joystick_button(11);
        return;
    } else if  ( input == JS_12 ) {
        register_joystick_button(12);
        unregister_joystick_button(12);
        return;
    } else if  ( input == JS_13 ) {
        register_joystick_button(13);
        unregister_joystick_button(13);
        return;
    } else if  ( input == JS_14 ) {
        register_joystick_button(14);
        unregister_joystick_button(14);
        return;
    } else if  ( input == JS_15 ) {
        register_joystick_button(15);
        unregister_joystick_button(15);
        return;
    } else if  ( input == JS_16 ) {
        register_joystick_button(16);
        unregister_joystick_button(16);
        return;
    } else if  ( input == JS_17 ) {
        register_joystick_button(17);
        unregister_joystick_button(17);
        return;
    } else if  ( input == JS_18 ) {
        register_joystick_button(18);
        unregister_joystick_button(18);
        return;
    } else if  ( input == JS_19 ) {
        register_joystick_button(19);
        unregister_joystick_button(19);
        return;
    } else if  ( input == JS_20 ) {
        register_joystick_button(20);
        unregister_joystick_button(20);
        return;
    } else if  ( input == JS_21 ) {
        register_joystick_button(21);
        unregister_joystick_button(21);
        return;
    } else if  ( input == JS_22 ) {
        register_joystick_button(22);
        unregister_joystick_button(22);
        return;
    } else if  ( input == JS_23 ) {
        register_joystick_button(23);
        unregister_joystick_button(23);
        return;
    } else if  ( input == JS_24 ) {
        register_joystick_button(24);
        unregister_joystick_button(24);
        return;
    } else if  ( input == JS_25 ) {
        register_joystick_button(25);
        unregister_joystick_button(25);
        return;
    } else if  ( input == JS_26 ) {
        register_joystick_button(26);
        unregister_joystick_button(26);
        return;
    } else if  ( input == JS_27 ) {
        register_joystick_button(27);
        unregister_joystick_button(27);
        return;
    } else if  ( input == JS_28 ) {
        register_joystick_button(28);
        unregister_joystick_button(28);
        return;
    } else if  ( input == JS_29 ) {
        register_joystick_button(29);
        unregister_joystick_button(29);
        return;
    } else if  ( input == JS_30 ) {
        register_joystick_button(30);
        unregister_joystick_button(30);
        return;
    } else if  ( input == JS_31 ) {
        register_joystick_button(31);
        unregister_joystick_button(31);
        return;
    #endif
    } else {
        tap_code(input);
        // register_code(input);
        // unregister_code(input);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
// left encoder

    switch(biton32(layer_state)){
        case 3:
            if (clockwise) {

                if (((get_mods() & MOD_MASK_CTRL)) && (get_mods() & MOD_MASK_SHIFT)) {
                    int i;
                    int p;

                    // create a temp array with the values from encoderhues
                    int tempH[RGBLED_NUM];
                    int tempS[RGBLED_NUM];
                    int tempV[RGBLED_NUM];
                    for (p = 0; p < RGBLED_NUM; ++p)
                    {
                        tempH[p] = encoderhues[p];
                        tempS[p] = encodersats[p];
                        tempV[p] = encodervals[p];
                    }

                    // offset the position of each element in encoderhues by -1, looping the first element to the last position
                    for (i = 0; i < RGBLED_NUM; ++i)
                    {
                        if (i == 0) {
                            encoderhues[0] = tempH[RGBLED_NUM-1];
                            encodersats[0] = tempS[RGBLED_NUM-1];
                            encodervals[0] = tempV[RGBLED_NUM-1];
                        } else {
                            encoderhues[i] = tempH[i-1];
                            encodersats[i] = tempS[i-1];
                            encodervals[i] = tempV[i-1];
                        }
                        rgblight_sethsv_at(encoderhues[i], encodersats[i], encodervals[i], i);
                    }



                } else if (get_mods() & MOD_MASK_GUI){
                    encodervals[index] += 16;
                    if (encodervals[index] >= 255){
                        encodervals[index] = 255;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else if (get_mods() & MOD_MASK_CTRL ){
                    int i;
                    for (i = 0; i < RGBLED_NUM; ++i)
                    {
                    encoderhues[i] += 4;
                    rgblight_sethsv_at(encoderhues[i], encodersats[i], encodervals[i], i);
                    }
                } else if  (get_mods() & MOD_MASK_SHIFT){
                    encodersats[index] = encodersats[index] + 16;
                    if (encodersats[index] > 255){
                        encodersats[index] = 255;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else {
                    if (encoderhues[index] == 27 && encodersats[index] == 110 && encodervals[index] == 100) {
                        encoderhues[index] = 4;
                        encodersats[index] = 255;
                        encodervals[index] = 255;
                    }
                    encoderhues[index] += 4;
                    // if (encoderhues[index] > 255){
                    //     encoderhues[index] = 255;
                    // }
                    // int hues = rgblight_get_hue();
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                }
            } else {
                if (((get_mods() & MOD_MASK_CTRL)) && (get_mods() & MOD_MASK_SHIFT)) {
                    int i;
                    int p;

                    // create a temp array with the values from encoderhues
                    int tempH[RGBLED_NUM];
                    int tempS[RGBLED_NUM];
                    int tempV[RGBLED_NUM];
                    for (p = 0; p < RGBLED_NUM; ++p)
                    {
                        tempH[p] = encoderhues[p];
                        tempS[p] = encodersats[p];
                        tempV[p] = encodervals[p];
                    }

                    // offset the position of each element in encoderhues by 1, looping the last element to the first position
                    for (i = 0; i < RGBLED_NUM; ++i)
                    {
                        if (i == RGBLED_NUM-1){
                            encoderhues[i] = tempH[0];
                            encodersats[i] = tempS[0];
                            encodervals[i] = tempV[0];
                            }
                        else{
                            encoderhues[i] = tempH[i+1];
                            encodersats[i] = tempS[i+1];
                            encodervals[i] = tempV[i+1];
                        }
                        rgblight_sethsv_at(encoderhues[i], encodersats[i], encodervals[i], i);
                    }


                } else if (get_mods() & MOD_MASK_SHIFT){
                    encodersats[index] = encodersats[index] - 16;
                    if (encodersats[index] < 0){
                        encodersats[index] = 0;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else if (get_mods() & MOD_MASK_GUI){
                    encodervals[index] = encodervals[index] - 16;
                    if (encodervals[index] < 0){
                        encodervals[index] = 0;
                    }
                    rgblight_sethsv_at(encoderhues[index], encodersats[index], encodervals[index], index);
                } else if (get_mods() & MOD_MASK_CTRL ){
                    int i;
                    for (i = 0; i < RGBLED_NUM; ++i)
                    {
                    encoderhues[i] -= 4;
                    rgblight_sethsv_at(encoderhues[i], encodersats[i], encodervals[i], i);
                    }
                } else {
                    if (encoderhues[index] == 27 && encodersats[index] == 110 && encodervals[index] == 100) {
                        encoderhues[index] = 4;
                        encodersats[index] = 255;
                        encodervals[index] = 255;
                    }
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
                tap_code16(encoderKeysL2[(index*2)+1]);
            } else {
                //backlight_decrease();
                tap_code16(encoderKeysL2[(index*2)]);
            }
            break;
        case 1:
            #if defined(MIDI_ENABLE)
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
            #endif
            break;
        default:
            #if defined(MIDI_ENABLE)
            if (clockwise) {
            midi_send_cc(&midi_device, 1, index+1, 63);
            } else {
            midi_send_cc(&midi_device, 1, index+1, 65);
            }
            #endif

            #if defined(JOYSTICK_ENABLE)
            if (clockwise) {

                 //backlight_increase();
                tap_code_catcher(gameHidKeys[(index*2)+1]);

                // tap_code_catcher(JS_0);
            } else {
                //backlight_decrease();
                tap_code_catcher(gameHidKeys[(index*2)]);
            }

            #endif

            // if (clockwise) {
            //     //backlight_increase();
            //     tap_code_catcher(encoderKeysL2[(index*2)+1]);
            // } else {
            //     //backlight_decrease();
            //     tap_code_catcher(encoderKeysL2[(index*2)]);
            // }
            break;
            //wait_ms(10);


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
        case 2: {
            // uint8_t index = data[2];
            uint8_t r     = data[2];
            uint8_t g     = data[3];
            uint8_t b     = data[4];
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
                case 4: {
                    // set all leds to a color with hsv input values

                    // Do something when pressed
                    // incoming values assumed to already be hsv
                    rgblight_sethsv_noeeprom(r,g,b);
                    int i;
                    for (i = 0; i < RGBLED_NUM; ++i)
                    {
                        encoderhues[i] = r;
                        encodersats[i] = g;
                        encodervals[i] = b;
                    }


                    break;
                }
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
                case 3:
                    // TODO: set only the specific LED(s) flag
                    // rgb_matrix_set_flags(LED_FLAG_NONE);
                    // uint8_t index = data[2];
                    // uint8_t h     = data[3];
                    // uint8_t s     = data[4];
                    // uint8_t v     = data[5];
                    // switch (data[1]) {
                    //     // Set one led to color
                    //     case 1:
                    //         // rgb_matrix_set_color(index, r, g, b);
                    //         // rgblight_setrgb_at(r, g, b, index);

                    rgblight_sethsv_at(r,g,b, index);
                    encoderhues[index] = r;
                    encodersats[index] = g;
                    encodervals[index] = b;
                    break;
                        // Set one row to color

                // }
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
