#include <_BNO055.h>
/////////////////////////////////// BNO Local Variables /////////////////////////////////////
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
uint8_t updateTimeMsBno = 30;
long lastUpdateBno = 0;

// TODO:
void _BNO055::setupBNO055(){
    lcd = new _LCD;
    offsetAngleForward=0.34444;   //1.7777     //(+)=RIGHT, (-)=LEFT
    offsetAngleTurn=1.5555;   //1.5555      //(+)=LEFT, (-)=RIGHT
    offsetAngle=offsetAngleForward;//0.0011
    // USING I2C ADDRESS 0X28
    // Serial.println("Before");
    if(!bno.begin(bno.OPERATION_MODE_IMUPLUS)){
      while(1);
    }      
    // Serial.println("After");
    // setCal();                                      // Set 9DOF Calibration Values
    bno.setExtCrystalUse(true);
    // calibrarBNO(Setpoint);
}

// TODO:
//**********************************
// Writes calibration data to 9DOF sensor//
void _BNO055::setCal(){
  bno.setMode( bno.OPERATION_MODE_CONFIG );    // Put into CONFIG_Mode
  delay(60);
  bno.setCalvalMRL(calMRL);                    // Send calibration values to BNO055
  bno.setCalvalMRM(calMRM);
  bno.setCalvalMOXL(calMOXL);
  bno.setCalvalMOXM(calMOXM);
  bno.setCalvalMOYL(calMOYL);
  bno.setCalvalMOYM(calMOYM);
  bno.setCalvalMOZL(calMOZL);
  bno.setCalvalMOZM(calMOZM);
  bno.setMode( bno.OPERATION_MODE_NDOF );    // Put into NDOF Mode
  delay(60);
    
}

// TODO:
//**********************************
// Reads calibration data from BNO055 sensor//
void _BNO055::readCal(){
  bno.setMode( bno.OPERATION_MODE_CONFIG );    // Put into CONFIG_Mode
  
  calMRL = bno.getCalvalMRL();                 // Read Magnetic calibration values
  calMRM = bno.getCalvalMRM();
  calMOXL = bno.getCalvalMOXL();
  calMOXM = bno.getCalvalMOXM();
  calMOYL = bno.getCalvalMOYL();
  calMOYM = bno.getCalvalMOYM();
  calMOZL = bno.getCalvalMOZL();
  calMOZM = bno.getCalvalMOZM();

  bno.setMode( bno.OPERATION_MODE_NDOF );    // Put into NDOF Mode
  delay(60);

}

// TODO:
//************************************
// Calibration Status
bool _BNO055::getCalStat(){
  byte cal = bno.getCalib();
  byte calSys = (0xC0 & cal) >> 6;                // Sys Status (0-3 value)
  byte calGyro = (0x30 & cal) >> 4;
  byte calAccel = (0x0C & cal) >> 2;
  byte calMag = (0x03 & cal) >> 0;                // Mag Status (0-3 value)
  
  //Serial.println(cal, BIN);                     // debug
  // Serial.print("System calibration status "); Serial.println(calSys);
  // Serial.print("Gyro   calibration status "); Serial.println(calGyro);
  // Serial.print("Accel  calibration status "); Serial.println(calAccel);
  // Serial.print("Mag    calibration status "); Serial.println(calMag);
  
  delay(1000);
  if(calGyro==3)
    return true;
  return false;
}

// TODO:
void _BNO055::calibrarBNO(double &Setpoint){
  double startTime = millis();  
  int x=0;                  
  while (x<2 || millis()<startTime+3000){                                                      // Mag cal status indicates complete
    readCal();                                                    // Continue to screen 5
    if(getCalStat())  x++;
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }  
  readBNO(Setpoint);
  Setpoint = rawInput;
  // delay(2000);
}

// TODO:
void _BNO055::readBNO(double &Setpoint){
    double distance;
    if(millis() - lastUpdateBno > updateTimeMsBno){
        if(millis()%300 == 0) Setpoint+=offsetAngle;
        // delay(20);
        bno.getEvent(&event);
        lastInput = rawInput;
        rawInput=round(bno.getVector(Adafruit_BNO055::VECTOR_EULER).x());   
        lastUpdateBno = millis(); 
    }  
}
