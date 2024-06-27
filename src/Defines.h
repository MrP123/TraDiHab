#ifndef _DEFINES_h
#define _DEFINES_h

//Teensy 3.6 "specific"
#define PWM_FREQUENCY 20000
#define PWM_RESOLUTION 12
#define PWM_MAX_VALUE ((1 << PWM_RESOLUTION) - 1)

#define ADC_RESOLUTION 12
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION) - 1)

//GLOBAL
#define DEBUG_PIN 21
#define LOOP_SAMPLE_TIME 1000	//in microseconds

//#define SPEED_LIMIT_MODE	//used to limit the speed of the motor --> uncomment to be used

#define CURRENT_TO_TORQUE 0.0194f	//Nm/A

#define ENCODER_STEPS (500 * 4)	//500 mechanical steps with quadrature decoding --> HED 5540, other encoders might use 1000 mech. steps
#define DEGREES_TO_STEPS (ENCODER_STEPS / 360.0f)
#define STEPS_TO_DEGREES (1.0f / DEGREES_TO_STEPS)

//PINS GLOBAL
#define SAFETY_SWITCH 28

//PS
#define PS_DIR 7
#define PS_PWM 8
#define PS_CURRENT 22
#define PS_PT100 A4	//Pt2 on PCB
#define PS_ENC_NUM 2
#define PS_ENC_I 12	//Increment signal of encoder --> not used by hardware decoder
#define PS_LIMIT_1 24
#define PS_LIMIT_2 25
#define PS_FAULT_CURRENT 10
#define PS_GEAR_RATIO (220.0f/8.0f)

//FE
#define FE_DIR 5
#define FE_PWM 6
#define FE_CURRENT 23
#define FE_PT100 A5	//Pt1 on PCB
#define FE_ENC_NUM 1
#define FE_ENC_I 11	//Increment signal of encoder --> not used by hardware decoder
#define FE_LIMIT_1 26
#define FE_LIMIT_2 27
#define FE_FAULT_CURRENT 9
#define FE_GEAR_RATIO (160.0f/8.0f)

#endif //_DEFINES_h