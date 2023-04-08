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
[_BL] = LAYOUT(
    TD(TD_GRV_ESC), KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,
    KC_TAB,         KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_ENT,   KC_DEL,
    KC_CAPS,        KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,            KC_PGUP,
    KC_LSFT,        KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
    KC_LCTL,        KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT),

    /* Keymap _FL: Function Layer
     */
[_FL] = LAYOUT(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,    _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR,  KC_SCRL,  KC_PAUS,   _______,  _______,
    _______,  _______,  RGB_SAI,  RGB_SAD,  RGB_TOG,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             KC_HOME,
    _______,  _______,  RGB_HUI,  RGB_HUD,  RGB_SPD,  RGB_SPI,  KC_MUTE,  KC_VOLU,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  _______,   RGB_VAI,  KC_END,
    _______,  _______,  _______,                                QK_BOOT,                                _______,  _______,  RGB_RMOD,  RGB_VAD,  RGB_MOD) 
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Grave, twice for ESC
    [TD_GRV_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, KC_ESC)
};

// Caps Word configuration
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_SLSH:
        case KC_0: // Ö
        case KC_MINS: // Ü
        case KC_EQL: // Ó
        case KC_LBRC: // Ő
        case KC_RBRC: // Ú
        case KC_SCLN: // É
        case KC_QUOT: // Á
        case KC_NUHS: // Ű
        case KC_NUBS: // Í
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_9:
        case KC_GRV: //0
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
