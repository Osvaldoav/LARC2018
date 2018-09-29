#include <_Traductor.h>

void _Traductor::horizontalLine(bool b){
    char c = b ? '8' : '2';
    movements->larc_moveBetweenVerticaBlackLine(false, c);
}

void _Traductor::throughtHorizontal(int dir){
    double cm = abs(dir) < 2 ? 20 : 60;
    char c = dir < 0 ? '6' : '4';
    movements->movePID_nCM(cm, false, c);
}

void _Traductor::throughtHorizontal2(int dir){
    double cm = dir < 2 ? 20 : dir < 3 ? 60 : dir < 4 ? 80 : 40;
    char c = dir < 0 : '6' : '4';
    movements->movePID_nCM(cm, false, c);
}

void _Traductor::girar(int angle){
    movements->spinPID(false, angle);
}

void _Traductor::avanzar(bool b){
    char c = b ? '8' : '2';
    movements->movePID_nCM(45, false, c);
}

void _Traductor::alineaPozo(){
    movements->larc_moveAndAlignToShip();
}

void _Traductor::alinearTren(){
    movements->movePID_nCM(32.55, false, '6');
}

void _Traductor::mecanismo(u_int8_t s){
    
}

void _Traductor::moveAtras(){
    movements->movePID_nCM(22.6, false, '4');
}

void _Traductor:moveToHorizontal(bool b){
    char c = b ? '8' : '2';
    movevements->larc_moveUntilBlackLine(false, c, true, false, false);
}
void _Traductor::horizontal(int lines){
    char c = lines < 0 ? '6' : '4';
    movements->larc_moveUntilBlackLines(false, c, );
    //pendiente
}

