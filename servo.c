/*
 * servo.c
 *
 *  Created on: Sep 21, 2019
 *      Author: kasik
 */

#include <stdint.h>
#include "servo.h"
#include "timer.h"
#include <avr/io.h>

//// OCR value for 0 position
const int MAX_POSITION = 44;
// OCR value for 180 position
const int MIN_POSITION = 6;

typedef struct
{
	Timer timer;
	int channel;
}servoData;

static const servoData servo0Data =
{
		.timer = timer2,
		.channel = 1
};

static const servoData servo1Data =
{
		.timer = timer1,
		.channel = 1
};

static const servoData servo2Data =
{
		.timer = timer1,
		.channel = 0
};

static const servoData servo3Data =
{
		.timer = timer0,
		.channel = 0
};

static const servoData *const servoDataArray[4] = {&servo0Data, &servo1Data, &servo2Data, &servo3Data};


/**
 * \brief Initializes resources for selected servo
 *
 * \param servo is the selected servo for initialization
 */
void servo_init(servo servo)
{
	//~60 Hz is needed; prescaler = clock/(f*MAX_COUNT)
	timer_init(servoDataArray[servo]->timer, p1024);
}


/**
 * \brief Sets the position in timer counts
 *
 * \param servo is the selected servo
 * \param value is the ocr value for the timer
 */
void servo_set(servo servo, uint16_t value)
{
	//Check whether requested position is in bounds
	if(value > MAX_POSITION)
	{
		value = MAX_POSITION;
	}
	else if(value < MIN_POSITION)
	{
		value = MIN_POSITION;
	}

	set_ocr(servoDataArray[servo]->timer, servoDataArray[servo]->channel, value);
}
