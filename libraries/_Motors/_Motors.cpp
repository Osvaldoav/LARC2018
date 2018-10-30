#include <_Motors.h>

//////////////// PINs DECLARATION "BICHO" ///////////////////
// const byte pinMotorFR_F = 52;
// const byte pinMotorFR_B = 53;
// const byte pinMotorFL_F = 46;
// const byte pinMotorFL_B = 47;
// const byte pinMotorBR_F = 50;
// const byte pinMotorBR_B = 51;
// const byte pinMotorBL_F = 49;
// const byte pinMotorBL_B = 48;
// const byte pinPWM_FR = 2; 
// const byte pinPWM_FL = 5;
// const byte pinPWM_BR = 3;
// const byte pinPWM_BL = 4;
//////////////// PINs DECLARATION "LARC" ///////////////////
const byte pinPWMFR_F = 8;
const byte pinPWMFR_B = 9;
const byte pinPWMFL_F = 7;
const byte pinPWMFL_B = 6;
const byte pinPWMBR_F = 5;
const byte pinPWMBR_B = 4;
const byte pinPWMBL_F = 3;
const byte pinPWMBL_B = 12;

const byte pinMechanismUp = 10;
const byte pinMechanismDown = 11; 
////////////////////// LOCAL VARIABLES /////////////////
bool boolFL_F;
bool boolFL_B;
bool boolBL_F;
bool boolBL_B;
bool boolFR_F;
bool boolFR_B;
bool boolBR_F;
bool boolBR_B;

double const HORIZONTAL_OFFSET_POWER = 40;//50 (12.4V)  40(11.28V)

void _Motors::setupMotors(){
    velFordFL = 200;//200
    velFordBL = 200;
    velFordFR = 200;
    velFordBR = 200; 
    velSlowFordFL = 120;
    velSlowFordBL = 120; 
    velSlowFordFR = 120;
    velSlowFordBR = 120;        
    velTurnFL = 175;
    velTurnBL = 175;
    velTurnFR = 175;
    velTurnBR = 175;
    velSlowTurnFL = 50;
    velSlowTurnBL = 50;
    velSlowTurnFR = 50;
    velSlowTurnBR = 50;   
    velHorFL = 200;//185
    velHorBL = (200-HORIZONTAL_OFFSET_POWER)*0.9666;//0.9666
    velHorFR = (200-HORIZONTAL_OFFSET_POWER);
    velHorBR = 200;       

    pinMode(pinPWMFR_F,OUTPUT);
    pinMode(pinPWMFR_B,OUTPUT);
    pinMode(pinPWMFL_F,OUTPUT);
    pinMode(pinPWMFL_B,OUTPUT);  
    pinMode(pinPWMBR_F,OUTPUT);
    pinMode(pinPWMBR_B,OUTPUT);
    pinMode(pinPWMBL_F,OUTPUT);
    pinMode(pinPWMBL_B,OUTPUT);  
}

// TODO:
double _Motors::slowGo(double time){
    return 100*(1-exp(-.2*time/5));
}

// TODO:
void _Motors::setMotor(bool newFL_F, bool newFL_B, bool newBL_F, bool newBL_B, bool newFR_F, bool newFR_B, bool newBR_F, bool newBR_B){
    boolFL_F = newFL_F;
    boolFL_B = newFL_B;
    boolBL_F = newBL_F;
    boolBL_B = newBL_B;
    boolFR_F = newFR_F;
    boolFR_B = newFR_B;
    boolBR_F = newBR_F;
    boolBR_B = newBR_B;
}

// TODO:
void _Motors::setVelocity(int left, int right){
    analogWrite(pinPWMFL_F, left);
    analogWrite(pinPWMFL_B, left);
    analogWrite(pinPWMBL_F, left);
    analogWrite(pinPWMBL_B, left);     
    analogWrite(pinPWMFR_F, right);
    analogWrite(pinPWMFR_B, right);     
    analogWrite(pinPWMBR_F, right);
    analogWrite(pinPWMBR_B, right);  
}

// TODO:
void _Motors::setVelocity(int frontLeft, int backLeft, int frontRight, int backRight){
    (boolFL_F)? analogWrite(pinPWMFL_F, frontLeft): analogWrite(pinPWMFL_F, 0); 
    (boolFL_B)? analogWrite(pinPWMFL_B, frontLeft): analogWrite(pinPWMFL_B, 0); 
    (boolBL_F)? analogWrite(pinPWMBL_F, backLeft): analogWrite(pinPWMBL_F, 0); 
    (boolBL_B)? analogWrite(pinPWMBL_B, backLeft): analogWrite(pinPWMBL_B, 0);      
    (boolFR_F)? analogWrite(pinPWMFR_F, frontRight): analogWrite(pinPWMFR_F, 0); 
    (boolFR_B)? analogWrite(pinPWMFR_B, frontRight): analogWrite(pinPWMFR_B, 0);      
    (boolBR_F)? analogWrite(pinPWMBR_F, backRight): analogWrite(pinPWMBR_F, 0); 
    (boolBR_B)? analogWrite(pinPWMBR_B, backRight): analogWrite(pinPWMBR_B, 0);  
}

// TODO:
//Stop motors
void _Motors::brake(){
    setMotor(0,0,0,0,0,0,0,0);
    setVelocity(0, 0);
}

// TODO:
void _Motors::moveMechanism(bool up){
    (up)? digitalWrite(pinMechanismUp,HIGH): digitalWrite(pinMechanismDown,HIGH);
}
// TODO:
void _Motors::stopMechanism(){
    digitalWrite(pinMechanismDown,LOW);
    digitalWrite(pinMechanismUp,LOW);
}