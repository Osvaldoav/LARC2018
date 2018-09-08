#include <_Movements.h>

void _Movements::setupMovements(){
    bno055 = new _BNO055;
    colorSensor = new _ColorSensor;
    encoder = new _Encoder;;
    pid = new _LibraryPID;
    motors = new _Motors;
    sharp = new _Sharp;
    lcd = new _LCD;
    timeFlight = new _TimeFlight;
}

// TODO:
/* DIRECTIONS
    ||  7   ||  8   ||  9   ||          || NorthWest || North || NorthEast ||
    ||  4   ||      ||  6   ||    =>    ||    West   ||       ||    East   ||
    ||  1   ||  2   ||  3   ||          || SouthWest || South || SouthEast ||
*/
void _Movements::setWheelsOutputsByMovement(bool goSlow, int direction){
    int angleDifference = pid->getAngleDifference(bno055->rawInput);
    pid->computeOutput(bno055->rawInput, bno055->lastInput);      
    if (goSlow) {
        pid->frontLeftOutput = motors->velSlowFL;
        pid->backLeftOutput = motors->velSlowBL;
        pid->frontRightOutput = motors->velSlowFR;
        pid->backRightOutput = motors->velSlowBR;
    } else {  
        pid->frontLeftOutput = motors->velGenFL;
        pid->backLeftOutput = motors->velGenBL;
        pid->frontRightOutput = motors->velGenFR;
        pid->backRightOutput = motors->velGenBR;
    }      
    switch(direction){
        case 7:         // NORTHWEST
            bno055->offsetAngle = bno055->offsetAngleForward;
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
        case 8:         // NORTH
            bno055->offsetAngle = -bno055->offsetAngleForward;
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
        case 9:         // NORTHEAST
            bno055->offsetAngle = bno055->offsetAngleForward;
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
        case 4:         // WEST
            bno055->offsetAngle = bno055->offsetAngleForward;
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
        case 6:         // EAST
            bno055->offsetAngle = bno055->offsetAngleForward;
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
        case 1:         // SOUTHWEST
            bno055->offsetAngle = -bno055->offsetAngleForward;
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
        case 2:             // SOUTH
            bno055->offsetAngle = -bno055->offsetAngleForward;   
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
        case 3:         // SOUTHEAST
            bno055->offsetAngle = -bno055->offsetAngleForward;
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
    }    
}
// TODO:
//forward with P correction
void _Movements::movePID(bool goSlow, int direction){
    bno055->readBNO(pid->Setpoint);
    // sharp->filtrateDistancesSharp();
    // colorSensor->readColor();   
    setWheelsOutputsByMovement(goSlow, direction);    
    pid->regulateOutputsMovePID(); 
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);
}

// TODO:
//forward with P correction for a specific time
void _Movements::movePID_nTime(int time, bool goSlow, int direction) {
    double startTime = millis();
    while(millis() < startTime+time)       // NORTHWEST
        movePID(goSlow, direction);
    motors->brake();
}

// TODO:
//Go forward the cm given in the parameter
void _Movements::movePID_nCM(int cm, bool goSlow, int direction){
    encoder->encoderState = 1;
    //Counts of encoder to get to the objective
    int untilSteps = (encoder->encoder30Cm / 30) * cm;
    //Restart encoder counts
    encoder->steps = 0;
    //Move with p correction until the encoder read the cm
    while (encoder->steps < untilSteps)
        movePID(goSlow, direction);
    motors->brake();
}

// TODO:
void _Movements::alignMechanism(int direction){
    int extraSlowVel = 75;     
    pid->frontLeftOutput = extraSlowVel;
    pid->backLeftOutput = extraSlowVel;
    pid->frontRightOutput = extraSlowVel;
    pid->backRightOutput = extraSlowVel; 
    ///////////    
    bno055->readBNO(pid->Setpoint);
    int angleDifference = pid->getAngleDifference(bno055->rawInput);               
    timeFlight->filtrateDistancesTimeFlight();
    double right = timeFlight->timeFlightRight.kalmanDistance;
    double left = timeFlight->timeFlightLeft.kalmanDistance;
    // Compute Outputs
    pid->computeOutput(bno055->rawInput, bno055->lastInput); 
    pid->computeOutputAlignMechanism(abs(right-left));
    if(pid->OutputAlignMechanism+pid->Output > 5){
        if((left>right) || (angleDifference < 0)){
            Serial.println("FIRST CASE");
            pid->frontLeftOutput += (pid->OutputAlignMechanism+pid->Output/2);
            pid->backLeftOutput += (pid->OutputAlignMechanism+pid->Output/2);
            pid->frontRightOutput -= (pid->OutputAlignMechanism+pid->Output/2);
            pid->backRightOutput -= (pid->OutputAlignMechanism+pid->Output/2);              
        }
        else if((right>left) || (angleDifference > 0)){
            Serial.println("\t\tSECOND CASE");
            pid->frontLeftOutput -= (pid->OutputAlignMechanism+pid->Output/2);
            pid->backLeftOutput -= (pid->OutputAlignMechanism+pid->Output/2);
            pid->frontRightOutput += (pid->OutputAlignMechanism+pid->Output/2);
            pid->backRightOutput += (pid->OutputAlignMechanism+pid->Output/2);       
        }  
    }    
    else{
        Serial.println("\t\t\t\tGOING WELL");
        pid->frontLeftOutput = extraSlowVel;
        pid->backLeftOutput = extraSlowVel;
        pid->frontRightOutput = extraSlowVel;
        pid->backRightOutput = extraSlowVel;      
    }          
    motors->setMotor(1, 0, 1, 0, 1, 0, 1, 0);
    pid->regulateVelocitiesSpecific(255, 0);
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);   
}

