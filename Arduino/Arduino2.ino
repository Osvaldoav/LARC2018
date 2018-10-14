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
///////////////////////////////////////////////SETUP////////////////////////////////////////////////
//void setup() {
//    Serial.begin(9600);
//    Wire.begin();
//    logic->traductor->movements->setupMovements();
//    logic->traductor->movements->pid->setupLibraryPID();
//    logic->traductor->movements->motors->setupMotors();
//    logic->traductor->movements->colorSensor->setupColorSensor();
//    logic->traductor->movements->bno055->setupBNO055();
//    logic->traductor->movements->timeFlight->setupTimeFlight();
//    pinMode(pinEncoder, INPUT_PULLUP);
//    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);
//
//    //Starts the communication
//    c_serial = _Serial::read();
//    _Serial::write('1');
//
//    //move forward
//    //move to right
//    //move forward
//    //stop
//    _Serial::write('1');
//    c_serial = _Serial::read();
//    logic->moveFirst(c_serial); // Se mueve al stack 6 o 7
//    _Serial::write(logic.grabContainer(c_serial));
//     
//}
//
//void loop(){
//  logic->stackToShip();
//  c_serial = _Serial.read();
//  if(c_serial == 'S'){
//    logic->gotoSecond(); // pendiente
//    _Serial::write('1');
//    c_serial = _Serial::read();
//    logic->moveSecond(c_serial); // pendiente
//    _Serial::write(logic.grabContainer(c_serial));
//  }else{
//    logic->shipToStack();
//    _Serial::write(logic.grabContainer(c_serial));
//  }
//}
