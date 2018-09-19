//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LARC 2018 //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#include <_Movements.h>     
#include <Wire.h>
_Movements *movements = new _Movements;

const byte pinEncoder = 18;

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
    movements->lcd->setupLCD();
    movements->tcrt5000->setupTCRT5000();
    movements->encoder->setupEncoder();
    pinMode(pinEncoder, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);   

    //CALIBRATION 
//      movements->lcd->onLed('g');    
//      movements->timeFlight->calibTimeFlights(12);    
//      movements->lcd->offLed('g'); 
//      delay(5000);   
}


void testSteps(){
  Serial.println(movements->encoder->steps);
}
void testMovements(){
  movements->encoder->encoderState = 1;
  movements->movePID(true,'8');
//  movements->movePID_nSec(1.5, false, '1');
//  delay(2000);
//  movements->spinPID(false, -90);
}
void squareUntilWall(){
    int del = 1000;
    movements->movePID_alignToPickContainer(8, '8');
    delay(del);
    movements->spinPID(false, -90);
    movements->movePID_alignToPickContainer(5, '8');
    delay(del);
    movements->spinPID(false, -90);
    movements->movePID_alignToPickContainer(5, '8');
    delay(del);
    movements->spinPID(false, -90);
    movements->movePID_alignToPickContainer(5, '8');
    delay(del);
    
    movements->movePID_alignToPickContainer(5, '2');
    delay(del);
    movements->spinPID(false, 90);
    movements->movePID_alignToPickContainer(5, '2');
    delay(del);
    movements->spinPID(false, 90);
    movements->movePID_alignToPickContainer(5, '2');
    delay(del);
    movements->spinPID(false, 90);
    movements->movePID_alignToPickContainer(5, '2');
    delay(del);
}
void aligningTofTest(){
//    movements->movePID_alignToPickContainer(8, '8');
    movements->align_tof();
//    Serial.print("///////////////////////////////////////////////////////////////////////");
//    movements->motors->brake();
//    movements->lcd->onLed('b');       
//    delay(6000);
//    movements->lcd->offLed('b');  
}
void tof_vs_sharp(){
//    movements->updateSensors(0,0,0,1,0);
//    Serial.print(movements->timeFlight->timeFlightLeft.kalmanDistance);
//    Serial.print(" ");
//    Serial.print(movements->timeFlight->timeFlightRight.kalmanDistance);
//    Serial.print(" ");
//    Serial.print((movements->timeFlight->timeFlightLeft.kalmanDistance - movements->timeFlight->timeFlightRight.kalmanDistance)*10);  
//
//    Serial.print("\t\t");
//
//    movements->updateSensors(0,0,1,0,0);
//    Serial.print(movements->sharp->sharpBL.kalmanDistance);
//    Serial.print(" ");
//    Serial.print(movements->sharp->sharpFR.kalmanDistance);
//    Serial.print(" ");
//    Serial.println((movements->sharp->sharpBL.kalmanDistance - movements->sharp->sharpFR.kalmanDistance)*10);   
  
     movements->timeFlight->timeFlight_RawKalman(movements->timeFlight->timeFlightLeft);
    // movements->sharp->sharp_RawKalman(movements->sharp->sharpBL);
}

void readTCRT5000(){
     movements->updateSensors(0,0,0,0,1);
     Serial.print(movements->tcrt5000->tcrtRight.kalmanDistance);
     Serial.print(" ");
     Serial.println(movements->tcrt5000->tcrtLeft.kalmanDistance);

//    movements->tcrt5000->tcrt5000_RawKalman(movements->tcrt5000->tcrtRight);  
}

void aligningTcrtTest(){
      movements->movePID_alignBetweenVerticalBlackLine(true, '8');
}

void loop(){
//  aligningTofTest();
//  squareUntilWall();
//  tof_vs_sharp();
  testMovements();
  testSteps();
//  readTCRT5000();
//  aligningTcrtTest();
}
