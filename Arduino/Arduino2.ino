////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// LARC 2018 //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
//#include <_Serial.h>
//#include <_Logic.h>
//#include <Wire.h>
//_Logic *logic = new _Logic;
//
//const byte pinEncoder = 40;
//const byte ledRed = 37;
//const byte ledGreen = 36;
//const byte ledBlue = 35;
//
//char c_serial;
//
//void blink(int t){
//    for(int i = 0; i < t; i++){
//        digitalWrite(13, HIGH);
//        delay(500);
//        digitalWrite(13, LOW);
//        delay(500);
//    }
//}
//
///////////////////////////////////////////////SETUP////////////////////////////////////////////////
//void setup() {
//   pinMode(13, HIGH);
//   Serial.begin(9600);
//   Wire.begin();
//   logic->traductor->movements->pid->setupLibraryPID();
//   logic->traductor->movements->motors->setupMotors();
//   logic->traductor->movements->colorSensor->setupColorSensor();
//   logic->traductor->movements->bno055->setupBNO055();
//   logic->traductor->movements->timeFlight->setupTimeFlight();
//   pinMode(pinEncoder, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);
//
//   //Starts the communication
//   c_serial = _Serial::read();
//   blink(2);
//   delay(1500);
//   _Serial::send('1');
//
//   //move forward
//   //move to right
//   //move forward 
//   //stop
//   blink(4);
//   delay(1500);
//   _Serial::send('1');
//   c_serial = _Serial::read();
//   blink(6);
//   delay(1500);
//   _Serial::send('1');
////    logic->firstPick(c_serial); // Se mueve al stack 6 o 7
////    _Serial::send(logic->grabContainer(c_serial));
//
//    
//}
//
//void loop(){
////  logic->stackToShip();
// c_serial = _Serial::read();
//  blink(3);
//  delay(1500);
////  if(c_serial == 'S'){
////    logic->gotoSecond(); // pendiente
////    _Serial::send('1');
////    c_serial = _Serial::read();
////    logic->moveSecond(c_serial); // pendiente
////    _Serial::send(logic->grabContainer(c_serial));
////  }else{
////    logic->shipToStack();
////    _Serial::send(logic->grabContainer(c_serial));
////  }
//    _Serial::send('1');
//}
