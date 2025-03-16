/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
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
#include "sendstring_hungarian.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
enum custom_layers {
    _BL,
    _FL,
};

enum {
    TD_GRV_ESC, //grave(0) - esc
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: Base Layer (Default Layer)
   */
[_BL] = LAYOUT_65_iso_blocker(
  TD(TD_GRV_ESC), HU_1,     HU_2,     HU_3,     HU_4,     HU_5,     HU_6,     HU_7,     HU_8,     HU_9,     HU_ODIA,  HU_UDIA,  HU_OACU,  KC_BSPC,  KC_INS,
  KC_TAB,         HU_Q,     HU_W,     HU_E,     HU_R,     HU_T,     HU_Z,     HU_U,     HU_I,     HU_O,     HU_P,     HU_ODAC,  HU_UACU,            KC_DEL,
  KC_CAPS,        HU_A,     HU_S,     HU_D,     HU_F,     HU_G,     HU_H,     HU_J,     HU_K,     HU_L,     HU_EACU,  HU_AACU,  HU_UDAC,  KC_ENT,   KC_HOME,
  KC_LSFT,        HU_IACU,  HU_Y,     HU_X,     HU_C,     HU_V,     HU_B,     HU_N,     HU_M,     HU_COMM,  HU_DOT,   HU_MINS,  KC_RSFT,  KC_UP,    KC_END,
  KC_LCTL,        KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT),

  /* Keymap _FL: Function Layer
   */
[_FL] = LAYOUT_65_iso_blocker(
  KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,    AC_TOGG,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR,  KC_SCRL,  KC_PAUS,             _______,
  _______,  _______,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  KC_PGUP,
  _______,  _______,  RM_HUED,  RM_SATD,  RM_SPDD,  RM_TOGG,  KC_MUTE,  KC_VOLU,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  _______,   RM_VALU,  KC_PGDN,
  _______,  _______,  _______,                                QK_BOOT,                                _______,  _______,  RM_PREV,   RM_VALD,  RM_NEXT) 
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Grave, twice for ESC
    [TD_GRV_ESC] = ACTION_TAP_DANCE_DOUBLE(HU_0, KC_ESC)
};

// Caps Word configuration
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case HU_A ... HU_Z:
        case HU_MINS:
        case HU_ODIA: // Ö
        case HU_UDIA: // Ü
        case HU_OACU: // Ó
        case HU_ODAC: // Ő
        case HU_UACU: // Ú
        case HU_EACU: // É
        case HU_AACU: // Á
        case HU_UDAC: // Ű
        case HU_IACU: // Í
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case HU_0: //0
        case HU_1 ... HU_9:
        case KC_BSPC:
        case KC_DEL:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

/* 
 * Handle LEDs
 */

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // CAPS LOCK: red indicator
    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t i = led_min; i < led_max; i++) {
            if (g_led_config.flags[i] & LED_FLAG_UNDERGLOW) {
                rgb_matrix_set_color(i, RGB_RED);
            }
        }
        rgb_matrix_set_color(30, RGB_RED); //caps lock
        rgb_matrix_set_color(44, RGB_RED); //l shift
    }
    return false;
}

void housekeeping_task_user(void) {
    if (is_caps_word_on())
    {
        rgb_matrix_set_color_all(RGB_ORANGE);
    }
    
    // Disable some LEDs
    rgb_matrix_set_color(0,  RGB_OFF); // esc
    rgb_matrix_set_color(13, RGB_OFF); // backspace
    rgb_matrix_set_color(28, RGB_OFF); // enter
    rgb_matrix_set_color(60, RGB_OFF); // win l
    rgb_matrix_set_color(61, RGB_OFF); // alt l
    rgb_matrix_set_color(62, RGB_OFF); // space
    rgb_matrix_set_color(63, RGB_OFF); // alt r
    rgb_matrix_set_color(64, RGB_OFF); // fn

}
