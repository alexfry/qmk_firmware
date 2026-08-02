// Copyright 2019 Alex Fry
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#ifdef RAW_ENABLE
#    include "raw_hid.h"
#endif
#ifdef VIA_ENABLE
#    include "via.h"
#    include "dynamic_keymap.h"
#    include "keymap_introspection.h"
#endif
#include "qmk_midi.h"

/*
 * FnCoder Raw HID (32-byte reports). Full doc: HOST_RAW_HID_PROTOCOL.md
 *
 *   0xFE  Host → KB   LED / status commands (existing)
 *   0xFC  Host → KB   Stream control (STREAM / PING)
 *   0xFD  KB → Host   Encoder events (TURN / BUTTON / PONG)
 *
 * Avoids VIA command IDs (0x01–0x15). Encoder events dual-emit with MIDI.
 */
#define FNCODER_HOST_CMD  0xFE /* host → keyboard: LED */
#define FNCODER_CTRL_CMD  0xFC /* host → keyboard: stream / ping */
#define FNCODER_EVT_CMD   0xFD /* keyboard → host: events */

#define FNCODER_CTRL_STREAM 0x01
#define FNCODER_CTRL_PING   0x02

#define FNCODER_EVT_TURN   0x01
#define FNCODER_EVT_BUTTON 0x02
#define FNCODER_EVT_PONG   0x7F

#ifndef RAW_EPSIZE
#    define RAW_EPSIZE 32
#endif

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

/*
 * VIA EEPROM may already be "valid" from before ENCODER_MAP existed, so
 * rotation slots were never written (clicks still work; turns do nothing).
 * Seed encoder_map → EEPROM once per firmware feature version.
 */
#if defined(ENCODER_MAP_ENABLE) && defined(VIA_ENABLE)
#    define FNCODER_USER_MAGIC      0xA84E
/* Bump when firmware encoder_map defaults change so EEPROM is reseeded. */
#    define FNCODER_ENCODER_MAP_VER 2

typedef union {
    uint32_t raw;
    struct {
        uint16_t magic;
        uint8_t  encoder_map_ver;
        uint8_t  reserved;
    };
} fncoder_user_config_t;

static void fncoder_seed_encoder_map(void) {
    for (uint8_t layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer++) {
        for (uint8_t encoder = 0; encoder < NUM_ENCODERS; encoder++) {
            dynamic_keymap_set_encoder(layer, encoder, true, keycode_at_encodermap_location_raw(layer, encoder, true));
            dynamic_keymap_set_encoder(layer, encoder, false, keycode_at_encodermap_location_raw(layer, encoder, false));
        }
    }
}

static void fncoder_maybe_seed_encoder_map(void) {
    fncoder_user_config_t cfg = {.raw = eeconfig_read_user()};
    if (cfg.magic == FNCODER_USER_MAGIC && cfg.encoder_map_ver >= FNCODER_ENCODER_MAP_VER) {
        return;
    }
    fncoder_seed_encoder_map();
    cfg.magic           = FNCODER_USER_MAGIC;
    cfg.encoder_map_ver = FNCODER_ENCODER_MAP_VER;
    cfg.reserved        = 0;
    eeconfig_update_user(cfg.raw);
}

void eeconfig_init_user(void) {
    /* Full EEPROM wipe — post_init will reseed encoder map. */
    eeconfig_update_user(0);
}
#endif /* ENCODER_MAP_ENABLE && VIA_ENABLE */

void keyboard_post_init_user(void) {
#if defined(ENCODER_MAP_ENABLE) && defined(VIA_ENABLE)
    /* Before animation so first turns after boot already send MIDI. */
    fncoder_maybe_seed_encoder_map();
#endif

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
    MI_CH12_Click,
    /*
     * Relative MIDI CC encoder turns (VIA encoder_map defaults).
     * 24 consecutive codes: for encoder i, CCW = FIRST+2*i, CW = FIRST+2*i+1.
     * Sends CC (i+1) value 65 (CCW) / 63 (CW) — same as the old encoder_update_user base layer.
     */
    MI_ENC_REL_FIRST,
    MI_ENC_REL_LAST = MI_ENC_REL_FIRST + 23,
    /*
     * Per-encoder status LED hue (old MO(3) + twist behaviour).
     * Same packing: CCW/CW pairs for encoders 0–11 → rgblight_sethsv_at(hue, 255, 255, i).
     */
    MI_ENC_HUE_FIRST,
    MI_ENC_HUE_LAST = MI_ENC_HUE_FIRST + 23
};

