/*
* StepperLibrary.h
*
* Created: 12/24/2015 8:35:02 PM
*  Author: Ashutosh
*/


#ifndef STEPPERLIBRARY_H_
#define STEPPERLIBRARY_H_

#include <avr/io.h>

class StepperLibrary
{
	public:
	StepperLibrary(int pin1, int pin2, int pin3, int pin4, uint8_t stepperInverted);
	void step(void);
	void setSteps(int steps);
	uint8_t InRestPosition(void);
	void stepperOff();
	void reset();

	private:
	void moveStepper(int step);
	
	char motorPin1,motorPin2,motorPin3,motorPin4;
	uint32_t stepDelay;
	int8_t lastStep;
	uint8_t invertDirection;
	int _totalSteps;
	int _currentStep;
};

#endif /* STEPPERLIBRARY_H_ */