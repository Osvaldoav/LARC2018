//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LARC 2018 //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#include <_Movements.h>     
#include <Wire.h>
_Movements *movements = new _Movements;

const byte pinEncoder = 40;

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
    pinMode(pinEncoder, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);   

    //CALIBRATION 
//      movements->lcd->onLed('g');    
//      movements->timeFlight->calibTimeFlights(12);    
//      movements->lcd->offLed('g'); 
//      delay(5000);   
}

void loop(){
//  movements->movePID_nWallCM(8, 8);
//  movements->alignMechanism();
//  Serial.print("///////////////////////////////////////////////////////////////////////");
//  movements->motors->brake();
//  movements->lcd->onLed('b');       
//  delay(6000);
//  movements->lcd->offLed('b');

//  movements->tcrt5000->filtrateDistancesTCRT5000();
//  Serial.print(movements->tcrt5000->tcrtRight.kalmanDistance);
//  Serial.print(" ");
//  Serial.println(movements->tcrt5000->tcrtLeft.kalmanDistance);

  movements->tcrt5000->tcrt5000_RawKalman(movements->tcrt5000->tcrtRight);


//  movements->motors->brake();
//  delay(2000);
//  delay(1000);
//  int del = 1000;
//  movements->movePID_nWallCM(8, 8);
//  delay(del);
//  movements->spinPID(-90);
//  movements->movePID_nWallCM(5, 8);
//  delay(del);
//  movements->spinPID(-90);
//  movements->movePID_nWallCM(5, 8);
//  delay(del);
//  movements->spinPID(-90);
//  movements->movePID_nWallCM(5, 8);
//  delay(del);
//  
//  movements->movePID_nWallCM(5, 2);
//  delay(del);
//  movements->spinPID(90);
//  movements->movePID_nWallCM(5, 2);
//  delay(del);
//  movements->spinPID(90);
//  movements->movePID_nWallCM(5, 2);
//  delay(del);
//  movements->spinPID(90);
//  movements->movePID_nWallCM(5, 2);
//  delay(del);
//
//  Serial.print(movements->timeFlight->getRawDistance(true));
//  Serial.print(" ");
//  Serial.println(movements->timeFlight->getRawDistance(false));

//  movements->timeFlight->filtrateDistancesTimeFlight();
//  Serial.print(movements->timeFlight->timeFlightLeft.kalmanDistance);
//  Serial.print(" ");
//  Serial.print(movements->timeFlight->timeFlightRight.kalmanDistance);
//  Serial.print(" ");
//  Serial.println((movements->timeFlight->timeFlightLeft.kalmanDistance - movements->timeFlight->timeFlightRight.kalmanDistance)*10);  
//
//  Serial.print("\t\t");
//
//  movements->sharp->filtrateDistancesSharp();
//  Serial.print(movements->sharp->sharpBL.kalmanDistance);
//  Serial.print(" ");
//  Serial.print(movements->sharp->sharpFR.kalmanDistance);
//  Serial.print(" ");
//  Serial.println((movements->sharp->sharpBL.kalmanDistance - movements->sharp->sharpFR.kalmanDistance)*10); 

//  movements->timeFlight->timeFlight_RawKalman(movements->timeFlight->timeFlightLeft);

//  movements->alignMechanism(8);

//   movements->movePID(false, 8);
//   movements->sharp->sharp_RawKalman(movements->sharp->sharpBL);

//  backwardP_alignWall(6, false, true);
  // PRE_LARC_TEST1();
//  readColor();
  
// movements->motors->setMotor(1, 0, 1, 0, 0, 1, 0, 1);
// movements->motors->setVelocity(85,85);
  
//  Setpoint = 90;
//  forwardP_nWallCM(4);
//  backwardP(false);
//  forwardP_nWallCM(4);
//  spinP(-90);
//  calculateNewSetpoint(-90);  

//  spinP(-90);
  
//  initializeSetpoint();
//  spin_P(360);
//  double startMillis = millis();
//  while(millis()<startMillis+1000)
//    forwardP(false);
//  stop(false);
//  delay(3000);
//  initializeSetpoint();
//  while(true){
//    filtrateDistancesSharp();
//    forwardP(false);
//    Serial.print(Setpoint);
//    Serial.print("\t");  
//    Serial.print(rawInput);
//    Serial.print("\t");
//    Serial.print(getAngleDifference());
//    Serial.print("\t");  
//    Serial.println(Output);
//     if(sharpFront.side){
//         spinP(90);
//         stop(false);
//         filtrateDistancesSharp();    
//     }
//  }

//  Serial.println(sharpFront.side);

//  rightPriotity(ultraFront, ultraRight, ultraLeft); 
//  firstControlChallenge();
//  secondControlChallenge();
//  thirdControlChallenge();
//  currentColor();

//  readBNO();
//  Serial.println(rawInput);
      
//  spin_P(90);
//  delay(1000);

//go();

//  calculateRawDistancesSharp();
//  Serial.print(sharpLeft.distance);
//  Serial.print("\t");
//  Serial.print(sharpFront.distance);
//  Serial.print("\t");
//  Serial.println(sharpRight.distance);
//  delay(5);

//  filtrateDistancesSharp();
//  Serial.print(sharpLeft.kalmanDistance);
//  Serial.print("\t");
//  Serial.print(sharpFront.kalmanDistance);
//  Serial.print("\t");
//  Serial.println(sharpRight.kalmanDistance);
//  delay(5);

//  oneStepMillis(true);
//  stop(true);
//  delay(2000);

//  hardCodedCalibration();
//  colorCalibration(0);
//  while(1){
//    readColor();
//    Serial.print(redColor.kalmanColor);
//    Serial.print("\t");
//    Serial.print(greenColor.kalmanColor);
//    Serial.print("\t");
//    Serial.println(blueColor.kalmanColor); 
//    Serial.println(currentColor());
//  } 

//  calibrarBNO();
//  while(1){   
//    sensors_event_t event;                   // Read 9DOF Sensor
//    bno.getEvent(&event); 
//    magDir = event.orientation.x;           // Read magnetic direction       
//    Serial.println(magDir);  
//    forwardPID();    
//  }  

//  readBNO();
//  Serial.println(rawInput);

//  xBNO_RawKalman(bno, event);
//  sharp_RawKalman(sharpFR);
//  sharp_RawKalman(sharpFL);
//  sharp_RawKalman(sharpBR);
//  sharp_RawKalman(sharpBL);
//  sharp_RawKalman(sharpBack);
//  sharp_RawKalman(sharpFront);  
}
