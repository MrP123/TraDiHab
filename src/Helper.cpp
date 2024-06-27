#include "Helper.h"
#include <math.h>

//Header file containing different helper functions

//https://gist.github.com/nadavmatalon/71ccaf154bc4bd71f811289e78c65918
double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
	return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float realmod(float x, float y){
	float result = fmod(x, y);
	return result >= 0 ? result : result + y;
}

uint32_t secToMicroSec(float sec){
	return (uint32_t)(sec * 1000000.0f);
}
