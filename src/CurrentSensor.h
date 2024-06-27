#ifndef _CURRENT_SENSOR_h
#define _CURRENT_SENSOR_h

#include "arduino.h"

class CurrentSensor{

public:

	CurrentSensor(uint8_t analogPin, uint8_t analogResolution, uint8_t faultPin);

	void sampleSensor();
	
	float getCurrent();
	float getAverageCurrent();

	uint16_t getRawCurrent();
	uint8_t getAdcResolution();

private:

	#define VOLTAGE_REFERENCE 3.3f
	#define MAX_CURRENT 30.0f	//in A
	#define SENSITIVITY 0.044f	//in V/A

	#define AVG_SAMPLES 16
	#define RUNNING_AVG_VALUES 32

	uint8_t _analogPin;
	uint8_t _analogResolution;

	uint8_t _faultPin;

	//ADC value representing the max/min value that can be measured
	float _maxMesValue;
	float _minMesValue;

	uint16_t runningAvgBuffer[RUNNING_AVG_VALUES] = { 0 };
	bool hasFilledUp = false;
	uint8_t index = 0;
	uint32_t averageSum;

	uint16_t _rawCurrent;		//contains ADC value of most recent conversion
	uint16_t _currentAverage;	//contains most recent ADC average value

	uint16_t update(void);
	void changeResolution(uint8_t analogResolution);
};

#endif	//_CURRENT_SENSOR_h