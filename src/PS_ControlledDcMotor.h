// PS_ControlledDcMotor.h

#ifndef _PSCONTROLLEDDCMOTOR_h
#define _PSCONTROLLEDDCMOTOR_h

#include "arduino.h"
#include "ControlledDcMotor.h"

class PS_ControlledDcMotor : public ControlledDcMotor {

public:

	PS_ControlledDcMotor(uint8_t motorDirPin, uint8_t motorPwmPin, double* input, double* output, double* setPoint, float Kp, float Ki, float Kd, const uint16_t intervall);

	void handleMotorControl();

	void zeroEncoder();
	int32_t getEncoderValue();

private:

	QuadDecode<PS_ENC_NUM> _encoder;
};

#endif