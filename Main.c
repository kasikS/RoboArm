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


/*typedef struct timerRegisters
{
		volatile uint8_t * tccra;
		volatile uint8_t * tccrb;
		volatile void * ocra;
		volatile void * ocrb;
		GPIOPort porta;
		GPIOPort portb;
		GPIOPin pina;
		GPIOPin pinb;
		uint8_t coma0;
		uint8_t coma1;
		uint8_t comb0;
		uint8_t comb1;
		uint8_t cs0;
		uint8_t cs1;
		uint8_t cs2;
		uint8_t wgm0;
		uint8_t wgm1;
		uint8_t wgm2;
		uint8_t wgm3;
		void (*cfg_wfm)(struct timerRegisters *registers);
		int (*set_ocr)(struct timerRegisters *registers, int channel, int counts);
}timerRegisters;

//configures fast non-inverted PWM
void configure_waveform_8b(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<(registers->wgm1))| (1<<(registers->wgm0));
	*(registers->tccrb) |= (0<<(registers->wgm2));
}


void configure_waveform_16b(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<(registers->wgm1))| (0<<(registers->wgm0));
	*(registers->tccrb) |= (1<<WGM13)|(1<<(registers->wgm2));
	ICR1 = 255;
}

int set_ocr_8b(timerRegisters *registers, int channel, int counts)
{
	if (channel > 1) return 0;

	volatile uint8_t * ocr;

	if(channel == 0)
	{
		ocr = (volatile uint8_t *)(registers->ocra);
	}
	else if (channel == 1)
	{
		ocr = (volatile uint8_t *)(registers->ocrb);
	}
	*ocr = counts;
	return 1;
}


int set_ocr_16b(timerRegisters *registers, int channel, int counts)
{
	if (channel > 1) return 0;

	volatile uint16_t * ocr;

	if(channel == 0)
	{
		ocr = (volatile uint16_t *)(registers->ocra);
	}
	else if (channel == 1)
	{
		ocr = (volatile uint16_t *)(registers->ocrb);
	}
	*ocr = counts;
	return 1;
}
*/

