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

#define MAX_COUNT	255
#define PRESCALER	p1024

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
		const int *prescaler;
		void (*cfg_wfm)(struct timerRegisters *registers);
		int (*set_ocr)(struct timerRegisters *registers, int channel, int counts);
}timerRegisters;

static const int prescaler_t0[] = {0b001, 0b010, 0b011, 0b100, 0b101};
static const int prescaler_t2[] = {0b001, 0b010, 0b100, 0b110, 0b111};

static void configure_waveform_8b(timerRegisters *registers);
static void configure_waveform_16b(timerRegisters *registers);
static int set_ocr_8b(timerRegisters *registers, int channel, int counts);
static int set_ocr_16b(timerRegisters *registers, int channel, int counts);

static const timerRegisters timer0Reg =
{
	.tccra = &TCCR0A,
	.tccrb = &TCCR0B,
	.ocra = &OCR0A,
	.ocrb = &OCR0B,
	.cfg_wfm = configure_waveform_8b,
	.set_ocr = set_ocr_8b,
	.coma0 = COM0A0,
	.coma1 = COM0A1,
	.comb0 = COM0B0,
	.comb1 = COM0B1,
	.cs0 = CS00,
	.cs1 = CS01,
	.cs2 = CS02,
	.wgm0 = WGM00,
	.wgm1 = WGM01,
	.wgm2 = WGM02,
	.prescaler = prescaler_t0,
	.porta = Port_D,
	.pina = Pin_6,
	.portb = Port_D,
	.pinb = Pin_5
};

static const timerRegisters timer1Reg =
{
	.tccra = &TCCR1A,
	.tccrb = &TCCR1B,
	.ocra = &OCR1A,
	.ocrb = &OCR1B,
	.cfg_wfm = configure_waveform_16b,
	.set_ocr = set_ocr_16b,
	.coma0 = COM1A0,
	.coma1 = COM1A1,
	.comb0 = COM1B0,
	.comb1 = COM1B1,
	.cs0 = CS10,
	.cs1 = CS11,
	.cs2 = CS12,
	.wgm0 = WGM10,
	.wgm1 = WGM11,
	.wgm2 = WGM12,
	.wgm3 = WGM13,
	.prescaler = prescaler_t0,
	.porta = Port_B,
	.pina = Pin_1,
	.portb = Port_B,
	.pinb = Pin_2
};

static const timerRegisters timer2Reg =
{
	.tccra = &TCCR2A,
	.tccrb = &TCCR2B,
	.ocra = &OCR2A,
	.ocrb = &OCR2B,
	.cfg_wfm = configure_waveform_8b,
	.set_ocr = set_ocr_8b,
	.coma0 = COM2A0,
	.coma1 = COM2A1,
	.comb0 = COM2B0,
	.comb1 = COM2B1,
	.cs0 = CS20,
	.cs1 = CS21,
	.cs2 = CS22,
	.wgm0 = WGM20,
	.wgm1 = WGM21,
	.wgm2 = WGM22,
	.prescaler = prescaler_t2,
	.porta = Port_D,
	.pina = Pin_3,
	.portb = Port_B,
	.pinb = Pin_3
};

static const timerRegisters *const timerRegistersArray[] = {&timer0Reg, &timer1Reg, &timer2Reg}; //try whether it can be modified!

/*
 * \brief initializes timer pins as outputs
 * \param timer specifies the timer
 */
void timer_init_gpio(Timer timer)//timerRegisters *registers)
{
	gpio_init(timerRegistersArray[timer]->porta, timerRegistersArray[timer]->pina, Output); // set pin of channel A as output
	gpio_init(timerRegistersArray[timer]->portb, timerRegistersArray[timer]->pinb, Output); // set pin of channel A as output
}

/*
 * \brief initializes the timer to clear the output on counter match
 * \param specifes the timer
 */
void timer_init_channels(Timer timer)//(timerRegisters *registers)
{
	*(timerRegistersArray[timer]->tccra) |= (1 << timerRegistersArray[timer]->coma1) | (0 << timerRegistersArray[timer]->coma0); // init channel A
	*(timerRegistersArray[timer]->tccra) |= (1 << timerRegistersArray[timer]->comb1) | (0 << timerRegistersArray[timer]->comb0); // init channel B
}

/*
 * \brief sets the clock prescaler
 * \param timer specifies the timer
 * \param prescaler specifies the value of the prescaler
 */
void timer_set_prescaler(Timer timer, Prescaler prescaler)//(timerRegisters *registers)
{

	int prescalerValue = timerRegistersArray[timer]->prescaler[prescaler];
	*(timerRegistersArray[timer]->tccrb) |= ( ((prescalerValue >> 2)&1) << timerRegistersArray[timer]->cs2)| (((prescalerValue >> 1)&1) << timerRegistersArray[timer]->cs1) | (((prescalerValue >> 0)&1) << timerRegistersArray[timer]->cs0);
}

/*
 * \brief configures the waveform generation mode - fast non inverted PWM
 * \param timer specifies the timer
 */
void timer_configure_waveform(Timer timer) //timerRegisters *registers)
{
	timerRegistersArray[timer]->cfg_wfm(timerRegistersArray[timer]);
}

/*
 * \brief initializes timer: initializes GPIO, channels, configures waveform generation mode, sets prescaler
 * \param timer specifies the timer
 * \param prescaler specifies the value of prescaler
 */
void timer_init(Timer timer)
{
	timer_init_gpio(timer);
	timer_init_channels(timer);
	timer_configure_waveform(timer);
	timer_set_prescaler(timer, p1024); //~60 Hz is needed; prescaler = clock/(f*MAX_COUNT)
}

/*
 * \brief sets the compare value
 * \param timer specifies the timer
 * \param counts compare value
 */
int set_ocr(Timer timer, int channel, int counts)
{
	if (channel > 1) return 0;
	if (counts > MAX_COUNT) return 0;

	timerRegistersArray[timer]->set_ocr(timerRegistersArray[timer], channel, counts);

	return 1;
}

/*
 * \brief configure waveform generation mode on 8bit timer
 * \praram registers timer's register structure
 */
static void configure_waveform_8b(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<(registers->wgm1))| (1<<(registers->wgm0));
	*(registers->tccrb) |= (0<<(registers->wgm2));
}

/*
 * \brief configure waveform generation mode on 16bit timer
 * \praram registers timer's register structure
 */
static void configure_waveform_16b(timerRegisters *registers)
{
	*(registers->tccra) |= (1<<(registers->wgm1))| (0<<(registers->wgm0));
	*(registers->tccrb) |= (1<<(registers->wgm3))|(1<<(registers->wgm2));
	ICR1 = 255;
}

/*
 * \brief sets the compare value on 8bit counter
 * \praram registers timer's register structure
 */
static int set_ocr_8b(timerRegisters *registers, int channel, int counts)
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

/*
 * \brief sets the compare value on 16bit counter
 * \praram registers timer's register structure
 */
static int set_ocr_16b(timerRegisters *registers, int channel, int counts)
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
