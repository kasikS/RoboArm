/*
 * serial.c
 *
 *  Created on: May 19, 2019
 *      Author: kasik
 */
#include <stdint.h>
#include "serial.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define SERIAL_BUFFER_SIZE 20
char serialBuffer[SERIAL_BUFFER_SIZE];
int writeIndex;
int readIndex;

/**
 * \brief Converts an integer to array of chars
 *
 * \param number is an integer to be converted
 * \param stringArray is an output array
 *
 * returns number of digits
 */
uint32_t itoa(uint32_t number, char stringArray[])
{
	uint32_t temp = 0;
	uint32_t digits = 0;
	uint32_t i = 0;

	if (number == 0)
	{
		stringArray[0] = '0';
		digits++;
	}

	while(number!=0)
	{
		stringArray[digits] = '0' + number % 10;
		number = number/10;
		digits++;
	}

	for(i=0; i< (digits/2); i++)
	{
		temp = stringArray[i];
		stringArray[i] = stringArray[digits-1-i];
		stringArray[digits-1-i] = temp;
	}

	return digits;
}

/**
 * \brief Converts an array of chars to integer
 *
 * \param digits is a number of digits present in the integer
 * \param stringArray is an input array
 * \param endianess specifies which digits is first in the array
 */
uint32_t toi(uint32_t digits, char stringArray[], int32_t endianess)
{
	uint32_t number = 0;
	uint32_t i = 0;

	if (digits == 0)
	{
		return 0;
	}


	for(i = 0; i < digits; i++)
	{
		if(!(stringArray[i] >= '0' && stringArray[i] <= '9'))
		{
			return 0;
		}
		number = number + (stringArray[i]-'0') * pow(10, (digits-i-1));
	}

	return number;
}

/**
 * \brief Initializes the serial port with requested baud rate.
 *
 * \param baud_rate is the baud rate to be used with the serial port.
 */
void serial_init(unsigned int baud_rate) //configurable frame format? which uart?
{
	/*	Set baud rate */
	uint16_t baudrate = F_CPU/16/baud_rate-1;
	UBRR0H = (uint8_t)(baudrate>>8);
	UBRR0L = (uint8_t)(0x00ff & baudrate);

	/*Enable receiver and transmitter, RX interrupt */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);

	/* 	Set frame format: 8data, 1stop bit */
	UCSR0C = 3<<UCSZ00;
}

/**
 *\brief  Sends a single character through the serial port.
 *
 * \param c is the character to be sent.
 */
void serial_putc(const char c)
{
	/*Waits for end of transmission*/
	while( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = c;
}

/**
 * \brief Sends a null-terminated string through the serial port.
 *
 * \param string is the data to be sent.
 */
void serial_puts(const char *string)
{
	while(*string)
	{
//		/*Waits for end of transmission*/
//		while( !( UCSR0A & (1<<UDRE0)) );

		/* Puts data into buffer, sends the data*/
		serial_putc(*string);
		++string;
	}
}

/**
 * \brief Sends a specified number of characters through the serial port.
 *
 * \param string is the data to be sent.
 * \param len is number of characters to be sent.
 */
void serial_write(const char *string, int len)
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
}

/**
 * \brief Receives a single character from the serial port.
 *
 * \param c is a pointer to the variable where the read character will be saved.
 */
void serial_getc(char *c)
{
	while(!(UCSR0A & (1<<RXC0)));

	*c = UDR0;
}

/**
 * \brief Sets a received character in the circular buffer
 *
 * \param c is a new character
 */
void set_char(char c)
{
	serialBuffer[writeIndex] = c;
	writeIndex = (writeIndex +1) % SERIAL_BUFFER_SIZE;
}

/**
 * \brief Returns next character from the circular buffer
 *
 * \param c is a pointer to the variable where the read character will be saved.
 */
int get_char(char *c)
{

	if (readIndex != writeIndex)
	{
		*c = (serialBuffer[readIndex]);
		readIndex = (readIndex +1) % SERIAL_BUFFER_SIZE;

		return 1;
	}
 return 0;
}

ISR(USART_RX_vect)
{
	   char receivedByte;
	   while(!(UCSR0A & (1<<RXC0)));
	   receivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	   set_char(receivedByte);
}
