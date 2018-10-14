//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LARC 2018 //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#include <_Logic.h>
#include <Wire.h>
_Logic *logic = new _Logic;

const byte pinEncoder = 18;

/////////////////////////////////////////////SETUP////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    Wire.begin();
    logic->traductor->movements->lcd->setupLCD();    
    logic->traductor->movements->pid->setupLibraryPID();
    logic->traductor->movements->motors->setupMotors();
    logic->traductor->movements->colorSensor->setupColorSensor();
    logic->traductor->movements->bno055->setupBNO055();
    logic->traductor->movements->timeFlight->setupTimeFlight();
    logic->traductor->movements->tcrt5000->setupTCRT5000();
    logic->traductor->movements->encoder->setupEncoder();
    pinMode(pinEncoder, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);   

    //CALIBRATION 
//      logic->traductor->movements->lcd->onLed('g');    
//      logic->traductor->movements->timeFlight->calibTimeFlights(4);    
//      logic->traductor->movements->lcd->offLed('g'); 
//      delay(5000);   
}

void testMovements(){ 
//    logic->traductor->movements->encoder->encoderState = 1;
//    logic->traductor->movements->movePID(false, '7');
    logic->traductor->movements->spinPID(true, -90);
    delay(5000);
//    Serial.println(logic->traductor->movements->encoder->steps);   
//    logic->traductor->movements->movePID_nSec(1.5, false, '1');
//    delay(2000);
//    logic->traductor->movements->spinPID(false, -90);
//    logic->traductor->movements->motors->brake();
//    delay(2000);
//    logic->traductor->movements->movePID_nCM(179, false, '6');
//    delay(3000);
//    logic->traductor->movements->movePID_nCM(179, false, '8');
//    delay(3000);    
}
void aligningTofTest(){
//    logic->traductor->movements->movePID_alignToPickContainer(2);
//    while(1);
    logic->traductor->movements->align_tof();
//    Serial.print("///////////////////////////////////////////////////////////////////////");
//    logic->traductor->movements->motors->brake();
//    logic->traductor->movements->lcd->onLed('b');       
//    delay(6000);
//    logic->traductor->movements->lcd->offLed('b');  
}
void tof_vs_sharp(){
    logic->traductor->movements->updateSensors(0,0,0,1,0,0);
    Serial.print(logic->traductor->movements->timeFlight->timeFlightLeft.kalmanDistance);
    Serial.print(" ");
    Serial.print(logic->traductor->movements->timeFlight->timeFlightRight.kalmanDistance);
    Serial.print(" ");
    Serial.println((logic->traductor->movements->timeFlight->timeFlightLeft.kalmanDistance - logic->traductor->movements->timeFlight->timeFlightRight.kalmanDistance)*10);  
//
//    Serial.print("\t\t");
//
//    logic->traductor->movements->updateSensors(0,0,1,0,0,0);
//    Serial.print(logic->traductor->movements->sharp->sharpBL.kalmanDistance);
//    Serial.print(" ");
//    Serial.print(logic->traductor->movements->sharp->sharpFR.kalmanDistance);
//    Serial.print(" ");
//    Serial.println((logic->traductor->movements->sharp->sharpBL.kalmanDistance - logic->traductor->movements->sharp->sharpFR.kalmanDistance)*10);   
  
//     logic->traductor->movements->timeFlight->timeFlight_RawKalman(movements->timeFlight->timeFlightLeft);
    // logic->traductor->movements->sharp->sharp_RawKalman(movements->sharp->sharpBL);
}

void readTCRT5000(){
     logic->traductor->movements->updateSensors(0,0,0,0,1,1);
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMechaLeft.kalmanDistance);
     Serial.print(" ");
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMechaRight.kalmanDistance);     
     Serial.print(" ");
     Serial.print(logic->traductor->movements->tcrt5000->tcrtSharpLeft.kalmanDistance);
     Serial.print(" ");     
     Serial.print(logic->traductor->movements->tcrt5000->tcrtSharpRight.kalmanDistance);
     
     Serial.print("\t\t");     
     
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMidFrontLeft.kalmanDistance);
     Serial.print(" ");
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMidFrontRight.kalmanDistance);
     Serial.print(" ");     
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMidDownLeft.kalmanDistance);
     Serial.print(" ");
     Serial.println(logic->traductor->movements->tcrt5000->tcrtMidDownRight.kalmanDistance);               
//    logic->traductor->movements->tcrt5000->tcrt5000_RawKalman(logic->traductor->movements->tcrt5000->tcrtFrontRight);  
}

void aligningTcrtTest(){
  logic->traductor->movements->larc_moveAndAlignToShip();
  delay(4000);
  while(1);
//      logic->traductor->movements->larc_alignBetweenVerticalBlackLine(true, '8');
}

void larc(){
  logic->traductor->movements->larc_moveUntilBlackLine(false, '8', true, false, true, false);
  logic->traductor->movements->larc_moveUntilBlackLine(false, '6', true, true, false, false);
  logic->traductor->movements->movePID_nCM(29, false, '8');
  delay(2000);
//  logic->traductor->movements->align_tof();
  logic->traductor->movements->larc_moveBetweenVerticalBlackLine(false, '2');
  logic->traductor->movements->movePID_nCM(21, false, '6');
  logic->traductor->movements->spinPID(true, 90);
  logic->traductor->movements->larc_moveAndAlignToShip();
//  logic->traductor->movements->lcd->printAlertSec("CONTAINER DROPPED", 5);
  while(1); 
}

void colorSensor(){
  logic->traductor->movements->colorSensor->readColor();
}

void logicTest(){
    logic->shipToStack('g');
    while(1);
}

void loop(){
//  aligningTofTest();
//  tof_vs_sharp();
//  testMovements();
//  testSteps();
//  readTCRT5000();
//  aligningTcrtTest();
//  larc();
//  logicTest();
//  colorSensor();
  if(-1)
    Serial.println("IF");
  else
  Serial.println("ELSE");
}
