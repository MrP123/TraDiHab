#include "DC_Motor.h"

DC_Motor::DC_Motor(uint8_t dirPin, uint8_t pwmPin){
	_dirPin = dirPin;
	_pwmPin = pwmPin;

	pinMode(_dirPin, OUTPUT);
	pinMode(_pwmPin, OUTPUT);
}

void DC_Motor::turn(bool dir, uint16_t pwm){
	_dir = dir;
	digitalWrite(_dirPin, _dir);
	analogWrite(_pwmPin, pwm);
}

void DC_Motor::turn(uint16_t pwm){
	turn(_dir, pwm);
}

void DC_Motor::stop(){
	turn(0);
}

void DC_Motor::setDir(bool dir){
	_dir = dir;
}

void DC_Motor::turnCW(uint16_t pwm){
	turn(0, pwm);
}

void DC_Motor::turnCCW(uint16_t pwm){
	turn(1, pwm);
}