/*
 * nhd_c12832.c
 *
 *  Created on: Jan 8, 2016
 *      Author: Matt
 */

#include "stm32f4xx_hal.h"
#include "mxconstants.h"
#include "nhd_c12832.h"
#include "font5x7.h"

extern SPI_HandleTypeDef hspi1;

void data_out(unsigned char i) //Data Output Serial Interface
{
 HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(SPI1_A0_GPIO_Port, SPI1_A0_Pin, GPIO_PIN_SET);

 HAL_SPI_Transmit(&hspi1, &i, 1, 200);

 HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void comm_out(unsigned char j) //Command Output Serial Interface
{
 HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(SPI1_A0_GPIO_Port, SPI1_A0_Pin, GPIO_PIN_RESET);

 HAL_SPI_Transmit(&hspi1, &j, 1, 200);

 HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void reset_LCD()
{
	HAL_GPIO_WritePin(SPI1_RESET_GPIO_Port, SPI1_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(RESETTIMEMS);
	HAL_GPIO_WritePin(SPI1_RESET_GPIO_Port, SPI1_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(RESETTIMEMS);
}

unsigned char invert_character(unsigned char c)
{
    c = (c & 0xF0) >> 4 | (c & 0x0F) << 4;
    c = (c & 0xCC) >> 2 | (c & 0x33) << 2;
    c = (c & 0xAA) >> 1 | (c & 0x55) << 1;
    return c;
}

void write_character(unsigned char c, uint8_t row, uint8_t col)
{
	if ((row > MAXROWS-1) || (col > MAXCOLS-1))
		return;

	col *= 6;

	comm_out(0x40);
	comm_out(0xB3-(row));
	comm_out(0x10+((col & 0xF0) >> 4));	// most significant 4 bits of column
	comm_out(0x00+(col & 0x0F));		// least significant 4 bits of column

	uint32_t i = 0;
	uint32_t pos = (c-32)*5;

	while (i < 5) {
		data_out(invert_character(font5x7[pos++]));	// font5x7 array is upside down
		i++;
	}
}

void write_string(char * c, uint8_t row, uint8_t col)
{
	char * outPos = c;
	uint8_t count = 0;
	while ((*outPos != 0x00) && (count++ < MAXCOLS)) {
		write_character(*outPos++, row, col++);
	}
}

void clear_LCD()
{
	uint8_t page = 0xB0;
	uint32_t i = 0;

	comm_out(0xAE);
	comm_out(0x40);
	while (page < 0xB4) {
		comm_out(page++);
		comm_out(0x10);
		comm_out(0x00);
		i = 0;
		while (i < 128) {
			data_out(0x00);
			i++;
		}
	}
	comm_out(0xAF);
}


void init_LCD()
{
comm_out(0xA0);
comm_out(0xAE);
comm_out(0xC0);
comm_out(0xA6);
comm_out(0xA2);
comm_out(0x2F);
comm_out(0x20);
comm_out(0x81);
comm_out(0x3F);
}

