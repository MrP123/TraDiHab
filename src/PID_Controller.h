#ifndef PID_CUSTOM_h
#define PID_CUSTOM_h

class PID
{

public:

	//Constants used in some of the functions below
#define AUTOMATIC	1
#define MANUAL	0
#define DIRECT  0
#define REVERSE  1
#define P_ON_M 0
#define P_ON_E 1

	PID(double*, double*, double*, double, double, double, int, int, uint16_t);	//has param for pOnE/pOnM
	PID(double*, double*, double*, double, double, double, int, uint16_t);

	void SetMode(int Mode);

	bool Compute();
	void SetOutputLimits(double, double);

	void SetTunings(double, double, double);
	void SetTunings(double, double, double, int); //has param for pOnE/pOnM

	void SetControllerDirection(int); //DIRECT --> Input+ == Output+, REVERSE --> Input- == Output+
	void SetSampleTime(int);

	int GetMode();
	int GetDirection();

private:
	void Initialize();

	//gains
	double kp;
	double ki;
	double kd;

	int controllerDirection;
	bool pOnE;		//is proportional on error (1) or measurement (0) active
	bool inAuto;

	double *myInput;
	double *myOutput;
	double *mySetpoint;

	double outputSum, lastInput;

	unsigned long SampleTime;
	double outMin, outMax;
};
#endif	//PID_CUSTOM

