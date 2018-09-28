//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LARC 2018 //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#include <_Movements.h>     
#include <_Serial.h>
#include <_Logic.h>
#include <Wire.h>
_Movements *movements = new _Movements;
_Logic logic = new _Logic;

const byte pinEncoder = 40;
const byte ledRed = 37;
const byte ledGreen = 36;
const byte ledBlue = 35;

char c_serial;

/////////////////////////////////////////////SETUP////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    Wire.begin();
    movements->setupMovements();
    movements->pid->setupLibraryPID();
    movements->motors->setupMotors();
    movements->colorSensor->setupColorSensor();
    movements->bno055->setupBNO055();
    movements->timeFlight->setupTimeFlight();
    pinMode(pinEncoder, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);

    //Starts the communication
    c_serial = _Serial::read();
    _Serial::write('1');

    //move forward
    //move to right
    //move forward
    //stop
    _Serial::write('1');
    c_serial = _Serial::read();
    logic.moveFirst(c_serial); // Se mueve al stack 6 o 7
    _Serial::write(logic.grabContainer(c_serial));
     
}

void loop(){
  logic.stackToShip();
  c_serial = _Serial.read();
  if(c_serial == 'S'){
    movements ->  gotoSecond(); // pendiente
    _Serial::write('1');
    c_serial = _Serial::read();
    logic.moveSecond(c_serial); // pendiente
    _Serial::write(logic.grabContainer(c_serial));
  }else{
    logic.shipToStack();
    _Serial::write(logic.grabContainer(c_serial));
  }
}
