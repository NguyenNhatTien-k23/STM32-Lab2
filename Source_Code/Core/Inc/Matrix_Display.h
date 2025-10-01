/*
 * Matrix_Display.h
 *
 *  Created on: Oct 1, 2025
 *      Author: Hi
 */

#ifndef INC_MATRIX_DISPLAY_H_
#define INC_MATRIX_DISPLAY_H_

#include <stdint.h>

#define BITMAP_LETTER_T ((uint64_t)0x181818181818FFFF)
#define BITMAP_LETTER_I	((uint64_t)0x0303030303030303)
#define BITMAP_LETTER_E	((uint64_t)0xFFFFC0FCFCC0FFFF)
#define BITMAP_LETTER_N	((uint64_t)0xC3C7CFDFFBF3E3C3)

#define STATE_INIT -1
#define STATE_BLANK 0
#define STATE_LETTER_T 1
#define STATE_LETTER_I 2
#define STATE_LETTER_E 3
#define STATE_LETTER_N 4
#define STATE_BIG_BLANK 5

#define COUNTER_BLANK 2
#define COUNTER_LETTER_T 8
#define COUNTER_LETTER_I 2
#define COUNTER_LETTER_E 8
#define COUNTER_LETTER_N 8
#define COUNTER_BIG_BLANK 4

/*Public function*/
void ClearMatrixDisplay();
void DisplayLedMatrix(int buffer_index);
void DisplayCollumn(uint8_t bit_map);

void UpdateLedMatrix();

/*Private function*/
void MatrixBufferShiftLeft();
void InsertBit(int shift_amount, uint64_t bitmap);

/*Public variable*/
extern uint8_t matrix_buffer[8];
extern int row_index;
#endif /* INC_MATRIX_DISPLAY_H_ */
