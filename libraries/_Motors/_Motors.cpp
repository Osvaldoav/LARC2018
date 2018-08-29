#include <_Motors.h>

//////////////// PINs DECLARATION ///////////////////
const byte pinMotorFR_F = 52;
const byte pinMotorFR_B = 53;
const byte pinMotorFL_F = 46;
const byte pinMotorFL_B = 47;
const byte pinMotorBR_F = 50;
const byte pinMotorBR_B = 51;
const byte pinMotorBL_F = 49;
const byte pinMotorBL_B = 48;
const byte pinPWM_FR = 2; 
const byte pinPWM_FL = 5;
const byte pinPWM_BR = 3;
const byte pinPWM_BL = 4;
////////////////////// LOCAL VARIABLES /////////////////

void _Motors::setupMotors(){
    velSlowFL = 82;
    velSlowBL = 82; 
    velSlowFR = 82;
    velSlowBR = 82;
    velGenFL = 115;
    velGenBL = 115;
    velGenFR = 115;
    velGenBR = 115;
    velTurnFL = 79;//55
    velTurnBL = 79;
    velTurnFR = 79;
    velTurnBR = 79;

    pinMode(pinMotorFR_F,OUTPUT);
    pinMode(pinMotorFR_B,OUTPUT);
    pinMode(pinMotorFL_F,OUTPUT);
    pinMode(pinMotorFL_B,OUTPUT);  
    pinMode(pinMotorBR_F,OUTPUT);
    pinMode(pinMotorBR_B,OUTPUT);
    pinMode(pinMotorBL_F,OUTPUT);
    pinMode(pinMotorBL_B,OUTPUT);  
    pinMode(pinPWM_FR,OUTPUT);
    pinMode(pinPWM_FL,OUTPUT);
    pinMode(pinPWM_BR,OUTPUT);
    pinMode(pinPWM_BL,OUTPUT);
}

// TODO:
double _Motors::slowGo(double time){
    return 100*(1-exp(-.2*time/5));
}

// TODO:
void _Motors::stop(bool isSpin){   
}      

// TODO:
void _Motors::go() {
}

// TODO:
void _Motors::back() {
}

// TODO:
void _Motors::setMotor(bool leftFrontF, bool leftFrontB, bool leftBackF, bool leftBackB, bool rightFrontF, bool rightFrontB, bool rightBackF, bool rightBackB){
    digitalWrite(pinMotorFR_F, rightFrontF);
    digitalWrite(pinMotorFR_B, rightFrontB);
    digitalWrite(pinMotorFL_F, leftFrontF);
    digitalWrite(pinMotorFL_B, leftFrontB);
    digitalWrite(pinMotorBR_F, rightBackF);
    digitalWrite(pinMotorBR_B, rightBackB);
    digitalWrite(pinMotorBL_F, leftBackF);
    digitalWrite(pinMotorBL_B, leftBackB);
}

// TODO:
void _Motors::setVelocity(int left, int right){
    analogWrite(pinPWM_FR, right);
    analogWrite(pinPWM_BR, right);
    analogWrite(pinPWM_FL, left);
    analogWrite(pinPWM_BL, left);
}

// TODO:
void _Motors::setVelocity(int leftFront, int leftBack, int rightFront, int rightBack){
    analogWrite(pinPWM_FR, rightFront);
    analogWrite(pinPWM_BR, rightBack);
    analogWrite(pinPWM_FL, leftFront);
    analogWrite(pinPWM_BL, leftBack);
}

// TODO:
//Stop motors
void _Motors::brake(){
    setVelocity(0, 0);
}
// TODO:
void _Motors::turnRight(int vel){
    setMotor(0, 1, 1, 0, 0, 1, 1, 0);
    setVelocity(vel, vel);
}
// TODO:
void _Motors::turnRight(int leftFront, int leftBack, int rightFront, int rightBack){
    setMotor(1, 0, 1, 0, 0, 1, 0, 1);
    setVelocity(leftFront, leftBack, rightFront, rightBack);
}
// TODO:
void _Motors::turnLeft(int vel){
    setMotor(1, 0, 0, 1, 1, 0, 0, 1);
    setVelocity(vel, vel);
}
// TODO:
void _Motors::turnLeft(int leftFront, int leftBack, int rightFront, int rightBack){
    setMotor(0, 1, 0, 1, 1, 0, 1, 0);
    setVelocity(leftFront, leftBack, rightFront, rightBack);
}
