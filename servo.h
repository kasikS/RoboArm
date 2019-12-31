/*
 * servo.h
 *
 *  Created on: Sep 21, 2019
 *      Author: kasik
 */

#ifndef SERVO_H_
#define SERVO_H_

typedef enum
{
	servo0,
	servo1,
	servo2,
	servo3
}servo;

extern const int MIN_PWM;
extern const int MAX_PWM;

void servo_init(servo servo);
void servo_set(servo servo, uint16_t value);

#endif /* SERVO_H_ */
