void calculateNewSetpoint(int angle) {
  int newSetpoint = Setpoint;
  newSetpoint += angle;
  if (newSetpoint < 0)
    newSetpoint = 360 + newSetpoint; //newSetpoint is (-)
  if (newSetpoint > 360)
    newSetpoint = newSetpoint%360;
  Setpoint = newSetpoint;
}
void setNewSetpoint(){
  Setpoint = rawInput;
}
int getAngleDifference() {
  int diffCompass = rawInput - Setpoint;
  if (diffCompass > 180) {
    diffCompass = -( 180-(diffCompass-180) );
  } else if (diffCompass < -180) {
    diffCompass = ( 180-((-diffCompass)-180) );
  }
  return diffCompass; // (-) conviene voltear a la derecha
}
int getAngleDerivate(){
  return rawInput - lastInput;
}

// error = getAngleDifference(), derivate = getAngleDerivate()
void computeOutput(){
	if(!inAuto)return; 					//To not get PID confused when forcing output
	unsigned long now = millis();
	int timeChange = (now-lastTime); 	//Time since last (sampleTime)
	if(timeChange>=SampleTime){
		// Calculate errors
		double error = abs(getAngleDifference());
		ITerm += (ki*error);
		if (ITerm>outMax)		ITerm=outMax;
		else if (ITerm<outMin)	ITerm=outMin;
		double dInput = getAngleDerivate();
		// Calculate output
	    if(!calibratedBNO && calibratedDone)				////////BNO MODIFIED/////////
	      Output = kp*1 + ITerm - kd*dInput;				/////////////////////////////
	    else
			  Output = kp*error + ITerm - kd*dInput;
			if (Output>outMax)		Output=outMax;
			else if (Output<outMin)	Output=outMin;
			// Save values fot next iteration
			lastInput=rawInput;
			lastTime=now;
	    if((!calibratedBNO && Output>0) || !calibratedDone) ////////BNO MODIFIED/////////
	      calibratedBNO=true;							                	/////////////////////////////
	}
}

double computeOutputWall(double baseVelocity, double angleDifference, double sharpsDifference, double wallDistance){ 
  if((!calibratedBNO && Output>0) || !calibratedDone)   
    calibratedBNO=true;      
	if (sharpsDifference > 45) sharpsDifference = 45;
	else if (sharpsDifference < -45) sharpsDifference = -45;  
  return baseVelocity + wallAngleKp*angleDifference + wallDistanceKp*wallDistance + wallSharpsKp*sharpsDifference;    
}

void regulateOutputsFrontPID(){
  if (frontRightOutput > maxForwardVel)     frontRightOutput = maxForwardVel;
  else if (frontRightOutput < 0)          frontRightOutput = 0;

  if (backRightOutput > maxForwardVel)      backRightOutput = maxForwardVel;
  else if (backRightOutput < 0)           backRightOutput = 0;

  if (frontLeftOutput > maxForwardVel)      frontLeftOutput = maxForwardVel;
  else if (frontLeftOutput < 0)           frontLeftOutput = 0;

  if (backLeftOutput > maxForwardVel)       backLeftOutput = maxForwardVel;
  else if (backLeftOutput < 0)            backLeftOutput = 0;
}
void regulateOutputsTurnPID(){
  if (frontRightOutput > maxTurnVel)     frontRightOutput = maxTurnVel;
  else if (frontRightOutput < 0)          frontRightOutput = 0;

  if (backRightOutput > maxTurnVel)      backRightOutput = maxTurnVel;
  else if (backRightOutput < 0)           backRightOutput = 0;

  if (frontLeftOutput > maxTurnVel)      frontLeftOutput = maxTurnVel;
  else if (frontLeftOutput < 0)           frontLeftOutput = 0;

  if (backLeftOutput > maxTurnVel)       backLeftOutput = maxTurnVel;
  else if (backLeftOutput < 0)            backLeftOutput = 0;
}

// All variables are already initialized, this functions are only used during process
void Initialize(){
	lastInput=rawInput;
	ITerm=Output;
	if (ITerm>outMax) 		ITerm=outMax;
	else if (ITerm<outMin) 	ITerm=outMin;
}
void SetTunings(double Kp, double Ki, double Kd){
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
void SetSampleTime(int NewSampleTime){
	if (NewSampleTime>0){
		double ratio = (double)NewSampleTime/(double)SampleTime;
		ki*=ratio;
		kd/=ratio;
		SampleTime = (unsigned long)NewSampleTime;
	}
}
void SetOutputLimits(double Min,double Max){
	if (Min>Max) return;
	outMin=Min;
	outMax=Max;
	if (Output>outMax)  	Output=outMax;
	else if (Output<outMin) Output=outMin;
	if (ITerm>outMax) 		ITerm=outMax;
	else if (ITerm<outMin) 	ITerm=outMin;
}
void SetMode(int Mode){
	bool newAuto = (Mode==AUTOMATIC);
	if (newAuto && !inAuto) { //Paracambiardemanualaautomático,inicializamosalgunosparámetros.
		Initialize();
	}
	inAuto=newAuto;
}
void SetControllerDirection(int Direction){
	controllerDirection=Direction;
}
