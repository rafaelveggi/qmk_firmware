#include QMK_KEYBOARD_H

enum tap_dance_codes {
    TD_ESC_GRV,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_GRV]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_GRAVE),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    TD(TD_ESC_GRV)  , KC_1   , KC_2   , KC_3   , KC_4   , KC_5  , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL , KC_BSPC,
    KC_TAB          , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T  , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC, KC_RBRC, KC_DEL ,
    MT(MOD_LSFT, KC_HOME), MT(MOD_LGUI, KC_A), MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F), KC_G, _______, KC_H, MT(MOD_RSFT, KC_J), MT(MOD_RCTL, KC_K), MT(MOD_LALT, KC_L), MT(MOD_RGUI, KC_SCLN), KC_QUOT, KC_ENT ,
    MT(MOD_LSFT, KC_END) , KC_Z, KC_X  , KC_C   , KC_V   , KC_B  , _______, _______, KC_N   , KC_M  ,  KC_COMM, KC_DOT , KC_SLSH, MT(MOD_RSFT, KC_UP),
    LALT(KC_TAB)    , KC_LGUI, KC_LALT, _______, _______, KC_SPC, _______, _______, _______, _______, LT(1, KC_BSLS), MT(MOD_RALT, KC_LEFT), MT(MOD_RGUI, KC_DOWN), MT(MOD_RCTL, KC_RIGHT)
  ),
  [1] = LAYOUT(
    _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5 , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),
};
