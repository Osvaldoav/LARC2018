/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////PRE LARC 2018/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
#include <StackArray.h>
#include <math.h>

//////////////////////////////////////////////LEDs////////////////////////////////////////////////
//AT
const byte ledRed = 37;
const byte ledGreen = 36;
const byte ledBlue = 35;

/////////////////////////////////////////////LCD///////////////////////////////////////////////////
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

///////////////////////////////////////////////SHARP///////////////////////////////////////////////////
#include <Estadisticas.h>
#include <QuickSort.h>
const byte pinSharpFR = A4;
const byte pinSharpFL = A3;
const byte pinSharpBR = A1;
const byte pinSharpBL = A2;
const byte pinSharpBack = A5;
const byte pinSharpFront = A0;

double sharpConsKp=28, sharpConsKi=0, sharpConsKd=0;
double SetpointWallDistance = 8, leftWallOutput, rightWallOutput, leftWheelOutput, rightWheelOutput;
const double cantReads = 7;

struct SharpKalman{
  SharpKalman(){
    varSensor = 1e-6; //Variance of sensor. The LESS, the MORE it looks like the raw input. (1e-6)
    varProcess = 1e-7; 
    P = 1.0;
    Pc = 0.0;
    G = 0.0;
    Xp = 0.0;
    Zp = 0.0;
    kalmanDistance = 0.0;
    rawDistance = 0.0;
    medianDistance = 0.0;
    side = false;
  }
  float varSensor; //Variance of sensor. The LESS, the MORE it looks like the raw input
  float varProcess; // The greater the variance, faster the sensor response
  float P;
  float Pc;
  float G;
  float Xp;
  float Zp;
  float kalmanDistance;
  float rawDistance;
  float medianDistance;
  bool side;
};
SharpKalman sharpFR;
SharpKalman sharpFL;
SharpKalman sharpBR;
SharpKalman sharpBL;
SharpKalman sharpBack;
SharpKalman sharpFront;


///////////////////////////////////////////////COLOR///////////////////////////////////////////////////
// Seleccion LARC
const byte sensorOut = 43;
const byte S2 = 42;
const byte S3 = 41;
const byte S1 = 40;
const byte S0 = 39;

bool colorRedDetected=false, colorGreenDetected=false, colorBlackDetected=false;
double r = 0, g = 0, b = 0;
const int num_col = 5;
int range = 10;
int color_position;     //  0        1       2       3        4
String color_names[5] = {"rojo", "azul", "verde", "negro", "blanco"};
bool switchColor=false;
int initialColors[3];
struct color{
  String nombre;
  double red;
  double green;
  double blue;
};
color color_position_arr[num_col];
struct ColorKalman{
  ColorKalman(){
    varSensor = 2e-6; //Variance of sensor. The LESS, the MORE it looks like the raw input.    2e-6
    varProcess = 1e-7; 
    P = 1.0;
    Pc = 0.0;
    G = 0.0;
    Xp = 0.0;
    Zp = 0.0;
    kalmanColor = 0.0;
    rawColor = 0.0;
  }
  float varSensor; //Variance of sensor. The LESS, the MORE it looks like the raw input
  float varProcess; // The greater the variance, faster the sensor response
  float P;
  float Pc;
  float G;
  float Xp;
  float Zp;
  float rawColor;
  float kalmanColor;
};
ColorKalman redColor;
ColorKalman blueColor;
ColorKalman greenColor;
int counter=0;


///////////////////////////////////////////ENCODER///////////////////////////////////////////////////
const byte pinEncoder = 40;

//Const for encoder. Perfect at 60cm, 30cm fail by -1.5cm, 100cm by 2cm, 150cm by 3cm
const long encoder30Cm = 4300L; // TODO: Better it depends in the distance.
//Steps counted by the encoder
volatile unsigned long steps = 0;
//State of the encoder
//0->Stop   1->Forward    2->Backwards
volatile byte encoderState = 0;


///////////////////////////////////////////BNO///////////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define BNO055_SAMPLERATE_DELAY_MS (50)     // was 100
Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t event; 
/* Global Variables  */
const float Pi = 3.14159265359;           // Pi constant
float magDir;                             // Direction (Magnetic)                 
byte calMRL = 0;                          // Mag Cal Values
byte calMRM = 0;
byte calMOXL = 0;
byte calMOXM = 0;
byte calMOYL = 0;
byte calMOYM = 0;
byte calMOZL = 0;
byte calMOZM = 0;

