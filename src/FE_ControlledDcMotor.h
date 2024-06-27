// FE_ControlledDcMotor.h

#ifndef _FECONTROLLEDDCMOTOR_h
#define _FECONTROLLEDDCMOTOR_h

#include "arduino.h"
#include "ControlledDcMotor.h"

class FE_ControlledDcMotor : public ControlledDcMotor {

public:

	FE_ControlledDcMotor(uint8_t motorDirPin, uint8_t motorPwmPin, double* input, double* output, double* setPoint, float Kp, float Ki, float Kd, const uint16_t intervall);

	void handleMotorControl();

	void zeroEncoder();
	int32_t getEncoderValue();

private:

	QuadDecode<FE_ENC_NUM> _encoder;
};

#endif