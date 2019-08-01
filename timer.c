/*
 * timer.c
 *
 *  Created on: Jul 15, 2019
 *      Author: kasik
 */

#include "timer.h"
#include <stdint.h>
#include <avr/io.h>
#include "gpio.h"


typedef struct timerRegisters
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


void init_gpio(timerRegisters *registers)
{
	GPIO_init(registers->porta, registers->pina, Output); // set pin of channel A as output
	GPIO_init(registers->portb, registers->pinb, Output); // set pin of channel A as output
}

void init_channels(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<registers->coma1) | (0<<registers->coma0); // init channel A
	*(registers->tccra) |= (1<<registers->comb1) | (0<<registers->comb0); // init channel B
}

void set_prescaler(timerRegisters *registers)
{
	*(registers->tccrb) |= (1<<registers->cs2)| (0<<registers->cs1) | (1<<registers->cs0);
}

void configure_waveform(timerRegisters *registers)
{
	registers->cfg_wfm(registers);
}

int set_ocr(timerRegisters *registers, int channel, int counts)
{
	if (channel > 1) return 0;
	if (counts > 255) return 0;//define needed for max number of counts?

	registers->set_ocr(registers, channel, counts);

	return 1;
}

//configures fast non-inverted PWM
void configure_waveform_8b(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<(registers->wgm1))| (1<<(registers->wgm0));
	*(registers->tccrb) |= (0<<(registers->wgm2));
}


void configure_waveform_16b(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<(registers->wgm1))| (0<<(registers->wgm0));
	*(registers->tccrb) |= (1<<(registers->wgm3))|(1<<(registers->wgm2));
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



//timerRegisters timer0;
//timer0.tccra = &TCCR0A;
//timer0.tccrb = &TCCR0B;
//timer0.ocra = &OCR0A;
//timer0.ocrb = &OCR0B;
//timer0.cfg_wfm = configure_waveform_8b;
//timer0.set_ocr = set_ocr_8b;
//timer0.coma0 = COM0A0;
//timer0.coma1 = COM0A1;
//timer0.comb0 = COM0B0;
//timer0.comb1 = COM0B1;
//timer0.cs0 = CS00;
//timer0.cs1 = CS01;
//timer0.cs2 = CS02;
//timer0.wgm0 = WGM00;
//timer0.wgm1 = WGM01;
//timer0.wgm2 = WGM02;
//timer0.porta = Port_D;
//timer0.pina = Pin_6;
//timer0.portb = Port_D;
//timer0.pinb = Pin_5;
//----------------------------------
//timerRegisters timer1;
//timer1.tccra = &TCCR1A;
//timer1.tccrb = &TCCR1B;
//timer1.ocra = &OCR1A;
//timer1.ocrb = &OCR1B;
//timer1.cfg_wfm = configure_waveform_16b;
//timer1.set_ocr = set_ocr_16b;
//timer1.coma0 = COM1A0;
//timer1.coma1 = COM1A1;
//timer1.comb0 = COM1B0;
//timer1.comb1 = COM1B1;
//timer1.cs0 = CS10;
//timer1.cs1 = CS11;
//timer1.cs2 = CS12;
//timer1.wgm0 = WGM10;
//timer1.wgm1 = WGM11;
//timer1.wgm2 = WGM12;
//timer1.wgm3 = WGM13;
//timer1.porta = Port_B;
//timer1.pina = Pin_1;
//timer1.portb = Port_B;
//timer1.pinb = Pin_2;
