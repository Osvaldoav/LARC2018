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
    servo.write(35);
}
// TODO:
void _Servo::pickContainer(){
    servo.write(150);
    delay(1000);
}
// TODO:
void _Servo::dropContainer(){
    servo.write(35);
    delay(1000);
}