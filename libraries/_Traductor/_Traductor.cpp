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
    movements->movePID_nCM(cm, false, c);
}

void _Traductor::throughtHorizontal2(int dir){
    double cm = abs(dir) < 2 ? 25 : abs(dir) < 3 ? 71 : abs(dir) < 4 ? 94 : 49;
    char c = dir < 0 ? '6' : '4';
    movements->movePID_nCM(cm, false, c);
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
    movements->movePID_nCM(13, false, '6');
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
    char vertical = tcrt? '8': '2';   
    movements->larc_moveUntilBlackLine(false, c, tcrt, true, b, false);
    do{
        movements->updateSensors(0,0,0,0,1,0);
        movements->movePID(false, vertical);
        if(tcrt && movements->tcrt5000->tcrtSharpLeft.kalmanDistance>movements->BLACKLINE_TRIGGER)
            break;  
        else if(!tcrt && movements->tcrt5000->tcrtSharpRight.kalmanDistance>movements->BLACKLINE_TRIGGER)
            break;              
    } while(1);    
    movements->motors->brake(); 
}

void _Traductor::moveAtrasHorizontal(){
    movements->larc_moveUntilBlackLine(false, '4', false, false, false, true);
}

void _Traductor::vertical(int lines){
    int steps = abs(lines) > 1 ? 46 : 19;
    char dir = lines < 0 ? '8' : '2';
    movements->movePID_nCM(steps, false, dir);
}

void _Traductor::alinearStack(){
    movements->movePID_nCM(3, false, '6');
    // movements->align_tof();
}