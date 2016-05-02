/*
* DynamicTimer.cpp
*
* Created: 1/4/2015 12:00:10 PM
*  Author: Ashutosh

Use:
DynamicTimer(duration, mode, handler)
duration: in ms > 10 max resolution 10 ms
mode:	PERIODIC (continous), ONE_SHOT
handler: func to invoke after timer elapsed (can be null) timer can still be polled)
*/

#include "DynamicTimer.h"
#include <avr/interrupt.h>
#include <stddef.h>

DynamicTimer::DynamicTimer(uint32_t delayMilliseconds, uint8_t mode, void (*handler)(void))
{
	_enabled = false;
	_timerDelay = delayMilliseconds;
	_TimerHandler = handler;
	_elapsedCounter = delayMilliseconds / TimerInterval;
	_ticked = 0;
	
	if(mode == ONE_SHOT || mode == PERIODIC)
	{
		_mode = mode;
	}
	else
	{
		_mode = PERIODIC;
	}
}

uint8_t const DynamicTimer::TimerInterval = 1;
uint8_t DynamicTimer::_running = false;

bool DynamicTimer::IsEnabled(void)
{
	return _enabled;
}

uint8_t DynamicTimer::_timerCount = 0;
DynamicTimer* DynamicTimer::_handlerList[10];
uint8_t DynamicTimer::_initialized = false;
uint32_t DynamicTimer::_elapsedMilliseconds = 0;

void DynamicTimer::RemoveTimer(DynamicTimer &timer)
{
	timer.Disable();
}

void DynamicTimer::AddTimer(DynamicTimer &timer)
{
	cli();
	_handlerList[_timerCount++] = &timer;
	sei();
}

void DynamicTimer::Initialize(uint32_t fcpu)
{
	if(_initialized)
	{
		return;
	}
	
	cli();

	_running = false;

	uint8_t timerCount = ((fcpu / 64) * TimerInterval) / 1000;
	
	#ifdef __AVR_ATtiny85__
	TCCR1 = (1 << CTC1);
	TIMSK |= (1 << OCIE1A);
	OCR1C = timerCount;			// 10ms at F_CPU / 1024 and 1 MHz
	TCCR1 |= 0b0111;	// CLK/64
	#else
	// Use timer0
	TCCR0A |= (1 << WGM01);
	OCR0A = timerCount;	// 10ms at F_CPU / 64 and 1 MHz ; 157 = 10ms
	TCCR0B |= ((1 << CS00) | (1 << CS01));
	TIMSK0 |= (1 << OCIE0A);
	#endif
	
	_initialized = true;
	
	sei();
}

void DynamicTimer::Enable()
{
	_enabled = true;
	_ticked = false;
}

void DynamicTimer::Disable()
{
	_enabled = false;
}

void DynamicTimer::Clear()
{
	_ticked = false;
}

uint32_t DynamicTimer::GetElapsedMillis()
{
	uint32_t val = 0;
	
	cli();
	val = _elapsedMilliseconds;
	sei();
	
	return val;
}

void DynamicTimer::RunHandler()
{
	_elapsedMilliseconds += TimerInterval;

	if(!_running)
	{
		_running = true;
		
		if(_timerCount)
		{
			for(int i = 0;i < _timerCount;i++)
			{
				DynamicTimer* timer = _handlerList[i];
				// Loop through the timers and decrement the counters
				if(timer != NULL && timer->_enabled)
				{
					// Reset and run the handlers for the ones that are zero
					if(!--(timer->_elapsedCounter))
					{
						timer->_elapsedCounter = timer->_timerDelay / DynamicTimer::TimerInterval;
						
						if(timer->_TimerHandler != NULL)
						{
							timer->_TimerHandler();
						}
						
						timer->_ticked = true;
						
						if(timer->_mode == ONE_SHOT)
						{
							timer->Disable();
						}
					}
				}
			}
		}
		
		_running = false;
	}
}

uint8_t DynamicTimer::Tick()
{
	return _ticked;
}

#ifdef __AVR_ATtiny85__
ISR(TIMER1_COMPA_vect)
{
	DynamicTimer::RunHandler();
}
#else
ISR(TIMER0_COMPA_vect)
{
	DynamicTimer::RunHandler();
}
#endif