/*
 * adc.h
 *
 *  Created on: Sep 23, 2019
 *      Author: kasik
 */

#ifndef ADC_H_
#define ADC_H_

typedef enum
{
	adc0,
	adc1,
	adc2,
	adc3
}adc;

extern const int MAX_COUNT_ADC;

void adc_init(void);
void adc_channel_init(adc adc);
uint16_t adc_read(adc adc);

#endif /* ADC_H_ */
