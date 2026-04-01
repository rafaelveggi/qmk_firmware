// Copyright 2024 Rafael Veggi
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Tap Dance declarations
enum td_keycodes {
    TD_HOME_END,
};

void td_home_end_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_HOME);
    } else {
        register_code(KC_END);
    }
}

void td_home_end_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code(KC_HOME);
    } else {
        unregister_code(KC_END);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_HOME_END] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_home_end_finished, td_home_end_reset),
};

#define TD_HE TD(TD_HOME_END)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,  KC_UP,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M,    TD_HE, KC_LEFT, KC_DOWN, KC_RGHT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_ESC,  KC_SPC,  OSL(1),     OSL(1),  KC_DEL, KC_LALT
                                      //`--------------------------'  `--------------------------'
    ),

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_EQL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_MINS, KC_UNDS, KC_LCBR, KC_RCBR, KC_PIPE,                      KC_BSLS, KC_SLSH, KC_COMM,  KC_DOT, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_ESC,  KC_SPC, _______,    _______,  KC_DEL, KC_LALT
                                      //`--------------------------'  `--------------------------'
    ),
};
