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
	portB = 1,
	portC = 2,
	portD = 3,
} GPIOPort;

typedef enum
{
	pin0 = 0x01,
	pin1 = 0x02,
	pin2 = 0x04,
	pin3 = 0x08,
	pin4 = 0x10,
	pin5 = 0x20,
	pin6 = 0x40,
	pin7 = 0x80,
}GPIOPin;

typedef enum
{
	input,
	output
}GPIOMode;

void gpio_init(GPIOPort portx, uint8_t pin, GPIOMode mode);
void gpio_set_bits(GPIOPort portx, uint16_t pin);
void gpio_clear_bits(GPIOPort portx, uint16_t pin);
void gpio_toggle_bits(GPIOPort portx, uint16_t pin);

#endif /* GPIO_H_ */
