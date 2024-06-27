#include "PS_ControlledDcMotor.h"

PS_ControlledDcMotor::PS_ControlledDcMotor(uint8_t motorDirPin, uint8_t motorPwmPin, double * input, double * output, double * setPoint, float Kp, float Ki, float Kd, const uint16_t intervall)
					: ControlledDcMotor(motorDirPin, motorPwmPin, input, output, setPoint, Kp, Ki, Kd, intervall){

	_encoder.setup();
	_encoder.zeroFTM();
	_encoder.start();
}

void PS_ControlledDcMotor::handleMotorControl(){
	int32_t lastEncoderPos = *(_input);
	*(_input) = _encoder.calcPosn();

	//calculate speed in steps per second
	_encoderSpeed = (int32_t)((float)(*(_input)-lastEncoderPos) / (float)(_sampleTime) * 1000000.0f);

	calculate();
}

void PS_ControlledDcMotor::zeroEncoder(){
	_encoder.zeroFTM();
}

int32_t PS_ControlledDcMotor::getEncoderValue(){
	return _encoder.calcPosn();
}
