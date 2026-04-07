// Copyright 2024 Rafael Veggi
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// ─── Layers ──────────────────────────────────────────────────────────────────

enum layers { BASE, FN };

// ─── Custom tap-hold keycodes ────────────────────────────────────────────────
//
// Notation used in docs: tap / hold (400 ms)
//
// Row 0:  q/!  w/@  e/#  r/$  t/%  y/^  u/&  i/*  o/(  p/)
// Row 1:  a/1  s/2  d/3  f/4  g/5  h/6  j/7  k/8  l/9  ;/0
// Row 2:  z/^Z x/_  c/-  v/+  b/=  n/}  m/{  ,/[  ./]  //^/
// Thumb:  Shift+Tab / Ctrl  (right outer)

enum custom_keycodes {
    CTH_Q = SAFE_RANGE,
    CTH_W,   CTH_E,    CTH_R,    CTH_T,
    CTH_Y,   CTH_U,    CTH_I,    CTH_O,    CTH_P,
    CTH_A,   CTH_S,    CTH_D,    CTH_F,    CTH_G,
    CTH_H,   CTH_J,    CTH_K,    CTH_L,    CTH_SCLN,
    CTH_Z,   CTH_X,    CTH_C,    CTH_V,    CTH_B,
    CTH_N,   CTH_M,    CTH_COMM, CTH_DOT,  CTH_SLSH,
    CTH_STAB,   // tap = Shift+Tab, hold = Ctrl
    CTH_COUNT
};

#define NUM_CTH (CTH_COUNT - CTH_Q)

// Tap keycodes — indexed by (keycode - CTH_Q)
const uint16_t PROGMEM cth_tap_kc[NUM_CTH] = {
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      // row 0, left
    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,      // row 0, right
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      // row 1, left
    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,   // row 1, right
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      // row 2, left
    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   // row 2, right
    S(KC_TAB),                                      // CTH_STAB
};

// Hold keycodes — same order
const uint16_t PROGMEM cth_hold_kc[NUM_CTH] = {
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,  // row 0, left
    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  // row 0, right
    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      // row 1, left
    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,      // row 1, right
    C(KC_Z), KC_UNDS, KC_MINS, KC_PLUS, KC_EQL,   // row 2, left
    KC_RCBR, KC_LCBR, KC_LBRC, KC_RBRC, C(KC_SLSH), // row 2, right
    KC_LCTL,                                        // CTH_STAB
};

// ─── Tap-hold runtime state ───────────────────────────────────────────────────

static uint16_t cth_timer[NUM_CTH];
static uint16_t cth_last_release[NUM_CTH];
static bool     cth_pressed[NUM_CTH];
static bool     cth_hold_fired[NUM_CTH];
static bool     cth_is_quick_tap[NUM_CTH];
static bool     cth_ever_released[NUM_CTH];

// If the same key is re-pressed within this window after release, a subsequent
// hold repeats the *tap* keycode instead of activating the hold keycode.
// This gives the tap-tap-hold-to-repeat behaviour.
#define CTH_QUICK_TAP_TERM 120

// CTH_STAB activates Ctrl immediately once another key is pressed, provided
// it has been held for at least this many ms (guards against roll-over typos).
#define STAB_INTERRUPT_TERM 50

// ─── Combos ──────────────────────────────────────────────────────────────────

