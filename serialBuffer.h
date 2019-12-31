/*
 * serialBuffer.h
 *
 *  Created on: Dec 15, 2019
 *      Author: kasik
 */

#ifndef SERIALBUFFER_H_
#define SERIALBUFFER_H_

#define SERIAL_BUFFER_SIZE 20
char serialBuffer[SERIAL_BUFFER_SIZE];
int writeIndex;
int readIndex;

#endif /* SERIALBUFFER_H_ */
