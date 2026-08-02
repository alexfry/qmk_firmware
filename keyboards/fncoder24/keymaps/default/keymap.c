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
 * FnCoder Raw HID (32-byte reports). Same namespaces as FnCoder84.
 * Full doc: keyboards/fncoder24/HOST_RAW_HID_PROTOCOL.md
 *   0xFE  Host → KB   LED / status
 *   0xFC  Host → KB   STREAM / PING
 *   0xFD  KB → Host   TURN / BUTTON / PONG
 *
 * MIDI dual-emit: channel 1, CC index 0–3, CW=65 / CCW=63 (historical 24 polarity).
 */
#define FNCODER_HOST_CMD  0xFE
#define FNCODER_CTRL_CMD  0xFC
#define FNCODER_EVT_CMD   0xFD

#define FNCODER_CTRL_STREAM 0x01
#define FNCODER_CTRL_PING   0x02

#define FNCODER_EVT_TURN   0x01
#define FNCODER_EVT_BUTTON 0x02
#define FNCODER_EVT_PONG   0x7F

#ifndef RAW_EPSIZE
#    define RAW_EPSIZE 32
#endif

/* Warm white settle (matches FnCoder84 / pre-refactor look). */
#define FNCODER_LED_HUE 27
#define FNCODER_LED_SAT 110
#define FNCODER_LED_VAL 100

#define FNCODER_BOOT_FLASH_HUE 17
#define FNCODER_BOOT_FLASH_SAT 180
#define FNCODER_BOOT_FLASH_VAL 255
#define FNCODER_BOOT_FLASH_MS  50

#define FNCODER_NUM_ENCODERS 4

enum layer_names {
    _BASE,
    _FN,
    _FN2,
    _FN3
};

/*
 * VIA EEPROM may predate ENCODER_MAP; seed once per version.
 * Magic low byte differs from 84 (0x24) so user EEPROM flags do not clash if shared tooling.
 */
#if defined(ENCODER_MAP_ENABLE) && defined(VIA_ENABLE)
#    define FNCODER_USER_MAGIC      0xA824
/* Bump when flash keymap / encoder_map / custom keycodes change so VIA EEPROM is rewritten. */
#    define FNCODER_ENCODER_MAP_VER 3

typedef union {
    uint32_t raw;
    struct {
        uint16_t magic;
        uint8_t  encoder_map_ver;
        uint8_t  reserved;
    };
} fncoder_user_config_t;

/*
 * VIA stores matrix keycodes and encoder maps in EEPROM. After firmware
 * layout changes, reseed BOTH from flash (encoder-only seed left lighting
 * on layer 0 while the flash keymap had already moved it to layer 1).
 */
static void fncoder_maybe_seed_via_defaults(void) {
    fncoder_user_config_t cfg = {.raw = eeconfig_read_user()};
    if (cfg.magic == FNCODER_USER_MAGIC && cfg.encoder_map_ver >= FNCODER_ENCODER_MAP_VER) {
        return;
    }
    dynamic_keymap_reset();
    cfg.magic           = FNCODER_USER_MAGIC;
    cfg.encoder_map_ver = FNCODER_ENCODER_MAP_VER;
    cfg.reserved        = 0;
    eeconfig_update_user(cfg.raw);
}

void eeconfig_init_user(void) {
    eeconfig_update_user(0);
}
#endif /* ENCODER_MAP_ENABLE && VIA_ENABLE */

void keyboard_post_init_user(void) {
#if defined(ENCODER_MAP_ENABLE) && defined(VIA_ENABLE)
    fncoder_maybe_seed_via_defaults();
#endif

    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

    rgblight_sethsv_noeeprom(FNCODER_LED_HUE, FNCODER_LED_SAT, 0);
    for (uint8_t i = 0; i < RGBLIGHT_LED_COUNT; i++) {
        rgblight_sethsv_at(FNCODER_BOOT_FLASH_HUE, FNCODER_BOOT_FLASH_SAT, FNCODER_BOOT_FLASH_VAL, i);
        wait_ms(FNCODER_BOOT_FLASH_MS);
        rgblight_sethsv_at(FNCODER_LED_HUE, FNCODER_LED_SAT, 0, i);
    }
    rgblight_sethsv_noeeprom(FNCODER_LED_HUE, FNCODER_LED_SAT, FNCODER_LED_VAL);
}

