#include "CurrentSensor.h"
#include "Helper.h"

CurrentSensor::CurrentSensor(uint8_t analogPin, uint8_t analogResolution, uint8_t faultPin){
	_analogPin = analogPin;
	_faultPin = faultPin;

	pinMode(_analogPin, INPUT);
	pinMode(_faultPin, INPUT);

	analogReadAveraging(AVG_SAMPLES);
	changeResolution(analogResolution);
}

void CurrentSensor::changeResolution(uint8_t analogResolution) {
	_analogResolution = analogResolution;
	analogReadResolution(analogResolution);

	uint16_t maxAdcValue = (1 << _analogResolution) - 1;
	_maxMesValue = (VOLTAGE_REFERENCE / 2 + SENSITIVITY * MAX_CURRENT) / VOLTAGE_REFERENCE * (float)maxAdcValue;
	_minMesValue = (VOLTAGE_REFERENCE / 2 - SENSITIVITY * MAX_CURRENT) / VOLTAGE_REFERENCE * (float)maxAdcValue;
}

//update reads the raw current ADC value
uint16_t CurrentSensor::update(){
	_rawCurrent = analogRead(_analogPin);
	return _rawCurrent;
}

//samples and filters the signal with running average filter
void CurrentSensor::sampleSensor(){
	//initial averaging is done in the ADC
	uint16_t rawCurrent = update();

	averageSum -= runningAvgBuffer[index];
	runningAvgBuffer[index] = rawCurrent;
	averageSum += runningAvgBuffer[index];
	index++;
	if (index >= RUNNING_AVG_VALUES){
		index = 0;
		hasFilledUp = true;
	}

	//only uses the amount of values from the array that are already filled to compute the average, otherwise the result will be too low
	if (!hasFilledUp) _currentAverage = (uint16_t)(averageSum / index);
	else _currentAverage = (uint16_t)(averageSum / RUNNING_AVG_VALUES);
}

//converts the raw current value from the ADC to a real current in A  --> ensures that the conversion is only ever done when really requested
float CurrentSensor::getCurrent(){
	return mapf(_rawCurrent, _minMesValue, _maxMesValue, -MAX_CURRENT, +MAX_CURRENT);
}

float CurrentSensor::getAverageCurrent(){
	return mapf(_currentAverage, _minMesValue, _maxMesValue, -MAX_CURRENT, +MAX_CURRENT);
}

uint16_t CurrentSensor::getRawCurrent(){
	return _rawCurrent;
}

uint8_t CurrentSensor::getAdcResolution(){
	return _analogResolution;
}