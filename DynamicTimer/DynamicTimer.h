/*
* DynamicTimer.h
*
* Created: 1/4/2015 12:00:47 PM
*  Author: Ashutosh
*/

#include <avr/io.h>

#ifndef DYNAMICTIMER_H_
#define DYNAMICTIMER_H_

#define ONE_SHOT 0
#define PERIODIC 1

#define MAX_TIMERS	10

#define millis	DynamicTimer::GetElapsedMillis

class DynamicTimer
{
	public:
	static void AddTimer(DynamicTimer &timer);
	static void RemoveTimer(DynamicTimer &timer);
	static void RunHandler();
	static uint32_t GetElapsedMillis();
	static void Initialize(uint32_t fcpu);
	
	void Enable();
	void Disable();
	
	uint8_t Tick();
	void Clear();
	bool IsEnabled(void);
	
	DynamicTimer(uint32_t delayMilliseconds, uint8_t timerMode, void (*handler)(void));
	
	protected:

	void (*_TimerHandler)();
	uint32_t _timerDelay;
	uint8_t _enabled;
	uint8_t _ticked;
	uint8_t _mode;
	uint32_t _elapsedCounter;
	
	static DynamicTimer *_handlerList[MAX_TIMERS];
	static uint8_t _timerCount;
	static const uint8_t TimerInterval;
	static uint8_t _running;
	static uint8_t _initialized;
	static uint32_t _elapsedMilliseconds;
};


#endif /* DYNAMICTIMER_H_ */