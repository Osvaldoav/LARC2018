#include <_Traductor.h>

_Traductor::_Traductor(){
    movements = new _Movements;
}

void _Traductor::horizontalLine(bool b){
    char c = b ? '8' : '2';
    movements->larc_moveBetweenVerticalBlackLine(false, c, false);
}

void _Traductor::throughtHorizontal(int dir){
    double cm = abs(dir) < 2 ? 22 : 66;
    char c = dir < 0 ? '6' : '4';
    movements->crazyMode=true;
    movements->movePID_nCM(cm, false, c);
    movements->crazyMode=false;
}

void _Traductor::throughtHorizontal2(int dir, bool front){
    double cm = abs(dir) < 2 ? 25 : abs(dir) < 3 ? 71 : abs(dir) < 4 ? 90 : 37;
    char c = dir < 0 ? '6' : '4';
    char frontDirection = front ? '2': '8';
    movements->movePID_nCM(4, false, frontDirection); 
    movements->crazyMode=true; 
    movements->movePID_nCM(cm, false, c);
    movements->crazyMode=false;
}

void _Traductor::girar(int angle){
    movements->spinPID(true, angle);
}

void _Traductor::avanzar(bool b){
    char c = b ? '8' : '2';
    movements->movePID_nCM(34, false, c);
}

void _Traductor::alinearPozo(){
    movements->larc_moveAndAlignToShip();
}

void _Traductor::alinearTren(){
    movements->movePID_nCM(5, false, '4');
    movements->larc_moveUntilBlackLine(false, '6', false, false, false, true);
    movements->movePID_nCM(20, false, '6');
}

void _Traductor::mecanismo(uint8_t newStack, uint8_t currentStack){
    movements->moveMechanism(newStack, currentStack);
}

void _Traductor::grabContainer(){
    movements->servo->pickContainer();
    delay(1000);
}

void _Traductor::dropContainer(){
    movements->servo->dropContainer();
    delay(1000);
}

void _Traductor::moveAtras(){
    movements->movePID_nCM(12, false, '4');
}

void _Traductor::moveToHorizontal(bool b){
    char c = b ? '8' : '2';
    movements->larc_moveUntilBlackLine(false, c, true, false, false, false);
}
void _Traductor::horizontal(int lines, bool tcrt){
    char c = lines < 0 ? '6' : '4'; 
    bool b = abs(lines) > 1;              
    movements->crazyMode=true;
    movements->larc_moveUntilBlackLine(false, c, tcrt, true, b, false);
    movements->crazyMode=false;
    char vertical = tcrt? '8': '2'; 
    do{
        movements->movePID(false, vertical);
        movements->updateSensors(0,0,0,0,1,1);        
        if(tcrt && movements->tcrt5000->tcrtSharpLeft.kalmanDistance>movements->BLACKLINE_TRIGGER)
            break;  
        else if(!tcrt && movements->tcrt5000->tcrtSharpRight.kalmanDistance>movements->BLACKLINE_TRIGGER)
            break;              
    } while(1);    
    movements->motors->brake();
}

void _Traductor::backUntilBlackLineSharps(bool tcrt){
    char vertical = tcrt? '8': '2'; 
    do{
        movements->movePID(false, vertical);
        movements->updateSensors(0,0,0,0,1,1);        
        if(tcrt && (movements->tcrt5000->tcrtSharpRight.kalmanDistance>movements->BLACKLINE_TRIGGER))
            break;  
        else if(!tcrt && (movements->tcrt5000->tcrtSharpLeft.kalmanDistance>movements->BLACKLINE_TRIGGER))        
            break;              
    } while(1);    
    movements->motors->brake();
}

void _Traductor::moveAtrasHorizontal(){
    movements->larc_moveUntilBlackLine(false, '4', false, true, false, true);
    movements->movePID_nCM(2.5, false, '4');
}

void _Traductor::vertical(int lines){
    int steps = abs(lines) > 1 ? 41.6 : 19;
    char dir = lines < 0 ? '8' : '2';
    movements->movePID_nCM(steps, false, dir);
}

void _Traductor::alinearStack(bool dir){ 
    char c = dir ? '6' : '4';
    movements->movePID_nCM(1, true, c);  
    delay(500); 
    // movements->alignLine();
    // movements->getCloseToStack();
    // movements->align_tof();
} 

