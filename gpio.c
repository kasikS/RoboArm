/*
 * gpio.c
 *
 *  Created on: Jan 6, 2019
 *      Author: kasik
 */

#include "gpio.h"

/*
 * Configures specified pin as input/output
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 * Mode - output/input
 */
void gpio_init(GPIOPort portx, uint8_t pin, GPIOMode mode)
{
	if (mode == output)
	{
		switch(portx)
		{
			case portB: DDRB|= pin; break;
			case portC: DDRC|= pin; break;
			case portD: DDRD|= pin; break;
			default: break;
		}
	}else
	{
		switch(portx)
		{
			case portB: DDRB &= ~pin; break;
			case portC: DDRC &= ~pin; break;
			case portD: DDRD &= ~pin; break;
			default: break;
		}
	}
}

/*
 * Sets specified bit
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 */
void gpio_set_bits(GPIOPort portx, uint16_t pin)
{
	switch(portx)
	{
		case portB: PORTB |= pin; break;
		case portC: PORTC |= pin; break;
		case portD: PORTD |= pin; break;
		default: break;
	}
}

/*
 * Clears specified bit
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 */
void gpio_clear_bits(GPIOPort portx, uint16_t pin)
{
	switch(portx)
	{
		case portB: PORTB &= ~pin; break;
		case portC: PORTC &= ~pin; break;
		case portD: PORTD &= ~pin; break;
		default: break;
	}
}

/*
 * Toggles specified bit
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 */
void gpio_toggle_bits(GPIOPort portx, uint16_t pin)
{
	switch(portx)
	{
		case portB: PORTB ^= pin; break;
		case portC: PORTC ^= pin; break;
		case portD: PORTD ^= pin; break;
		default: break;
	}
}
