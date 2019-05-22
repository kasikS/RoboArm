/*
 * serial.c
 *
 *  Created on: May 19, 2019
 *      Author: kasik
 */

#include "serial.h"
#include <stdint.h>
#include <avr/io.h>

/**
 * Initializes the serial port with requested baud rate.
 * @param baud_rate is the baud rate to be used with the serial port.
 * @return false in case of error.
 */
int serial_init(unsigned int baud_rate) //configurable frame format? which uart?
{
	/*	Set baud rate */
	uint16_t baudrate = F_CPU/16/baud_rate-1;
	UBRR0H = (uint8_t)(baudrate>>8);
	UBRR0L = (uint8_t)(0x00ff & baudrate);

	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	/* 	Set frame format: 8data, 1stop bit */
	UCSR0C = 3<<UCSZ00;
	return 0;
}

/**
 * Sends a single character through the serial port.
 * @param c is the character to be sent.
 * @return false in case of error.
 */
int serial_putc(const char c)
{
	UDR0 = c;
	return 0;
}

/**
 * Sends a null-terminated string through the serial port.
 * @param string is the data to be sent.
 * @return false in case of error.
 */
int serial_puts(const char *string)
{
	while(*string)
	{
		/*Waits for end of transmission*/
		while( !( UCSR0A & (1<<UDRE0)) );

		/* Puts data into buffer, sends the data*/
		serial_putc(*string);
		++string;
	}
	return 0;
}

/**
 * Sends a specified number of characters through the serial port.
 * @param string is the data to be sent.
 * @param len is number of characters to be sent.
 * @return false in case of error.
 */
int serial_write(const char *string, int len)
{
	int i;
	for(i=0; i<len; i++)
	{
		/*Waits for end of transmission*/
		while( !( UCSR0A & (1<<UDRE0)) );

		/* Puts data into buffer, sends the data*/
		serial_putc(*string);
		++string;
	}
	return 0;
}

/**
 * Receives a single character from the serial port.
 * @param c is a pointer to the variable where the read character will be saved.
 * If there is nothing in the buffer, it will not modified.
 * @return true if a character was received.
 */
int serial_getc(char *c) // TODO handle by innterrupts
{
	while(!(UCSR0A & (1<<RXC0)));

	*c = UDR0;

	return 0;
}
