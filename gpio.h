/*
 * gpio.h
 *
 *  Created on: Jan 6, 2019
 *      Author: kasik
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

typedef enum
{
	Port_B = 1,
	Port_C = 2,
	Port_D = 3,
} GPIOPort;

typedef enum
{
	Pin_0 = 0x01,
	Pin_1 = 0x02,
	Pin_2 = 0x04,
	Pin_3 = 0x08,
	Pin_4 = 0x10,
	Pin_5 = 0x20,
	Pin_6 = 0x40,
	Pin_7 = 0x80,
}GPIOPin;

typedef enum
{
	Input,
	Output
}GPIOMode;

void gpio_init(GPIOPort Portx, uint8_t Pin, GPIOMode Mode);
void gpio_set_bits(GPIOPort Portx, uint16_t Pin);
void gpio_clear_bits(GPIOPort Portx, uint16_t Pin);
void gpio_toggle_bits(GPIOPort Portx, uint16_t Pin);

#endif /* GPIO_H_ */
