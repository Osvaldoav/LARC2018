#include <_Movements.h>

// TODO:
void _Movements::setupMovements(){
    bno055 = new _BNO055;
    colorSensor = new _ColorSensor;
    encoder = new _Encoder;;
    pid = new _LibraryPID;
    motors = new _Motors;
    sharp = new _Sharp;
    lcd = new _LCD;
    timeFlight = new _TimeFlight;
    tcrt5000 = new _TCRT5000;
}
// TODO:
// Decide what sensors to update (in alphabetical )
void _Movements::updateSensors(bool readBNO, bool readColorSensor, bool readSharp, bool readTOF, bool readTCRT){
    if(readBNO)             bno055->readBNO(pid->Setpoint);
    if(readColorSensor)     colorSensor->readColor();       
    if(readSharp)           sharp->filtrateDistancesSharp(); 
    if(readTOF)             timeFlight->filtrateDistancesTimeFlight();
    if(readTCRT)            tcrt5000->filtrateDistancesTCRT5000();        
}

// FIXME:
/* DIRECTIONS
        ||  7  ||  8  ||  9  ||          || NorthWest || North || NorthEast ||
        ||  4  ||     ||  6  ||    =>    ||    West   ||       ||    East   ||
        ||  1  ||  2  ||  3  ||          || SouthWest || South || SouthEast ||
               ||  T  ||                              || TURN  ||
*/
// TODO:
void _Movements::setBaseVelocitiesByDirection(bool goSlow, char direction){
    if(direction == 'T'){
        if (goSlow) {
            pid->frontLeftOutput = motors->velSlowTurnFL;
            pid->backLeftOutput = motors->velSlowTurnBL;
            pid->frontRightOutput = motors->velSlowTurnFR;
            pid->backRightOutput = motors->velSlowTurnBR;
        } else {  
            pid->frontLeftOutput = motors->velFordFL;
            pid->backLeftOutput = motors->velFordBL;
            pid->frontRightOutput = motors->velFordFR;
            pid->backRightOutput = motors->velFordBR;    
        }
    } else{
        if (goSlow) {
            pid->frontLeftOutput = motors->velSlowFordFL;
            pid->backLeftOutput = motors->velSlowFordBL;
            pid->frontRightOutput = motors->velSlowFordFR;
            pid->backRightOutput = motors->velSlowFordBR;
        } else {  
            pid->frontLeftOutput = motors->velFordFL;
            pid->backLeftOutput = motors->velFordBL;
            pid->frontRightOutput = motors->velFordFR;
            pid->backRightOutput = motors->velFordBR;
        }            
    }      
}
// TODO:
void _Movements::setBaseSpecificVelocities(double vFL, double vBL, double vFR, double vBR){
    pid->frontLeftOutput = vFL;
    pid->backLeftOutput = vBL;
    pid->frontRightOutput = vFR;
    pid->backRightOutput = vBR;   
}
// TODO:
void _Movements::calculateAngleOutputsByDirection(bool goSlow, char direction){
    int angleDifference = pid->getAngleDifference(bno055->rawInput);      
    switch(direction){
        case '7':         // NORTHWEST
            bno055->offsetAngle = bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT                
                pid->frontLeftOutput -= 0;
                pid->backLeftOutput -= pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += 0;   
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput += 0;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput -= pid->Output;
                pid->backRightOutput -= 0;  
            }    
            motors->setMotor(0, 0, 1, 0, 1, 0, 0, 0);
            break;
        case '8':         // NORTH
            bno055->offsetAngle = -bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput -= pid->Output;
                pid->backLeftOutput -= pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += pid->Output;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput -= pid->Output;
                pid->backRightOutput -= pid->Output;   
            }     
            motors->setMotor(1, 0, 1, 0, 1, 0, 1, 0);
            break;
        case '9':         // NORTHEAST
            bno055->offsetAngle = bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput -= pid->Output;
                pid->backLeftOutput -= 0;
                pid->frontRightOutput += 0;
                pid->backRightOutput += pid->Output;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput += 0;
                pid->frontRightOutput -= 0;
                pid->backRightOutput -= pid->Output;   
            }    
            motors->setMotor(1, 0, 0, 0, 0, 0, 1, 0);
            break;
        case '4':         // WEST
            bno055->offsetAngle = bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput -= pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput -= pid->Output;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput -= pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput -= pid->Output;
                pid->backRightOutput += pid->Output;   
            }    
            motors->setMotor(0, 1, 1, 0, 1, 0, 0, 1);
            break;
        case '6':         // EAST
            bno055->offsetAngle = bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput -= pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput -= pid->Output;
                pid->backRightOutput += pid->Output;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput -= pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput -= pid->Output;   
            }    
            motors->setMotor(1, 0, 0, 1, 0, 1, 1, 0);
            break;
        case '1':         // SOUTHWEST
            bno055->offsetAngle = -bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput -= 0;
                pid->frontRightOutput += 0;
                pid->backRightOutput -= pid->Output;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput -= pid->Output;
                pid->backLeftOutput += 0;
                pid->frontRightOutput -= 0;
                pid->backRightOutput += pid->Output;   
            }    
            motors->setMotor(0, 1, 0, 0, 0, 0, 0, 1);
            break;
        case '2':             // SOUTH
            bno055->offsetAngle = -bno055->offsetAngleForward;   
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput -= pid->Output;
                pid->backRightOutput -= pid->Output;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput -= pid->Output;
                pid->backLeftOutput -= pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += pid->Output;   
            }    
            motors->setMotor(0, 1, 0, 1, 0, 1, 0, 1);
            break;
        case '3':         // SOUTHEAST
            bno055->offsetAngle = -bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 0){ // MOVE TO THE LEFT
                pid->frontLeftOutput -= 0;
                pid->backLeftOutput -= pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += 0;     
            }
            else if(angleDifference < 0){ // MOVE TO THE RIGHT
                pid->frontLeftOutput += 0;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput -= pid->Output;
                pid->backRightOutput -= 0;   
            }    
            motors->setMotor(0, 0, 0, 1, 0, 1, 0, 0);
            break;
        case 'T':         // TURN
            bno055->offsetAngle = bno055->offsetAngleTurn;          
            pid->SetTunings(pid->turnKp, pid->turnKi, pid->turnKd);
            pid->computeOutput(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 1){ // MOVE TO THE LEFT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += pid->Output; 
                motors->setMotor(0, 1, 0, 1, 1, 0, 1, 0);            
            }
            else if(angleDifference < -1){ // MOVE TO THE RIGHT
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += pid->Output;        
                motors->setMotor(1, 0, 1, 0, 0, 1, 0, 1);
            }  
            break;                     
    }    
}
// TODO:
void _Movements::verifyAndUploadOutputsByDirection(char direction){
    if(direction == 'T')
        pid->regulateOutputsTurnPID();
    else
        pid->regulateOutputsFordPID();
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);
}
// TODO:
void _Movements::verifySpecificAndUploadOutputs(double velMin, double velMax){
    pid->regulateOutputsSpecific(velMax, velMin);
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);
}
// TODO:
//forward with P correction
void _Movements::movePID(bool goSlow, char direction){
//  Update Sensors
    updateSensors(1,0,1,1,0);
//  Update Outputs
    setBaseVelocitiesByDirection(goSlow, direction);
    calculateAngleOutputsByDirection(goSlow, direction);
    verifyAndUploadOutputsByDirection(direction);   
}

