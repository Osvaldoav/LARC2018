#include <_LibraryPID.h>

#define MANUAL 0
#define AUTOMATIC 1
#define DIRECT 0
#define REVERSE 1
/////// PID PREFERENCES ///////
int controllerDirection=DIRECT;
int SampleTime=50;
double outMin=0,outMax=255;
bool inAuto=true;
/////// PID CONSTANTS ///////
double alignKp=0, alignKi=0, alignKd=0;
double turnKp=17.6, turnKi=0, turnKd=0.9;//2.4d // 18.3
double forwardKp=10.2, forwardKi=0, forwardKd=6.3;//4.8d
double kp=forwardKp, ki=forwardKi, kd=forwardKd;
double wallSharpsKp=11; //2.3, 140
/////// LOCAL VARIABLES ////////////
double ITerm=0;
unsigned long lastTime;
double maxTurnVel=140;
double maxForwardVel=255;


// TODO:
void _LibraryPID::setupLibraryPID(){
		/////// PID CONSTANTS ///////
		alignKp=0, alignKi=0, alignKd=0;
		turnKp=17.6, turnKi=0, turnKd=0.9;//2.4d // 18.3
		forwardKp=19, forwardKi=0, forwardKd=0;//4.8d

		Setpoint=0;
		frontLeftOutput=0, frontRightOutput=0, backLeftOutput=0, backRightOutput=0, Output=0;
}

// TODO:
void _LibraryPID::calculateNewSetpoint(int angle) {
		int newSetpoint = Setpoint;
		newSetpoint += angle;
		if (newSetpoint < 0)
			newSetpoint = 360 + newSetpoint; //newSetpoint is (-)
		if (newSetpoint > 360)
			newSetpoint = newSetpoint%360;
		Setpoint = newSetpoint;
}
// TODO:
void _LibraryPID::setNewSetpoint(double rawInput){
  	Setpoint = rawInput;
}
// TODO:
int _LibraryPID::getAngleDifference(double rawInput) {
		int diffCompass = rawInput - Setpoint;
		if (diffCompass > 180) {
			diffCompass = -( 180-(diffCompass-180) );
		} else if (diffCompass < -180) {
			diffCompass = ( 180-((-diffCompass)-180) );
		}
		return diffCompass; // (-) conviene voltear a la derecha
}
// TODO:
int _LibraryPID::getAngleDerivate(double rawInput, double lastInput){
  	return rawInput - lastInput;
}
// TODO:
void _LibraryPID::computeOutput(double rawInput, double &lastInput){
		if(!inAuto)return; 					//To not get PID confused when forcing output
		unsigned long now = millis();
		int timeChange = (now-lastTime); 	//Time since last (sampleTime)
		if(timeChange>=SampleTime){
				// Calculate errors
				double error = abs(getAngleDifference(rawInput));
				ITerm += (ki*error);
				if (ITerm>outMax)		ITerm=outMax;
				else if (ITerm<outMin)	ITerm=outMin;
				double dInput = getAngleDerivate(rawInput, lastInput);
				// Calculate output
				Output = kp*error + ITerm - kd*dInput;
				if (Output>outMax)		Output=outMax;
				else if (Output<outMin)	Output=outMin;
				// Save values fot next iteration
				lastInput=rawInput;
				lastTime=now;
		}
}
// TODO:
void _LibraryPID::computeOutputAlignMechanism(double distanceOffset){   
	OutputAlignMechanism = wallSharpsKp*distanceOffset;    
}
// TODO:
void _LibraryPID::regulateOutputsFordPID(){
		if (frontRightOutput > maxForwardVel)     frontRightOutput = maxForwardVel;
		else if (frontRightOutput < 0)          frontRightOutput = 0;

		if (backRightOutput > maxForwardVel)      backRightOutput = maxForwardVel;
		else if (backRightOutput < 0)           backRightOutput = 0;

		if (frontLeftOutput > maxForwardVel)      frontLeftOutput = maxForwardVel;
		else if (frontLeftOutput < 0)           frontLeftOutput = 0;

		if (backLeftOutput > maxForwardVel)       backLeftOutput = maxForwardVel;
		else if (backLeftOutput < 0)            backLeftOutput = 0;
}
// TODO:
void _LibraryPID::regulateOutputsTurnPID(){
		if (frontRightOutput > maxTurnVel)     frontRightOutput = maxTurnVel;
		else if (frontRightOutput < 0)          frontRightOutput = 0;

		if (backRightOutput > maxTurnVel)      backRightOutput = maxTurnVel;
		else if (backRightOutput < 0)           backRightOutput = 0;

		if (frontLeftOutput > maxTurnVel)      frontLeftOutput = maxTurnVel;
		else if (frontLeftOutput < 0)           frontLeftOutput = 0;

		if (backLeftOutput > maxTurnVel)       backLeftOutput = maxTurnVel;
		else if (backLeftOutput < 0)            backLeftOutput = 0;
}
void _LibraryPID::regulateOutputsSpecific(double velocityMax, double velocityMin){
		if (frontRightOutput > velocityMax)     frontRightOutput = velocityMax;
		else if (frontRightOutput < velocityMin)          frontRightOutput = velocityMin;

		if (backRightOutput > velocityMax)      backRightOutput = velocityMax;
		else if (backRightOutput < velocityMin)           backRightOutput = velocityMin;

		if (frontLeftOutput > velocityMax)      frontLeftOutput = velocityMax;
		else if (frontLeftOutput < velocityMin)           frontLeftOutput = velocityMin;

		if (backLeftOutput > velocityMax)       backLeftOutput = velocityMax;
		else if (backLeftOutput < velocityMin)            backLeftOutput = velocityMin;
}
// TODO:
// All variables are already initialized, this functions are only used during process
void _LibraryPID::Initialize(double rawInput, double &lastInput){
		lastInput=rawInput;
		ITerm=Output;
		if (ITerm>outMax) 		ITerm=outMax;
		else if (ITerm<outMin) 	ITerm=outMin;
}
// TODO:
void _LibraryPID::SetTunings(double Kp, double Ki, double Kd){
		if (Kp<0 || Ki<0 || Kd<0) 	return;
		double SampleTimeInSec = ((double)SampleTime) / 1000;
		kp = Kp;
		ki = Ki*SampleTimeInSec;
		kd = Kd/SampleTimeInSec;
		if (controllerDirection==REVERSE){
			kp=(0-kp);
			ki=(0-ki);
			kd=(0-kd);
		}
}
// TODO:
void _LibraryPID::SetSampleTime(int NewSampleTime){
		if (NewSampleTime>0){
			double ratio = (double)NewSampleTime/(double)SampleTime;
			ki*=ratio;
			kd/=ratio;
			SampleTime = (unsigned long)NewSampleTime;
		}
}
// TODO:
void _LibraryPID::SetOutputLimits(double Min,double Max){
		if (Min>Max) return;
		outMin=Min;
		outMax=Max;
		if (Output>outMax)  	Output=outMax;
		else if (Output<outMin) Output=outMin;
		if (ITerm>outMax) 		ITerm=outMax;
		else if (ITerm<outMin) 	ITerm=outMin;
}
// TODO:
void _LibraryPID::SetMode(int Mode, double rawInput, double &lastInput){
		bool newAuto = (Mode==AUTOMATIC);
		if (newAuto && !inAuto) { //Paracambiardemanualaautomático,inicializamosalgunosparámetros.
			Initialize(rawInput, lastInput);
		}
		inAuto=newAuto;
}
// TODO:
void _LibraryPID::SetControllerDirection(int Direction){
		controllerDirection=Direction;
}
