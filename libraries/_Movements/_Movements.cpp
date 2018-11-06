#include <_Movements.h>
///////////////////////////////// PINs ////////////////////////////////
const byte limitSwitch = 30;

/////////////////////////// LOCAL VARIABLES ///////////////////////////

_Movements::_Movements(){
    pinMode(limitSwitch, INPUT);
    bno055 = new _BNO055;
    colorSensor = new _ColorSensor;
    encoder = new _Encoder;
    pid = new _LibraryPID;
    motors = new _Motors;
    sharp = new _Sharp;
    lcd = new _LCD;      
    timeFlight = new _TimeFlight;
    tcrt5000 = new _TCRT5000;
    servo = new _Servo;
    BLACKLINE_TRIGGER = 300;//300
    BLACKLINE_TRIGGER_SHIP = 30;//300
    untilStepsMechanism = 0;
    crazyMode=false;
    crazyRight=false;
    // uint8_t updateCrazyTimeMs = 400;
    // long lastCrazyUpdate = 0;
    moveCalled=0;    
} 

void _Movements::initMechanism(){
    while(digitalRead(limitSwitch)){    //limitSwitch is pressed when == 0
        motors->moveMechanism(true);        
    }
    motors->stopMechanism();
    encoder->encoderStateMechanism = 1;
    encoder->stepsMechanism = 0;
    while(encoder->stepsMechanism < 7600){    //limitSwitch is pressed when == 0
        motors->moveMechanism(false);        
    }    
    motors->stopMechanism();
    encoder->encoderStateMechanism = 1;   
    moveMechanism(4,5); 
}

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
    // Function types:
    //      1 -> (0      -   x/20)   (0-5% del trayecto)
    //      2 -> (x/20   -   20x/17) (5-85% del trayecto)
    //      3 -> (20x/17 -   x)      (85-100% del trayecto)
