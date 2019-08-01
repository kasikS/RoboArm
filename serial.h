/*
 * serial.h
 *
 *  Created on: May 15, 2019
 *      Author: kasik
 */

#ifndef SERIAL_H_
#define SERIAL_H_

void serial_init(unsigned int baud_rate);
void serial_putc(const char c);
void serial_puts(const char *string);
void serial_write(const char *string, int len);
void serial_getc(char *c);


#endif /* SERIAL_H_ */
