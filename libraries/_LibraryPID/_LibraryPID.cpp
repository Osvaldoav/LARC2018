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
double kp, ki, kd;
double tof_Kp=9, tcrtVerticalBlackLine_kp=0.0009; //2.3, 140
/////// LOCAL VARIABLES ////////////
double ITerm=0;
unsigned long lastTime;


// TODO:
void _LibraryPID::setupLibraryPID(){
		/////// PID CONSTANTS ///////
		alignKp=0, alignKi=0, alignKd=0;
		turnKp=8.6, turnKi=0, turnKd=0.9;//2.4d // 18.3
		forwardKp=15, forwardKi=0.8, forwardKd=0;//4.3d
		/////// VELOCITIES CONTANTS ////////
		maxTurnVel=255;      
        minTurnVel=0;
        maxForwardVel=255;
        minForwardVel=0; 

		Setpoint=0;
		frontLeftOutput=0, frontRightOutput=0, backLeftOutput=0, backRightOutput=0, Output=0;
		OutputAlignMechanism=0, OutputVerticalBlackLine=0;
}

// TODO:
void _LibraryPID::calculateNewSetpoint(double angle) {
		double newSetpoint = Setpoint;
		newSetpoint += angle;
		if (newSetpoint < 0)
			newSetpoint = 360 + newSetpoint; //newSetpoint is (-)
		if (newSetpoint > 360)
			newSetpoint = fmod(newSetpoint, 360);
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
void _LibraryPID::computeOutput_bno(double rawInput, double &lastInput){
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
void _LibraryPID::computeOutput_tof(double distanceOffset){   
	OutputAlignMechanism = tof_Kp*distanceOffset;    
}
// FIXME:
/* TCRT positions
        ||  7  ||     ||  9  ||          || FL ||    || FR ||
        ||     ||     ||     ||    =>    ||    ||    ||    ||
        ||  1  ||     ||  3  ||          || BL ||    || BR ||
*/
// TODO:
char _LibraryPID::computeOutput_tcrtVerticalLine(double whiteFL, double whiteBL, double whiteFR, double whiteBR){
	double max=0;
	char tcrt;
	// Serial.print(whiteFL);	Serial.print(" ");
	// Serial.print(whiteBL);	Serial.print(" ");
	// Serial.print(whiteFR);	Serial.print(" ");
	// Serial.print(whiteBR);	Serial.print("        ");
// 	Linealize TCRT values
	whiteFL=(1000-whiteFL-100); whiteBL=(1000-whiteBL-100); 
	whiteFR=(1000-whiteFR-100); whiteBR=(1000-whiteBR-100); 
	if(whiteFL<0) whiteFL=0;  	if(whiteBL<0) whiteBL=0;
	if(whiteFR<0) whiteFR=0; 	if(whiteBR<0) whiteBR=0;
	// Serial.print(whiteFL);	Serial.print(" ");
	// Serial.print(whiteBL);	Serial.print(" ");
	// Serial.print(whiteFR);	Serial.print(" ");
	// Serial.print(whiteBR);	Serial.print("        ");	
//	Look for max white value
	double outputFL = tcrtVerticalBlackLine_kp*whiteFL;
	if(outputFL > max){ max=outputFL; tcrt='7'; }
	double outputBL = tcrtVerticalBlackLine_kp*whiteBL;
	if(outputBL > max){ max=outputBL; tcrt='1'; }
	double outputFR = tcrtVerticalBlackLine_kp*whiteFR;
	if(outputFR > max){ max=outputFR; tcrt='9'; }
	double outputBR = tcrtVerticalBlackLine_kp*whiteBR;
	if(outputBR > max){ max=outputBR; tcrt='3'; }
//	Return variables
	// Serial.print(outputFL);	Serial.print(" ");
	// Serial.print(outputBL);	Serial.print(" ");
	// Serial.print(outputFR);	Serial.print(" ");
	// Serial.print(outputBR); Serial.print("        ");
	OutputVerticalBlackLine = max;
	// Serial.print(OutputVerticalBlackLine); Serial.print(" ");
	// Serial.println(tcrt);
	return tcrt;
}
// TODO:
void _LibraryPID::regulateOutputsPID(double velocityMax, double velocityMin){
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