enum custom_keycodes {
    /* Encoder push: MIDI CC click + Raw HID BUTTON (same idea as FnCoder84). */
    MI_CH1_Click = SAFE_RANGE,
    MI_CH2_Click,
    MI_CH3_Click,
    MI_CH4_Click,
    /*
     * Relative MIDI + HID turns for encoder_map.
     * Encoder i: CCW = FIRST+2*i, CW = FIRST+2*i+1.
     * MIDI: channel 1, controller i (0–3), CW=65 / CCW=63.
     */
    MI_ENC_REL_FIRST,
    MI_ENC_REL_LAST = MI_ENC_REL_FIRST + (FNCODER_NUM_ENCODERS * 2) - 1,
    /* Per-encoder status LED hue (FN layer / MO(1) + twist). */
    MI_ENC_HUE_FIRST,
    MI_ENC_HUE_LAST = MI_ENC_HUE_FIRST + (FNCODER_NUM_ENCODERS * 2) - 1
};

#define MI_E_CCW(i) ((uint16_t)(MI_ENC_REL_FIRST + (i) * 2))
#define MI_E_CW(i)  ((uint16_t)(MI_ENC_REL_FIRST + (i) * 2 + 1))
#define MI_H_CCW(i) ((uint16_t)(MI_ENC_HUE_FIRST + (i) * 2))
#define MI_H_CW(i)  ((uint16_t)(MI_ENC_HUE_FIRST + (i) * 2 + 1))

static uint8_t encoderhues[FNCODER_NUM_ENCODERS] = {27, 27, 27, 27};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        /* Encoder pushes: MIDI click + HID BUTTON (not lighting). */
        MI_CH1_Click, MI_CH2_Click, MI_CH3_Click, MI_CH4_Click,
        MO(1),        KC_PSLS,      KC_PAST,      KC_PMNS,
        KC_P7,        KC_P8,        KC_P9,        KC_PPLS,
        KC_P4,        KC_P5,        KC_P6,
        KC_P1,        KC_P2,        KC_P3,        KC_PENT,
        KC_P0,        KC_PDOT
    ),
    [_FN] = LAYOUT(
        /* Lighting controls moved here (hold MO(1)). */
        BL_TOGG, BL_STEP, UG_TOGG, UG_NEXT,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
        _______, QK_BOOT
    ),
    [_FN2] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______
    ),
    [_FN3] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______
    ),
};

#ifdef RAW_ENABLE
/* Off by default so unsolicited 0xFD reports do not desync VIA's Raw HID
 * command/response pairing. Host apps send FC 01 01 (STREAM on) first. */
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
    int8_t delta = clockwise ? (int8_t)1 : (int8_t)-1;
    fncoder_raw_send_event(FNCODER_EVT_TURN, index, (uint8_t)delta);
}

static void fncoder_raw_send_button(uint8_t index, bool pressed) {
    fncoder_raw_send_event(FNCODER_EVT_BUTTON, index, pressed ? 1 : 0);
}
#endif /* RAW_ENABLE */