int main (void)
{
//	gpio: cfg as input;cfg as output with state;toggle; set; clear;
//	timer: clock src, presc; set max count; enable a/b; set compare a/b; configure pwm

	GPIO_init(Port_B, Pin_5, Output);


//-------------
	//PWM for servo config; non inverted, prescaler = 64, fast pwm:
	//50Hz signal
	//16Mhz clock
	//fast pwm
/////////////////////////
//	DDRB |= 1<<DDB1; //timer 1 channel a
//	DDRB |= 1<<DDB2; //timer 1 channel b

//	volatile uint8_t * tccra[3] = {&TCCR0A, &TCCR1A, &TCCR2A};
//	volatile uint8_t * tccrb[3] = {&TCCR0B, &TCCR1B, &TCCR2B};


//	TCCR1A |= (1<<COM1A1) | (1<<COM1A0)| (1<<WGM11); // inverted
//	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);
//	ICR1 = 4999; //(16 MHz/64*50 Hz) -1 //maximum counts, prescaler needed as the timer max 6535
//	(tccra[0])



//	TCCR1A |= (1<<COM1A1) | (0<<COM1A0)| (1<<COM1B1) | (0<<COM1B0)| (1<<WGM11);
//	TCCR1B |= (1<<WGM13) | (1<<WGM12) |(1<<CS12)| (0<<CS11) | (1<<CS10);
//	ICR1 = 255; //(16 MHz/64*50 Hz) -1 //maximum counts, prescaler needed as the timer max 6535
/////////////

//	DDRD |= 1<<DDD6; //timer 0 channel a
//	DDRD |= 1<<DDD5; //timer 0 channel b
//
//	TCCR0A |= (1<<COM0A1) | (0<<COM0A0)| (1<<COM0B1) | (0<<COM0B0)| (1<<WGM01)| (1<<WGM00); // non-inverted
//	TCCR0B |= (0<<WGM02) | (1<<CS02)| (0<<CS01) | (1<<CS00);
//	T0 is 8bit counter. f=16000000(256*prescaler) because max count is 256!;
//	with prescaler 1024, 1 count = 64us, frequency of the signal is 61Hz

/*	timerRegisters timer0;
	timer0.tccra = &TCCR0A;
	timer0.tccrb = &TCCR0B;
	timer0.ocra = &OCR0A;
	timer0.ocrb = &OCR0B;
	timer0.cfg_wfm = configure_waveform_8b;
	timer0.set_ocr = set_ocr_8b;
	timer0.coma0 = COM0A0;
	timer0.coma1 = COM0A1;
	timer0.comb0 = COM0B0;
	timer0.comb1 = COM0B1;
	timer0.cs0 = CS00;
	timer0.cs1 = CS01;
	timer0.cs2 = CS02;
	timer0.wgm0 = WGM00;
	timer0.wgm1 = WGM01;
	timer0.wgm2 = WGM02;
	timer0.porta = Port_D;
	timer0.pina = Pin_6;
	timer0.portb = Port_D;
	timer0.pinb = Pin_5;

	GPIO_init(timer0.porta, timer0.pina, Output); // set pin of channel A as output
	GPIO_init(timer0.portb, timer0.pinb, Output); // set pin of channel A as output

	*(timer0.tccra) |= (1<<timer0.coma1) | (0<<timer0.coma0); // init channel A
	*(timer0.tccra) |= (1<<timer0.comb1) | (0<<timer0.comb0); // init channel B

	//	T0 is 8bit counter. f=16000000(256*prescaler) because max count is 256!;
	//	with prescaler 1024, 1 count = 64us, frequency of the signal is 61Hz
	*(timer0.tccrb) |= (1<<timer0.cs2)| (0<<timer0.cs1) | (1<<timer0.cs0); // set prescaler

	timer0.cfg_wfm(&timer0);

//////////////////////////////
	timerRegisters timer1;
	timer1.tccra = &TCCR1A;
	timer1.tccrb = &TCCR1B;
	timer1.ocra = &OCR1A;
	timer1.ocrb = &OCR1B;
	timer1.cfg_wfm = configure_waveform_16b;
	timer1.set_ocr = set_ocr_16b;
	timer1.coma0 = COM1A0;
	timer1.coma1 = COM1A1;
	timer1.comb0 = COM1B0;
	timer1.comb1 = COM1B1;
	timer1.cs0 = CS10;
	timer1.cs1 = CS11;
	timer1.cs2 = CS12;
	timer1.wgm0 = WGM10;
	timer1.wgm1 = WGM11;
	timer1.wgm2 = WGM12;
	timer1.wgm3 = WGM13;
	timer1.porta = Port_B;
	timer1.pina = Pin_1;
	timer1.portb = Port_B;
	timer1.pinb = Pin_2;

	GPIO_init(timer1.porta, timer1.pina, Output); // set pin of channel A as output
	GPIO_init(timer1.portb, timer1.pinb, Output); // set pin of channel A as output

	*(timer1.tccra) |= (1<<timer1.coma1) | (0<<timer1.coma0); // init channel A
	*(timer1.tccra) |= (1<<timer1.comb1) | (0<<timer1.comb0); // init channel B

	//	T1 is 16bit counter
	//	with prescaler 1024, 1 count = 64us
	*(timer1.tccrb) |= (1<<timer1.cs2)| (0<<timer1.cs1) | (1<<timer1.cs0); // set prescaler

	timer1.cfg_wfm(&timer1);
*/

//	  counts = (5000*width_us)/20000us
//    Servo Angle 0 degrees requires pulse width of 0.388ms(388uS) so value of OCR1A = 388us/4us = 97; measured -> 130
//    Servo Angle 90 degrees requires pulse width of 1.264ms(1264uS) so value of OCR1A = 1264us/4us = 316
//    Servo Angle 180 degrees requires pulse width of 2.140ms(2140uS) so value of OCR1A = 2140us/4us = 535; measured -> 620
//----------------

	char c='a';
	serial_init(9600);

	while(1)
	{
		GPIO_toggle_bits(Port_B, Pin_5); // toggle led
		OCR1A=6;//4999 - 130;
//		OCR0A=6;
//		timer0.set_ocr(&timer0, 0, 6);
//		timer0.set_ocr(&timer1, 0, 6);
//		OCR1B=4999 - 130;

		_delay_ms(1000);
		GPIO_toggle_bits(Port_B, Pin_5); // toggle led
		OCR1A=22;//4999 - 350;//620;
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