// TODO:
double _Movements::evaluatePWMFunctionByType(double baseVelocity, double xCM, double totalCM){
    double m, b;
    if(totalCM == 0)
        return baseVelocity;
    int functionType = (xCM < totalCM*0.05)? 1 : (xCM < totalCM*0.85)? 2 : 3;
    if(functionType == 1){
        b = motors->INITIAL_VELOCITY;
        m = (baseVelocity-b)/(totalCM*0.05);
        // return baseVelocity;
    }
    else if(functionType == 2){
        return baseVelocity;
    } 
    else if(functionType == 3){
        b = baseVelocity;
        m = (motors->FINAL_VELOCITY-b)/(totalCM*0.15);
    }   
    return b + xCM*m;
}
// FIXME:
/* DIRECTIONS
        ||  7  ||  8  ||  9  ||          || NorthWest || North || NorthEast ||
        ||  4  ||     ||  6  ||    =>    ||    West   ||       ||    East   ||
        ||  1  ||  2  ||  3  ||          || SouthWest || South || SouthEast ||
               ||  T  ||                              || TURN  ||
*/
// TODO:
void _Movements::setBaseVelocitiesByDirection(bool goSlow, char direction, double xCM, double totalCM){
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
    } else if(direction=='8' || direction=='2'){
        if (goSlow) {
            pid->frontLeftOutput = evaluatePWMFunctionByType(motors->velSlowFordFL, xCM, totalCM);
            pid->backLeftOutput = evaluatePWMFunctionByType(motors->velSlowFordBL, xCM, totalCM);
            pid->frontRightOutput = evaluatePWMFunctionByType(motors->velSlowFordFR, xCM, totalCM);
            pid->backRightOutput = evaluatePWMFunctionByType(motors->velSlowFordBR, xCM, totalCM);
        } else {  
            pid->frontLeftOutput = evaluatePWMFunctionByType(motors->velFordFL, xCM, totalCM);
            pid->backLeftOutput = evaluatePWMFunctionByType(motors->velFordBL, xCM, totalCM);
            pid->frontRightOutput = evaluatePWMFunctionByType(motors->velFordFR, xCM, totalCM);
            pid->backRightOutput = evaluatePWMFunctionByType(motors->velFordBR, xCM, totalCM);
        }            
    }      
    else if(direction=='4' || direction=='6'){
        pid->frontLeftOutput = evaluatePWMFunctionByType(motors->velHorFL, xCM, totalCM);
        pid->backLeftOutput = evaluatePWMFunctionByType(motors->velHorBL, xCM, totalCM);
        pid->frontRightOutput = evaluatePWMFunctionByType(motors->velHorFR, xCM, totalCM);
        pid->backRightOutput = evaluatePWMFunctionByType(motors->velHorBR, xCM, totalCM);        
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
            // bno055->offsetAngle = bno055->offsetAngleForward;
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
            // bno055->offsetAngle = bno055->offsetAngleForward;
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
            pid->SetTunings(pid->horizontalKp, pid->horizontalKi, pid->horizontalKd);
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
            pid->SetTunings(pid->horizontalKp, pid->horizontalKi, pid->horizontalKd);
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
            // bno055->offsetAngle = -bno055->offsetAngleForward;
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
            // bno055->offsetAngle = -bno055->offsetAngleForward;
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
            pid->SetTunings(pid->turnKp, pid->turnKi, pid->turnKd);
            pid->computeOutput_bno(bno055->rawInput, bno055->lastInput);
            if(angleDifference > 1){ // MOVE TO THE LEFT
                bno055->offsetAngle = bno055->offsetAngleTurn;   
                pid->frontLeftOutput += pid->Output;
                pid->backLeftOutput += pid->Output;
                pid->frontRightOutput += pid->Output;
                pid->backRightOutput += pid->Output; 
                motors->setMotor(0, 1, 0, 1, 1, 0, 1, 0);            
            }
            else if(angleDifference < -1){ // MOVE TO THE RIGHT
                bno055->offsetAngle = -bno055->offsetAngleTurn;   
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
    // if(moveCalled>8 && crazyMode){
    //     crazyMove(direction);
    //     moveCalled=0;
    // }
    updateSensors(1,0,0,0,1,1);
    setBaseVelocitiesByDirection(goSlow, direction, 0, 0);
    calculateAngleOutputsByDirection(goSlow, direction);         
    verifyAndUploadOutputsByDirection(direction);   
    if(encoder->stepsMechanism >= untilStepsMechanism){
        motors->stopMechanism();
        untilStepsMechanism=0;
        encoder->encoderStateMechanism = 0; 
    }   
    // moveCalled++;     
} 
// TODO:
//forward with P correction
void _Movements::movePID(bool goSlow, char direction, double xCM, double totalCM){   
    updateSensors(1,0,0,0,1,1);
    setBaseVelocitiesByDirection(goSlow, direction, xCM, totalCM);
    calculateAngleOutputsByDirection(goSlow, direction);         
    verifyAndUploadOutputsByDirection(direction);   
    if(encoder->stepsMechanism >= untilStepsMechanism){
        motors->stopMechanism();
        untilStepsMechanism=0;
        encoder->encoderStateMechanism = 0; 
    }    
} 
// TODO:
void _Movements::crazyMove(char direction){
    if(direction=='6' && crazyRight){
        pid->frontLeftOutput = 255;
        pid->backLeftOutput = 255;
        pid->frontRightOutput = 255;
        pid->backRightOutput = 255;
        motors->setMotor(1, 0, 1, 0, 0, 1, 0, 1);             
        crazyRight=!crazyRight;
    }
    else if(direction=='6' && !crazyRight){
        pid->frontLeftOutput = 255;
        pid->backLeftOutput = 255;
        pid->frontRightOutput = 255;
        pid->backRightOutput = 255;                
        motors->setMotor(0, 1, 0, 1, 1, 0, 1, 0);
        crazyRight=!crazyRight;
    }
    else if(direction=='4' && crazyRight){
        pid->frontLeftOutput = 255;
        pid->backLeftOutput = 255;
        pid->frontRightOutput = 255;
        pid->backRightOutput = 255;
        motors->setMotor(1, 0, 1, 0, 0, 1, 0, 1);             
        crazyRight=!crazyRight;
    }
    else if(direction=='4' && !crazyRight){
        pid->frontLeftOutput = 255;
        pid->backLeftOutput = 255;
        pid->frontRightOutput = 255;
        pid->backRightOutput = 255;                
        motors->setMotor(0, 1, 0, 1, 1, 0, 1, 0);
        crazyRight=!crazyRight;
    } 
    verifyAndUploadOutputsByDirection(direction); 
    delay(20);     
}
// TODO:
void _Movements::spinPID(bool goSlow, double newAngle){
    int maxTime=2000;
    if(abs(newAngle) == 180)
        maxTime=2500; 
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
    } while(millis() < startTime+maxTime);    
    motors->brake();    
}
// TODO:
void _Movements::turnPID(bool goSlow) {
//  Update Sensors    
    updateSensors(0,0,0,0,0,0);    
//  Update Outputs
    setBaseVelocitiesByDirection(goSlow, 'T', 0, 0);
    calculateAngleOutputsByDirection(goSlow, 'T'); 
    verifyAndUploadOutputsByDirection('T'); 
    if(encoder->stepsMechanism >= untilStepsMechanism){
        untilStepsMechanism=0;
        motors->stopMechanism();
        encoder->encoderStateMechanism = 0; 
    }              
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
    encoder->encoderStateFR=1; encoder->encoderStateBL=1;
//  Counts of encoder to get to the objective
    int untilSteps = (encoder->encoder30Cm / 30) * cm;
//  Restart encoder counts
    encoder->stepsFR=0; encoder->stepsBL=0;    
//  Move with p correction until the encoder read the cm
    while (encoder->stepsFR < untilSteps){              
        // movePID(goSlow, direction, encoder->stepsFR, untilSteps);
        movePID(goSlow, direction, 0,0);        
    }
    motors->brake();
    encoder->encoderStateFR=0; encoder->encoderStateBL=0;
}
// TODO:
void _Movements::align_tof(){
//  Local Variables    
    int countCorrect = 0;
    double startTime = millis();
//  Init baseVelocity as SlowTurnVel and offSetAngle
    bno055->offsetAngle = bno055->offsetAngleTurn; 
    // Serial.println("START");
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
    } while(millis() < startTime+3000);
    motors->brake();
    updateSensors(1,0,0,0,0,0);
    pid->Setpoint = bno055->rawInput;
}
// TODO:
//Go forward until finding a wall at a certain distance
void _Movements::getCloseToStack(){
    for(int i=0; i<10; i++)
        updateSensors(0,0,0,1,0,0);
//  While not at ceratin distance from wall
    do{                 
        if (timeFlight->timeFlightRight.kalmanDistance<0.6 || timeFlight->timeFlightLeft.kalmanDistance<0.6)
            break;    
        movePID(true, '6');                        
        updateSensors(0,0,0,1,0,0);
    } while(1);
    motors->brake();
    delay(100);  
}
// TODO:
void _Movements::moveToShip(bool goBack){
    int inSeaArea=0;
    do{   //Move Until Ship
        // lcd->print("Move to Ship:", "moving");
        updateSensors(0,0,0,0,1,1);
        movePID(true, '6');
        if(goBack){
            if(tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP 
            || tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
                motors->brake();
                delay(20);
                if(++inSeaArea > 1)
                    break;        
            }            
        }
        else{
            if(tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP 
            || tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
                motors->brake();
                delay(20);                
                if(++inSeaArea > 3)
                    break;        
            }                     
        }
    } while(1);
    // lcd->print("Move to Ship:", "DONE");
    motors->brake();  
    if(goBack)  movePID_nCM(3, false, '4');  
}
// TODO:
void _Movements::alignShip(){
    int doneAligning=0;    
    do{
        // lcd->print("Alignments:", doneAligning);      
        updateSensors(0,0,0,0,1,0);
        if(tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP && tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
            if(++doneAligning > 2)  break;
            else
                movePID_nCM(1.5, true, '4');
        }    
        if(tcrt5000->tcrtMechaLeft.kalmanDistance<tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP && tcrt5000->tcrtMechaRight.kalmanDistance<tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
            movePID(true, '6');
            delay(25);
        }
        else if(tcrt5000->tcrtMechaLeft.kalmanDistance>=tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP && tcrt5000->tcrtMechaRight.kalmanDistance<tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
            movePID_nCM(1.6, true, '4');
            delay(40);
            pid->calculateNewSetpoint(-0.5);
            for(int i=0; i<80; i++){
                turnPID(true);  
            }   
        }
        else if(tcrt5000->tcrtMechaLeft.kalmanDistance<tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP && tcrt5000->tcrtMechaRight.kalmanDistance>=tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
            movePID_nCM(1.6, true, '4');
            delay(40);
            pid->calculateNewSetpoint(0.5);
            for(int i=0; i<80; i++){
                turnPID(true);  
            }      
        }        
    } while(1);
    // movePID_nCM(0.5, true, '4');     
    motors->brake();   
    delay(50);
    updateSensors(1,0,0,0,0,0);    
    pid->Setpoint = bno055->rawInput;    
}
// TODO:
void _Movements::alignFirstShip(){
    char direction;
    updateSensors(0,0,0,0,1,0);
    if(tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP && tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP)
        direction = '8';
    else if(tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP)
        direction = '2';
    else if(tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP)
        direction = '8';
    do{
        updateSensors(0,0,0,0,1,0);
        if(direction == '2'){
            if(tcrt5000->tcrtMechaRight.kalmanDistance<tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP){
                motors->brake();
                movePID_nCM(8.5, true, '8');                  
                break;
            }
            movePID(true, '2');
        }
        else if(direction == '8'){
            if(tcrt5000->tcrtMechaLeft.kalmanDistance<tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP){
                motors->brake();
                movePID_nCM(8.5, true, '2');                  
                break;
            }
            movePID(true, '8');
        }        
    } while(1);
}
// TODO:
void _Movements::centerContainer(char orientation){
    double stepsToMove = 6.7;
    char direction;    
    // for(int i=0; i<30; i++)
    updateSensors(0,0,0,1,0,0);
    if(orientation == ' '){
        if(timeFlight->timeFlightLeft.kalmanDistance<10 && timeFlight->timeFlightRight.kalmanDistance<10)
            direction = '8';
        else if(timeFlight->timeFlightRight.kalmanDistance<10)
            direction = '2';
        else if(timeFlight->timeFlightLeft.kalmanDistance<10)
            direction = '8'; 
    }   
    else
        direction = orientation;
    do{
        updateSensors(0,0,0,1,0,0);
        if(direction == '2'){
            if(timeFlight->timeFlightRight.kalmanDistance > 7){
                motors->brake();
                movePID_nCM(stepsToMove, true, '8');                  
                break;
            }
            movePID(true, '2');
        }
        else if(direction == '8'){
            if(timeFlight->timeFlightLeft.kalmanDistance > 7){
                motors->brake();
                movePID_nCM(stepsToMove, true, '2');                  
                break;
            }
            movePID(true, '8');
        }  
    } while(1);
}
// FIXME: 
/*
    secondLine => TRUE only when it is needed to ignore the first horizontal black line detected
    pointingNorth => TRUE when robot is pointing front to the trains
    goVerticalLine => TRUE when vertical black line is the destination
*/
// TODO:
void _Movements::larc_moveUntilBlackLine(bool goSlow, char direction, bool frontTCRT, bool goVerticalLine, bool secondLine, bool tcrtSharps, bool takePhoto){
    int nLine=0;
    do{
        updateSensors(0,0,0,0,1,1);
        movePID(goSlow, direction);
        if(direction=='8'){
            if(tcrt5000->tcrtMidFrontRight.kalmanDistance>BLACKLINE_TRIGGER || tcrt5000->tcrtMidFrontLeft.kalmanDistance>BLACKLINE_TRIGGER)
                if(secondLine){
                    if(++nLine<2)   delay(400);
                    else            break;
                } 
                else                break;
        }    
        else if(direction=='2'){
            if(tcrt5000->tcrtMidDownRight.kalmanDistance>BLACKLINE_TRIGGER || tcrt5000->tcrtMidDownLeft.kalmanDistance>BLACKLINE_TRIGGER)
                if(secondLine){
                    if(++nLine<2)   delay(400);
                    else            break;
                } 
                else                break;
        }               
        else if(direction=='4' || direction=='6'){
            if(tcrtSharps){
                if(tcrt5000->tcrtSharpLeft.kalmanDistance>BLACKLINE_TRIGGER || tcrt5000->tcrtSharpRight.kalmanDistance>BLACKLINE_TRIGGER)
                    if(secondLine){
                        if(++nLine<2)   delay(1);
                        else            break;
                    } 
                    else                break;
            }
            if(frontTCRT){
                if(direction=='4'){
                    if(tcrt5000->tcrtMidFrontLeft.kalmanDistance>BLACKLINE_TRIGGER)
                        if(secondLine){
                            if(++nLine<2)   delay(500);
                            else            break;
                        } 
                        else                break;
                } 
                else if(direction=='6'){
                    if(tcrt5000->tcrtMidFrontRight.kalmanDistance>BLACKLINE_TRIGGER)
                        if(secondLine){
                            if(++nLine<2)   delay(500);
                            else            break;
                        } 
                        else                break;
                }                        
            }
            else{
                if(direction=='4'){
                    if(tcrt5000->tcrtMidDownLeft.kalmanDistance>BLACKLINE_TRIGGER)
                        if(secondLine){
                            if(++nLine<2)   delay(500);
                            else            break;
                        } 
                        else                break;
                } 
                else if(direction=='6'){
                    if(tcrt5000->tcrtMidDownRight.kalmanDistance>BLACKLINE_TRIGGER)
                        if(secondLine){
                            if(++nLine<2)   delay(500);
                            else            break;
                        } 
                        else                break;
                }      
            }
        } 
    } while(1);
    motors->brake();
    if(direction=='8')
        movePID_nCM(1.8, true, '8');
    else if(direction=='2')
        movePID_nCM(1.8, true, '2'); 
    else if(takePhoto){
        if(direction=='6')
            movePID_nCM(1.2, true, '6');
            // movePID_nCM(0, true, '6');
        else if(direction=='4')
            movePID_nCM(1.2, true, '4'); 
            // movePID_nCM(3.5, true, '4');
    }
    else if(goVerticalLine){
        if(direction=='4' && frontTCRT)
            movePID_nCM(1.9, true, '4'); 
        else if(direction=='6' && frontTCRT)
            movePID_nCM(1.1, true, '6');               
        else if(direction=='6' && !frontTCRT)
            movePID_nCM(1.1, true, '6');     
        else if(direction=='4' && !frontTCRT)
            movePID_nCM(1.9, true, '4');                               
    }     
    else{
        if(direction=='6')
            movePID_nCM(1.8, true, '6');
            // movePID_nCM(0, true, '6');
        else if(direction=='4')
            movePID_nCM(1.8, true, '4'); 
            // movePID_nCM(3.5, true, '4');        
    }               
    motors->brake();               
}
// TODO:
void _Movements::larc_moveBetweenVerticalBlackLine(bool goSlow, char direction, bool shipToStack){
    while(1){
        updateSensors(0,0,0,0,1,0);
        movePID(goSlow, direction);
        if(shipToStack){
            if(direction == '8'){
                if(tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP)
                    break;
            }
            else if(direction == '2'){
                if(tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP)
                    break;
            }   
        }
        else{
            if(direction == '8'){
                if(tcrt5000->tcrtMechaRight.kalmanDistance>tcrt5000->rightMechanism+BLACKLINE_TRIGGER_SHIP)
                    break;
            }
            else if(direction == '2'){
                if(tcrt5000->tcrtMechaLeft.kalmanDistance>tcrt5000->leftMechanism+BLACKLINE_TRIGGER_SHIP)
                    break;
            }
        }     
    }
    motors->brake();
}
// TODO:
void _Movements::moveMechanism(int lastStackLevel, int newStackLevel){
    encoder->encoderStateMechanism = 1;  
    int difference = newStackLevel - lastStackLevel;
    if(lastStackLevel<1)    lastStackLevel=1; 
    if(lastStackLevel>5)    lastStackLevel=5;
    if(newStackLevel<1)     newStackLevel=1;
    if(newStackLevel>5)      newStackLevel=5;
    (lastStackLevel == 1 || newStackLevel == 1) ?
        untilStepsMechanism = 7100 * abs(newStackLevel - lastStackLevel) - 4200:
        untilStepsMechanism = 7100 * abs(newStackLevel - lastStackLevel);
//  Restart encoder counts
    encoder->stepsMechanism = 0;
//  Move with p correction until the encoder read the cm
    while (encoder->stepsMechanism < untilStepsMechanism)   
        (newStackLevel > lastStackLevel) ? motors->moveMechanism(true): motors->moveMechanism(false);
    motors->stopMechanism();
    untilStepsMechanism=0;
    encoder->encoderStateMechanism = 0;  
}
// TODO:
void _Movements::alignLine(){
    double velocityToAlign = 70;      
    int times=0;
    do{
        updateSensors(0,0,0,0,1,1);  
        if(tcrt5000->tcrtMidFrontLeft.kalmanDistance<BLACKLINE_TRIGGER && tcrt5000->tcrtMidDownLeft.kalmanDistance<BLACKLINE_TRIGGER && tcrt5000->tcrtMidFrontRight.kalmanDistance<BLACKLINE_TRIGGER && tcrt5000->tcrtMidDownRight.kalmanDistance<BLACKLINE_TRIGGER){
            // lcd->print("Done", "");
            motors->brake();
            times++;
            delay(100);
            if(times > 10)
                break;
        }
        else if(tcrt5000->tcrtMidFrontLeft.kalmanDistance>BLACKLINE_TRIGGER || tcrt5000->tcrtMidDownRight.kalmanDistance>BLACKLINE_TRIGGER){
            // lcd->print("Move Left", "");
            pid->frontLeftOutput = velocityToAlign;
            pid->backLeftOutput = velocityToAlign;
            pid->frontRightOutput = velocityToAlign;
            pid->backRightOutput = velocityToAlign;
            motors->setMotor(0, 1, 0, 1, 1, 0, 1, 0);   
            motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);            
        }
        else if(tcrt5000->tcrtMidFrontRight.kalmanDistance>BLACKLINE_TRIGGER || tcrt5000->tcrtMidDownLeft.kalmanDistance>BLACKLINE_TRIGGER){
            // lcd->print("Move Right", "");
            pid->frontLeftOutput = velocityToAlign;
            pid->backLeftOutput = velocityToAlign;
            pid->frontRightOutput = velocityToAlign;
            pid->backRightOutput = velocityToAlign;
            motors->setMotor(1, 0, 1, 0, 0, 1, 0, 1);
            motors->setVelocity(pid->frontLeftOutput, pid->backLeftOutput, pid->frontRightOutput, pid->backRightOutput);                
        }        
    } while(1);
    updateSensors(1,0,0,0,0,0);
    pid->Setpoint = bno055->rawInput;
}
// TODO:
void _Movements::moveMechanismForAligning(bool before){
    encoder->encoderStateMechanism = 1;  
        untilStepsMechanism = 800;
//  Restart encoder counts
    encoder->stepsMechanism = 0;
//  Move with p correction until the encoder read the cm 
    (before) ? motors->moveMechanism(true): motors->moveMechanism(false);     
}