enum combo_events {
    // Arrows — right hand, vertical (home + top row, same column)
    CMB_LEFT, CMB_DOWN, CMB_UP, CMB_RIGHT,
    // Word / line jump — right hand, vertical (home + bottom row)
    CMB_WLEFT, CMB_SFTDN, CMB_SFTUP, CMB_WRIGHT,
    // Home / PgUp / PgDn / End — left hand, vertical (home + top row)
    CMB_HOME, CMB_PGUP, CMB_PGDN, CMB_END,
    // Extended — left hand, vertical (home + bottom row)
    CMB_SFTHOME, CMB_CTPGUP, CMB_CTPGDN, CMB_SFTEND,
    // Misc
    CMB_PSCR,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH;

// Right hand — home + top (arrows)
const uint16_t PROGMEM cmb_hy[]   = {CTH_H, CTH_Y,    COMBO_END};
const uint16_t PROGMEM cmb_ju[]   = {CTH_J, CTH_U,    COMBO_END};
const uint16_t PROGMEM cmb_ki[]   = {CTH_K, CTH_I,    COMBO_END};
const uint16_t PROGMEM cmb_lo[]   = {CTH_L, CTH_O,    COMBO_END};

// Right hand — home + bottom (word/sel)
const uint16_t PROGMEM cmb_hn[]   = {CTH_H, CTH_N,    COMBO_END};
const uint16_t PROGMEM cmb_jm[]   = {CTH_J, CTH_M,    COMBO_END};
const uint16_t PROGMEM cmb_kc[]   = {CTH_K, CTH_COMM, COMBO_END};
const uint16_t PROGMEM cmb_ld[]   = {CTH_L, CTH_DOT,  COMBO_END};

// Left hand — home + top (page nav)
const uint16_t PROGMEM cmb_sw[]   = {CTH_S, CTH_W,    COMBO_END};
const uint16_t PROGMEM cmb_de[]   = {CTH_D, CTH_E,    COMBO_END};
const uint16_t PROGMEM cmb_fr[]   = {CTH_F, CTH_R,    COMBO_END};
const uint16_t PROGMEM cmb_gt[]   = {CTH_G, CTH_T,    COMBO_END};

// Left hand — home + bottom (extended)
const uint16_t PROGMEM cmb_sx[]   = {CTH_S, CTH_X,    COMBO_END};
const uint16_t PROGMEM cmb_dc[]   = {CTH_D, CTH_C,    COMBO_END};
const uint16_t PROGMEM cmb_fv[]   = {CTH_F, CTH_V,    COMBO_END};
const uint16_t PROGMEM cmb_gb[]   = {CTH_G, CTH_B,    COMBO_END};

// PrintScreen — right pinky column, top + home
const uint16_t PROGMEM cmb_pscr[] = {CTH_P, LGUI_T(KC_QUOT), COMBO_END};

combo_t key_combos[] = {
    [CMB_LEFT]    = COMBO(cmb_hy,   KC_LEFT),
    [CMB_DOWN]    = COMBO(cmb_ju,   KC_DOWN),
    [CMB_UP]      = COMBO(cmb_ki,   KC_UP),
    [CMB_RIGHT]   = COMBO(cmb_lo,   KC_RIGHT),
    [CMB_WLEFT]   = COMBO(cmb_hn,   C(KC_LEFT)),
    [CMB_SFTDN]   = COMBO(cmb_jm,   S(KC_DOWN)),
    [CMB_SFTUP]   = COMBO(cmb_kc,   S(KC_UP)),
    [CMB_WRIGHT]  = COMBO(cmb_ld,   C(KC_RIGHT)),
    [CMB_HOME]    = COMBO(cmb_sw,   KC_HOME),
    [CMB_PGUP]    = COMBO(cmb_de,   KC_PGUP),
    [CMB_PGDN]    = COMBO(cmb_fr,   KC_PGDN),
    [CMB_END]     = COMBO(cmb_gt,   KC_END),
    [CMB_SFTHOME] = COMBO(cmb_sx,   S(KC_HOME)),
    [CMB_CTPGUP]  = COMBO(cmb_dc,   C(KC_PGUP)),
    [CMB_CTPGDN]  = COMBO(cmb_fv,   C(KC_PGDN)),
    [CMB_SFTEND]  = COMBO(cmb_gb,   S(KC_END)),
    [CMB_PSCR]    = COMBO(cmb_pscr, KC_PSCR),
};

// ─── Keymaps ─────────────────────────────────────────────────────────────────

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_split_3x6_3(
    //  `~ / Fn             q / !      w / @      e / #      r / $      t / %  ////     y / ^      u / &      i / *         o / (         p / )     \ | / Fn    
    LT(FN, KC_GRV), CTH_Q, CTH_W, CTH_E, CTH_R, CTH_T, /**/ CTH_Y, CTH_U, CTH_I,    CTH_O,    CTH_P,    LT(FN, KC_BSLS),
    //  Esc / Ctrl          a / 1      s / 2      d / 3      f / 4      g / 5  ////     h / 6      j / 7      k / 8         l / 9         ; / 0     '" / Win
    LCTL_T(KC_ESC), CTH_A, CTH_S, CTH_D, CTH_F, CTH_G, /**/ CTH_H, CTH_J, CTH_K,    CTH_L,    CTH_SCLN, LGUI_T(KC_QUOT),        
    //  Shift               z / ^Z     x / _      c / -      v / +      b / =  ////     n / }      m / {      , / [         . / ]         / / ^/    Shift
    KC_LSFT,        CTH_Z, CTH_X, CTH_C, CTH_V, CTH_B, /**/ CTH_N, CTH_M, CTH_COMM, CTH_DOT,  CTH_SLSH, KC_RSFT,
    //                        Tab/Ctrl            Space       Enter/Alt        ////     Del/AltGr           Bksp         Shift+Tab / Ctrl
                          LCTL_T(KC_TAB), KC_SPC, LALT_T(KC_ENT),  /**/ RALT_T(KC_DEL), KC_BSPC, CTH_STAB
    ),

