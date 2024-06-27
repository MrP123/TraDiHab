/**********************************************************************************************
* CUSTOM VERSION OF THE PID LIBRARY (Modified by MTPANNY) --> USES MICROSECONDS INSTEAD OF MILLISECONDS AND OTHER TWEAKS/IMPROVEMENTS
* Arduino PID Library - Version 1.1.1
* by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
*
* This Library is licensed under a GPLv3 License
**********************************************************************************************/

#include "Arduino.h"
#include "Defines.h"

#include "PID_Controller.h"


PID::PID(double* Input, double* Output, double* Setpoint, double Kp, double Ki, double Kd, int POn, int ControllerDirection, uint16_t sampleTime){
	myOutput = Output;
	myInput = Input;
	mySetpoint = Setpoint;
	inAuto = false;

	//default output limit corresponds to the pwm limits of the used microcontroller
	PID::SetOutputLimits(0, PWM_MAX_VALUE);

	SampleTime = sampleTime;

	PID::SetControllerDirection(ControllerDirection);
	PID::SetTunings(Kp, Ki, Kd, POn);
}

//Defaults to proportional on error
PID::PID(double* Input, double* Output, double* Setpoint, double Kp, double Ki, double Kd, int ControllerDirection, uint16_t sampleTime)
	:PID::PID(Input, Output, Setpoint, Kp, Ki, Kd, P_ON_E, ControllerDirection, sampleTime){}


//Must be called with constant frequency matching the one specified in sample time
bool PID::Compute(){
	if (!inAuto) return false;

	/*Compute all the working error variables*/
	double input = *myInput;
	double error = *mySetpoint - input;
	double dInput = (input - lastInput);
	outputSum += (ki * error);

	/*Add Proportional on Measurement, if P_ON_M is specified*/
	if (!pOnE) outputSum -= kp * dInput;

	if (outputSum > outMax) outputSum = outMax;
	else if (outputSum < outMin) outputSum = outMin;

	/*Add Proportional on Error, if P_ON_E is specified*/
	double output;
	if (pOnE) output = kp * error;
	else output = 0;

	/*Compute Rest of PID Output*/
	output += outputSum - kd * dInput;

	if (output > outMax) output = outMax;
	else if (output < outMin) output = outMin;
	*myOutput = output;

	/*Remember some variables for next time*/
	lastInput = input;
	return true;
}


void PID::SetTunings(double Kp, double Ki, double Kd, int POn){
	if (Kp < 0 || Ki < 0 || Kd < 0) return;

	pOnE = POn ? P_ON_E : P_ON_M;

	double SampleTimeInSec = ((double)SampleTime) / 1000000;
	kp = Kp;
	ki = Ki * SampleTimeInSec;
	kd = Kd / SampleTimeInSec;

	if (controllerDirection == REVERSE)
	{
		kp = (0 - kp);
		ki = (0 - ki);
		kd = (0 - kd);
	}
}

//set tunings using the last-rembered pOnE setting
void PID::SetTunings(double Kp, double Ki, double Kd){
	SetTunings(Kp, Ki, Kd, pOnE);
}

//Sample time in microseconds
void PID::SetSampleTime(int NewSampleTime) {
	if (NewSampleTime < 0) return;

	double ratio = (double)NewSampleTime / (double)SampleTime;
	ki *= ratio;
	kd /= ratio;
	SampleTime = (unsigned long)NewSampleTime;
}


void PID::SetOutputLimits(double Min, double Max){
	if (Min >= Max) return;
	outMin = Min;
	outMax = Max;

	//limit output when in auto mode and already past new limits
	if (inAuto){
		if (*myOutput > outMax) *myOutput = outMax;
		else if (*myOutput < outMin) *myOutput = outMin;

		if (outputSum > outMax) outputSum = outMax;
		else if (outputSum < outMin) outputSum = outMin;
	}
}

//Manual (0) or Automatic (non zero) --> handles transition from manual to auto automatically
void PID::SetMode(int Mode){
	bool newAuto = (Mode == AUTOMATIC);

	if (newAuto && !inAuto){
		//we just went from manual to auto
		PID::Initialize();
	}
	inAuto = newAuto;
}
//Allows transfer from manual to automatic
void PID::Initialize(){
	outputSum = *myOutput;
	lastInput = *myInput;
	if (outputSum > outMax) outputSum = outMax;
	else if (outputSum < outMin) outputSum = outMin;
}

// The PID will either be connected to a DIRECT acting process (+Output leads to +Input) or a REVERSE acting process(+Output leads to -Input.)
void PID::SetControllerDirection(int Direction){
	if (inAuto && Direction != controllerDirection)
	{
		kp = (0 - kp);
		ki = (0 - ki);
		kd = (0 - kd);
	}
	controllerDirection = Direction;
}

int PID::GetMode(){
	return  inAuto ? AUTOMATIC : MANUAL;
}

int PID::GetDirection(){
	return controllerDirection;
}