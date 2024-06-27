#ifndef _HELPER_h
#define _HELPER_h

#include <stdint.h>

//Header file containing different helper functions

//https://gist.github.com/nadavmatalon/71ccaf154bc4bd71f811289e78c65918
double mapf(double val, double in_min, double in_max, double out_min, double out_max);

float realmod(float x, float y);

uint32_t secToMicroSec(float sec);

#endif //_HELPER_h