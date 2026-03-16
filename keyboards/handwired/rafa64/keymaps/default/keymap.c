#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_GRAVE        , KC_1   , KC_2   , KC_3   , KC_4   , KC_5  , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL , KC_BSPC,
    KC_TAB          , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T  , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC, KC_RBRC, KC_DEL ,
    LCTL_T(KC_ESC)  , KC_A   , KC_S   , KC_D   , KC_F   , KC_G  , _______, KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT, KC_ENT ,
    SC_LSPO         , KC_NUBS, KC_Z   , KC_X   , KC_C   , KC_V  , _______, KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
    KC_0            , KC_LALT, KC_LGUI, _______, _______, KC_SPC, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RIGHT
  ),
};
