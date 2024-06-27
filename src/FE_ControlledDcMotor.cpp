#include "FE_ControlledDcMotor.h"

FE_ControlledDcMotor::FE_ControlledDcMotor(uint8_t motorDirPin, uint8_t motorPwmPin, double * input, double * output, double * setPoint, float Kp, float Ki, float Kd, const uint16_t intervall)
					: ControlledDcMotor(motorDirPin, motorPwmPin, input, output, setPoint, Kp, Ki, Kd, intervall){

	_encoder.setup();
	_encoder.zeroFTM();
	_encoder.start();
}

void FE_ControlledDcMotor::handleMotorControl(){
	int32_t lastEncoderPos = *(_input);
	*(_input) = _encoder.calcPosn();

	//calculate speed in steps per second
	_encoderSpeed = (int32_t)((float)(*(_input)-lastEncoderPos) / (float)(_sampleTime) * 1000000.0f);

	calculate();
}

void FE_ControlledDcMotor::zeroEncoder(){
	_encoder.zeroFTM();
}

int32_t FE_ControlledDcMotor::getEncoderValue(){
	return _encoder.calcPosn();
}
