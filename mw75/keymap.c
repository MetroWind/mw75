/* Copyright 2020 MetroWind
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
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

enum layer_names {
    _BASE,
    _NUMPAD,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ortho_5x15(
        KC_GRV,  KC_1,    KC_2,    KC_3,        KC_4,   KC_5,   KC_6,    KC_ESC,  KC_BSPC, KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS, KC_EQL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,        KC_R,   KC_T,   KC_DEL,  KC_LBRC, KC_RBRC, KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,        KC_F,   KC_G,   KC_NO,   KC_UP,   KC_NO,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        SC_LSPO, KC_Z,    KC_X,    KC_C,        KC_V,   KC_B,   KC_LEFT, KC_DOWN, KC_RGHT, KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, SC_RSPC,
        KC_LCTL, KC_LALT, KC_LGUI, TG(_NUMPAD), KC_SPC, KC_ENT, KC_SPC,  KC_SPC,  KC_BSPC, KC_ENT, KC_SPC, KC_RGUI, KC_RALT, MO(_FN), KC_RCTL),
    [_NUMPAD] = LAYOUT_ortho_5x15(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P7,   KC_P8,   KC_P9,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P4,   KC_P5,   KC_P6,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P1,   KC_P2,   KC_P3,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P0,   KC_P0,   KC_PDOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    [_FN] = LAYOUT_ortho_5x15(
        AG_TOGG, KC_F1,   KC_F2,    KC_F3,   KC_F4,   KC_F5, KC_F6, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,  KC_F12,
        BL_TOGG, BL_STEP, BL_BRTG,  BL_UP,   BL_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,   KC_NO,
        RGB_TOG, RGB_MOD, RGB_RMOD, RGB_VAI, RGB_VAD, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_TRNS, QK_BOOT),
};

#ifdef OTHER_KEYMAP_C
#    include OTHER_KEYMAP_C
#endif // OTHER_KEYMAP_C

// Force enable numlock
void led_set_keymap(uint8_t usb_led) {
    if (!host_keyboard_led_state().num_lock) {
        register_code(KC_NUM_LOCK);
        unregister_code(KC_NUM_LOCK);
    }
}

// RGB indicators
#if defined(RGB_MATRIX_ENABLE)

#define CAPS_LOCK_MAX_BRIGHTNESS 0xFF
#ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #undef CAPS_LOCK_MAX_BRIGHTNESS
    #define CAPS_LOCK_MAX_BRIGHTNESS RGB_MATRIX_MAXIMUM_BRIGHTNESS
#endif

#define CAPS_LOCK_VAL_STEP 8
#ifdef RGB_MATRIX_VAL_STEP
    #undef CAPS_LOCK_VAL_STEP
    #define CAPS_LOCK_VAL_STEP RGB_MATRIX_VAL_STEP
#endif

const int NUMPAD_LED_INDICES[] = {
    8, 11, 21, 22, 23, 36, 37, 38, 51, 52, 53};

bool rgb_matrix_indicators_user(void)
{
    // Capslock indicator
    if(host_keyboard_led_state().caps_lock)
    {
        uint8_t b = rgb_matrix_get_val();
        if(b < CAPS_LOCK_VAL_STEP)
        {
            b = CAPS_LOCK_VAL_STEP;
        }
        else if(b < (CAPS_LOCK_MAX_BRIGHTNESS - CAPS_LOCK_VAL_STEP))
        {
            b += CAPS_LOCK_VAL_STEP;  // one step more than current brightness
        }
        else
        {
            b = CAPS_LOCK_MAX_BRIGHTNESS;
        }
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, b, b, b);  // white, with the adjusted brightness
    }
    // Layer indicators
    switch (get_highest_layer(layer_state|default_layer_state))
    {
    case _NUMPAD:
        for(size_t i = 0; i < NELEMS(NUMPAD_LED_INDICES); i++)
        {
            rgb_matrix_set_color(NUMPAD_LED_INDICES[i], RGB_GREEN);
        }
        break;
    default:
        break;
    }

    return false;
}

#endif
