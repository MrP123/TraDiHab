//MTPANNY - 2020-01-02
//Ported to PlatformIO by Matthias Panny - 2024-06-27

/*
NOTES:
Serial Monitor auf COM-port mit Namen "USB Serial Device", NICHT "Communications Port"
Could use ADC library (https://github.com/pedvide/ADC) for better usage of ADC --> supports internal averaging better

TODO:
home function fertig implementieren und testen!!!!!!!!
*/

#include "Defines.h"
#include "Helper.h"
#include "QuadDecode_def.h"

#include "CurrentSensor.h"
#include "Waveform.h"

#include "PS_ControlledDcMotor.h"
#include "FE_ControlledDcMotor.h"

#include <IntervalTimer.h>

IntervalTimer timer;	//handles main control loop at fixed frequency

//============FORWARD DECLARATIONS============
void timerLoopISR();
void safetyStopISR();

void serialPrint();
void serialPosition();

//============MODE DEFINITIONS============
#define ENCODER_ONLY_MODE 0	//set to 1 for usage needed for Unity program, otherwise set to 0
const uint8_t divisor = 5;
volatile uint8_t sendCounter = 0;

//============GlOBAL CONTROLLER PARAMETER============
//gain difference in K_duty is 255/4095 --> factor 16 compared to old values
//--> good enough for this device, but proper system identification & controller design would be better
const float Kp = 1.248;
const float Ki = 0.576;
const float Kd = 0.032;

//============PRONATION-SUPINATION============
double encoderPosPS = 0;	//input
double commandPS = 0;		//output
double setPointPS = 0;		//setPoint in encoder steps

PS_ControlledDcMotor motorPS(PS_DIR, PS_PWM, &encoderPosPS, &commandPS, &setPointPS, Kp, Ki, Kd, LOOP_SAMPLE_TIME);
CurrentSensor currentSensorPS(PS_CURRENT, ADC_RESOLUTION, PS_FAULT_CURRENT);

uint32_t waveformAmplitudePS = lroundf(20 * DEGREES_TO_STEPS * PS_GEAR_RATIO);
Waveform setPointWaveformPS(Waveform::TRIANGLE, secToMicroSec(4.0f), waveformAmplitudePS);

//============FLEXION-EXTENSION============
double encoderPosFE = 0;	//input
double commandFE = 0;		//output
double setPointFE = 0;		//setPoint in encoder steps

FE_ControlledDcMotor motorFE(FE_DIR, FE_PWM, &encoderPosFE, &commandFE, &setPointFE, Kp, Ki, Kd, LOOP_SAMPLE_TIME);
CurrentSensor currentSensorFE(FE_CURRENT, ADC_RESOLUTION, FE_FAULT_CURRENT);

uint32_t waveformAmplitudeFE = lroundf(20 * DEGREES_TO_STEPS * FE_GEAR_RATIO);
Waveform setPointWaveformFE(Waveform::TRIANGLE, secToMicroSec(4.0f), waveformAmplitudeFE);

//===========================================
volatile uint64_t timeCounter = 0;	//stores the time since the program started in microseconds, used for trajectory generation

volatile bool running = false;
volatile bool hasRun = false;
volatile bool restartController = false;

//=================DEBUGGING=================
volatile uint8_t toggleValue = 0;
volatile char incomingData = 0;

void setupPins(){
	pinMode(DEBUG_PIN, OUTPUT);

	pinMode(SAFETY_SWITCH, INPUT);
	attachInterrupt(SAFETY_SWITCH, safetyStopISR, FALLING);
	
	pinMode(PS_ENC_I, INPUT);
	pinMode(FE_ENC_I, INPUT);

	pinMode(PS_LIMIT_1, INPUT);
	pinMode(PS_LIMIT_2, INPUT);
	pinMode(FE_LIMIT_1, INPUT);
	pinMode(FE_LIMIT_2, INPUT);
}

void setupPWM(){
	analogWriteFrequency(FE_PWM, PWM_FREQUENCY);
	analogWriteFrequency(PS_PWM, PWM_FREQUENCY);
	analogWriteResolution(PWM_RESOLUTION);
}

