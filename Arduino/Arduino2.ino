//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// LARC 2018 //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////    
//#include <_Serial.h>
//#include <_Logic.h>
//#include <Wire.h>
//_Logic *logic = new _Logic;
////
////const byte pinEncoder = 40;
////const byte ledRed = 37;
////const byte ledGreen = 36;
////const byte ledBlue = 35;
////
//char c_serial;
////
////void blink(int t){
////   for(int i = 0; i < t; i++){
////       digitalWrite(13, HIGH);
////       delay(500);
////       digitalWrite(13, LOW);
////       delay(500);
////   }
////}
////
////char color;
////
/////////////////////////////////////////////////SETUP////////////////////////////////////////////////
////void setup() {
////  pinMode(13, OUTPUT);
////  Serial.begin(9600);
//////   Wire.begin();
//////   logic->traductor->movements->pid->setupLibraryPID();
//////   logic->traductor->movements->motors->setupMotors();
//////   logic->traductor->movements->colorSensor->setupColorSensor();
//////   logic->traductor->movements->bno055->setupBNO055();
//////   logic->traductor->movements->timeFlight->setupTimeFlight();
//////   pinMode(pinEncoder, INPUT_PULLUP);
//////   attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);
////
////  //Starts the communication
////  c_serial = _Serial::read();
////  blink(2);
////  _Serial::send('1');
////
////  //move forward
////  //move to right
////  //move forward 
////  //stop
////  blink(2);
////  _Serial::send('1');
////  c_serial = _Serial::read();
////  color = c_serial > 98 ? 'R' : c_serial > 65 ? 'G' : 'B';
////  blink(2);
////  _Serial::send(color);
//////    logic->firstPick(c_serial); // Se mueve al stack 6 o 7
//////    _Serial::send(logic->grabContainer(c_serial));
////
////   
////}
////
////void loop(){
//////  logic->stackToShip();
////c_serial = _Serial::read();
////color = c_serial > 98 ? 'R' : c_serial > 65 ? 'G' : 'B';
////blink(2);
//////  if(c_serial == 'S'){
//////    logic->gotoSecond(); // pendiente
//////    _Serial::send('1');
//////    c_serial = _Serial::read();
//////    logic->moveSecond(c_serial); // pendiente
//////    _Serial::send(logic->grabContainer(c_serial));
//////  }else{
//////    logic->shipToStack();
//////    _Serial::send(logic->grabContainer(c_serial));
//////  }
////   _Serial::send(color);
////}
//
//void setup(){
//    Serial.begin(9600);
//    Wire.begin(); 
//    logic->traductor->movements->lcd->setupLCD(); 
//  c_serial = _Serial::read();
//  _Serial::send('1');
//  _Serial::send('1');  
//}
//
//void loop(){
//  c_serial = _Serial::read();
//  logic->traductor->LcdPrint("STACK", c_serial);
//  delay(5
//  00);
//  _Serial::send(c_serial+1);
//}

