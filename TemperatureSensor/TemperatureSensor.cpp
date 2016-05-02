/*
* TemperatureSensor.cpp
*
* Created: 12/24/2015 9:02:13 PM
* Author: Ashutosh
*/


#include "TemperatureSensor.h"
#include "DynamicTimer/DynamicTimer.h"
#include "ArduinoPort/ArduinoPort.h"

#define BAUD_RATE 9600
//#define  F_CPU 1000000

//#define _LOG_

#include "BasicSerial/BasicSerial.h"

#define REF_VOLTAGE 2482
//#define REF_VOLTAGE 2495
//#define REF_VOLTAGE 5060

// default constructor
TemperatureSensor::TemperatureSensor(int sensorPin, unsigned long samplingInterval = 50)
{
	for (int i = 0;i < TEMP_SAMPLES;i++)
	{
		this->avgArray[i] = 0;
	}
	
    this->sensorPin = sensorPin;
    this->samplingInterval = samplingInterval;
    this->lastSampleTime = 0;
    this->readingCount = 0;
    
    this->avgTemp = 0.0;
    this->index = 0;
        
    pinMode(this->sensorPin, INPUT);
} //TemperatureSensor

bool TemperatureSensor::readTemp(float& temperature)
{
    if(millis() - lastSampleTime > samplingInterval)
    {
        long val = analogRead(this->sensorPin);
        
        //Serial.println(val);
		TxLong(val);
        TxByte(':');
		
        //val = map(val, 0, 1023, 0, 5000);
        // Using external reference
        val = map(val, 0, 1023, 0, REF_VOLTAGE);
        // Serial.println(val);
		TxFloat(REF_VOLTAGE);
		TxByte(':');

		TxLong(val);
		TxByte(':');
		        
        float temp = (val - 400.0)/19.5;

		TxFloat(temp);
		TxByte(':');
		        
        CalculateAverage(temp);

		TxFloat(avgTemp);
		TxByte('\n');

        lastSampleTime = millis();
        
        temperature = avgTemp;
        if(readingCount < TEMP_SAMPLES)
        {
            readingCount++;
			return false;
        }
        else
		{
			readingCount = 0;
			return true;
		}
    }
    
    temperature = avgTemp;
    return false;
}

float TemperatureSensor::CalculateAverage(float temp)
{
#if TEMP_SAMPLES > 1
    float prev = avgArray[index];
    avgArray[index++] = temp;
    index = index % TEMP_SAMPLES;
    
    avgTemp += ((temp - prev) / TEMP_SAMPLES);
#else
    avgTemp = temp;
#endif

return avgTemp;
}
