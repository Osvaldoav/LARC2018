#include <_Servo.h>

//////////////////////// PINs DECLARATION ////////////////////////
int servoPWM = 13;
//////////////////////// LOCAL VARIABLES /////////////////////////
int minValue=670, maxValue=2400;
int pos = 0;    // variable to store the servo position
Servo servo;  // create servo object to control a servo

// TODO:
void _Servo::setupServo(){
    servo.attach(servoPWM, minValue, maxValue);
    servo.write(175);
}
// TODO:
void _Servo::pickContainer(){
    servo.write(175);
}
// TODO:
void _Servo::dropContainer(){
    servo.write(0);
}