#ifndef _TEMPERATURE_SENSOR_h
#define _TEMPERATURE_SENSOR_h

#include "arduino.h"

class TemperatureSensor {

public:

	TemperatureSensor(uint8_t analogPin);

	float measure(void);
	float getLastTemperature(void);

private:

	uint8_t _analogPin;
	float _temperature;

	void measureTemp(uint8_t pin);

};

#endif	//_TEMPERATURE_SENSOR_h
