// ControlledDcMotor.h

#ifndef _CONTROLLEDDCMOTOR_h
#define _CONTROLLEDDCMOTOR_h

#include "arduino.h"

#include "Defines.h"
#include "QuadDecode_def.h"

#include "DC_Motor.h"
#include "PID_Controller.h"

class ControlledDcMotor{

public:
	ControlledDcMotor(uint8_t motorDirPin, uint8_t motorPwmPin, 
						double* input, double* output, double* setPoint, float Kp, float Ki, float Kd, 
						const uint16_t intervall);

	virtual void handleMotorControl() = 0;
	void calculate();

	void rotateMotor(uint8_t dir, uint16_t pwm);
	void stopMotor();
	
	void setControllerMode(bool mode);
	void tuneController(float Kp, float Ki, float Kd);

	double getInputValue();
	double getOutputValue();
	double getSetPointValue();

	virtual int32_t getEncoderValue() = 0;
	virtual void zeroEncoder() = 0;

	int32_t getEncoderSpeed();

protected:

	double* _input;
	double* _output;
	double* _setPoint;

	uint8_t _sampleTime;
	int32_t _encoderSpeed;

	DC_Motor _motor;
	PID _controller;
};

#endif