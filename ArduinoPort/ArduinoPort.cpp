/*
* ArduinoPort.cpp
*
* Created: 1/30/2016 7:17:58 PM
*  Author: Ashutosh
*/

#include "avr\pgmspace.h"

#include "ArduinoPort.h"
#include "avr/io.h"
#include <avr/interrupt.h>

#ifdef __AVR_ATtiny85__
volatile uint8_t* ArduinoPort::DirectionPorts[] = {&DDRB};
volatile uint8_t* ArduinoPort::OutputPorts[] = {&PORTB};
volatile uint8_t* ArduinoPort::InputPorts[] = {&PINB};
#else
volatile uint8_t* ArduinoPort::DirectionPorts[] = {&DDRD, &DDRB, &DDRC};
volatile uint8_t* ArduinoPort::OutputPorts[] = {&PORTD, &PORTB, &PORTC};
volatile uint8_t* ArduinoPort::InputPorts[] = {&PIND, &PINB, &PINC};
#endif

const uint8_t  digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER2B,
	NOT_ON_TIMER,
	TIMER0B,
	TIMER0A,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 8 - port B */
	TIMER1A,
	TIMER1B,
	TIMER2A,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

uint8_t  ArduinoPort::analog_reference = DEFAULT;

void ArduinoPort::_init()
{
	// this needs to be called before setup() or some functions won't
	// work there
	sei();
	
	//// on the ATmega168, timer 0 is also used for fast hardware pwm
	//// (using phase-correct PWM would mean that timer 0 overflowed half as often
	//// resulting in different millis() behavior on the ATmega8 and ATmega168)
	//#if defined(TCCR0A) && defined(WGM01)
	//bitSet(TCCR0A, WGM01);
	//bitSet(TCCR0A, WGM00);
	//#endif
//
	//#if defined(TIMSK0) && defined(TOIE0)
	//bitSet(TIMSK0, TOIE0);
	//#else
	//#error	Timer 0 overflow interrupt not set correctly
	//#endif
//
	//// timers 1 and 2 are used for phase-correct hardware pwm
	//// this is better for motors as it ensures an even waveform
	//// note, however, that fast pwm mode can achieve a frequency of up
	//// 8 MHz (with a 16 MHz clock) at 50% duty cycle
//
	//#if defined(TCCR1B) && defined(CS11) && defined(CS10)
	//TCCR1B = 0;
//
	//// set timer 1 prescale factor to 64
	//bitSet(TCCR1B, CS11);
	//#if F_CPU >= 8000000L
	//bitSet(TCCR1B, CS10);
	//#endif
	//#elif defined(TCCR1) && defined(CS11) && defined(CS10)
	//bitSet(TCCR1, CS11);
	//#if F_CPU >= 8000000L
	//bitSet(TCCR1, CS10);
	//#endif
	//#endif
	//// put timer 1 in 8-bit phase correct pwm mode
	//#if defined(TCCR1A) && defined(WGM10)
	//bitSet(TCCR1A, WGM10);
	//#endif
//
	//// set timer 2 prescale factor to 64
	//#if defined(TCCR2) && defined(CS22)
	//bitSet(TCCR2, CS22);
	//#elif defined(TCCR2B) && defined(CS22)
	//bitSet(TCCR2B, CS22);
	////#else
	//// Timer 2 not finished (may not be present on this CPU)
	//#endif
//
	//// configure timer 2 for phase correct pwm (8-bit)
	//#if defined(TCCR2) && defined(WGM20)
	//bitSet(TCCR2, WGM20);
	//#elif defined(TCCR2A) && defined(WGM20)
	//bitSet(TCCR2A, WGM20);
	////#else
	//// Timer 2 not finished (may not be present on this CPU)
	//#endif
//
	//#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
	//bitSet(TCCR3B, CS31);		// set timer 3 prescale factor to 64
	//bitSet(TCCR3B, CS30);
	//bitSet(TCCR3A, WGM30);		// put timer 3 in 8-bit phase correct pwm mode
	//#endif
//
	//#if defined(TCCR4A) && defined(TCCR4B) && defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
	//bitSet(TCCR4B, CS42);		// set timer4 prescale factor to 64
	//bitSet(TCCR4B, CS41);
	//bitSet(TCCR4B, CS40);
	//bitSet(TCCR4D, WGM40);		// put timer 4 in phase- and frequency-correct PWM mode
	//bitSet(TCCR4A, PWM4A);		// enable PWM mode for comparator OCR4A
	//bitSet(TCCR4C, PWM4D);		// enable PWM mode for comparator OCR4D
	//#else /* beginning of timer4 block for ATMEGA1280 and ATMEGA2560 */
	//#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
	//bitSet(TCCR4B, CS41);		// set timer 4 prescale factor to 64
	//bitSet(TCCR4B, CS40);
	//bitSet(TCCR4A, WGM40);		// put timer 4 in 8-bit phase correct pwm mode
	//#endif
	//#endif /* end timer4 block for ATMEGA1280/2560 and similar */
//
	//#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
	//bitSet(TCCR5B, CS51);		// set timer 5 prescale factor to 64
	//bitSet(TCCR5B, CS50);
	//bitSet(TCCR5A, WGM50);		// put timer 5 in 8-bit phase correct pwm mode
	//#endif
	
	#if defined(ADCSRA)
	// set a2d prescaler so we are inside the desired 50-200 KHz range.
	#if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
	bitSet(ADCSRA, ADPS2);
	bitSet(ADCSRA, ADPS1);
	bitSet(ADCSRA, ADPS0);
	#elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
	bitSet(ADCSRA, ADPS2);
	bitSet(ADCSRA, ADPS1);
	bitClear(ADCSRA, ADPS0);
	#elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
	bitSet(ADCSRA, ADPS2);
	bitClear(ADCSRA, ADPS1);
	bitSet(ADCSRA, ADPS0);
	#elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
	bitSet(ADCSRA, ADPS2);
	bitClear(ADCSRA, ADPS1);
	bitClear(ADCSRA, ADPS0);
	#elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
	bitClear(ADCSRA, ADPS2);
	bitSet(ADCSRA, ADPS1);
	bitSet(ADCSRA, ADPS0);
	#else // 128 kHz / 2 = 64 KHz -> This is the closest you can get, the prescaler is 2
	bitClear(ADCSRA, ADPS2);
	bitClear(ADCSRA, ADPS1);
	bitSet(ADCSRA, ADPS0);
	#endif
	// enable a2d conversions
	bitSet(ADCSRA, ADEN);
	#endif
}