static void midi_click(uint8_t cc, bool pressed) {
    /* Same gate style as FnCoder84: 127 press / 0 release. CC = encoder index 0–3. */
    midi_send_cc(&midi_device, 1, cc, pressed ? 127 : 0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= MI_CH1_Click && keycode <= MI_CH4_Click) {
        uint8_t idx = (uint8_t)(keycode - MI_CH1_Click);
        midi_click(idx, record->event.pressed);
#ifdef RAW_ENABLE
        fncoder_raw_send_button(idx, record->event.pressed);
#endif
        return false;
    }
    if (keycode >= MI_ENC_REL_FIRST && keycode <= MI_ENC_REL_LAST) {
        if (record->event.pressed) {
            uint8_t idx = (uint8_t)((keycode - MI_ENC_REL_FIRST) / 2);
            bool    cw  = ((keycode - MI_ENC_REL_FIRST) % 2) != 0;
            /* Historical 24 polarity: CW=65, CCW=63, controller = index 0–3. */
            midi_send_cc(&midi_device, 1, idx, cw ? 65 : 63);
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
            rgblight_sethsv_at(encoderhues[idx], 255, 255, idx);
#ifdef RAW_ENABLE
            fncoder_raw_send_turn(idx, cw);
#endif
        }
        return false;
    }
    return true;
}

#if defined(ENCODER_MAP_ENABLE)
/*
 * VIA-remappable encoder map (4 knobs).
 * Base: relative MIDI + HID. FN: per-LED hue. FN2/FN3: transparent.
 */
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = {
        ENCODER_CCW_CW(MI_E_CCW(0), MI_E_CW(0)),
        ENCODER_CCW_CW(MI_E_CCW(1), MI_E_CW(1)),
        ENCODER_CCW_CW(MI_E_CCW(2), MI_E_CW(2)),
        ENCODER_CCW_CW(MI_E_CCW(3), MI_E_CW(3)),
    },
    [_FN] = {
        ENCODER_CCW_CW(MI_H_CCW(0), MI_H_CW(0)),
        ENCODER_CCW_CW(MI_H_CCW(1), MI_H_CW(1)),
        ENCODER_CCW_CW(MI_H_CCW(2), MI_H_CW(2)),
        ENCODER_CCW_CW(MI_H_CCW(3), MI_H_CW(3)),
    },
    [_FN2] = {
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
    },
    [_FN3] = {
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
        ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
    },
};
#else /* !ENCODER_MAP_ENABLE */

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index >= FNCODER_NUM_ENCODERS) {
        return false;
    }

#ifdef RAW_ENABLE
    fncoder_raw_send_turn(index, clockwise);
#endif

    switch (get_highest_layer(layer_state)) {
        case _FN:
            if (clockwise) {
                encoderhues[index] = (uint8_t)((encoderhues[index] + 4) & 0xFF);
            } else {
                encoderhues[index] = (uint8_t)((encoderhues[index] - 4) & 0xFF);
            }
            rgblight_sethsv_at(encoderhues[index], 255, 255, index);
            break;
        default:
            midi_send_cc(&midi_device, 1, index, clockwise ? 65 : 63);
            break;
    }
    wait_ms(40);
    return false;
}
#endif /* ENCODER_MAP_ENABLE */

#ifdef RAW_ENABLE
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
                        if (led_index < RGBLIGHT_LED_COUNT) {
                            rgblight_setrgb_at(r, g, b, led_index);
                        }
                        break;
                    case 2:
                        /* Range groups: only group 1 maps to all 4 encoder LEDs on this board. */
                        if (led_index == 1) {
                            for (uint8_t i = 0; i < RGBLIGHT_LED_COUNT; i++) {
                                rgblight_setrgb_at(r, g, b, i);
                            }
                        }
                        break;
                }
            }
            break;
        default:
            break;
    }
}

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
            uint8_t cookie           = (length >= 2) ? cmd[1] : 0;
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
bool via_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] != FNCODER_HOST_CMD && data[0] != FNCODER_CTRL_CMD) {
        return false;
    }
    fncoder_handle_host_packet(data, length);
    return true;
}
#    else
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (data[0] == FNCODER_HOST_CMD || data[0] == FNCODER_CTRL_CMD) {
        fncoder_handle_host_packet(data, length);
        return;
    }
    fncoder_handle_led_command(data, length);
}
#    endif
#endif /* RAW_ENABLE */
