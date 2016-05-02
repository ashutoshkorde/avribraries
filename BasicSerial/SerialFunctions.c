/*
 * BasicSerial.c
 *
 * Created: 8/2/2015 6:16:08 PM
 *  Author: Ashutosh
 */ 

#include <stdio.h>
#include <stdlib.h>

extern void TxTimedByte(char, char);

void TxTimedStringLn(const char* str, char txDelay)
{
	while(*str)	{
		TxTimedByte(*str++, txDelay);
	}
	
	TxTimedByte('\n', txDelay);
}

void TxTimedString(const char* str, char txDelay)
{
	while(*str)	{
		TxTimedByte(*str++, txDelay);
	}
}

void TxTimedFloat(float f, char txDelay)
{
	char str[20];
	//int16_t i = f;
	//float fra = f - i;
	//int16_t r = fra * 100;
	dtostrf(f, 6, 2, str);
	//sprintf(str, "%d.%d", i, r);
	
	TxTimedString(str, txDelay);
}

void TxTimedLong(long l, char txDelay)
{
	char str[20];
	sprintf(str, "%ld", l);
	
	TxTimedString(str, txDelay);
}