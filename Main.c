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
#include <avr/interrupt.h>
#include "serialBuffer.h"

#define F_CPU 16000000UL

//servo:
// 1 d11 - PB3 -D3?????
// 2 d10 - PB2
// 3 d9  - PB1
// 4 d6  - PD6


typedef struct
{
	uint32_t adcData;
	uint32_t minPwm;
	uint32_t maxPwm;
	uint32_t rangePwm;
	uint32_t countsPwm;
}joint;

#define MAX_ANGLE		180
#define MIN_ANGLE		0
#define RANGE_ANGLE		(MAX_ANGLE-MIN_ANGLE)

int main (void)
{
	gpio_init(portB, pin5, output); //led

	serial_init(9600);

	servo_init(servo0);
	servo_init(servo1);
	servo_init(servo2);
	servo_init(servo3);

	adc_init();
	adc_channel_init(adc0);
	adc_channel_init(adc1);
	adc_channel_init(adc2);
	adc_channel_init(adc3);

	char c;
	sei(); //enable interrupts


	//buffer for characters after itoa
	uint8_t chars[12] = {0,};

	//number of characters to send via serial
	uint32_t length=0;

	joint jointArray[4] = {
			{.minPwm = 6, .maxPwm = 44},
			{.minPwm = 7, .maxPwm = 26},
			{.minPwm = 9, .maxPwm = 30},
			{.minPwm = 6, .maxPwm = 15}
	};

	for(int i = 0; i < 4; i++)
	{
		jointArray[i].rangePwm = jointArray[i].maxPwm - jointArray[i].minPwm;
	}

	char commandArray[16];
	int commandIndex=0;

	int degrees = 0;
	int joint = 0;
	int mutex = 1;


	while(1)
	{
		gpio_toggle_bits(portB, pin5); // toggle led

		if(get_char(&c)) //check whether new character arrived
		{

			if(c == '\r') //check whether command submitted
			{
				if(((commandArray[0] >= '0') && (commandArray[0] <= '3')) && (mutex==0) && (commandIndex > 0))
				{
					joint = toi(1, &commandArray[0],1); 	//check which motor
					degrees = toi(commandIndex-1, &commandArray[1],1); 	//convert requested degrees to number

					length = itoa(joint, chars);
					length = itoa(degrees, chars);

					//calculate counts for requested angle
					jointArray[joint].countsPwm = ((MAX_PWM-MIN_PWM)*(degrees-MIN_ANGLE))/RANGE_ANGLE + MIN_PWM;

					//check ranges of the move
					if(jointArray[joint].countsPwm > jointArray[joint].maxPwm)
					{
						jointArray[joint].countsPwm = jointArray[joint].maxPwm;
					}else if(jointArray[joint].countsPwm < jointArray[joint].minPwm)
					{
						jointArray[joint].countsPwm = jointArray[joint].minPwm;
					}

					servo_set(joint, jointArray[joint].countsPwm);

				}else if ((commandArray[0] == 'r') && (commandIndex == 1)) //release
				{
					mutex = 1;
				}else if ((commandArray[0] == 'c') && (commandIndex == 1)) //claim
				{
					mutex = 0;
				}

				commandIndex = 0;
			}
			else //not submitted yet, add to the array
			{
				commandArray[commandIndex] = c;
				commandIndex = (commandIndex +1) %16;
			}
		}

		//use adc only if not reserved remotely!
		if(mutex)
		{
			for(int i = 0; i < 4; i++)
			{
				jointArray[i].adcData = adc_read(i);
				jointArray[i].countsPwm = (jointArray[i].rangePwm * jointArray[i].adcData)/MAX_COUNT_ADC + jointArray[i].minPwm;
				servo_set(i, jointArray[i].countsPwm);
			};
		}

		_delay_ms(10);

	}
	return 0;
}
