/*
Copyright 2019 Massdrop Inc.

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

#include "alt.h"

#include "d51_util.h"
#include "debug.h"
#include "clks.h"
#include <string.h>

#define MCU_PORTS_USED  2 //PA, PB

matrix_row_t mlatest[MATRIX_ROWS];
matrix_row_t mlast[MATRIX_ROWS];
matrix_row_t mdebounced[MATRIX_ROWS];

uint8_t row_ports[] = { MATRIX_ROW_PORTS };
uint8_t row_pins[] = { MATRIX_ROW_PINS };
uint8_t col_ports[] = { MATRIX_COL_PORTS };
uint8_t col_pins[] = { MATRIX_COL_PINS };
uint32_t row_masks[MCU_PORTS_USED]; //Array size must match number of unique MCU ports used for reads (PA, PB, PC, etc)

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

void matrix_init(void)
{
    memset(mlatest, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(mlast, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(mdebounced, 0, MATRIX_ROWS * sizeof(matrix_row_t));

    memset(row_masks, 0, sizeof(row_masks));

    //Inputs
    uint8_t row;
    for (row = 0; row < MATRIX_ROWS; row++)
    {
        PORT->Group[row_ports[row]].DIRCLR.reg = 1 << row_pins[row]; //Input
        PORT->Group[row_ports[row]].OUTCLR.reg = 1 << row_pins[row]; //Low
        PORT->Group[row_ports[row]].PINCFG[row_pins[row]].bit.INEN = 1; //Input Enable,
        PORT->Group[row_ports[row]].PINCFG[row_pins[row]].bit.PULLEN = 1; //Pull Enable
        row_masks[row_ports[row]] |= 1 << row_pins[row]; //Add pin to proper row mask
    }

    //Outputs
    uint8_t col;
    for (col = 0; col < MATRIX_COLS; col++)
    {
        PORT->Group[col_ports[col]].DIRSET.reg = 1 << col_pins[col]; //Output
        PORT->Group[col_ports[col]].OUTCLR.reg = 1 << col_pins[col]; //Low
    }

    matrix_init_quantum();
}

uint64_t mdebouncing = 0;
bool debouncing = false;

uint8_t matrix_scan(void)
{
    uint64_t timer;
    uint8_t row;
    uint8_t col;
    uint32_t scans[MCU_PORTS_USED]; //Array size must match number of unique MCU ports used for reads (PA, PB, PC, etc)

    memset(mlatest, 0, MATRIX_ROWS * sizeof(matrix_row_t)); //Zero the result buffer

    for (col = 0; col < MATRIX_COLS; col++)
    {
        //Set output
        PORT->Group[col_ports[col]].OUTSET.reg = 1 << col_pins[col];        //Set col output
        wait_us(1);                                                         //Delay for output

        //Read input (add unique ports as needed, PA, PB, PC, etc)
        scans[PA] = PORT->Group[PA].IN.reg & row_masks[PA];                 //Read PA row pins data
        scans[PB] = PORT->Group[PB].IN.reg & row_masks[PB];                 //Read PA row pins data

        //Clear output
        PORT->Group[col_ports[col]].OUTCLR.reg = 1 << col_pins[col];        //Clear col output

        for (row = 0; row < MATRIX_ROWS; row++)
        {
            //Move scan bits from scans array into proper row bit locations
            if (scans[row_ports[row]] & (1 << row_pins[row]))
                mlatest[row] |= 1 << col;
        }
    }

    timer = timer_read64();

    for (row = 0; row < MATRIX_ROWS; row++)
    {
        if (mlast[row] != mlatest[row]) {
            debouncing = true;
            mdebouncing = timer + DEBOUNCE;
        }

        mlast[row] = mlatest[row];
    }

    if (debouncing && timer >= mdebouncing)
    {
        for (row = 0; row < MATRIX_ROWS; row++) {
            mdebounced[row] = mlatest[row];
        }

        mdebouncing = 0;
        debouncing = false;
    }

    matrix_scan_quantum();

    return 1;
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return mdebounced[row];
}

void matrix_print(void)
{
    char buf[(MATRIX_COLS+8)*(MATRIX_ROWS+1)] = "R C";
    char *pbuf = buf+3;
    uint32_t cols;
    uint32_t rows;
    matrix_row_t row;

    for (cols = 1; cols <= MATRIX_COLS; cols++)
    {
        *pbuf = (cols%10)+48;
        pbuf++;
    }
    *pbuf = '\r'; pbuf++;
    *pbuf = '\n'; pbuf++;

    for (rows = 1; rows <= MATRIX_ROWS; rows++)
    {
        row = matrix_get_row(rows-1);
        if (rows < 10) { *pbuf = rows+48; pbuf++; *pbuf = ' '; pbuf++; *pbuf = ' '; pbuf++; }
        else { *pbuf = (rows/10)+48; pbuf++; *pbuf = (rows%10)+48; pbuf++; *pbuf = ' '; pbuf++; }
        for (cols = 0; cols < MATRIX_COLS; cols++)
        {
            if (row & 1 << cols) *pbuf = 'X';
            else                 *pbuf = '.';
            pbuf++;
        }
        *pbuf = '\r'; pbuf++;
        *pbuf = '\n'; pbuf++;
    }
    *pbuf = 0;
    dprint(buf);
}