    [FN] = LAYOUT_split_3x6_3(
    //  (Fn key)   F11       F12       ---        ---       ---    //  CapsLk     Ins       NumLk     Pause      ScLk     (Fn key)
        _______, KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,        KC_CAPS, KC_INS,  KC_NUM,  KC_PAUSE, KC_SCRL, _______,
    //  Esc/Ctrl   F1        F2        F3        F4        F5 //  F6         F7        F8        F9        F10       Win
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_LGUI,
    //  Shift      ---       Play      Stop      Prev      Next //  Mute       Vol-      Vol+      Bri-      Bri+      Shift
        _______, XXXXXXX, KC_MPLY, KC_MSTP, KC_MPRV, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, _______,
                                     _______, _______, _______,         _______, _______, _______
    ),
};

// ─── process_record_user ─────────────────────────────────────────────────────

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t stab_idx = CTH_STAB - CTH_Q;

    // CTH_STAB: activate Ctrl immediately when another key is pressed,
    // provided the thumb has been held long enough to rule out a roll-over.
    if (record->event.pressed && keycode != CTH_STAB) {
        if (cth_pressed[stab_idx] && !cth_hold_fired[stab_idx] &&
            timer_elapsed(cth_timer[stab_idx]) >= STAB_INTERRUPT_TERM) {
            cth_hold_fired[stab_idx]   = true;
            cth_is_quick_tap[stab_idx] = false;
            register_code(KC_LCTL);
        }
    }

    if (keycode >= CTH_Q && keycode < CTH_COUNT) {
        uint8_t idx = keycode - CTH_Q;

        if (record->event.pressed) {
            // Quick-tap detection: same key re-pressed shortly after release
            // → subsequent hold repeats the tap keycode rather than the hold keycode.
            cth_is_quick_tap[idx] = cth_ever_released[idx] &&
                                    timer_elapsed(cth_last_release[idx]) < CTH_QUICK_TAP_TERM;
            cth_timer[idx]      = timer_read();
            cth_pressed[idx]    = true;
            cth_hold_fired[idx] = false;

        } else {
            // Guard: if pressed was never recorded (key was absorbed by a combo)
            // there is nothing to send or release.
            if (!cth_pressed[idx]) return false;

            cth_pressed[idx] = false;

            if (!cth_hold_fired[idx]) {
                // Released before hold threshold → send tap keycode
                tap_code16(pgm_read_word(&cth_tap_kc[idx]));
            } else {
                // Hold was active → release whichever key was registered
                unregister_code16(
                    cth_is_quick_tap[idx]
                        ? pgm_read_word(&cth_tap_kc[idx])
                        : pgm_read_word(&cth_hold_kc[idx])
                );
            }

            cth_ever_released[idx] = true;
            cth_last_release[idx]  = timer_read();
            cth_hold_fired[idx]    = false;
        }
        return false;
    }

    return true;
}

// ─── matrix_scan_user ────────────────────────────────────────────────────────

void matrix_scan_user(void) {
    for (uint8_t idx = 0; idx < NUM_CTH; idx++) {
        if (cth_pressed[idx] && !cth_hold_fired[idx] &&
            timer_elapsed(cth_timer[idx]) >= TAPPING_TERM) {

            cth_hold_fired[idx] = true;
            register_code16(
                cth_is_quick_tap[idx]
                    ? pgm_read_word(&cth_tap_kc[idx])
                    : pgm_read_word(&cth_hold_kc[idx])
            );
        }
    }
}
