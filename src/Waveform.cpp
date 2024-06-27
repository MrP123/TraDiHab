#include "Waveform.h"
#include <math.h>
#include "Helper.h"

Waveform::Waveform(Type type, uint32_t period, float amplitude, uint32_t delayTime){
	_type = type;
	
	_period = period;
	_delay = delayTime;
	_periodInSeconds = (float)_period / 1000000.0f;

	_amplitude = amplitude;
}

Waveform::Waveform(Type type, uint32_t period, float amplitude) : Waveform(type, period, amplitude, 0) {}

double Waveform::getValue(uint32_t time){
	if (time <= _delay) return 0.0;	//return 0 when delay time is not yet passed

	uint32_t delayedTime = time - _delay;
	float timeInSeconds = (float)delayedTime / 1000000.0f;	//time in microseconds
	double returnValue = 0;

	switch (_type){
		case SIN:
			returnValue = _amplitude * sinf(2.0f * PI / _periodInSeconds * timeInSeconds);
			break;

		case STEP:
			return _amplitude;
			break;

		case TRIANGLE:
			//implementation from https://en.wikipedia.org/wiki/Triangle_wave
			//returnValue = _amplitude * 2.0 / PI * asin(sin(2.0 * PI / periodInSeconds * timeInSeconds)); //probably too slow

			//fmod does not return the desired result --> now uses realmod in Helper.h --> https://stackoverflow.com/questions/28888619/modulo-function-in-c-that-behaves-like-mod-in-matlab
			float absValue;	//prevents initialization error --> don't exactly know why
			absValue = realmod((timeInSeconds - (_periodInSeconds / 4.0f)), _periodInSeconds) - (_periodInSeconds / 2.0f);
			returnValue = (_amplitude * 4.0f / _periodInSeconds) * abs(absValue) - _amplitude;
			break;

		case ZERO:
			returnValue = 0.0;
			break;
	}

	return returnValue;
}