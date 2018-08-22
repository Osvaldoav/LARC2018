double slowGo(double time){
  return 100*(1-exp(-.2*time/5));
}

void stop(bool isSpin){   
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);  
  if(isSpin){    
//    analogWrite(motorR1, 235);
//    analogWrite(motorR2, 0);
//    analogWrite(motorL1, 235);
//    analogWrite(motorL2, 0); 
//    delay(70);   
  }                                                                  
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0); 
//  delay(100); 
}      

void go() {
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 100);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 100); 
}

void back() {
  analogWrite(motorR1, 110);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 110);
  analogWrite(motorL2, 0); 
    delay(2000);
}

void setMotor(bool leftFrontF, bool leftFrontB, bool leftBackF, bool leftBackB, bool rightFrontF, bool rightFrontB, bool rightBackF, bool rightBackB){
  digitalWrite(pinMotorFR_F, rightFrontF);
  digitalWrite(pinMotorFR_B, rightFrontB);
  digitalWrite(pinMotorFL_F, leftFrontF);
  digitalWrite(pinMotorFL_B, leftFrontB);
  digitalWrite(pinMotorBR_F, rightBackF);
  digitalWrite(pinMotorBR_B, rightBackB);
  digitalWrite(pinMotorBL_F, leftBackF);
  digitalWrite(pinMotorBL_B, leftBackB);
}

void setVelocity(int left, int right){
  if(calibratedBNO){
    analogWrite(pinPWM_FR, right);
    analogWrite(pinPWM_BR, right);
    analogWrite(pinPWM_FL, left);
    analogWrite(pinPWM_BL, left);
  }
}
void setVelocity(int leftFront, int leftBack, int rightFront, int rightBack){
  if(calibratedBNO){
  analogWrite(pinPWM_FR, rightFront);
  analogWrite(pinPWM_BR, rightBack);
  analogWrite(pinPWM_FL, leftFront);
  analogWrite(pinPWM_BL, leftBack);
  }
}

//Stop motors
void brake(){
  setVelocity(0, 0);
}

void turnRight(int vel){
  setMotor(0, 1, 1, 0, 0, 1, 1, 0);
  setVelocity(vel, vel);
}
void turnRight(int leftFront, int leftBack, int rightFront, int rightBack){
  setMotor(1, 0, 1, 0, 0, 1, 0, 1);
  setVelocity(leftFront, leftBack, rightFront, rightBack);
}

void turnLeft(int vel){
  setMotor(1, 0, 0, 1, 1, 0, 0, 1);
  setVelocity(vel, vel);
}
void turnLeft(int leftFront, int leftBack, int rightFront, int rightBack){
  setMotor(0, 1, 0, 1, 1, 0, 1, 0);
  setVelocity(leftFront, leftBack, rightFront, rightBack);
}
