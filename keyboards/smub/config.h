/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H


#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    QMK
#define PRODUCT         QMK
#define DESCRIPTION     QMK Keyboard

#define RGBLED_NUM 8
#define RGB_DI_PIN D7

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 2

#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_LIMIT_VAL 10

#define KEY1_PIN D1
#define KEY2_PIN D2

#define DEBOUNCING_DELAY 5
#define FORCE_NKRO

#define NO_USB_STARTUP_CHECK

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)




#endif