// TODO:
//forward with P correction for a specific time
void _Movements::movePID_nTime(int time, bool goSlow, char direction) {
    double startTime = millis();
    while(millis() < startTime+time)       
        movePID(goSlow, direction);
    motors->brake();
}

// TODO:
//Go forward the cm given in the parameter
void _Movements::movePID_nCM(int cm, bool goSlow, char direction){
    encoder->encoderState = 1;
//  Counts of encoder to get to the objective
    int untilSteps = (encoder->encoder30Cm / 30) * cm;
//  Restart encoder counts
    encoder->steps = 0;
//  Move with p correction until the encoder read the cm
    while (encoder->steps < untilSteps)
        movePID(goSlow, direction);
    motors->brake();
}

// TODO:
void _Movements::alignMechanism(){
//  Local Variables    
    int countCorrect = 0;
    double startTime = millis();
//  Init baseVelocity as SlowTurnVel and offSetAngle
    bno055->offsetAngle = bno055->offsetAngleTurn; 
    setBaseVelocitiesByDirection(true, 'T');      
    do{               
        updateSensors(0,0,0,1,0);
        double right = timeFlight->timeFlightRight.kalmanDistance;
        double left = timeFlight->timeFlightLeft.kalmanDistance;
        pid->computeOutputAlignMechanism(abs(right-left));
        Serial.print(right);
        Serial.print(" ");
        Serial.print(left);
        Serial.print(" ");
        Serial.print(abs(right-left));
        Serial.print(" ");        
        Serial.println(pid->OutputAlignMechanism);         
        if(abs(right-left) > 0.19){          
            if(right>left){
                // Serial.println("FIRST CASE");
                pid->frontLeftOutput += pid->OutputAlignMechanism;
                pid->backLeftOutput += pid->OutputAlignMechanism;
                pid->frontRightOutput += pid->OutputAlignMechanism;
                pid->backRightOutput += pid->OutputAlignMechanism; 
                motors->setMotor(0, 1, 0, 1, 1, 0, 1, 0);            
            }
            else if(left>right){
                // Serial.println("\t\tSECOND CASE");
                pid->frontLeftOutput += pid->OutputAlignMechanism;
                pid->backLeftOutput += pid->OutputAlignMechanism;
                pid->frontRightOutput += pid->OutputAlignMechanism;
                pid->backRightOutput += pid->OutputAlignMechanism;        
                motors->setMotor(1, 0, 1, 0, 0, 1, 0, 1);  
            }  
            else{
                motors->brake();
                delay(350);                  
            }  
            verifyAndUploadOutputsByDirection('T'); 
        }    
        else{
            // Serial.println("\t\t\t\tGOING WELL");
            motors->brake();
            delay(300);            
            if(++countCorrect == 6)     break;     
        }           
    } while(millis() < startTime+1600);
    updateSensors(1,0,0,0,0);
    pid->Setpoint = bno055->rawInput;
}

