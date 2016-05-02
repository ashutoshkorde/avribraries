/*
 * ArduinoPort.h
 *
 * Created: 1/30/2016 6:55:30 PM
 *  Author: Ashutosh
 */ 


#ifndef ARDUINOPORT_H_
#define ARDUINOPORT_H_
#define F_CPU	1000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include "util/delay.h"
//#include <inttypes.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define pinMode	ArduinoPort::_pinMode
#define digitalWrite(x, y)	(ArduinoPort::_digitalWrite(x, y))
#define digitalRead(x)	(ArduinoPort::_digitalRead(x))
#define analogRead(x)	(ArduinoPort::_analogRead(x))
#define analogReference(x)	(ArduinoPort::_analogReference(x))
#define map	ArduinoPort::_map

#define  delay(x)	_delay_ms(x)

#define HIGH	1
#define	LOW		0

#define OUTPUT 1
#define INPUT 0
#define INPUT_PULLUP 2

#define DEFAULT	1
#define INTERNAL 3
#define EXTERNAL 0

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define A0	14
#define A1	15
#define A2	16
#define A3	17
#define A4	18
#define A5	19
#define A6	20
#define A7	21

class ArduinoPort
{
	public:
		static void _pinMode(uint8_t pin, uint8_t mode);
		static void _digitalWrite(uint8_t pin, uint8_t value);
		static uint8_t _digitalRead(uint8_t pin);
		static uint16_t _analogRead(uint8_t pin);
		static volatile uint8_t*  MapRegister(uint8_t& pin, volatile uint8_t** ports);
		static void _analogReference(uint8_t reference);
		static void _analogWrite(uint8_t pin, uint8_t val);
		//static volatile uint8_t* MapPortRegister(uint8_t& pin);
		static int32_t _map(int32_t inputVal, int32_t inputLow, int32_t inputHigh, int32_t outputMin, int32_t outputMax); 	// (val, 0, 1023, 0, REF_VOLTAGE)
		static void _init();

		static volatile uint8_t* DirectionPorts[3];
		static volatile uint8_t* OutputPorts[3];
		static volatile uint8_t* InputPorts[3];		
		
	private:
		static uint8_t analog_reference;
	};
	
#endif /* ARDUINOPORT_H_ */