#define MI_E_CCW(i) ((uint16_t)(MI_ENC_REL_FIRST + (i) * 2))
#define MI_E_CW(i)  ((uint16_t)(MI_ENC_REL_FIRST + (i) * 2 + 1))
#define MI_H_CCW(i) ((uint16_t)(MI_ENC_HUE_FIRST + (i) * 2))
#define MI_H_CW(i)  ((uint16_t)(MI_ENC_HUE_FIRST + (i) * 2 + 1))

/* Per-encoder LED hue state (MO(3) twist / MI_ENC_HUE_*). */
static uint8_t encoderhues[12] = {27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27};

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

#ifdef RAW_ENABLE
/* Off by default so unsolicited 0xFD reports do not desync VIA's Raw HID
 * command/response pairing. Host apps (and the monitor script) send
 * FC 01 01 (STREAM on) after open. */
static bool    fncoder_hid_stream_enabled = false;
static uint8_t fncoder_evt_seq            = 0;

static void fncoder_raw_send_event(uint8_t msg_type, uint8_t index, uint8_t payload) {
    if (!fncoder_hid_stream_enabled && msg_type != FNCODER_EVT_PONG) {
        return;
    }
    uint8_t data[RAW_EPSIZE] = {0};
    data[0]                  = FNCODER_EVT_CMD;
    data[1]                  = msg_type;
    data[2]                  = index;
    data[3]                  = payload;
    data[4]                  = fncoder_evt_seq++;
    raw_hid_send(data, RAW_EPSIZE);
}

static void fncoder_raw_send_turn(uint8_t index, bool clockwise) {
    /* Relative continuous: +1 CW, -1 CCW (int8). */
    int8_t delta = clockwise ? (int8_t)1 : (int8_t)-1;
    fncoder_raw_send_event(FNCODER_EVT_TURN, index, (uint8_t)delta);
}

static void fncoder_raw_send_button(uint8_t index, bool pressed) {
    fncoder_raw_send_event(FNCODER_EVT_BUTTON, index, pressed ? 1 : 0);
}
#endif /* RAW_ENABLE */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= MI_CH1_Click && keycode <= MI_CH12_Click) {
        uint8_t idx = (uint8_t)(keycode - MI_CH1_Click);
        midi_click(idx + 1, record->event.pressed);
#ifdef RAW_ENABLE
        fncoder_raw_send_button(idx, record->event.pressed);
#endif
        return false;
    }
    if (keycode >= MI_ENC_REL_FIRST && keycode <= MI_ENC_REL_LAST) {
        if (record->event.pressed) {
            uint8_t idx = (uint8_t)((keycode - MI_ENC_REL_FIRST) / 2);
            bool    cw  = ((keycode - MI_ENC_REL_FIRST) % 2) != 0;
            /* Relative MIDI CC (channel 1, controller idx+1). */
            midi_send_cc(&midi_device, 1, idx + 1, cw ? 63 : 65);
#ifdef RAW_ENABLE
            fncoder_raw_send_turn(idx, cw);
#endif
        }
        return false;
    }
    if (keycode >= MI_ENC_HUE_FIRST && keycode <= MI_ENC_HUE_LAST) {
        if (record->event.pressed) {
            uint8_t idx = (uint8_t)((keycode - MI_ENC_HUE_FIRST) / 2);
            bool    cw  = ((keycode - MI_ENC_HUE_FIRST) % 2) != 0;
            if (cw) {
                encoderhues[idx] = (uint8_t)((encoderhues[idx] + 4) & 0xFF);
            } else {
                encoderhues[idx] = (uint8_t)((encoderhues[idx] - 4) & 0xFF);
            }
            /* Full sat/val so the chosen hue is obvious on that encoder LED. */
            rgblight_sethsv_at(encoderhues[idx], 255, 255, idx);
#ifdef RAW_ENABLE
            /* Still report relative turn to host while colour-tweaking. */
            fncoder_raw_send_turn(idx, cw);
#endif
        }
        return false;
    }
    return true;
}

