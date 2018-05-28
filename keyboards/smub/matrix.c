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

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"

#include <string.h>

#include "smub.h"
#include "config.h"

#ifndef DEBOUNCE
#   define DEBOUNCE	0
#endif



#if 0
static uint8_t key_pins[MATRIX_COLS] = KEY_PINS;

//===================
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
//static uint8_t matrix_debounce[MATRIX_COLS];
#endif


#define KEY_PIN(x) _SFR_IO8((x >> 4) + 0)
#define KEY_DDR(x) _SFR_IO8((x >> 4) + 1)
#define KEY_PORT(x) _SFR_IO8((x >> 4) + 2)
#define KEY_BV(x) _BV((x & 0xF))

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_stage[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static uint16_t debouncing_time;
static bool debouncing = false;

__attribute__ ((weak))
void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    /* The DDRx register controls the I/O direction of the pin
     * Clearing the bit associated with the pin in DDR sets the
     * pin I/O direction to INPUT */
    KEY_DDR(KEY1_PIN) &= ~KEY_BV(KEY1_PIN);

    /* The PORTx register controls the output state of the pin
     * When the pin is set to INPUT, writing 1 to the bit associated
     * with the pin enables the pullup resistor for that pin */
    KEY_PORT(KEY1_PIN) |= KEY_BV(KEY1_PIN);


    KEY_DDR(KEY2_PIN) &= ~KEY_BV(KEY2_PIN);
    KEY_PORT(KEY2_PIN) |= KEY_BV(KEY2_PIN);



    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
        matrix_stage[i] = 0;
    }
 


    matrix_init_quantum();

}

uint8_t matrix_scan(void)
{
    /* The PINx register contains the current state of the pin and is 
     * read only. */
    matrix_stage[0] =   ((KEY_PIN(KEY1_PIN) & KEY_BV(KEY1_PIN)) ? 0 : (1<<0)) 
                      | ((KEY_PIN(KEY2_PIN) & KEY_BV(KEY2_PIN)) ? 0 : (1<<1));

    if (memcmp(matrix_debouncing, matrix_stage, sizeof(matrix)) != 0) {
        debouncing = true;
        debouncing_time = timer_read();
    }

    matrix_debouncing[0] = matrix_stage[0];

    if (debouncing && (timer_elapsed(debouncing_time) > 20)) {
        for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
            matrix[i] = matrix_debouncing[i];
        }
        debouncing = false;
    }

    matrix_scan_quantum();

    return 1;
}

#if 0
void matrix_init(void)
{
    debug_config.enable = true;


    for (int i = 0; i < MATRIX_COLS; i++)
    {
        uint8_t pin = key_pins[i];

        /* Set pin as input */
        PIN_DDRx(pin) &= ~PIN_BITVALUE(pin);
        /* Turn on pin pullup resistor */
        PIN_PORTx(pin) |= PIN_BITVALUE(pint);
    }

    DDRB &= ~(1<<1);
    PORTB |= (1<<1);


    matrix[0] = 0;
   
}

uint8_t matrix_scan(void)
{
    /* Poll all input lines and store state in matrix[0] */

     matrix_row_t row_state = 0; //matrix[0];
    for (int col = 0; col < MATRIX_COLS; col++)
    {
        uint8_t pin = key_pins[col];    

        /* Check the state of the pin */
        uint8_t pin_state = (PIN_PINx(pin) & PIN_BITVALUE(pin));
        uint8_t col_value = ((pin_state == 0) << col);
        row_state |= col_value;
#if 0
        /* If the state of the pin has changed */
        if (col_value != (matrix[0] & (1 << col)))
        {
            /* Debounce the pin by incrementing per pin debounce counter
             * until it reaches the threshold set by DEBOUNCE_THRESHOLD
             * The state will only change once is has remained the same for 
             * DEBOUNCE_THRESHOLD intervals */
            uint8_t debounce = matrix_debounce[PIN_INDEX(pin)]++;
            if (debounce > DEBOUNCE_THRESHOLD)
            {
                if (pin_state == 0) 
                    row_state |= (1 << col);
                else
                    row_state &= ~(1 << col);
                matrix_debounce[PIN_INDEX(pin)] = 0;
            }
        }
        else
        {
            /* Reset the debounce counter to 0 every time the state is the same */
            matrix_debounce[PIN_INDEX(pin)] = 0;
        } 
#endif
       
    }
    matrix[0] = row_state;
    return 1;
}
#endif

bool matrix_is_modified(void)
{
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}


