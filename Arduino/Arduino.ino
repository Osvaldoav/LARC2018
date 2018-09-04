//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LARC 2018 //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#include <_Movements.h>     
#include <Wire.h>
_Movements *movements = new _Movements;

const byte pinEncoder = 40;
const byte ledRed = 37;
const byte ledGreen = 36;
const byte ledBlue = 35;

/////////////////////////////////////////////SETUP////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    Wire.begin();
    movements->setupMovements();
    movements->pid->setupLibraryPID();
    movements->motors->setupMotors();
    movements->colorSensor->setupColorSensor();
    movements->bno055->setupBNO055();
    pinMode(pinEncoder, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);

    //LEDS
    pinMode(ledGreen,OUTPUT);
    pinMode(ledRed,OUTPUT);
    pinMode(ledBlue,OUTPUT);    
}

void loop(){
  movements->movePID_nWallCM(7, 8);
  movements->spinPID(-90);
  movements->movePID_nWallCM(5, 8);
  movements->spinPID(-90);
  movements->movePID_nWallCM(5, 8);
  movements->spinPID(-90);
  movements->movePID_nWallCM(5, 8);
  
  movements->movePID_nWallCM(5, 2);
  movements->spinPID(90);
  movements->movePID_nWallCM(5, 2);
  movements->spinPID(90);
  movements->movePID_nWallCM(5, 2);
  movements->spinPID(90);
  movements->movePID_nWallCM(5, 2);
  movements->spinPID(90);  
  

//  movements->sharp->filtrateDistancesSharp();
//  Serial.print(movements->sharp->sharpFR.kalmanDistance);
//  Serial.print(" ");
//  Serial.println(movements->sharp->sharpBL.kalmanDistance);

//   movements->movePID(false, 2);
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