#if defined(ENCODER_MAP_ENABLE)
/*
 * VIA-remappable encoder map (requires ENCODER_MAP_ENABLE in the via keymap).
 * Base / FN1: relative MIDI CC per encoder (custom keycodes).
 * FN2: legacy key pairs (scroll / arrows / letters / backlight / volume).
 * FN3: per-encoder status LED hue (hold MO(3), twist that knob).
 * FN4: transparent.
 *
 * Note: ENCODER_MAP replaces encoder_update_user entirely on via builds.
 */
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = {
        ENCODER_CCW_CW(MI_E_CCW(0), MI_E_CW(0)),   ENCODER_CCW_CW(MI_E_CCW(1), MI_E_CW(1)),
        ENCODER_CCW_CW(MI_E_CCW(2), MI_E_CW(2)),   ENCODER_CCW_CW(MI_E_CCW(3), MI_E_CW(3)),
        ENCODER_CCW_CW(MI_E_CCW(4), MI_E_CW(4)),   ENCODER_CCW_CW(MI_E_CCW(5), MI_E_CW(5)),
        ENCODER_CCW_CW(MI_E_CCW(6), MI_E_CW(6)),   ENCODER_CCW_CW(MI_E_CCW(7), MI_E_CW(7)),
        ENCODER_CCW_CW(MI_E_CCW(8), MI_E_CW(8)),   ENCODER_CCW_CW(MI_E_CCW(9), MI_E_CW(9)),
        ENCODER_CCW_CW(MI_E_CCW(10), MI_E_CW(10)), ENCODER_CCW_CW(MI_E_CCW(11), MI_E_CW(11)),
    },
    [_FN1] = {
        ENCODER_CCW_CW(MI_E_CCW(0), MI_E_CW(0)),   ENCODER_CCW_CW(MI_E_CCW(1), MI_E_CW(1)),
        ENCODER_CCW_CW(MI_E_CCW(2), MI_E_CW(2)),   ENCODER_CCW_CW(MI_E_CCW(3), MI_E_CW(3)),
        ENCODER_CCW_CW(MI_E_CCW(4), MI_E_CW(4)),   ENCODER_CCW_CW(MI_E_CCW(5), MI_E_CW(5)),
        ENCODER_CCW_CW(MI_E_CCW(6), MI_E_CW(6)),   ENCODER_CCW_CW(MI_E_CCW(7), MI_E_CW(7)),
        ENCODER_CCW_CW(MI_E_CCW(8), MI_E_CW(8)),   ENCODER_CCW_CW(MI_E_CCW(9), MI_E_CW(9)),
        ENCODER_CCW_CW(MI_E_CCW(10), MI_E_CW(10)), ENCODER_CCW_CW(MI_E_CCW(11), MI_E_CW(11)),
    },
    [_FN2] = {
        ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(KC_LEFT, KC_RGHT),
        ENCODER_CCW_CW(KC_UP, KC_DOWN),   ENCODER_CCW_CW(KC_G, KC_H),
        ENCODER_CCW_CW(KC_I, KC_J),       ENCODER_CCW_CW(KC_K, KC_L),
        ENCODER_CCW_CW(KC_M, KC_N),       ENCODER_CCW_CW(KC_O, KC_P),
        ENCODER_CCW_CW(KC_Q, KC_R),       ENCODER_CCW_CW(BL_DOWN, BL_UP),
        ENCODER_CCW_CW(BL_DOWN, BL_UP),   ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
    },
    [_FN3] = {
        ENCODER_CCW_CW(MI_H_CCW(0), MI_H_CW(0)),   ENCODER_CCW_CW(MI_H_CCW(1), MI_H_CW(1)),
        ENCODER_CCW_CW(MI_H_CCW(2), MI_H_CW(2)),   ENCODER_CCW_CW(MI_H_CCW(3), MI_H_CW(3)),
        ENCODER_CCW_CW(MI_H_CCW(4), MI_H_CW(4)),   ENCODER_CCW_CW(MI_H_CCW(5), MI_H_CW(5)),
        ENCODER_CCW_CW(MI_H_CCW(6), MI_H_CW(6)),   ENCODER_CCW_CW(MI_H_CCW(7), MI_H_CW(7)),
        ENCODER_CCW_CW(MI_H_CCW(8), MI_H_CW(8)),   ENCODER_CCW_CW(MI_H_CCW(9), MI_H_CW(9)),
        ENCODER_CCW_CW(MI_H_CCW(10), MI_H_CW(10)), ENCODER_CCW_CW(MI_H_CCW(11), MI_H_CW(11)),
    },
    [_FN4] = {
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
    },
};
#else /* !ENCODER_MAP_ENABLE — default keymap: layer-aware callback */

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