/////////////////////////////////////////////MOTORS///////////////////////////////////////////////
// Seleccion LARC
const byte motorL1 = 3;
const byte motorL2 = 5;
const byte motorR1 = 9;
const byte motorR2 = 6;
// PRE LARC 3.0
const byte pinMotorFR_F = 52;
const byte pinMotorFR_B = 53;
const byte pinMotorFL_F = 46;
const byte pinMotorFL_B = 47;
const byte pinMotorBR_F = 50;
const byte pinMotorBR_B = 51;
const byte pinMotorBL_F = 49;
const byte pinMotorBL_B = 48;
const byte pinPWM_FR = 2; 
const byte pinPWM_FL = 5;
const byte pinPWM_BR = 3;
const byte pinPWM_BL = 4;

// Seleccion LARC
double velGenDer = 120;
double velGenIzq = 120;
double velGenDerBack = 70;
double velGenIzqBack = 70;
// PRE LARC 3.0
double velSlowFL = 82;
double velSlowBL = 82;
double velSlowFR = 82;
double velSlowBR = 82;
double velGenFL = 115;
double velGenBL = 115;
double velGenFR = 115;
double velGenBR = 115;
double velTurnFL = 79;//55
double velTurnBL = 79;
double velTurnFR = 79;
double velTurnBR = 79;

double maxTurnVel=140;
double maxForwardVel=255;

///////////////////////////////////////////  PID ///////////////////////////////////////////////////
#include <PID_v1.h>
#define MANUAL 0
#define AUTOMATIC 1
#define DIRECT 0
#define REVERSE 1
/////// PID PREFERENCES ///////65
int controllerDirection=DIRECT;
int SampleTime=50;
double outMin=0,outMax=255;
bool inAuto=true;
/////// PID CONSTANTS ///////
double alignKp=0, alignKi=0, alignKd=0;
double turnKp=17.6, turnKi=0, turnKd=0.9;//2.4d // 18.3
double forwardKp=19, forwardKi=0, forwardKd=0;//4.8d
double kp=forwardKp, ki=forwardKi, kd=forwardKd;
double wallAngleKp=4, wallDistanceKp=13, wallSharpsKp=5; 
/////// PID VALUES ////////////
double Setpoint, rawInput, lastInput, ITerm=0;
double frontLeftOutput=0, frontRightOutput=0, backLeftOutput=0, backRightOutput=0, Output=0;
double offsetAngleForward=1.179, offsetAngleTurn=1.9899, offsetAngle=offsetAngleForward;//0.0011
unsigned long lastTime;
bool calibratedBNO=false, calibratedDone=false;


/////////////////////////////////////////////SETUP////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(motorR1,OUTPUT);
  pinMode(motorR2,OUTPUT);
  pinMode(motorL1,OUTPUT);
  pinMode(motorL2,OUTPUT);  
  // MOTORS
  pinMode(pinMotorFR_F,OUTPUT);
  pinMode(pinMotorFR_B,OUTPUT);
  pinMode(pinMotorFL_F,OUTPUT);
  pinMode(pinMotorFL_B,OUTPUT);  
  pinMode(pinMotorBR_F,OUTPUT);
  pinMode(pinMotorBR_B,OUTPUT);
  pinMode(pinMotorBL_F,OUTPUT);
  pinMode(pinMotorBL_B,OUTPUT);  
  pinMode(pinPWM_FR,OUTPUT);
  pinMode(pinPWM_FL,OUTPUT);
  pinMode(pinPWM_BR,OUTPUT);
  pinMode(pinPWM_BL,OUTPUT);  
  //ENCODER
  pinMode(pinEncoder, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinEncoder), encoderStep, CHANGE);
  //LEDS
  pinMode(ledGreen,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(ledBlue,OUTPUT);    
  //COLOR SENSOR
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);   
//  colorCalibration(1);
//  hardCodedCalibration();
  // BNO
  delay(400);
//  if(!bno.begin()){
  if(!bno.begin(bno.OPERATION_MODE_IMUPLUS)){
    Serial.print("BNO055 not detected");         // Debug
    while(1);
  }
  Serial.println("9DOF (BNO055) started");       // Debug
//  setCal();                                      // Set 9DOF Calibration Values
  bno.setExtCrystalUse(true);
//  calibrarBNO();
//  delay(30000);
  Setpoint = 0;
  delay(2000);  

}

void loop(){
//  backwardP_alignWall(6, false, true);
  PRE_LARC_TEST1();
//  readColor();
  
//  setMotor(1, 0, 1, 0, 0, 1, 0, 1);
//  setVelocity(85,85);
  
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
