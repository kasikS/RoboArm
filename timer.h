/*
 * timer.h
 *
 *  Created on: Jul 15, 2019
 *      Author: kasik
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef enum
{
	p1,
	p8,
	p64,
	p256,
	p1024
} Prescaler;

typedef enum
{
	timer0,
	timer1,
	timer2
}Timer;

void timer_init(Timer timer);
void timer_init_gpio(Timer timer);
void timer_init_channels(Timer timer);
void timer_set_prescaler(Timer timer, Prescaler prescaler);
void timer_configure_waveform(Timer timer);
int timer_set_ocr(Timer timer, int channel, int counts);

#endif /* TIMER_H_ */
