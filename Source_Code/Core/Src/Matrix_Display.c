/*
 * Matrix_Display.c
 *
 *  Created on: Oct 1, 2025
 *      Author: Hi
 */

#include "main.h"
#include "Matrix_Display.h"

uint8_t matrix_buffer[8] = {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, 0x00};
int row_index = 0;

int matrix_state = STATE_INIT;
int matrix_next_state = STATE_LETTER_T;
int matrix_counter = -1;
int matrix_next_counter = COUNTER_LETTER_T;

void ClearMatrixDisplay(){
	HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, SET);
	HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, SET);
	HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, SET);
	HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, SET);
	HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, SET);
	HAL_GPIO_WritePin(ROW5_GPIO_Port, ROW5_Pin, SET);
	HAL_GPIO_WritePin(ROW6_GPIO_Port, ROW6_Pin, SET);
	HAL_GPIO_WritePin(ROW7_GPIO_Port, ROW7_Pin, SET);
}

void DisplayCollumn(uint8_t bit_map){ //7<-0
	HAL_GPIO_WritePin(ENM0_GPIO_Port, ENM0_Pin, !(bit_map & 0x01));
	HAL_GPIO_WritePin(ENM1_GPIO_Port, ENM1_Pin, !(bit_map & 0x02));
	HAL_GPIO_WritePin(ENM2_GPIO_Port, ENM2_Pin, !(bit_map & 0x04));
	HAL_GPIO_WritePin(ENM3_GPIO_Port, ENM3_Pin, !(bit_map & 0x08));
	HAL_GPIO_WritePin(ENM4_GPIO_Port, ENM4_Pin, !(bit_map & 0x10));
	HAL_GPIO_WritePin(ENM5_GPIO_Port, ENM5_Pin, !(bit_map & 0x20));
	HAL_GPIO_WritePin(ENM6_GPIO_Port, ENM6_Pin, !(bit_map & 0x40));
	HAL_GPIO_WritePin(ENM7_GPIO_Port, ENM7_Pin, !(bit_map & 0x80));
}

void DisplayLedMatrix(int buffer_index){
	ClearMatrixDisplay();
	DisplayCollumn(matrix_buffer[buffer_index]);
	HAL_GPIO_WritePin(GPIOB, ROW0_Pin << buffer_index, RESET);
}

void UpdateLedMatrix(){
	switch(matrix_state){
	case STATE_INIT:
		if(1){
			matrix_state = matrix_next_state;
			matrix_counter = matrix_next_counter;
		}
		break;

	case STATE_BLANK:
		MatrixBufferShiftLeft();
		matrix_counter--;
		if(matrix_counter <= 0){
			matrix_state = matrix_next_state;
			matrix_counter = matrix_next_counter;
		}
		break;

	case STATE_LETTER_T:
		MatrixBufferShiftLeft();
		InsertBit(--matrix_counter, BITMAP_LETTER_T);
		if(matrix_counter <= 0){
			matrix_state = STATE_BLANK;
			matrix_counter = COUNTER_BLANK;

			matrix_next_state = STATE_LETTER_I;
			matrix_next_counter = COUNTER_LETTER_I;
		}
		break;

	case STATE_LETTER_I:
		MatrixBufferShiftLeft();
		InsertBit(--matrix_counter, BITMAP_LETTER_I);
		if(matrix_counter <= 0){
			matrix_state = STATE_BLANK;
			matrix_counter = COUNTER_BLANK;

			matrix_next_state = STATE_LETTER_E;
			matrix_next_counter = COUNTER_LETTER_E;
		}
		break;

	case STATE_LETTER_E:
		MatrixBufferShiftLeft();
		InsertBit(--matrix_counter, BITMAP_LETTER_E);
		if(matrix_counter <= 0){
			matrix_state = STATE_BLANK;
			matrix_counter = COUNTER_BLANK;

			matrix_next_state = STATE_LETTER_N;
			matrix_next_counter = COUNTER_LETTER_N;
		}
		break;

	case STATE_LETTER_N:
		MatrixBufferShiftLeft();
		InsertBit(--matrix_counter, BITMAP_LETTER_N);
		if(matrix_counter <= 0){
			matrix_state = STATE_BIG_BLANK;
			matrix_counter = COUNTER_BIG_BLANK;
		}
		break;

	case STATE_BIG_BLANK:
		MatrixBufferShiftLeft();
		--matrix_counter;
		if(matrix_counter <= 0){
			matrix_state = STATE_LETTER_T;
			matrix_counter = COUNTER_LETTER_T;
		}
		break;

	default:
		break;
	}
}

void MatrixBufferShiftLeft(){
	matrix_buffer[0] = matrix_buffer[0] << 1;
	matrix_buffer[1] = matrix_buffer[1] << 1;
	matrix_buffer[2] = matrix_buffer[2] << 1;
	matrix_buffer[3] = matrix_buffer[3] << 1;
	matrix_buffer[4] = matrix_buffer[4] << 1;
	matrix_buffer[5] = matrix_buffer[5] << 1;
	matrix_buffer[6] = matrix_buffer[6] << 1;
	matrix_buffer[7] = matrix_buffer[7] << 1;
}

/**
 * Every 8 bit in the bitmap represent a row
 * So to select the column we desire we:
 *  - Shift the bit we want into LSB with each row is 8-bit apart in the bit map
 * 	- Bit-wise and it with number 1 to extract it
 * 	- Bit-wise or the matrix_buffer with the result we get to have the new matrix_buffer
 */
void InsertBit(int shift_amount, uint64_t bitmap){
	uint64_t start_bit = bitmap >> shift_amount;

	matrix_buffer[0] = matrix_buffer[0] | ((start_bit >> (0 * 8)) & (uint64_t)1);
	matrix_buffer[1] = matrix_buffer[1] | ((start_bit >> (1 * 8)) & (uint64_t)1);
	matrix_buffer[2] = matrix_buffer[2] | ((start_bit >> (2 * 8)) & (uint64_t)1);
	matrix_buffer[3] = matrix_buffer[3] | ((start_bit >> (3 * 8)) & (uint64_t)1);
	matrix_buffer[4] = matrix_buffer[4] | ((start_bit >> (4 * 8)) & (uint64_t)1);
	matrix_buffer[5] = matrix_buffer[5] | ((start_bit >> (5 * 8)) & (uint64_t)1);
	matrix_buffer[6] = matrix_buffer[6] | ((start_bit >> (6 * 8)) & (uint64_t)1);
	matrix_buffer[7] = matrix_buffer[7] | ((start_bit >> (7 * 8)) & (uint64_t)1);

}