// TODO:
//Go forward until finding a wall at a certain distance
void _Movements::movePID_nWallCM(int cmDistance, char direction){
//  Update Sensors    
    updateSensors(0,0,0,1,0);
    timeFlight->filtrateDistancesTimeFlight();
//  Local Variables    
    int actualDistance, countCorrect=0;    
    actualDistance = (timeFlight->timeFlightRight.kalmanDistance+timeFlight->timeFlightLeft.kalmanDistance)/2;
    bool ready = actualDistance == cmDistance ? true : false;
    bool isAligned=false;
//  Fake velSlowFord    
    int extraSlowVel = 75;
    int lastSlowVelFL=motors->velSlowFordFL, lastSlowVelBL=motors->velSlowFordBL;
    int lastSlowVelFR=motors->velSlowFordFR, lastSlowVelBR=motors->velSlowFordBR;     
    motors->velSlowFordFL = extraSlowVel;
    motors->velSlowFordBL = extraSlowVel; 
    motors->velSlowFordFR = extraSlowVel;
    motors->velSlowFordBR = extraSlowVel;   
//  While not at ceratin distance from wall
    while (!ready){                 
        if (actualDistance > cmDistance + 2){
            if((actualDistance-cmDistance < 9) && !isAligned){
                // Serial.println("ALIGNING");
                lcd->onLed('r');
                motors->brake();
                delay(100);
                alignMechanism();
                alignMechanism();
                isAligned=true;
                lcd->offLed('r');
            }         
            else
                movePID(true, 8);    
            countCorrect = 0;                            
        }
        else if (actualDistance < cmDistance - 2){ //To close from wall
            movePID(true, 2);
            countCorrect = 0;
        }
        else{                                      //Already at the distance with an error of +- 2 cm.
            motors->brake();
            delay(100);
            if (++countCorrect == 8) ready = true;
        }
        updateSensors(0,0,0,1,0);
        actualDistance = (timeFlight->timeFlightRight.kalmanDistance+timeFlight->timeFlightLeft.kalmanDistance)/2;
    }
    motors->brake();
//  SlowFord velocities back to normal
    motors->velSlowFordFL = lastSlowVelFL;
    motors->velSlowFordBL = lastSlowVelBL; 
    motors->velSlowFordFR = lastSlowVelFR;
    motors->velSlowFordBR = lastSlowVelBR;    
}

// TODO:
// Go forward following: a setpoint, the distance to the wall, the distance between both sharps
void _Movements::movePID_alignWall(double wallDistanceSetpoint, bool goSlow, bool leftWall, int direction){

}

// TODO:
void _Movements::spinPID(bool goSlow, int newAngle){
    if(abs(newAngle) == 180){
        if(newAngle == 180){
            spinPID(goSlow, 90);
            spinPID(goSlow, 90);
        }
        else if(newAngle == -180){
            spinPID(goSlow, -90);
            spinPID(goSlow, -90);
        }    
        return ;
    }
    else if(abs(newAngle) == 360){
        if(newAngle == 360){
            spinPID(goSlow, 180);
            spinPID(goSlow, 180);
        }
        else if(newAngle == -360){
            spinPID(goSlow, -180);
            spinPID(goSlow, -180);
        }    
        return ;
    }
    pid->calculateNewSetpoint(newAngle);    
    int x = 0;
    double startTime = millis();    
    do {
        bno055->readBNO(pid->Setpoint);
        int angleDifference = pid->getAngleDifference(bno055->rawInput);
        if (abs(angleDifference) > 1) {
            turnPID(goSlow);    
            x = 0;
        } else {
            motors->brake();
            delay(155);      
            if (++x == 3) break;
        }
    } while(millis() < startTime+5000);    
}

// TODO:
void _Movements::turnPID(bool goSlow) {
//  Update Sensors    
    updateSensors(0,0,0,1,0);    
//  Update Outputs
    setBaseVelocitiesByDirection(goSlow, 'T');
    calculateAngleOutputsByDirection(goSlow, 'T'); 
    verifyAndUploadOutputsByDirection('T');           
}