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
#include "servo.h"
#include "adc.h"

#define F_CPU 16000000UL

//servo:
// 1 d11 - PB3
// 2 d10 - PB2
// 3 d9  - PB1
// 4 d6  - PD6


int main (void)
{
//	gpio: cfg as input;cfg as output with state;toggle; set; clear;
//	timer: clock src, presc; set max count; enable a/b; set compare a/b; configure pwm

	gpio_init(portB, pin5, output); //led

	serial_init(9600);
//	timer_init(timer0, p1024);
	servo_init(servo3);

	adc_init();
	adc_channel_init(adc0);
	adc_channel_init(adc1);
	adc_channel_init(adc2);
	adc_channel_init(adc3);

	char c='a';


	///////adc
	//cADMUX.MUX[3:0] channel selection ok
	//ADCSRB.ADTS trigger source		needed?
//	ADCSRA.ADPS prescaler 				ok
//	ADCSRA.ADATE auto trigger enable	neeed?
	//ADCSRA.ADEN enable				ok
	//PRR.PRADC write 0;
//	loop
//	ADCSRA.ADSC write 1 - start conversion (cleared when done)
//	ADC Data Registers (ADCL and ADCH), and the ADC Interrupt Flag (ADCSRA.ADIF) is set

	//buffer for characters after itoa
	uint8_t chars[12] = {0,};

	//number of characters to send via serial
	uint32_t length=0;


//	PRR &= !(1<<0);
//	gpio_init(portC, pin0, input); 	//pin as input ADC1
//	ADMUX |= 0 <<7 | 1 << 6;			//vcc as ref
//	ADMUX |= 0 <<3 | 0 << 2 | 0 <<1 | 0 << 0; //select ADC1
//
//	ADCSRA |= 1 << 2 | 1 <<1 | 1 << 0; //prescaler to 128
//	ADCSRA |= 1 << 7; //enable

	uint32_t adcData =0;
	uint16_t pwmCounts = 0;
	uint16_t pwmRange = MAX_POSITION-MIN_POSITION;


	while(1)
	{
		gpio_toggle_bits(portB, pin5); // toggle led
//		set_ocr(timer0,0, 6);
//		OCR1A=6;//4999 - 130;
//		OCR0A=6;
//		timer0.set_ocr(&timer0, 0, 6);
//		timer0.set_ocr(&timer1, 0, 6);
//		OCR1B=4999 - 130;

//		_delay_ms(1000);
		gpio_toggle_bits(portB, pin5); // toggle led
//		set_ocr(timer0, 0, 44);//44 max?
//		OCR1A=22;//4999 - 350;//620;
//		OCR0A=22;
//		timer0.set_ocr(&timer0, 0, 22);
//		timer0.set_ocr(&timer1, 0, 22);
//		OCR1B=4999 - 350;//620;

//		serial_getc(&c);
//		serial_putc(c);

//		serial_putc('s');

//		ADCSRA |= 1 << 6; // start conversion
//		adcData = ADCL;
//
//		adcData |= (ADCH << 8);

//		percent = (adcData*100)/maxCount;

		adcData = adc_read(adc3);
		pwmCounts = (pwmRange*adcData)/MAX_COUNT + MIN_POSITION;
//		set_ocr(timer0,0, pwmCounts);

//		servoCounts = servo_set(servo3,pwmCounts);


		length = itoa(pwmCounts, chars);
		serial_write(chars, length);

		serial_putc('\n');
		serial_putc('\r');

		_delay_ms(100);

	}
	return 0;
}
