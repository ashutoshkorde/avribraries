/*
* StepperLibrary.cpp
*
* Created: 12/24/2015 8:34:45 PM
*  Author: Ashutosh
*/

#define F_CPU	1000000UL
#define BAUD_RATE	9600

//#define _LOG_

#include "ArduinoPort/ArduinoPort.h"
#include <stdlib.h>
#include "StepperLibrary.h"
#include "BasicSerial/BasicSerial.h"

StepperLibrary::StepperLibrary(int pin1, int pin2, int pin3, int pin4, uint8_t invertControl = false)
{
	this->motorPin1 = pin1;
	this->motorPin2 = pin2;
	this->motorPin3 = pin3;
	this->motorPin4 = pin4;
	
	this->invertDirection = invertControl;
	this->lastStep = 0;
	this->_totalSteps = 0;
	this->_currentStep = 0;
	
	pinMode(motorPin1, OUTPUT);
	pinMode(motorPin2, OUTPUT);
	pinMode(motorPin3, OUTPUT);
	pinMode(motorPin4, OUTPUT);
}

void StepperLibrary::setSteps(int16_t steps)
{
	if(steps < 0)
	{
		_currentStep = _currentStep - steps;
	}
	else
	{
		_totalSteps += steps;
	}
	
	if(_currentStep > 0 && _totalSteps > 0)
	{
		uint16_t diff = _currentStep > _totalSteps ? _totalSteps : _currentStep;
		_currentStep -= diff;
		_totalSteps -= diff;
	}
	
	TxLong(_currentStep);
	TxByte(':');
	TxLong(_totalSteps);
	TxByte('\n');
}

uint8_t StepperLibrary::InRestPosition(void)
{
	return _currentStep == _totalSteps;
}

void StepperLibrary::step(void)
{
	TxLong(_currentStep);
	TxByte(':');
	TxLong(_totalSteps);
	TxByte('\n');

	if(_currentStep != _totalSteps)
	{
		int8_t direction = _currentStep < _totalSteps ? 1 : -1;
		if(invertDirection)
		{
			direction *= -1;
		}
		
		moveStepper(_currentStep);
		
		_currentStep += direction;
	}
}

void StepperLibrary::moveStepper(int16_t step)
{
	switch (step % 4) {
		case 0:    // 1100
		digitalWrite(motorPin1, HIGH);
		digitalWrite(motorPin2, HIGH);
		digitalWrite(motorPin3, LOW);
		digitalWrite(motorPin4, LOW);
		break;
		case 1:    // 0110
		digitalWrite(motorPin1, LOW);
		digitalWrite(motorPin2, HIGH);
		digitalWrite(motorPin3, HIGH);
		digitalWrite(motorPin4, LOW);
		break;
		case 2:    //0011
		digitalWrite(motorPin1, LOW);
		digitalWrite(motorPin2, LOW);
		digitalWrite(motorPin3, HIGH);
		digitalWrite(motorPin4, HIGH);
		break;
		case 3:    //1001
		digitalWrite(motorPin1, HIGH);
		digitalWrite(motorPin2, LOW);
		digitalWrite(motorPin3, LOW);
		digitalWrite(motorPin4, HIGH);
		break;
	}
}

void StepperLibrary::reset()
{
	_totalSteps = 0;
	_currentStep = 0;
}

void StepperLibrary::stepperOff()
{
	digitalWrite(motorPin1, LOW);
	digitalWrite(motorPin2, LOW);
	digitalWrite(motorPin3, LOW);
	digitalWrite(motorPin4, LOW);
}

