#include "ControlledDcMotor.h"

ControlledDcMotor::ControlledDcMotor(uint8_t motorDirPin, uint8_t motorPwmPin, double* input, double* output, double* setPoint, float Kp, float Ki, float Kd, const uint16_t intervall)
	: _motor(motorDirPin, motorPwmPin), _controller(input, output, setPoint, Kp, Ki, Kd, DIRECT, intervall){
	
	_sampleTime = intervall;
	_controller.SetOutputLimits(-PWM_MAX_VALUE, PWM_MAX_VALUE);	//-255 to 255 so both directions work
	_controller.SetMode(AUTOMATIC);

	_input = input;
	_output = output;
	_setPoint = setPoint;

	//zeroEncoder(); //handled in derived class
}

//virtual function that has been replaced in derived class
//void ControlledDcMotor::handleMotorDriving(){}

//this function is called in handleMotorDriving of each derived class
void ControlledDcMotor::calculate() {
	//Encoder is being read in each instance of the derived classes --> see virtual func. handleEncoder

	_controller.Compute();

	if (*(_output) < 0) _motor.setDir(0);
	else _motor.setDir(1);
	uint16_t pwm = constrain(abs(*(_output)), 0, PWM_MAX_VALUE);

	#if SPEED_LIMIT_MODE
	pwm = constrain(pwm, 0, 50);
	#endif // SPEED_LIMIT_MODE

	_motor.turn(pwm);
}

void ControlledDcMotor::tuneController(float Kp, float Ki, float Kd){
	_controller.SetTunings(Kp, Ki, Kd);
}

void ControlledDcMotor::rotateMotor(uint8_t dir, uint16_t pwm){
	setControllerMode(MANUAL);
	_motor.turn(dir, pwm);
}

void ControlledDcMotor::stopMotor(){
	_motor.stop();
	setControllerMode(MANUAL);
}

void ControlledDcMotor::setControllerMode(bool mode){
	_controller.SetMode(mode);
}

/*
//now a virtual function --> old implementation
void ControlledDcMotor::zeroEncoder(){
	//_encoder.write(0);
}
*/

/*
//now a virtual function --> old implementation
int32_t ControlledDcMotor::getEncoderValue(){
	//return _encoder.read();
}
*/

double ControlledDcMotor::getInputValue(){
	return *(_input);
}

double ControlledDcMotor::getOutputValue(){
	return *(_output);
}

double ControlledDcMotor::getSetPointValue(){
	return *(_setPoint);
}

int32_t ControlledDcMotor::getEncoderSpeed(){
	return _encoderSpeed;
}