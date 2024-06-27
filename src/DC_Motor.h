// DC_Motor.h

#ifndef _DC_MOTOR_h
#define _DC_MOTOR_h

#include "arduino.h"

class DC_Motor{

public:
	DC_Motor(uint8_t dirPin, uint8_t pwmPin);

	void turn(bool dir, uint16_t pwm);
	void turn(uint16_t pwm);
	void stop();

	void setDir(bool dir);

	void turnCW(uint16_t pwm);
	void turnCCW(uint16_t pwm);

private:
	uint8_t _dirPin;
	uint8_t _pwmPin;

	bool _dir;	//0 is CW, 1 is CCW
};

#endif