void _Traductor::gotoFirst(){
    movements->larc_moveUntilBlackLine(false, '8', true, false, true, false);
    movements->movePID_nCM(1.8, false, '8');
    movements->crazyMode=true;
    movements->larc_moveUntilBlackLine(false, '6', true, true, false, false);
    movements->crazyMode=false;
    do{
        movements->movePID(false, '2');
        movements->updateSensors(0,0,0,0,1,1);
    } while(movements->tcrt5000->tcrtMechaLeft.kalmanDistance < movements->BLACKLINE_TRIGGER);
    movements->movePID_nCM(31, false, '8');
    delay(1000);
}

void _Traductor::gotoSecond(){
    updateMechanismMovement(1, 4, false);
    moveAtrasHorizontal();
    movements->spinPID(true, -90);
    movements->movePID_nCM(1.8, false, '8');   
    movements->crazyMode=true; 
    movements->larc_moveUntilBlackLine(false, '6', true, true, false, false);
    movements->crazyMode=false;
    do{
        movements->movePID(false, '2');
        movements->updateSensors(0,0,0,0,1,1);
    } while(movements->tcrt5000->tcrtMechaLeft.kalmanDistance < movements->BLACKLINE_TRIGGER);   
    waitForMechanism();
    movements->movePID_nCM(30, false, '8');
    delay(1000);
}

void _Traductor::pickFirst(int stack){
    // char c = stack%4 < 3 ? '8' : '2';
    int steps = stack%4 < 3 ? 24: 2.5;
    movements->movePID_nCM(steps, false, '8');
}
// TODO:
void _Traductor::LcdPrint(String name, char value){
    movements->lcd->print(name, value);
}
// TODO:
void _Traductor::LcdPrint(String name, int value){
    movements->lcd->print(name, value);
}
// TODO:
void _Traductor::LcdPrint(String name, String value){
    movements->lcd->print(name, value);
}
// TODO:
void _Traductor::LcdPrint(String name, double value){
    movements->lcd->print(name, value);
}
// TODO:
void _Traductor::updateMechanismMovement(int actualLevel, int newLevel, bool train){
    // set new untilStepsMechanism value
    movements->encoder->encoderStateMechanism = 1;
    if (actualLevel == 1 || newLevel == 1 || train)
        movements->untilStepsMechanism = 7100 * abs(newLevel - actualLevel) - 3750; //6900 en fantasma
    else 
        movements->untilStepsMechanism = 7100 * abs(newLevel - actualLevel);
    // reset steps count
    movements->encoder->stepsMechanism = 0;    
    // start moving
    if (newLevel > actualLevel)            
        movements->motors->moveMechanism(true);
    else
        movements->motors->moveMechanism(false);    
}
// TODO:
void _Traductor::waitForMechanism(){
    movements->motors->brake();
    while(movements->encoder->encoderStateMechanism == 1){
        if(movements->encoder->stepsMechanism >= movements->untilStepsMechanism){
            movements->untilStepsMechanism=0;
            movements->motors->stopMechanism();
            movements->encoder->encoderStateMechanism = 0; 
        } 
    } 
    movements->motors->brake();
}
// TODO:
void _Traductor::fixContainerSteps(char greenShip){
    bool dir = (greenShip) ? '2' : '8';
    movements->movePID_nCM(3.5, false, '2');
}
// TODO:
void _Traductor::centerContainer(){
    movements->movePID_nCM(1.5, false, '4');
    for(int i=0; i<50; i++)
        movements->updateSensors(0,0,0,1,0,0);
    do{
        movements->movePID(true, '2');
        movements->updateSensors(0,0,0,1,0,0);
        // LcdPrint("TOF distance:", movements->timeFlight->timeFlightRight.kalmanDistance);
    } while(movements->timeFlight->timeFlightRight.kalmanDistance < 7);
    movements->motors->brake();
    movements->movePID_nCM(8, false, '8');
    alinearPozo();
}
// TODO:
// when (stackToShip == true) -> level is your actual level
// when (stackToShip == false) -> level is your next level
void _Traductor::setTrainLevel(bool stackToShip){
    (stackToShip) ? updateMechanismMovement(3, 2, true): updateMechanismMovement(2, 3, true);
}