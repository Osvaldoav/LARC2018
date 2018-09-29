#include <_Movements.h>
/////////////////////////// LOCAL VARIABLES ///////////////////////////
double const BLACKLINE_TRIGGER = 380;

// TODO:
char _Movements::oppositeDirection(char direction){
    switch (direction){
        case '7':         // NORTHWEST
            return '3';
        case '8':         // NORTH
            return '2';
        case '9':         // NORTHEAST
            return '1';
        case '4':         // WEST
            return '6';
        case '6':         // EAST
            return '4';
        case '1':         // SOUTHWEST
            return '9';
        case '2':             // SOUTH
            return '8';
        case '3':         // SOUTHEAST 
            return '7';              
    }
}
// TODO:
// Decide what sensors to update (in alphabetical )
void _Movements::updateSensors(bool readBNO, bool readColorSensor, bool readSharp, bool readTOF, bool readTCRTContainer, bool readTCRTVertical){
    if(readBNO)             bno055->readBNO(pid->Setpoint);
    if(readColorSensor)     colorSensor->readColor();       
    if(readSharp)           sharp->filtrateDistancesSharp(); 
    if(readTOF)             timeFlight->filtrateDistancesTimeFlight();
    if(readTCRTContainer)   tcrt5000->filtrateDistancesTCRT5000(true);  
    if(readTCRTVertical)    tcrt5000->filtrateDistancesTCRT5000(false);    
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
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            bno055->offsetAngle = bno055->offsetAngleForward;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            // bno055->offsetAngle = -bno055->offsetAngleForward;
            bno055->offsetAngle = 0;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            // bno055->offsetAngle = -bno055->offsetAngleForward;
            bno055->offsetAngle = 0;
            pid->SetTunings(pid->forwardKp, pid->forwardKi, pid->forwardKd);
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
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
        pid->regulateOutputsPID(pid->maxTurnVel, pid->minTurnVel);
    else
        pid->regulateOutputsPID(pid->maxForwardVel, pid->minForwardVel);
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);
}
// TODO:
void _Movements::verifySpecificAndUploadOutputs(double velMin, double velMax){
    pid->regulateOutputsPID(velMax, velMin);
    motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);
}
// TODO:
//forward with P correction
void _Movements::movePID(bool goSlow, char direction){
//  Update Sensors
    updateSensors(1,0,0,1,0,0);
//  Update Outputs
    setBaseVelocitiesByDirection(goSlow, direction);
    calculateAngleOutputsByDirection(goSlow, direction);
    verifyAndUploadOutputsByDirection(direction);   
}
// TODO:
void _Movements::spinPID(bool goSlow, double newAngle){
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
    updateSensors(0,0,0,0,0,0);    
//  Update Outputs
    setBaseVelocitiesByDirection(goSlow, 'T');
    calculateAngleOutputsByDirection(goSlow, 'T'); 
    verifyAndUploadOutputsByDirection('T');           
}
// TODO:
//forward with P correction for a specific time
void _Movements::movePID_nSec(double time, bool goSlow, char direction) {
    double startTime = millis();
    while(millis() < startTime+(time*1000))       
        movePID(goSlow, direction);
    motors->brake();
}
// TODO:
//Go forward the cm given in the parameter
void _Movements::movePID_nCM(double cm, bool goSlow, char direction){
    encoder->encoderState = 1;  
//  Counts of encoder to get to the objective
    int untilSteps = (encoder->encoder30Cm / 30) * cm;
//  Restart encoder counts
    encoder->steps = 0;
//  Move with p correction until the encoder read the cm
    while (encoder->steps < untilSteps){              
        movePID(goSlow, direction);
    }
    motors->brake();
}
// TODO:
void _Movements::align_tof(){
//  Local Variables    
    int countCorrect = 0;
    double startTime = millis();
//  Init baseVelocity as SlowTurnVel and offSetAngle
    bno055->offsetAngle = bno055->offsetAngleTurn; 
    Serial.println("START");
    do{               
        updateSensors(0,0,0,1,0,0);
        double right = timeFlight->timeFlightRight.kalmanDistance;
        double left = timeFlight->  timeFlightLeft.kalmanDistance;
        pid->computeOutput_tof(abs(right-left));
        pid->frontLeftOutput    =  52;
        pid->backLeftOutput     =  52;
        pid->frontRightOutput   =  52;
        pid->backRightOutput    =  52;               
        if(abs(right-left) > 0.2){          //0.19
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
                delay(200);       
                if(++countCorrect == 6)     break;                              
            }  
            verifyAndUploadOutputsByDirection('T'); 
        }    
        else{
            // Serial.println("\t\t\t\tGOING WELL");
            motors->brake();
            delay(200);            
            if(++countCorrect == 6)     break;     
        }      
        Serial.print(right);
        Serial.print(" ");
        Serial.print(left);
        Serial.print(" ");
        Serial.print(abs(right-left));
        Serial.print(" ");        
        Serial.print(pid->OutputAlignMechanism);       
        Serial.print(" ");        
        Serial.println(pid->frontRightOutput);              
    } while(millis() < startTime+3000);
    motors->brake();
    updateSensors(1,0,0,0,0,0);
    pid->Setpoint = bno055->rawInput;
}
// TODO:
//Go forward until finding a wall at a certain distance
void _Movements::larc_alignToPickContainer(int cmDistance){
//  Update Sensors    
    updateSensors(0,0,0,1,0,0);
    timeFlight->filtrateDistancesTimeFlight();
//  Local Variables    
    int actualDistance, countCorrect=0;    
    actualDistance = (timeFlight->timeFlightRight.kalmanDistance+timeFlight->timeFlightLeft.kalmanDistance)/2;
    bool ready = actualDistance == cmDistance ? true : false;
//  Fake velSlowFord    
    int extraSlowVel = motors->velSlowFordFL-20;
    int lastSlowVelFL=motors->velSlowFordFL, lastSlowVelBL=motors->velSlowFordBL;
    int lastSlowVelFR=motors->velSlowFordFR, lastSlowVelBR=motors->velSlowFordBR;     
    motors->velSlowFordFL = motors->velSlowFordFL;
    motors->velSlowFordBL = motors->velSlowFordBL; 
    motors->velSlowFordFR = motors->velSlowFordFR;
    motors->velSlowFordBR = motors->velSlowFordBR;   
//  While not at ceratin distance from wall
    while (!ready){                 
        if (actualDistance > cmDistance + 2){
            movePID(true, '4');    
            countCorrect = 0;                            
        }
        else if (actualDistance < cmDistance - 2){ //To close from wall
            movePID(true, '6');
            countCorrect = 0;
        }
        else{                                      //Already at the distance with an error of +- 2 cm.
            motors->brake();
            delay(100);
            if (++countCorrect == 8) ready = true;
        }
        updateSensors(0,0,0,1,0,0);
        actualDistance = (timeFlight->timeFlightRight.kalmanDistance+timeFlight->timeFlightLeft.kalmanDistance)/2;
    }
    lcd->onLed('r');
    motors->brake();
    delay(100);
    align_tof();
    align_tof();
    lcd->offLed('r');    
    motors->brake();
//  SlowFord velocities back to normal
    motors->velSlowFordFL = lastSlowVelFL; 
    motors->velSlowFordBL = lastSlowVelBL; 
    motors->velSlowFordFR = lastSlowVelFR;
    motors->velSlowFordBR = lastSlowVelBR;    
}
// TODO:
void _Movements::larc_moveAndAlignToShip(){
    while(1){   //Move Until Ship
        updateSensors(0,0,0,0,1,0);
        movePID(false, '4');
        if(tcrt5000->tcrtMidFrontLeft.kalmanDistance>BLACKLINE_TRIGGER 
            || tcrt5000->tcrtDownRight.kalmanDistance>BLACKLINE_TRIGGER)
            break;        
    }
    movePID_nCM(2.6, false, '6'); 
    motors->brake();  
    while(tcrt5000->tcrtFrontLeft.kalmanDistance<BLACKLINE_TRIGGER || tcrt5000->tcrtFrontRight.kalmanDistance<BLACKLINE_TRIGGER){
        updateSensors(0,0,0,0,1,0);
        if(tcrt5000->tcrtFrontLeft.kalmanDistance<BLACKLINE_TRIGGER && tcrt5000->tcrtFrontRight.kalmanDistance<BLACKLINE_TRIGGER)
            movePID(true, '4');
        else if(tcrt5000->tcrtFrontLeft.kalmanDistance>=BLACKLINE_TRIGGER && tcrt5000->tcrtFrontRight.kalmanDistance<BLACKLINE_TRIGGER){
            movePID_nCM(0.4, false, '6');
            pid->calculateNewSetpoint(-0.5);
            for(int i=0; i<100; i++){
                turnPID(false);  
            }   
        }
        else if(tcrt5000->tcrtFrontLeft.kalmanDistance<BLACKLINE_TRIGGER && tcrt5000->tcrtFrontRight.kalmanDistance>=BLACKLINE_TRIGGER){
            movePID_nCM(0.4, false, '6');
            pid->calculateNewSetpoint(0.5);
            for(int i=0; i<100; i++){
                turnPID(false);  
            }      
        }        
    }      
    movePID_nCM(2.6, false, '6');     
    motors->brake();   
    delay(300);
    pid->Setpoint = bno055->rawInput;    
}
// FIXME: 
/*
    secondLine => TRUE only when it is needed to ignore the first horizontal black line detected
    pointingNorth => TRUE when robot is pointing front to the trains
    goVerticalLine => TRUE when vertical black line is the destination
*/
// TODO:
void _Movements::larc_moveUntilBlackLine(bool goSlow, char direction, bool pointingNorth, bool goVerticalLine, bool secondLine){
    int nLine=0;
    do{
        updateSensors(0,0,0,0,1,1);
        movePID(goSlow, direction);
        if(direction=='8'){
            if(tcrt5000->tcrtMidFrontRight.kalmanDistance>BLACKLINE_TRIGGER && tcrt5000->tcrtMidFrontLeft.kalmanDistance>BLACKLINE_TRIGGER)
                if(secondLine){
                    if(++nLine<2)   delay(200);
                    else            break;
                } 
                else                break;
        }    
        else if(direction=='2'){
            if(tcrt5000->tcrtMidDownRight.kalmanDistance>BLACKLINE_TRIGGER && tcrt5000->tcrtMidDownLeft.kalmanDistance>BLACKLINE_TRIGGER)
                if(secondLine){
                    if(++nLine<2)   delay(200);
                    else            break;
                } 
                else                break;
        }               
        else if(direction=='4' || direction=='6'){
            if(pointingNorth){
                if(direction=='4'){
                    if(tcrt5000->tcrtMidFrontLeft.kalmanDistance>BLACKLINE_TRIGGER+20)
                        break;
                } 
                else if(direction=='6'){
                    if(tcrt5000->tcrtMidFrontRight.kalmanDistance>BLACKLINE_TRIGGER+20)
                        break;
                }                        
            }
            else{
                if(direction=='4'){
                    if(tcrt5000->tcrtMidDownLeft.kalmanDistance>BLACKLINE_TRIGGER+20)
                        break;
                } 
                else if(direction=='6'){
                    if(tcrt5000->tcrtMidDownRight.kalmanDistance>BLACKLINE_TRIGGER+20)
                        break;
                }      
            }
        } 
    } while(1);
    motors->brake();
    if(direction=='8')
        movePID_nCM(1.8, false, '8');
    else if(direction=='2')
        movePID_nCM(1.8, false, '2'); 
    else if(goVerticalLine){
        if(direction=='6')
            movePID_nCM(0.5, false, '6');
        else if(direction=='4')
            movePID_nCM(0.5, false, '4'); 
    }     
    else{
        if(direction=='6')
            movePID_nCM(1.8, false, '6');
        else if(direction=='4')
            movePID_nCM(1.8, false, '4');         
    }               
    motors->brake();           
}
// TODO:
void _Movements::larc_moveBetweenVerticalBlackLine(bool goSlow, char direction){
    while(1){
        updateSensors(0,0,0,0,1,0);
        // char tcrtPosition = pid->computeOutput_tcrtVerticalLine(
        //     tcrt5000->tcrtMidFrontLeft.kalmanDistance,
        //     tcrt5000->tcrtMidDownLeft.kalmanDistance,
        //     tcrt5000->tcrtMidFrontRight.kalmanDistance,
        //     tcrt5000->tcrtMidDownRight.kalmanDistance
        // );
        // if(pid->OutputVerticalBlackLine > 0.5){
        //     if(tcrtPosition=='7' || tcrtPosition=='3')
        //         pid->calculateNewSetpoint(pid->OutputVerticalBlackLine);
        //     else if (tcrtPosition=='9' || tcrtPosition=='1')
        //         pid->calculateNewSetpoint(-pid->OutputVerticalBlackLine);
        // }
        // movePID(goSlow, direction);
        movePID(goSlow, direction);
        if(direction == '8'){
            if(tcrt5000->tcrtFrontLeft.kalmanDistance>BLACKLINE_TRIGGER)
                break;
        }
        else if(direction == '2'){
            if(tcrt5000->tcrtFrontRight.kalmanDistance>BLACKLINE_TRIGGER)
                break;
        }        
    }
    motors->brake();
}