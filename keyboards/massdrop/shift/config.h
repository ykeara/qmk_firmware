/*
Copyright 2015 Jun Wako <wakojun@gmail.com>

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

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID           0x04D8
#define PRODUCT_ID          0xEE65
#define DEVICE_VER          0x0100

#define MANUFACTURER        "Massdrop Inc."
#define PRODUCT             "Massdrop SHIFT Keyboard"
#define SERIAL_NUM          "Unavailable"

/* key matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 13

/* MCU Port name definitions */
#define PA 0
#define PB 1

/* Shift register IDs for key matrix */
/* SRBASE must be defined greater than MCU Port name definitions */
#define SRBASE 10
#define SRMAX (SRBASE + SERCOM_INST_NUM)
#define S4 (SRBASE + 4)

/* Macros to differentiate between MCU and Shift Register usage */
#define PORT_IS_MCU(x) (x < PORT_GROUPS)
#define PORT_IS_SR(x) (x >= SRBASE && x < SRMAX)

/* Port and Pin definition of key row hardware configuration */
#define MATRIX_ROW_PORTS PA, PA, PA, PA, PA, PA, PA, PA
#define MATRIX_ROW_PINS   0,  1,  2,  3,  4,  5,  6,  7

/* Port and Pin definition of key column hardware configuration */
#define MATRIX_COL_PORTS S4, S4, S4, S4, S4, S4, S4, S4, S4, S4, S4, S4, S4
#define MATRIX_COL_PINS   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12

/* This Shift Register expands available hardware output lines to control additional peripherals */
/* It uses four lines from the MCU to provide 16 output lines */
/* Shift Register Clock configuration (MCU to ShiftRegister.RCLK) */
#define SR_EXP_RCLK_PORT            PB
#define SR_EXP_RCLK_PIN             14
/* Shift Register Output Enable configuration (MCU to ShiftRegister.OE_N) */
#define SR_EXP_OE_N_PORT            PB
#define SR_EXP_OE_N_PIN             15
/* SERCOM port to use for Shift Register SPI */
/* DATAOUT and SCLK must be configured to use hardware pins of this port */
#define SR_EXP_SERCOM_NUM           2
/* Shift Register SPI Data Out configuration (MCU.SERCOMx.PAD[0] to ShiftRegister.SER) */
#define SR_EXP_DATAOUT_PORT         PA
#define SR_EXP_DATAOUT_PIN          12
#define SR_EXP_DATAOUT_MUX          2
/* Shift Register SPI Serial Clock configuration (MCU.SERCOMx.PAD[1] to ShiftRegister.SRCLK) */
#define SR_EXP_SCLK_PORT            PA
#define SR_EXP_SCLK_PIN             13
#define SR_EXP_SCLK_MUX             2

/* This Shift Register is used as the Key Matrix Column Driver */
/* It uses four lines from the MCU to provide 16 output lines */
/* Shift Register Clock configuration (MCU to ShiftRegister.RCLK) */
#define SR_KC_RCLK_PORT             PB
#define SR_KC_RCLK_PIN              6
/* Shift Register Output Enable configuration (MCU to ShiftRegister.OE_N) */
#define SR_KC_OE_N_PORT             PB
#define SR_KC_OE_N_PIN              7
/* SERCOM port to use for Shift Register SPI */
/* DATAOUT and SCLK must be configured to use hardware pins of this port */
#define SR_KC_SERCOM_NUM            4
/* Shift Register SPI Data Out configuration (MCU.SERCOMx.PAD[0] to ShiftRegister.SER) */
#define SR_KC_DATAOUT_PORT          PB
#define SR_KC_DATAOUT_PIN           8
#define SR_KC_DATAOUT_MUX           3
/* Shift Register SPI Serial Clock configuration (MCU.SERCOMx.PAD[1] to ShiftRegister.SRCLK) */
#define SR_KC_SCLK_PORT             PB
#define SR_KC_SCLK_PIN              9
#define SR_KC_SCLK_MUX              3

/* Debug LED (Small LED Located near MCU) */
/* PCB M15 */
#define DEBUG_LED_ENABLE            1
#define DEBUG_LED_PORT              PA
#define DEBUG_LED_PIN               27

/* Additional debugging ports */
/* PCB M44 */
#define DEBUG_PORT1_ENABLE          1
#define DEBUG_PORT1_PORT            PB
#define DEBUG_PORT1_PIN             31
/* PCB M45 */
#define DEBUG_PORT2_ENABLE          1
#define DEBUG_PORT2_PORT            PB
#define DEBUG_PORT2_PIN             10
/* PCB M46 */
#define DEBUG_PORT3_ENABLE          1
#define DEBUG_PORT3_PORT            PB
#define DEBUG_PORT3_PIN             11

/* Debug Boot Tracing - During boot sequence, ground this pin to halt and display debug code using Debug LED */
/* This is useful in determining which hardware device may have malfunctioned or is improperly configured */
/* Feature is automatically disabled after successful boot */
/* PCB M27 */
#define DEBUG_BOOT_TRACING_ENABLE   1
#define DEBUG_BOOT_TRACING_PORT     PB
#define DEBUG_BOOT_TRACING_PIN      23

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

// Required BOOT key hold time (in ms) for restarting to bootloader -PS081419
#define BOOTKEY_HOLD_MS		2000

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

/* Force boot in NKRO mode */
//#define FORCE_NKRO

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_LED_PROCESS_LIMIT 15
#define RGB_MATRIX_LED_FLUSH_LIMIT 10
#define RGB_MATRIX_EXTRA_TOG

#include "config_led.h"