#ifdef RAW_ENABLE
    fncoder_raw_send_turn(index, clockwise);
#endif

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
#endif /* ENCODER_MAP_ENABLE */

#ifdef RAW_ENABLE
static void set_led_range(int start, int stop, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = start; i <= stop; i++) {
        rgblight_setrgb_at(r, g, b, i);
    }
}

/* LED command body: command at [0] (legacy unprefixed layout after stripping 0xFE). */
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

/* Host → keyboard 0xFC stream control / ping. */
static void fncoder_handle_ctrl_command(const uint8_t *cmd, uint8_t length) {
    if (length < 1) {
        return;
    }
    switch (cmd[0]) {
        case FNCODER_CTRL_STREAM:
            if (length >= 2) {
                fncoder_hid_stream_enabled = (cmd[1] != 0);
            }
            break;
        case FNCODER_CTRL_PING: {
            uint8_t cookie = (length >= 2) ? cmd[1] : 0;
            /* PONG always sent (even if stream is off) so path checks work. */
            uint8_t data[RAW_EPSIZE] = {0};
            data[0]                  = FNCODER_EVT_CMD;
            data[1]                  = FNCODER_EVT_PONG;
            data[2]                  = 0;
            data[3]                  = cookie;
            data[4]                  = fncoder_evt_seq++;
            raw_hid_send(data, RAW_EPSIZE);
            break;
        }
        default:
            break;
    }
}

/* Full host packet: demux by first byte (0xFE LEDs, 0xFC control). */
static void fncoder_handle_host_packet(const uint8_t *data, uint8_t length) {
    if (length < 1) {
        return;
    }
    switch (data[0]) {
        case FNCODER_HOST_CMD:
            if (length > 1) {
                fncoder_handle_led_command(&data[1], length - 1);
            }
            break;
        case FNCODER_CTRL_CMD:
            if (length > 1) {
                fncoder_handle_ctrl_command(&data[1], length - 1);
            }
            break;
        default:
            break;
    }
}

#    ifdef VIA_ENABLE
/*
 * VIA owns raw_hid_receive. Claim FnCoder packets (0xFE / 0xFC) only.
 */
bool via_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] != FNCODER_HOST_CMD && data[0] != FNCODER_CTRL_CMD) {
        return false;
    }
    fncoder_handle_host_packet(data, length);
    /* Returning true = fully handled (PING already sent its own 0xFD PONG). */
    return true;
}
#    else
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (data[0] == FNCODER_HOST_CMD || data[0] == FNCODER_CTRL_CMD) {
        fncoder_handle_host_packet(data, length);
        return;
    }
    /* Legacy desktop app: unprefixed LED commands (not safe under VIA). */
    fncoder_handle_led_command(data, length);
}
#    endif
#endif
