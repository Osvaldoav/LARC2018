//forward with P correction
void forwardP(bool goSlow){
  offsetAngle = offsetAngleForward;
  readBNO();    
  filtrateDistancesSharp();  
  readColor();
  if (goSlow) {
    frontLeftOutput = velSlowFL;
    backLeftOutput = velSlowBL;
    frontRightOutput = velSlowFR;
    backRightOutput = velSlowBR;
  } else {
    frontLeftOutput = velGenFL;
    backLeftOutput = velGenBL;
    frontRightOutput = velGenFR;
    backRightOutput = velGenBR;
  }
  int angleDifference = getAngleDifference();
  computeOutput();  
  if(angleDifference > 0){ // MOVE TO THE LEFT
    frontLeftOutput -= Output;
    backLeftOutput -= Output;
    frontRightOutput += Output;
    backRightOutput += Output;     
  }
  else if(angleDifference < 0){ // MOVE TO THE RIGHT
    frontLeftOutput += Output;
    backLeftOutput += Output;
    frontRightOutput -= Output;
    backRightOutput -= Output;   
  }   
//  Serial.print(rawInput);
//  Serial.print("\t");      
//  Serial.print(Setpoint);
//  Serial.print("\t");      
//  Serial.print(angleDifference);
//  Serial.print("\t");
//  Serial.print(frontLeftOutput);
//  Serial.print("\t");
//  Serial.print(frontRightOutput);
//  Serial.print("\t\t\t");
//  Serial.println(Output);    
  regulateOutputsFrontPID(); 
  setMotor(1, 0, 1, 0, 1, 0, 1, 0);
  setVelocity(frontLeftOutput, backLeftOutput, frontRightOutput, backRightOutput);
}
//forward with P correction for a specific time
void forwardP_nTime(int time, bool goSlow) {
  double startTime = millis();
  while(millis() < startTime+time)
    forwardP(goSlow);
  brake();
}
//Go forward the cm given in the parameter
void forwardP_nCM(int cm, bool goSlow){
  encoderState = 1;
  //Counts of encoder to get to the objective
  int untilSteps = (encoder30Cm / 30) * cm;
  //Restart encoder counts
  steps = 0;
  //Move with p correction until the encoder read the cm
  while (steps < untilSteps)
    forwardP(goSlow);
  brake();
}
//Go forward until finding a wall at a certain distance
void forwardP_nWallCM(int cmDistance, bool checkColor){
  encoderState = 1;
  //Get Sharp Distance
  filtrateDistancesSharp(); 
  int actualDistance = sharpFront.kalmanDistance;
  bool ready = actualDistance == cmDistance ? true : false;
  bool goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
  int countCorrect = 0;
  int actualColor;
  //While not at ceratin distance from wall
  while (!ready){                 
    ////////////////////////////MODIFIED////////////////////////////////
    (checkColor)? actualColor=currentColor(): actualColor=10;
    if (actualDistance > cmDistance + 2){     //To far from wall  
      if(actualColor == -1){                                                   //NOT DETECTING COLOR
        forwardP(true);
//        Serial.println("NOT COLOR");
      }
      else{
        if(actualColor == 3){                                                 //DETECTING COLOR BLACK
          brake();
//          stopColor(initialColors[counter%3]);
//          counter++;
          turnOnRedLED();
          forwardP_nTime(goSlow, 1300); 
//          Serial.println("\t\tCOLOR BLACK");
        }
        else if(actualColor == 1){
          brake();
          turnOnBlueLED();
          forwardP_nTime(goSlow, 1300); 
//          Serial.println("\t\tCOLOR BLUE");
        }          
        forwardP(goSlow);
      }
    ////////////////////////////////////////////////////////////////////
      countCorrect = 0;
    }
    else if (actualDistance < cmDistance - 2){ //To close from wall
      backwardP(goSlow);
      countCorrect = 0;
    }
    else{                                      //Already at the distance with an error of +- 2 cm.
      brake();
      if (++countCorrect == 5) ready = true;
    }
    filtrateDistancesSharp(); 
    actualDistance = sharpFront.kalmanDistance;
    goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
  }
  brake();
}
// Go forward following: a setpoint, the distance to the wall, the distance between both sharps
void forwardP_alignWall(double wallDistanceSetpoint, bool goSlow, bool leftWall){
  double frontDistance, backDistance, sharpsDifference;
  double wallDistanceReal, wallDistanceDifference;
  readBNO();    
  filtrateDistancesSharp();  
  readColor();
  if (goSlow) {
    frontLeftOutput = velSlowFL;
    backLeftOutput = velSlowBL;
    frontRightOutput = velSlowFR;
    backRightOutput = velSlowBR;
  } else {
    frontLeftOutput = velGenFL;
    backLeftOutput = velGenBL;
    frontRightOutput = velGenFR;
    backRightOutput = velGenBR;
  }  
  if(leftWall){                               // Choose which sharps to read depending on the wall of (leftWall)
    frontDistance = sharpFL.kalmanDistance;
    backDistance = sharpBL.kalmanDistance; 
  } else{
    frontDistance = sharpFR.kalmanDistance;
    backDistance = sharpBR.kalmanDistance; 
  }
  wallDistanceReal = (frontDistance + backDistance)/2;        // Average of distance to the wall                                                  EXAMPLES USING LEFT WALL
  wallDistanceDifference = wallDistanceReal - wallDistanceSetpoint;     // Difference between what you want and where you are.     (if > 0)->TOCLOSE[moveRight]   (if < 0)->TOFAR[moveLeft]
  sharpsDifference = frontDistance - backDistance;    // Difference between the distance of both sharps.                           (if > 0)->[turnLeft]           (if < 0)->[turnRight}
  int angleDifference = getAngleDifference();  
  if(leftWall){
    frontLeftOutput = computeOutputWall(frontLeftOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
    backLeftOutput = computeOutputWall(backLeftOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
    frontRightOutput = computeOutputWall(frontRightOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    backRightOutput = computeOutputWall(backRightOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
  } else{    
    frontLeftOutput = computeOutputWall(frontLeftOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    backLeftOutput = computeOutputWall(backLeftOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    frontRightOutput = computeOutputWall(frontRightOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
    backRightOutput = computeOutputWall(backRightOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);    
  }
  regulateOutputsFrontPID(); 
//  Serial.print(frontLeftOutput);
//  Serial.print("\t");
//  Serial.print(backLeftOutput);
//  Serial.print("\t");
//  Serial.print(frontRightOutput);
//  Serial.print("\t");    
//  Serial.println(backRightOutput);   
  setMotor(1, 0, 1, 0, 1, 0, 1, 0); 
  setVelocity(frontLeftOutput, backLeftOutput, frontRightOutput, backRightOutput);
}

//Backwards with P correction
void backwardP(bool goSlow){
  readBNO();    
  filtrateDistancesSharp(); 
  readColor();
  if (goSlow) {
    frontLeftOutput = velSlowFL;
    backLeftOutput = velSlowBL;
    frontRightOutput = velSlowFR;
    backRightOutput = velSlowBR;
  } else {
    frontLeftOutput = velGenFL;
    backLeftOutput = velGenBL;
    frontRightOutput = velGenFR;
    backRightOutput = velGenBR;
  }
  int angleDifference = getAngleDifference();
  computeOutput();
  if(angleDifference > 0){ // MOVE TO THE LEFT
    frontLeftOutput += Output;
    backLeftOutput += Output;
    frontRightOutput -= Output;
    backRightOutput -= Output;     
  }
  else if(angleDifference < 0){ // MOVE TO THE RIGHT
    frontLeftOutput -= Output;
    backLeftOutput -= Output;
    frontRightOutput += Output;
    backRightOutput += Output;   
  }  
  regulateOutputsFrontPID(); 
  setMotor(0, 1, 0, 1, 0, 1, 0, 1);
  setVelocity(frontLeftOutput, backLeftOutput, frontRightOutput, backRightOutput);
}
//Backwards with P correction for a specific time
void backwardP_nTime(int time, bool goSlow) {
  double startTime = millis();
  while(millis() < startTime+time)
    backwardP(goSlow);
  brake();
}
//Go backward the cm given in the parameter, Nestor style
void backwardP_nCM(int cm, bool goSlow){
  encoderState = 1;
  //Counts of encoder to get to the objective
  int untilSteps = (encoder30Cm / 30) * cm;
  //Restart encoder counts
  steps = 0;
  //Move with p correction until the encoder read the cm
  while (steps < untilSteps)
    backwardP(goSlow);
  brake();
}
//Go forward until finding a wall at a certain distance
void backwardP_nWallCM(int cmDistance, bool checkColor){
  encoderState = 1;
  //Get Sharp Distance
  filtrateDistancesSharp(); 
  int actualDistance = sharpBack.kalmanDistance; //BackSharp
  bool ready = actualDistance == cmDistance ? true : false;
  bool goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
  int countCorrect = 0;
  int actualColor;
  //While not at ceratin distance from wall
  while (!ready){        
    ////////////////////////////MODIFIED////////////////////////////////   
    (checkColor)? actualColor=currentColor(): actualColor=10;
    if (actualDistance > cmDistance + 2){     //To far from wall
      if(actualColor == -1)                                                   //NOT DETECTING COLOR
        backwardP(true);
      else{
        if(actualColor == 3){                                                 //DETECTING COLOR BLACK
          turnOnRedLED();
          backwardP_nTime(goSlow, 100); 
        }
        else if(actualColor == 1){                                            //DETECTING COLOR BLUE
          stopColor(initialColors[counter%3]);
          counter++;
          backwardP_nTime(goSlow, 100); 
        }
        backwardP(goSlow);
      }
   /////////////////////////////////////////      
      countCorrect = 0;    
    }
    else if (actualDistance < cmDistance - 2){ //To close from wall
      forwardP(goSlow);
      countCorrect = 0;
    }
    else{                                      //Already at the distance with an error of +- 2 cm.
      brake();
      if (++countCorrect == 5) ready = true;
    }
    filtrateDistancesSharp(); 
    actualDistance = sharpBack.kalmanDistance;
    goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
  }
  brake();
}

void spinP(int newAngle){
  if(abs(newAngle) == 180){
    if(newAngle == 180){
      spinP(90);
      spinP(90);
    }
    else if(newAngle == -180){
      spinP(-90);
      spinP(-90);
    }    
    return ;
  }
  else if(abs(newAngle) == 360){
    if(newAngle == 360){
      spinP(180);
      spinP(180);
    }
    else if(newAngle == -360){
      spinP(-180);
      spinP(-180);
    }    
    return ;
  }
  calculateNewSetpoint(newAngle);    
  SetTunings(turnKp, turnKi, turnKd);
  int x = 0;
  double startTime = millis();
  offsetAngle = offsetAngleTurn;  
  do {
    readBNO();
    int angleDifference = getAngleDifference();
    if (abs(angleDifference) > 1) {
      turnP();
//      Serial.print(rawInput);
//      Serial.print("\t");      
//      Serial.print(Setpoint);
//      Serial.print("\t");      
//      Serial.print(angleDifference);
//      Serial.print("\t");
//      Serial.print(frontLeftOutput);
//      Serial.print("\t");
//      Serial.print(frontRightOutput);
//      Serial.print("\t");
//      Serial.println(Output);    
      x = 0;
    } else {
      brake();
      delay(155);      
      if (++x == 3) break;
    }
  } while(millis() < startTime+5000);
  SetTunings(forwardKp, forwardKi, forwardKd);
  offsetAngle = offsetAngleForward;
}

void turnP() {
  readBNO();
  filtrateDistancesSharp(); 
  frontLeftOutput = velTurnFL;
  backLeftOutput = velTurnBL;
  frontRightOutput = velTurnFR;
  backRightOutput = velTurnBR;
  int angleDifference = getAngleDifference();
  computeOutput();
  if(angleDifference > 1){ // MOVE TO THE LEFT
    frontLeftOutput += Output;
    backLeftOutput += Output;
    frontRightOutput += Output;
    backRightOutput += Output; 
    setMotor(0, 1, 0, 1, 1, 0, 1, 0);            
  }
  else if(angleDifference < -1){ // MOVE TO THE RIGHT
    frontLeftOutput += Output;
    backLeftOutput += Output;
    frontRightOutput += Output;
    backRightOutput += Output;        
    setMotor(1, 0, 1, 0, 0, 1, 0, 1);
  }         
  regulateOutputsTurnPID();
  setVelocity(frontLeftOutput, backLeftOutput, frontRightOutput, backRightOutput);          
}

