/*
* AsyncBlink.cpp
*
* Created: 2/28/2016 9:55:54 AM
* Author: Ashutosh
*/

#define F_CPU 1000000UL
#define BAUD_RATE	9600

#include "AsyncBlink.h"

// default constructor
AsyncBlink::AsyncBlink(uint32_t pattern, uint8_t length, uint8_t repeat) : _index(0), _repeat(repeat), _enabled(false), _currentRun(repeat)
{
	_pattern = pattern;
	_patternLength = length;
} //AsyncBlink

void AsyncBlink::SetPattern(uint32_t pattern, uint8_t length, uint8_t repeat, uint8_t overwrite)
{
	if(!_enabled || overwrite)
	{	
		_enabled = false;

		_patternLength = length;
		_pattern = pattern;
	
		_currentRun = _repeat = repeat;
		_index = 0;

		_enabled = true;
	}
}

uint8_t AsyncBlink::GetNextState(void)
{
	if(!_enabled)
	{
		return false;
	}
	
	uint8_t val = ((_pattern >> _index) & 1);

	_index++;
	
	if(_index == _patternLength)
	{
		_index = 0;

		if(!--_currentRun)
		{
			_enabled = false;
			_currentRun = _repeat;
		}
	}

	return val;
}
