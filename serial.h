/*
 * serial.h
 *
 *  Created on: May 15, 2019
 *      Author: kasik
 */

#ifndef SERIAL_H_
#define SERIAL_H_

uint32_t itoa(uint32_t number, char stringArray[]);
void serial_init(unsigned int baud_rate);
void serial_putc(const char c);
void serial_puts(const char *string);
void serial_write(const char *string, int len);
void serial_getc(char *c);
void set_char(char c);
int get_char(char *c);
uint32_t toi(uint32_t digits, char stringArray[], int32_t endianess);


#endif /* SERIAL_H_ */
