/*
 * adc.c
 *
 *  Created on: Sep 23, 2019
 *      Author: kasik
 */


#include <stdint.h>
#include "gpio.h"
#include "adc.h"
#include <avr/io.h>

const int MAX_COUNT_ADC = 1023;

typedef struct
{
	GPIOPort portx;
	uint8_t pin;
}adcData;

static const adcData adc0Data =
{
		.portx = portC,
		.pin = 0
};

static const adcData adc1Data =
{
		.portx = portC,
		.pin = 1
};

static const adcData adc2Data =
{
		.portx = portC,
		.pin = 2
};

static const adcData adc3Data =
{
		.portx = portC,
		.pin = 3
};

static const adcData * const adcDataArray[4] = {&adc0Data, &adc1Data, &adc2Data, &adc3Data };


/**
 * \brief Initializes adc
 *
 */
void adc_init(void)
{
	PRR &= !(1<<0);
	ADMUX |= 0 <<7 | 1 << 6;			//vcc as ref
	ADCSRA |= 1 << 2 | 1 <<1 | 1 << 0; //prescaler to 128 ->make it selectable?
	ADCSRA |= 1 << 7; //enable
}


/**
 * \brief Initializes channel od adc
 *
 * \param adc is the selected adc for initialization
 */
void adc_channel_init(adc adc)
{
	gpio_init(adcDataArray[adc]->portx, adcDataArray[adc]->pin, input);
}


/**
 * \brief Read the adc value
 *
 * \param adc is the selected adc for initialization
 */
uint16_t adc_read(adc adc)
{
	uint16_t adcValue = 0;
	ADMUX = adc; //select channel

	ADCSRA |= 1 << 6; // start conversion
	while(ADCSRA & (1<<6));
	adcValue = ADCL; //read low byte
	adcValue |= (ADCH << 8); //read high byte

	return adcValue;
}
