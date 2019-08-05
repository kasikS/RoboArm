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
void gpio_init(GPIOPort Portx, uint8_t Pin, GPIOMode Mode)
{
	if (Mode == Output)
	{
		switch(Portx)
		{
			case Port_B: DDRB|= Pin; break;
			case Port_C: DDRC|= Pin; break;
			case Port_D: DDRD|= Pin; break;
			default: break;
		}
	}else
	{
		switch(Portx)
		{
			case Port_B: DDRB &= ~Pin; break;
			case Port_C: DDRC &= ~Pin; break;
			case Port_D: DDRD &= ~Pin; break;
			default: break;
		}
	}
}

/*
 * Sets specified bit
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 */
void gpio_set_bits(GPIOPort Portx, uint16_t Pin)
{
	switch(Portx)
	{
		case Port_B: PORTB |= Pin; break;
		case Port_C: PORTC |= Pin; break;
		case Port_D: PORTD |= Pin; break;
		default: break;
	}
}

/*
 * Clears specified bit
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 */
void gpio_clear_bits(GPIOPort Portx, uint16_t Pin)
{
	switch(Portx)
	{
		case Port_B: PORTB &= ~Pin; break;
		case Port_C: PORTC &= ~Pin; break;
		case Port_D: PORTD &= ~Pin; break;
		default: break;
	}
}

/*
 * Toggles specified bit
 * Portx - selected GPIO peripheral
 * Pin - specifies the port pins. This parameter can be any combination of GPIOPinx where x can be (0..7).
 */
void gpio_toggle_bits(GPIOPort Portx, uint16_t Pin)
{
	switch(Portx)
	{
		case Port_B: PORTB ^= Pin; break;
		case Port_C: PORTC ^= Pin; break;
		case Port_D: PORTD ^= Pin; break;
		default: break;
	}
}
