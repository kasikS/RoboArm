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
#include "timer.h"
#define F_CPU 16000000UL

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

	gpio_init(Port_B, Pin_5, Output);
	timer_init(timer0);

	char c='a';
	serial_init(9600);
	while(1)
	{
		gpio_toggle_bits(Port_B, Pin_5); // toggle led
		set_ocr(timer0,0, 6);
//		OCR1A=6;//4999 - 130;
//		OCR0A=6;
//		timer0.set_ocr(&timer0, 0, 6);
//		timer0.set_ocr(&timer1, 0, 6);
//		OCR1B=4999 - 130;

		_delay_ms(1000);
		gpio_toggle_bits(Port_B, Pin_5); // toggle led
		set_ocr(timer0, 0, 22);
//		OCR1A=22;//4999 - 350;//620;
//		OCR0A=22;
//		timer0.set_ocr(&timer0, 0, 22);
//		timer0.set_ocr(&timer1, 0, 22);
//		OCR1B=4999 - 350;//620;

//		serial_getc(&c);
		serial_putc(c);

		_delay_ms(1000);

	}
	return 0;
}
