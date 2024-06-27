#include "TemperatureSensor.h"
#include "Defines.h"

TemperatureSensor::TemperatureSensor(uint8_t analogPin){
	_analogPin = analogPin;
	pinMode(_analogPin, INPUT);
}

float TemperatureSensor::measure(void){
	measureTemp(_analogPin);
	return _temperature;
}

float TemperatureSensor::getLastTemperature(void) {
	return _temperature;
}

void TemperatureSensor::measureTemp(uint8_t pin) {
	uint16_t adcValue = analogRead(pin);
	float voltage = adcValue / ADC_MAX_VALUE * 3.3f;
	float deltaR = (voltage - 0.1412) * 14.1854; //change in resistance from 100 ohms, theoretical slope from LTSpice = 14.0661

	_temperature = deltaR * 2.754245;	//2.754245 °C/Ohm --> linear fit for PT100
}