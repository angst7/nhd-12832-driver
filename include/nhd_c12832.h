/*
 * nhd_c12832.h
 *
 *  Created on: Jan 8, 2016
 *      Author: Matt
 */

#ifndef NHD_C12832_H_
#define NHD_C12832_H_

#define MAXROWS 4
#define MAXCOLS 21
#define RESETTIMEMS 100

void data_out(unsigned char i);
void comm_out(unsigned char j);
void write_character(unsigned char c, uint8_t row, uint8_t col);
void write_string(char * c, uint8_t row, uint8_t col);
void reset_LCD();
void clear_LCD();
void test_LCD();
void init_LCD();

#endif /* NHD_C12832_H_ */
