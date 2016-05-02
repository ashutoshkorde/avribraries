/*
* TemperatureSensor.h
*
* Created: 12/24/2015 9:02:13 PM
* Author: Ashutosh
*/

#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

#define TEMP_SAMPLES 4

#include "ArduinoPort/ArduinoPort.h"

class TemperatureSensor
{
    //variables
    public:
    private:
    int sensorPin;
    unsigned long samplingInterval;
    unsigned long lastSampleTime;
    unsigned char readingCount;
    
    float avgArray[TEMP_SAMPLES];
    float avgTemp;
    int index;
    
    //functions
    public:
    TemperatureSensor(int sensorPin, unsigned long interval);
    bool readTemp(float &temperature);
    float CalculateAverage(float temp);
    
    private:

}; //TemperatureSensor

#endif //__TEMPERATURESENSOR_H__
