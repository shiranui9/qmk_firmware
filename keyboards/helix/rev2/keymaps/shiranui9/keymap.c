#include "helix.h"
#include "bootloader.h"
#include "action_layer.h"
#include "eeconfig.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#include "LUFA/Drivers/Peripheral/TWI.h"
#ifdef AUDIO_ENABLE
#include "audio.h"
#endif
#ifdef SSD1306OLED
#include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number
{
  _QWERTY = 0,
  _LOWER,
  _FnL1,
  _FnR1,
  _FnL2,
  _FnR2,
  _FnL3,
  _FnR3,
  _FnL4,
  _FnR4
};

enum custom_keycodes
{
  QWERTY = SAFE_RANGE,
  FnL1,
  FnR1,
  FnL2,
  FnR2,
  FnL3,
  FnR3,
  FnL4,
  FnR4,
  EISU,
  KANA,
  ZERO2,
  RGBRST
};

enum macro_keycodes
{
  KC_SAMPLEMACRO,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
//Macros
#define M_SAMPLE M(KC_SAMPLEMACRO)

#if HELIX_ROWS == 5
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | ESC  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  |   -  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Tab  |   q  |   w  |   e  |   r  |   t  |             |   y  |   u  |   i  |   o  |   p  |   @  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Eisu |   a  |   s  |   d  |   f  |   g  |             |   h  |   j  |   k  |   l  |   ;  |   :  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   z  |   x  |   c  |   v  |   b  | FnL3 | FnR3 |   n  |   m  |   ,  |   .  |   /  |   _  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | FnL4 | Ctrl |  Win |  Alt | FnL1 | Space| FnL2 | FnR2 | Space| FnR1 | KANA |  Alt |  App | FnR4 |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_QWERTY] = KEYMAP( \
       KC_ESC,    KC_1,    KC_2,    KC_3,      KC_4,   KC_5,                           KC_6,       KC_7,     KC_8,    KC_9,    KC_0,   KC_MINS, \
       KC_TAB,    KC_Q,    KC_W,    KC_E,      KC_R,   KC_T,                           KC_Y,       KC_U,     KC_I,    KC_O,    KC_P,   KC_BSLS, \
     KC_LANG2,    KC_A,    KC_S,    KC_D,      KC_F,   KC_G,                           KC_H,       KC_J,     KC_K,    KC_L, KC_SCLN,   KC_RCTL, \
      KC_LSFT,    KC_Z,    KC_X,    KC_C,      KC_V,   KC_B,  MO(_FnL3),  MO(_FnR3),   KC_N,       KC_M,  KC_COMM,  KC_DOT, KC_SLSH,   KC_UNDS, \
    MO(_FnL4), KC_LCTL, KC_LWIN, KC_LALT, MO(_FnL1), KC_SPC,  MO(_FnL2),  MO(_FnR2), KC_SPC,  MO(_FnR1), KC_LANG1, KC_RALT,  KC_APP, MO(_FnR4)  \
  ),

