/*
 * Main.c
 *
 *  Created on: Oct 7, 2018
 *      Author: kasik
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "gpio.h"
#include "serial.h"
#define F_CPU 16000000UL
//#define BAUD 9600
//#define MYUBRR F_CPU/16/BAUD-1


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

//usart:
//rx - PD0
//tx - PD1

int main (void)
{
//	gpio: cfg as input;cfg as output with state;toggle; set; clear;
//	timer: clock src, presc; set max count; enable a/b; set compare a/b; configure pwm

	GPIO_init(Port_B, Pin_5, Output);


//-------------
	//PWM for servo config; non inverted, prescaler = 64, fast pwm:
	//50Hz signal
	//16Mhz clock
//	DDRB |= 1<<DDB1; //timer 1 channel a
//	DDRB |= 1<<DDB2; //timer 1 channel b

//	TCCR1A |= (1<<COM1A1) | (1<<COM1B1)| (1<<WGM11);
//	TCCR1B |=(1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);
//	ICR1 = 4999; //(16 MHz/64*50 Hz) -1 //maximum counts, prescaler needed as the timer max 6535

//	  counts = 5000*width_us/20000us
//    Servo Angle 0 degrees requires pulse width of 0.388ms(388uS) so value of OCR1A = 388us/4us = 97
//    Servo Angle 90 degrees requires pulse width of 1.264ms(1264uS) so value of OCR1A = 1264us/4us = 316
//    Servo Angle 180 degrees requires pulse width of 2.140ms(2140uS) so value of OCR1A = 2140us/4us = 535

//	OCR1A=97;   //0 degree
//	OCR1A=316;   //90 degree
//----------------

	char c;
	serial_init(9600);

	while(1)
	{
		GPIO_toggle_bits(Port_B, Pin_5); // toggle led
//		OCR1A=97;
//		OCR1B=97;
		_delay_ms(1000);
		GPIO_toggle_bits(Port_B, Pin_5); // toggle led
//		OCR1A=200;
//		OCR1B=200;

		serial_getc(&c);
		serial_putc(c);

		_delay_ms(1000);

	}
	return 0;
}
