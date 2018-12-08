/*
 * Main.c
 *
 *  Created on: Oct 7, 2018
 *      Author: kasik
 */
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
//#define __AVR_ATmega328P__// defined in -mmcu?

//servo:
// 1 d11 - PB3
// 2 d10 - PB2
// 3 d9  - PB1
// 4 d6  - PD6

//potentiometers:
// a0 - PC0
// a1 - PC1
// a2 - PC2
// a3 - PC3

int main (void)
{

	DDRB |= 1<<DDB5; //set PB5 as output
	PORTB |= 1<<PB5;

	//PWM for servo config; non inverted, prescaler = 64, fast pwm:
	DDRB |= 1<<DDB1;
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1)| (1<<WGM11);
	TCCR1B |=(1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);
	ICR1 = 4999; //(16 MHz/64*50 Hz) -1


//    Servo Angle 0 degrees requires pulse width of 0.388ms(388uS) so value of OCR1A = 388us/4us = 97
//    Servo Angle 90 degrees requires pulse width of 1.264ms(1264uS) so value of OCR1A = 1264us/4us = 316
//    Servo Angle 180 degrees requires pulse width of 2.140ms(2140uS) so value of OCR1A = 2140us/4us = 535

//	OCR1A=97;   //0 degree
//	OCR1A=316;   //90 degree



	while(1)
	{
		PORTB ^= 1<<PB5; // toggle led
		OCR1A=97;
		_delay_ms(1000);
		PORTB ^= 1<<PB5; // toggle led
		OCR1A=316;
		_delay_ms(1000);

	}
	return 0;
}
