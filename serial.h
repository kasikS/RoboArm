/*
 * serial.h
 *
 *  Created on: May 15, 2019
 *      Author: kasik
 */

#ifndef SERIAL_H_
#define SERIAL_H_

int serial_init(unsigned int baud_rate);
int serial_putc(const char c);
int serial_puts(const char *string);
int serial_write(const char *string, int len);
int serial_getc(char *c);


#endif /* SERIAL_H_ */
