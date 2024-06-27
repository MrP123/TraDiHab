#ifndef _WAVEFORM_h
#define _WAVEFORM_h

#include "arduino.h"

class Waveform{

public:

	enum Type {
		SIN, TRIANGLE, STEP, ZERO
	};

	Waveform(Type type, uint32_t period, float amplitude, uint32_t delayTime);
	Waveform(Type type, uint32_t period, float amplitude);

	double getValue(uint32_t time);

private:

	Type _type;
	uint32_t _period;	//in microseconds
	uint32_t _delay;
	float _periodInSeconds;
	float _amplitude;
};

#endif	//_WAVEFORM_h