// TODO:
//Go forward until finding a wall at a certain distance
void _Movements::movePID_nWallCM(int cmDistance, int direction){
    int actualDistance;
    timeFlight->filtrateDistancesTimeFlight();
    actualDistance = (timeFlight->timeFlightRight.kalmanDistance+timeFlight->timeFlightLeft.kalmanDistance)/2;
    bool ready = actualDistance == cmDistance ? true : false;
    int countCorrect = 0;
    //While not at ceratin distance from wall
    while (!ready){                 
        if (actualDistance > cmDistance + 2){
            alignMechanism(direction);
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
        timeFlight->filtrateDistancesTimeFlight();
        actualDistance = (timeFlight->timeFlightRight.kalmanDistance+timeFlight->timeFlightLeft.kalmanDistance)/2;
    }
    motors->brake();
    alignMechanism(direction);
}

// TODO:
// Go forward following: a setpoint, the distance to the wall, the distance between both sharps
void _Movements::movePID_alignWall(double wallDistanceSetpoint, bool goSlow, bool leftWall, int direction){
    // double frontDistance, backDistance, sharpsDifference;
    // double wallDistanceReal, wallDistanceDifference;
    // bno055->readBNO(pid->Setpoint);    
    // sharp->filtrateDistancesSharp();  
    // colorSensor->readColor();
    // if (goSlow) {
    //     pid->frontLeftOutput = motors->velSlowFL;
    //     pid->backLeftOutput = motors->velSlowBL;
    //     pid->frontRightOutput = motors->velSlowFR;
    //     pid->backRightOutput = motors->velSlowBR;
    // } else {
    //     pid->frontLeftOutput = motors->velGenFL;
    //     pid->backLeftOutput = motors->velGenBL;
    //     pid->frontRightOutput = motors->velGenFR;
    //     pid->backRightOutput = motors->velGenBR;
    // }  
    // if(leftWall){                               // Choose which sharps to read depending on the wall of (leftWall)
    //     frontDistance = sharp->sharpFL.kalmanDistance;
    //     backDistance = sharp->sharpBL.kalmanDistance; 
    // } else{
    //     frontDistance = sharp->sharpFR.kalmanDistance;
    //     backDistance = sharp->sharpBR.kalmanDistance; 
    // }
    // wallDistanceReal = (frontDistance + backDistance)/2;        // Average of distance to the wall                                                  EXAMPLES USING LEFT WALL
    // wallDistanceDifference = wallDistanceReal - wallDistanceSetpoint;     // Difference between what you want and where you are.     (if > 0)->TOCLOSE[moveRight]   (if < 0)->TOFAR[moveLeft]
    // sharpsDifference = frontDistance - backDistance;    // Difference between the distance of both sharps.                           (if > 0)->[turnLeft]           (if < 0)->[turnRight}
    // int angleDifference = pid->getAngleDifference(bno055->rawInput);  
    // if(leftWall){
    //     pid->frontLeftOutput = pid->computeOutputWall(pid->frontLeftOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
    //     pid->backLeftOutput = pid->computeOutputWall(pid->backLeftOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
    //     pid->frontRightOutput = pid->computeOutputWall(pid->frontRightOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    //     pid->backRightOutput = pid->computeOutputWall(pid->backRightOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    // } else{    
    //     pid->frontLeftOutput = pid->computeOutputWall(pid->frontLeftOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    //     pid->backLeftOutput = pid->computeOutputWall(pid->backLeftOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    //     pid->frontRightOutput = pid->computeOutputWall(pid->frontRightOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
    //     pid->backRightOutput = pid->computeOutputWall(pid->backRightOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);    
    // }
    // pid->regulateOutputsFrontPID(); 
    // //  Serial.print(frontLeftOutput);
    // //  Serial.print("\t");
    // //  Serial.print(backLeftOutput);
    // //  Serial.print("\t");
    // //  Serial.print(frontRightOutput);
    // //  Serial.print("\t");    
    // //  Serial.println(backRightOutput);   
    // motors->setMotor(1, 0, 1, 0, 1, 0, 1, 0); 
    // motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);
}

// TODO:
void _Movements::spinPID(int newAngle){
    if(abs(newAngle) == 180){
        if(newAngle == 180){
            spinPID(90);
            spinPID(90);
        }
        else if(newAngle == -180){
            spinPID(-90);
            spinPID(-90);
        }    
        return ;
    }
    else if(abs(newAngle) == 360){
        if(newAngle == 360){
            spinPID(180);
            spinPID(180);
        }
        else if(newAngle == -360){
            spinPID(-180);
            spinPID(-180);
        }    
        return ;
    }
    pid->calculateNewSetpoint(newAngle);    
    pid->SetTunings(pid->turnKp, pid->turnKi, pid->turnKd);
    int x = 0;
    double startTime = millis();
    bno055->offsetAngle = bno055->offsetAngleTurn;  
    do {
        bno055->readBNO(pid->Setpoint);
        int angleDifference = pid->getAngleDifference(bno055->rawInput);
        if (abs(angleDifference) > 1) {
        turnPID();    
        x = 0;
        } else {
        motors->brake();
        delay(155);      
        if (++x == 3) break;
        }
    } while(millis() < startTime+5000);
    pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
    bno055->offsetAngle = bno055->offsetAngleForward;
}

// TODO:
void _Movements::turnPID() {
    bno055->readBNO(pid->Setpoint);
    sharp->filtrateDistancesSharp(); 
    pid->frontLeftOutput = motors->velTurnFL;
    pid->backLeftOutput = motors->velTurnBL;
    pid->frontRightOutput = motors->velTurnFR;
    pid->backRightOutput = motors->velTurnBR;
    int angleDifference = pid->getAngleDifference(bno055->rawInput);
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
    pid->regulateOutputsTurnPID();
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);          
}