int32_t ArduinoPort::_map(int32_t inputVal, int32_t inputLow, int32_t inputHigh, int32_t outputMin, int32_t outputMax)
{
	return (inputVal - inputLow) * (outputMax - outputMin) / (inputHigh - inputLow) + outputMin;
}

void ArduinoPort::_analogReference(uint8_t reference)
{
	// Should be < than 4 and not reserved
	if(reference <= 4 && reference != 2)
	{
		analog_reference = reference;
	}
}

void ArduinoPort::_pinMode(uint8_t pin, uint8_t mode)
{
	if(pin > 21)
	{
		return;
	}
	
	uint8_t direction = (mode == OUTPUT);
	
	uint8_t temp = pin;
	volatile uint8_t *directionRegister = MapRegister(temp, ArduinoPort::DirectionPorts);
	bitWrite(*directionRegister, temp, direction);
	
	if(mode == INPUT_PULLUP)
	{
		digitalWrite(pin, HIGH);
	}
}

uint16_t ArduinoPort::_analogRead(uint8_t pin)
{
	pin -= 14;
	ADMUX = (analog_reference << 6) | (pin & 0x07);
	
	// Start conversion
	bitSet(ADCSRA, ADSC);
	
	// ADSC is cleared when the conversion finishes
	while (bitRead(ADCSRA, ADSC));
	delay(1);
	
	uint8_t low  = ADCL;
	uint8_t high = ADCH;

	// combine the two bytes
	return (high << 8) | low;
}

void ArduinoPort::_analogWrite(uint8_t pin, uint8_t val)
{
	_pinMode(pin, OUTPUT);
	
	if (val == 0 || pin > 13)
	{
		_digitalWrite(pin, LOW);
	}
	else if (val == 255)
	{
		_digitalWrite(pin, HIGH);
	}
	else
	{
		switch(digital_pin_to_timer_PGM[pin])
		{
			#if defined(TCCR0A) && defined(COM0A1)
			case TIMER0A:
			// connect pwm to pin on timer 0, channel A
			bitSet(TCCR0A, COM0A1);
			OCR0A = val; // set pwm duty
			break;
			#endif

			#if defined(TCCR0A) && defined(COM0B1)
			case TIMER0B:
			// connect pwm to pin on timer 0, channel B
			bitSet(TCCR0A, COM0B1);
			OCR0B = val; // set pwm duty
			break;
			#endif

			#if defined(TCCR1A) && defined(COM1A1)
			case TIMER1A:
			// connect pwm to pin on timer 1, channel A
			bitSet(TCCR1A, COM1A1);
			OCR1A = val; // set pwm duty
			break;
			#endif

			#if defined(TCCR1A) && defined(COM1B1)
			case TIMER1B:
			// connect pwm to pin on timer 1, channel B
			bitSet(TCCR1A, COM1B1);
			OCR1B = val; // set pwm duty
			break;
			#endif

			#if defined(TCCR2A) && defined(COM2A1)
			case TIMER2A:
			// connect pwm to pin on timer 2, channel A
			bitSet(TCCR2A, COM2A1);
			OCR2A = val; // set pwm duty
			break;
			#endif

			#if defined(TCCR2A) && defined(COM2B1)
			case TIMER2B:
			// connect pwm to pin on timer 2, channel B
			bitSet(TCCR2A, COM2B1);
			OCR2B = val; // set pwm duty
			break;
			#endif

			case NOT_ON_TIMER:
			default:
			if (val < 128) {
				digitalWrite(pin, LOW);
				} else {
				digitalWrite(pin, HIGH);
			}
		}
	}
}

volatile uint8_t*  ArduinoPort::MapRegister(uint8_t& pin, volatile uint8_t** ports)
{
	volatile uint8_t *directionRegister;
	
	// Based of ATMega328P
	if(pin <= 7)
	{
		directionRegister = ports[0];
		// D0-D7
	}
	else if(pin <= 14)
	{
		// Map to B0-B5
		pin -= 8;
		directionRegister = ports[1];
	}
	else
	{
		pin -= 14;
		directionRegister = ports[2];
	}
	
	return directionRegister;
}

void ArduinoPort::_digitalWrite(uint8_t pin, uint8_t value)
{
	volatile uint8_t* portRegister = MapRegister(pin, ArduinoPort::OutputPorts);
	
	value = value > 0;
	
	bitWrite(*portRegister, pin, value);
}

uint8_t ArduinoPort::_digitalRead(uint8_t pin)
{
	volatile uint8_t* portRegister = MapRegister(pin, ArduinoPort::InputPorts);
	
	return bitRead(*portRegister, pin);
}