/* FnL1(Arrow or Ctrl)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  ESC |  ^1  |  ^2  |  ^3  |  ^4  |  ^5  |             |      |      |   (  |   )  |  Ins |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | ^Tab |  ^q  |  ^w  |  ^e  |  ^r  |  ^t  |             |  BS  | ^Home|  Up  | PgUp |  Del |  ^@  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Caps |  ^a  |  ^s  |  ^d  |  ^f  |  ^g  |             | Home | Left | Down | Right|  End |  ^:  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|  ^z  |  ^x  |  ^c  |  ^v  |  ^b  |      |      | Enter| ^End |  App |PgDown|  ^/  |  ^\  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      | TRNS | Space| Shift| Shift| Space| Ctrl |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnL1] = KEYMAP( \
          KC_ESC, LCTL(KC_1), LCTL(KC_2), LCTL(KC_3), LCTL(KC_4), LCTL(KC_5),                   XXXXXXX,       XXXXXXX, KC_LPRN,  KC_RPRN,        KC_INS,       XXXXXXX, \
    LCTL(KC_TAB), LCTL(KC_Q), LCTL(KC_W), LCTL(KC_E), LCTL(KC_R), LCTL(KC_T),                   KC_BSPC, LCTL(KC_HOME),   KC_UP,  KC_PGUP,        KC_DEL,   LCTL(KC_AT), \
         KC_CAPS, LCTL(KC_A), LCTL(KC_S), LCTL(KC_D), LCTL(KC_F), LCTL(KC_G),                   KC_HOME,       KC_LEFT, KC_DOWN, KC_RIGHT,        KC_END, LCTL(KC_COLN), \
         KC_LSFT, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_B), XXXXXXX, XXXXXXX,  KC_ENT,  LCTL(KC_END),  KC_APP,  KC_PGDN, LCTL(KC_SLSH), LCTL(KC_BSLS), \
         XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,     KC_SPC, KC_LSFT, KC_RSFT,  KC_SPC,       KC_RCTL, XXXXXXX,  XXXXXXX,       XXXXXXX,       XXXXXXX  \
  ),

/* FnR1(+Ctrl)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  ESC |  ^1  |  ^2  |  ^3  |  ^4  |  ^5  |             |  ^6  |  ^7  |  ^8  |  ^9  |  ^0  |  ^-  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | ^Tab |  ^q  |  ^w  |  ^e  |  ^r  |  ^t  |             |  ^y  |  ^u  |  ^i  |  ^o  |  ^p  |  ^@  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Caps |  ^a  |  ^s  |  ^d  |  ^f  |  ^g  |             |  ^h  |  ^j  |  ^k  |  ^l  |  ^;  |  ^:  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|  ^z  |  ^x  |  ^c  |  ^v  |  ^b  |      |      |  ^n  |  ^m  |  ^,  |  ^.  |  ^/  |  ^\  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      | Space| Shift| Shift| Space| TRNS |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnR1] = KEYMAP( \
          KC_ESC, LCTL(KC_1), LCTL(KC_2), LCTL(KC_3), LCTL(KC_4), LCTL(KC_5),                   LCTL(KC_6), LCTL(KC_7),    LCTL(KC_8),    LCTL(KC_9),    LCTL(KC_0), LCTL(KC_MINS), \
    LCTL(KC_TAB), LCTL(KC_Q), LCTL(KC_W), LCTL(KC_E), LCTL(KC_R), LCTL(KC_T),                   LCTL(KC_Y), LCTL(KC_U),    LCTL(KC_I),    LCTL(KC_O),    LCTL(KC_P),   LCTL(KC_AT), \
         KC_CAPS, LCTL(KC_A), LCTL(KC_S), LCTL(KC_D), LCTL(KC_F), LCTL(KC_G),                   LCTL(KC_H), LCTL(KC_J),    LCTL(KC_K),    LCTL(KC_L), LCTL(KC_SCLN), LCTL(KC_COLN), \
         KC_LSFT, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_B), XXXXXXX, XXXXXXX, LCTL(KC_N), LCTL(KC_M), LCTL(KC_COMM),  LCTL(KC_DOT), LCTL(KC_SLSH), LCTL(KC_BSLS), \
         XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,     KC_SPC, KC_LSFT, KC_RSFT,  KC_SPC,       _______,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX  \
  ),

/* FnL2(Shift)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  ESC |   !  |   "  |   #  |   $  |   %  |             |   &  |   '  |   (  |   )  |      |   =  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | +Tab |  +Q  |  +W  |  +E  |  +R  |  +T  |             |  +Y  |  +U  |  +I  |  +O  |  +P  |   `  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Caps |  +A  |  +S  |  +D  |  +F  |  +G  |             |  +H  |  +J  |  +K  |  +L  |   +  |   *  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|  +Z  |  +X  |  +C  |  +V  |  +B  |      |      |  +N  |  +M  |   <  |   >  |   ?  |   \  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      | TRNS |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnL2] = KEYMAP( \
       KC_ESC, KC_EXLM, KC_DQUO, KC_HASH,  KC_DLR, KC_PERC,                   KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, XXXXXXX,  KC_EQL, \
    S(KC_TAB), S(KC_Q), S(KC_W), S(KC_E), S(KC_R), S(KC_T),                   S(KC_Y), S(KC_U), S(KC_I), S(KC_O), S(KC_P),  KC_GRV, \
      KC_CAPS, S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G),                   S(KC_H), S(KC_J), S(KC_K), S(KC_L), KC_PLUS, KC_ASTR, \
      KC_LSFT, S(KC_Z), S(KC_X), S(KC_C), S(KC_V), S(KC_B), XXXXXXX, XXXXXXX, S(KC_N), S(KC_M),   KC_LT,   KC_GT, KC_QUES,   KC_RO, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
  ),

/* FnR2(Mouse or Shift)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  ESC |      |      |      |      |      |             |   &  |   '  |   (  |   )  |      |   =  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | +Tab |      |      | M-Up | Up-W |      |             |  +Y  |  +U  |  +I  |  +O  |  +P  |   `  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Caps |Left-W|M-Left|M-Down|M-Rigt|Rigt-W|             |  +H  |  +J  |  +K  |  +L  |   +  |   *  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|Accel2|Accel1|Accel0|Down-W|Clik-M|      |      |  +N  |  +M  |   <  |   >  |   ?  |   \  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |Clik-4|Clik-L|Clik-R|Clik-5| TRNS |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnR2] = KEYMAP( \
       KC_ESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, XXXXXXX,  KC_EQL, \
    S(KC_TAB), XXXXXXX, XXXXXXX, KC_MS_U, KC_WH_U, XXXXXXX,                   S(KC_Y), S(KC_U), S(KC_I), S(KC_O), S(KC_P),  KC_GRV, \
      KC_CAPS, KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R,                   S(KC_H), S(KC_J), S(KC_K), S(KC_L), KC_PLUS, KC_ASTR, \
      KC_LSFT, KC_ACL2, KC_ACL1, KC_ACL0, KC_WH_D, KC_BTN3, XXXXXXX, XXXXXXX, S(KC_N), S(KC_M),   KC_LT,   KC_GT, KC_QUES,   KC_RO, \
      XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN4, KC_BTN1, KC_BTN2, KC_BTN5, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
  ),

/* FnL3(Brackets)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  ESC |      |      |      |      |      |             |      |      |   (  |   )  |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   ~  |   ^  |   {  |   }  |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   |  |   \  |   [  |   ]  |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      | TRNS |      |      |      |   <  |   >  |   ?  |   \  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnL3] = KEYMAP( \
       KC_ESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, KC_LPRN, KC_RPRN, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_TILD, KC_CIRC, KC_LCBR, KC_RCBR, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_PIPE, KC_JYEN, KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_LABK, KC_RABK, KC_QUES,   KC_RO, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
  ),

/* FnR3(Fxx)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  ESC |  F1  |  F2  |  F3  |  F4  |  F5  |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  F21 |  F1  |  F2  |  F3  |  F4  |  F5  |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  F22 |  F6  |  F7  |  F8  |  F9  |  F10 |             |  Ins | ScrLk|      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |  F23 |  F11 |  F12 |  F13 |  F14 |  F15 |      | TRNS | Pause|      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |  F24 |  F16 |  F17 |  F18 |  F19 |  F20 |      | PtScr|      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnR3] = KEYMAP( \
      KC_ESC,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      KC_F21,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      KC_F22,  KC_F6,  KC_F7,  KC_F8,  KC_F9, KC_F10,                    KC_INS, KC_SLCK, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      KC_F23, KC_F11, KC_F12, KC_F13, KC_F14, KC_F15, XXXXXXX, _______, KC_PAUS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      KC_F24, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, XXXXXXX, KC_PSCR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
  ),

/* FnL4(TenKey)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |      |             |      |  Num |   /  |   *  |   -  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |  BS  |   7  |   8  |   9  |   *  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |   4  |   5  |   6  |   +  |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      | Enter|   1  |   2  |   3  |   /  |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | TRNS |      |      |      |      |      |      |      |      |   0  |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnL4] = KEYMAP( \
       KC_ESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_LNUM, KC_PSLS, KC_PAST, KC_PMNS, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_BSPC, KC_KP_7, KC_KP_8, KC_KP_9, KC_PAST, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PENT, KC_KP_1, KC_KP_2, KC_KP_3, KC_PSLS, XXXXXXX, \
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_KP_0, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
  ),

/* FnR4(RGB)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      | Reset|             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      | Mute |RGBRST|             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | VAL+ | SAT+ | HUE+ | Vol+ |RGB md|             |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      | VAL- | SAT- | HUE- | Vol- |RGBTOG|      |      |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      | TRNS |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_FnR4] = KEYMAP( \
       KC_ESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  RESET ,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE,  RGBRST,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      XXXXXXX, RGB_VAI, RGB_SAI, RGB_HUI, KC_VOLU, RGB_MOD,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      XXXXXXX, RGB_VAD, RGB_SAD, RGB_HUD, KC_VOLD, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______  \
  )
};

#else
#error "undefined keymaps"
#endif

#ifdef AUDIO_ENABLE

float tone_qwerty[][2] = SONG(QWERTY_SOUND);
float tone_plover[][2] = SONG(PLOVER_SOUND);
float tone_plover_gb[][2] = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2] = SONG(MUSIC_SCALE_SOUND);
#endif

static int current_default_layer;

uint32_t default_layer_state_set_kb(uint32_t state) {
  // 1<<_QWERTY  - 1 == 1 - 1 == _QWERTY (=0)
  current_default_layer = state - 1;
  if (current_default_layer == 3)
    current_default_layer -= 1;
  if (current_default_layer == 7)
    current_default_layer -= 4;
  return state;
}

void update_base_layer(int base)
{
  //if (current_default_layer != base) {
    //eeconfig_update_default_layer(1UL << base);
    //default_layer_set(1UL << base);
  //}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        update_base_layer(_QWERTY);
      }
      return false;
      break;
    case FnL1:
      if (record->event.pressed) {
        update_base_layer(_FnL1);
      }
      return false;
      break;
    case FnR1:
      if (record->event.pressed) {
        update_base_layer(_FnR1);
      }
      return false;
      break;
    case FnL2:
      if (record->event.pressed) {
        update_base_layer(_FnL2);
      }
      return false;
      break;
    case FnR2:
      if (record->event.pressed) {
        update_base_layer(_FnR2);
      }
      return false;
      break;
    case FnL3:
      if (record->event.pressed) {
        update_base_layer(_FnL3);
      }
      return false;
      break;
    case FnR3:
      if (record->event.pressed) {
        update_base_layer(_FnR3);
      }
      return false;
      break;
    case FnL4:
      if (record->event.pressed) {
        update_base_layer(_FnL4);
      }
      return false;
      break;
    case FnR4:
      if (record->event.pressed) {
        update_base_layer(_FnR4);
      }
      return false;
      break;
    case ZERO2:
      if (record->event.pressed) {
        SEND_STRING("00");
      }
      return false;
      break;
    case EISU:
      if (record->event.pressed) {
        if (keymap_config.swap_lalt_lgui == false) {
          register_code(KC_LANG2);
        } else {
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG2);
      }
      return false;
      break;
    case KANA:
      if (record->event.pressed) {
        if (keymap_config.swap_lalt_lgui == false) {
          register_code(KC_LANG1);
        } else {
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case RGBRST:
#ifdef RGBLIGHT_ENABLE
    if (record->event.pressed) {
      eeconfig_update_rgblight_default();
      rgblight_enable();
    }
#endif
    break;
  }
  return true;
}

void matrix_init_user(void) {
#ifdef AUDIO_ENABLE
  startup_user();
#endif
//SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED
  TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
  iota_gfx_init(!has_usb()); // turns on the display
#endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
  iota_gfx_task(); // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                   const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

static void render_logo(struct CharacterMatrix *matrix) {
  static char logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
      0};
  matrix_write(matrix, logo);
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_ANIMATIONS)
  char buf[30];
  if (rgblight_config.enable)
  {
    snprintf(buf, sizeof(buf), " LED %2d: %d,%d,%d ",
             rgblight_config.mode,
             rgblight_config.hue / RGBLIGHT_HUE_STEP,
             rgblight_config.sat / RGBLIGHT_SAT_STEP,
             rgblight_config.val / RGBLIGHT_VAL_STEP);
    matrix_write(matrix, buf);
  }
#endif
  //matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}

static const char Qwerty_name[] PROGMEM = " Qwerty";

static const char FnL1_name[] PROGMEM = ":Arrow";
static const char FnR1_name[] PROGMEM = ":+Ctrl";
static const char FnL2_name[] PROGMEM = ":+Shift";
static const char FnR2_name[] PROGMEM = ":Mouse";
static const char FnL3_name[] PROGMEM = ":Brackets";
static const char FnR3_name[] PROGMEM = ":Fxx,PtScr,etc";
static const char FnL4_name[] PROGMEM = ":TenKey";
static const char FnR4_name[] PROGMEM = ":VOL,LED RGB";

static const char *layer_names[] = {
    [_QWERTY] = Qwerty_name,
    [_FnL1] = FnL1_name,
    [_FnR1] = FnR1_name,
    [_FnL2] = FnL2_name,
    [_FnR2] = FnR2_name,
    [_FnL3] = FnL3_name,
    [_FnR3] = FnR3_name,
    [_FnL4] = FnL4_name,
    [_FnR4] = FnR4_name
};


void render_status(struct CharacterMatrix *matrix) {
  // Render to mode icon
  static char logo[][2][3] = {{{0x95, 0x96, 0}, {0xb5, 0xb6, 0}}, {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}}};
  if (keymap_config.swap_lalt_lgui == false) {
    matrix_write(matrix, logo[0][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[0][1]);
  } else {
    matrix_write(matrix, logo[1][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[1][1]);
  }
  // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
  int name_num;
  uint32_t lstate;
  matrix_write_P(matrix, layer_names[current_default_layer]);
  matrix_write_P(matrix, PSTR("\n"));
  for (lstate = layer_state, name_num = 0;
       lstate && name_num < sizeof(layer_names) / sizeof(char *);
       lstate >>= 1, name_num++) {
    if ((lstate & 1) != 0) {
      if (layer_names[name_num]) {
        matrix_write_P(matrix, layer_names[name_num]);
      }
    }
  }

  // Host Keyboard LED Status
  char led[40];
  snprintf(led, sizeof(led), "\n%s  %s  %s",
           (host_keyboard_leds() & (1 << USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
           (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
           (host_keyboard_leds() & (1 << USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
  matrix_write(matrix, led);
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable)
  {
    return;
  }
#endif

  matrix_clear(&matrix);
  if (is_master)
  {
    render_status(&matrix);
  }
  else
  {
    render_logo(&matrix);
  }
  matrix_update(&display, &matrix);
}

#endif
