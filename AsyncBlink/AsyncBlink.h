/*
* AsyncBlink.h
*
* Created: 2/28/2016 9:55:54 AM
* Author: Ashutosh
*/


#ifndef __ASYNCBLINK_H__
#define __ASYNCBLINK_H__

#include <stdint.h>

class AsyncBlink
{
	//variables
	public:
	protected:
	uint32_t _pattern;
	uint8_t _patternLength;
	uint8_t _index;
	uint8_t _repeat;
	uint8_t _enabled;
	uint8_t _currentRun;
	
	private:

	//functions
	public:
	AsyncBlink(uint32_t pattern, uint8_t length, uint8_t repeat = true);
	uint8_t GetNextState(void);
	void SetPattern(uint32_t pattern, uint8_t length, uint8_t repeat, uint8_t overwrite = true);
	void Enable() { _enabled = true; }
	void Disable() { _enabled = false; }
		
	protected:
	private:

}; //AsyncBlink

#endif //__ASYNCBLINK_H__
