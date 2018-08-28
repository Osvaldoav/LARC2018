//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////LARC 2018 ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
const byte pinEncoder = 40;
#include <_Movements.h>     
_Movements movements;

const byte ledRed = 37;
const byte ledGreen = 36;
const byte ledBlue = 35;

/////////////////////////////////////////////SETUP////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    movements.pid.setupLibraryPID();
    movements.motors.setupMotors();
    pinMode(pinEncoder, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);
    movements.colorSensor.setupColorSensor();
    movements.bno055.setupBNO055();

    //LEDS
    pinMode(ledGreen,OUTPUT);
    pinMode(ledRed,OUTPUT);
    pinMode(ledBlue,OUTPUT);    
}

void loop(){
  Serial.print("oli k ase");
   movements.forwardP(false);

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
