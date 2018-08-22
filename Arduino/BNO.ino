//**********************************
// Writes calibration data to 9DOF sensor//
void setCal(){
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
//**********************************
// Reads calibration data from BNO055 sensor//
void readCal(){
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
//************************************
// Calibration Status
bool getCalStat(){
  byte cal = bno.getCalib();
  byte calSys = (0xC0 & cal) >> 6;                // Sys Status (0-3 value)
  byte calGyro = (0x30 & cal) >> 4;
  byte calAccel = (0x0C & cal) >> 2;
  byte calMag = (0x03 & cal) >> 0;                // Mag Status (0-3 value)
  
  //Serial.println(cal, BIN);                     // debug
  Serial.print("System calibration status "); Serial.println(calSys);
  Serial.print("Gyro   calibration status "); Serial.println(calGyro);
  Serial.print("Accel  calibration status "); Serial.println(calAccel);
  Serial.print("Mag    calibration status "); Serial.println(calMag);
  
  delay(1000);
  if(calMag==3 && calGyro==3)
    return true;
  return false;
}

//void readPosition(Adafruit_BNO055 &bno, sensors_event_t &event, MPU6050 &mpu, char selectGyro){
//  if(selectGyro=='B')
//    readBNO(bno, event);
//  else if(selectGyro=='I')
//    readIMU(mpu);
//}

double filterReadBNO(){
  double sum=0;
  double times=50;
  for(double i=0; i<times; i++){
    readBNO();
    sum+=rawInput;
  }
  return sum/times;
}

void initializeSetpoint(){
  digitalWrite(ledRed, HIGH);
  Setpoint = filterReadBNO();
  digitalWrite(ledRed, LOW);
  delay(5000);
}

void calibrarBNO(){
  calibratedDone=true;
  bool calibrated = false; 
  double startTime = millis();  
  int x=0;                  
  while (x<2 || millis()<startTime+12000){                                                      // Mag cal status indicates complete
    readCal();            
    calibrated = getCalStat();                                          // Continue to screen 5
    if(getCalStat())  x++;
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }  
  digitalWrite(ledBlue, HIGH);
  readBNO();
  Setpoint = rawInput;
  delay(4000);
  digitalWrite(ledBlue, LOW);
}

void readBNO(){
  if(millis()%200 == 0 && !calibratedBNO) Setpoint-=offsetAngle;
  delay(20);
  bno.getEvent(&event);
  lastInput = rawInput;
  rawInput=round(bno.getVector(Adafruit_BNO055::VECTOR_EULER).x());

//   readCal();                                                       // Continue to screen 5
//   getCalStat();
//   delay(BNO055_SAMPLERATE_DELAY_MS);  
  
//  rawInput=event.orientation.x;
//  Input = rawInput;
//  if(Input>180) 
//    Input = Input - 360;
//  fakeInput = Input;
//  if((abs(Setpoint)==90&&abs(lastSetpoint)==180) || abs(Setpoint)==180)
//    fakeInput = fakeSetpoint > 0 ? abs(Input) : -1*abs(Input);
}

//void printBNO(sensors_event_t event){
void printBNO(){
  Serial.print("X: ");
  Serial.print(event.orientation.x, 5);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 5);
  Serial.print("\t\tZ: ");
  Serial.println(event.orientation.z, 5);
}

//int getCompass(){
//  int angle = round(bno.getVector(Adafruit_BNO055::VECTOR_EULER).x());
//  return angle;
//}