void setup(){
	Serial.begin(115200); // USB is always 12 Mbit/sec, but a Baud rate has to be specified --> 115200 matches the one used in the Unity program

	analogReadResolution(ADC_RESOLUTION);
	setupPins();
	setupPWM();

	//homing has to be done before starting the timer loop
	//homeDevice();

	motorPS.zeroEncoder();
	motorFE.zeroEncoder();

	timer.begin(timerLoopISR, LOOP_SAMPLE_TIME);
	timeCounter = 0;

	#if SPEED_LIMIT_MODE
	Serial.println("SPEED LIMIT MODE ENGAGED");
	#endif

	//ensure that motors are stopped when only using the encoder output
	#if ENCODER_ONLY_MODE
	motorPS.stopMotor();
	motorFE.stopMotor();
	#endif // ENCODER_ONLY_MODE
}

void loop(){

	//should probably be improved --> FSM would maybe be nice
	#if !ENCODER_ONLY_MODE
	if (Serial.available() > 0) {
		incomingData = Serial.read();
		if (incomingData == 's' && !running) {
			Serial.println("Starting!");
			running = true;

			//restarted after having stopped --> restart controlelr
			if (hasRun) restartController = true;
		}
		else {
			Serial.println("Stopping!");
			running = false;
		}
	}
	#endif // !ENCODER_ONLY_MODE
}

//runs with a frequency of 1.0f/LOOP_SAMPLE_TIME
void timerLoopISR() {

	#if ENCODER_ONLY_MODE
	if (sendCounter >= 5) {
		serialPosition();
		sendCounter = 0;
	}else
		sendCounter++;
	#else
	if (!running && hasRun) {
		motorPS.stopMotor();
		motorFE.stopMotor();
		return;
	}

	if (!running) return;

	if (restartController) {
		motorPS.setControllerMode(AUTOMATIC);
		motorFE.setControllerMode(AUTOMATIC);
	}

	hasRun = true;
	timeCounter += LOOP_SAMPLE_TIME;

	//setpoint calculation should/could probably be moved from isr to loop, because timeCounter would be correct anyway
	setPointPS = setPointWaveformPS.getValue(timeCounter);
	setPointFE = setPointWaveformFE.getValue(timeCounter);

	currentSensorPS.sampleSensor();
	currentSensorFE.sampleSensor();

	motorPS.handleMotorControl();
	motorFE.handleMotorControl();

	serialPrint();
	#endif // ENCODER_ONLY_MODE
}

void safetyStopISR() {
	running = false;
	motorPS.stopMotor();
	motorFE.stopMotor();
}

//not finished yet --> just concept
void homeDevice() {
	motorPS.rotateMotor(0, PWM_MAX_VALUE / 16);
	
	int32_t position1 = 0;
	int32_t position2 = 0;

	bool limitHit = false;
	while (!limitHit) {
		limitHit = digitalReadFast(PS_LIMIT_1) || digitalReadFast(PS_LIMIT_2);
	}
	motorPS.stopMotor();
	position1 = motorPS.getEncoderValue();

	motorPS.rotateMotor(1, PWM_MAX_VALUE / 16);
	limitHit = false;
	while (!limitHit) {
		limitHit = digitalReadFast(PS_LIMIT_1) || digitalReadFast(PS_LIMIT_2);
	}
	motorPS.stopMotor();
	position2 = motorPS.getEncoderValue();

	//new home pos if symmetric between pos1 & pos2 --> ((pos1 + pos2) / 2) + min(pos1, pos2) --> go there and zero encoder
	int32_t newHome = min(position1, position2) + (abs(position1 - position2) / 2);
	setPointPS = newHome;
	motorPS.setControllerMode(AUTOMATIC);

	while(motorPS.getEncoderValue() > newHome * 1.05f || motorPS.getEncoderValue() < newHome * 0.95f){
		//delay
	}
	motorPS.stopMotor();
	motorPS.zeroEncoder();
}

//should later be optimized to send raw byte data instead of strings --> would transmit significantly less data --> faster
//should also include a start & end byte to allow verification of a correct data transmission
void serialPrint() {
	Serial.print(setPointPS);
	Serial.print(" ");
	Serial.print(motorPS.getEncoderValue());
	Serial.print(" ");
	Serial.println(currentSensorPS.getAverageCurrent());
	
	Serial.print(setPointFE);
	Serial.print(" ");
	Serial.print(motorFE.getEncoderValue());
	Serial.print(" ");
	Serial.println(currentSensorFE.getAverageCurrent());
}

//used for the Unity program interface only
void serialPosition() {
	Serial.print(motorPS.getEncoderValue());
	Serial.print(" ");
	Serial.println(motorFE.getEncoderValue());
}

//used for testing purposes only
void toggleTestPin() {
	toggleValue ^= 1;
	digitalWriteFast(DEBUG_PIN, toggleValue);
}