#include <_Movements.h>

// TODO:
//forward with P correction
void _Movements::forwardP(bool goSlow){
    bno055.offsetAngle = bno055.offsetAngleForward;
    bno055.readBNO(pid.Setpoint);
    // Serial.print(pid.Setpoint);
    Serial.print(" ");
    Serial.println(bno055.rawInput);
    // Serial.print(" ");
    sharp.filtrateDistancesSharp();
    colorSensor.readColor(); 
    if (goSlow) {
        pid.frontLeftOutput = motors.velSlowFL;
        pid.backLeftOutput = motors.velSlowBL;
        pid.frontRightOutput = motors.velSlowFR;
        pid.backRightOutput = motors.velSlowBR;
    } else {  
        pid.frontLeftOutput = motors.velGenFL;
        pid.backLeftOutput = motors.velGenBL;
        pid.frontRightOutput = motors.velGenFR;
        pid.backRightOutput = motors.velGenBR;
    }
    int angleDifference = pid.getAngleDifference(bno055.rawInput);
    // Serial.print(angleDifference);
    // Serial.print(" ");
    pid.computeOutput(bno055.rawInput, bno055.lastInput);  
    // Serial.println(pid.Output);
    if(angleDifference > 0){ // MOVE TO THE LEFT
        pid.frontLeftOutput -= pid.Output;
        pid.backLeftOutput -= pid.Output;
        pid.frontRightOutput += pid.Output;
        pid.backRightOutput += pid.Output;     
    }
    else if(angleDifference < 0){ // MOVE TO THE RIGHT
        pid.frontLeftOutput += pid.Output;
        pid.backLeftOutput += pid.Output;
        pid.frontRightOutput -= pid.Output;
        pid.backRightOutput -= pid.Output;   
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
    pid.regulateOutputsFrontPID(); 
    motors.setMotor(1, 0, 1, 0, 1, 0, 1, 0);
    motors.setVelocity(pid.frontLeftOutput, pid.backLeftOutput, pid.frontRightOutput, pid.backRightOutput);
}

// TODO:
//forward with P correction for a specific time
void _Movements::forwardP_nTime(int time, bool goSlow) {
    double startTime = millis();
    while(millis() < startTime+time)
        forwardP(goSlow);
    motors.brake();
}

// TODO:
//Go forward the cm given in the parameter
void _Movements::forwardP_nCM(int cm, bool goSlow){
    encoder.encoderState = 1;
    //Counts of encoder to get to the objective
    int untilSteps = (encoder.encoder30Cm / 30) * cm;
    //Restart encoder counts
    encoder.steps = 0;
    //Move with p correction until the encoder read the cm
    while (encoder.steps < untilSteps)
        forwardP(goSlow);
    motors.brake();
}
// TODO:
//Go forward until finding a wall at a certain distance
void _Movements::forwardP_nWallCM(int cmDistance, bool checkColor){
    encoder.encoderState = 1;
    //Get Sharp Distance
    sharp.filtrateDistancesSharp(); 
    int actualDistance = sharp.sharpFront.kalmanDistance;
    bool ready = actualDistance == cmDistance ? true : false;
    bool goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
    int countCorrect = 0;
    int actualColor;
    //While not at ceratin distance from wall
    while (!ready){                 
        ////////////////////////////MODIFIED////////////////////////////////
        (checkColor)? actualColor=colorSensor.currentColor(): actualColor=10;
        if (actualDistance > cmDistance + 2){     //To far from wall  
        if(actualColor == -1){                                                   //NOT DETECTING COLOR
            forwardP(true);
    //        Serial.println("NOT COLOR");
        }
        else{
            if(actualColor == 3){                                                 //DETECTING COLOR BLACK
            motors.brake();
            forwardP_nTime(goSlow, 1300); 
    //          Serial.println("\t\tCOLOR BLACK");
            }
            else if(actualColor == 1){
            motors.brake();
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
        motors.brake();
        if (++countCorrect == 5) ready = true;
        }
        sharp.filtrateDistancesSharp(); 
        actualDistance = sharp.sharpFront.kalmanDistance;
        goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
    }
    motors.brake();
}

// TODO:
// Go forward following: a setpoint, the distance to the wall, the distance between both sharps
void _Movements::forwardP_alignWall(double wallDistanceSetpoint, bool goSlow, bool leftWall){
    double frontDistance, backDistance, sharpsDifference;
    double wallDistanceReal, wallDistanceDifference;
    bno055.readBNO(pid.Setpoint);    
    sharp.filtrateDistancesSharp();  
    colorSensor.readColor();
    if (goSlow) {
        pid.frontLeftOutput = motors.velSlowFL;
        pid.backLeftOutput = motors.velSlowBL;
        pid.frontRightOutput = motors.velSlowFR;
        pid.backRightOutput = motors.velSlowBR;
    } else {
        pid.frontLeftOutput = motors.velGenFL;
        pid.backLeftOutput = motors.velGenBL;
        pid.frontRightOutput = motors.velGenFR;
        pid.backRightOutput = motors.velGenBR;
    }  
    if(leftWall){                               // Choose which sharps to read depending on the wall of (leftWall)
        frontDistance = sharp.sharpFL.kalmanDistance;
        backDistance = sharp.sharpBL.kalmanDistance; 
    } else{
        frontDistance = sharp.sharpFR.kalmanDistance;
        backDistance = sharp.sharpBR.kalmanDistance; 
    }
    wallDistanceReal = (frontDistance + backDistance)/2;        // Average of distance to the wall                                                  EXAMPLES USING LEFT WALL
    wallDistanceDifference = wallDistanceReal - wallDistanceSetpoint;     // Difference between what you want and where you are.     (if > 0).TOCLOSE[moveRight]   (if < 0).TOFAR[moveLeft]
    sharpsDifference = frontDistance - backDistance;    // Difference between the distance of both sharps.                           (if > 0).[turnLeft]           (if < 0).[turnRight}
    int angleDifference = pid.getAngleDifference(bno055.rawInput);  
    if(leftWall){
        pid.frontLeftOutput = pid.computeOutputWall(pid.frontLeftOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
        pid.backLeftOutput = pid.computeOutputWall(pid.backLeftOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
        pid.frontRightOutput = pid.computeOutputWall(pid.frontRightOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
        pid.backRightOutput = pid.computeOutputWall(pid.backRightOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
    } else{    
        pid.frontLeftOutput = pid.computeOutputWall(pid.frontLeftOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
        pid.backLeftOutput = pid.computeOutputWall(pid.backLeftOutput, angleDifference, sharpsDifference, wallDistanceDifference);   // Calculate special output (using only P);
        pid.frontRightOutput = pid.computeOutputWall(pid.frontRightOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);
        pid.backRightOutput = pid.computeOutputWall(pid.backRightOutput, -angleDifference, -sharpsDifference, -wallDistanceDifference);   // Calculate special output (using only P);    
    }
    pid.regulateOutputsFrontPID(); 
    //  Serial.print(frontLeftOutput);
    //  Serial.print("\t");
    //  Serial.print(backLeftOutput);
    //  Serial.print("\t");
    //  Serial.print(frontRightOutput);
    //  Serial.print("\t");    
    //  Serial.println(backRightOutput);   
    motors.setMotor(1, 0, 1, 0, 1, 0, 1, 0); 
    motors.setVelocity(pid.frontLeftOutput, pid.backLeftOutput, pid.frontRightOutput, pid.backRightOutput);
}

// TODO:
//Backwards with P correction
void _Movements::backwardP(bool goSlow){
    bno055.readBNO(pid.Setpoint);    
    sharp.filtrateDistancesSharp(); 
    colorSensor.readColor();
    if (goSlow) {
        pid.backLeftOutput = motors.velSlowBL;
        pid.frontLeftOutput = motors.velSlowFL;
        pid.frontRightOutput = motors.velSlowFR;
        pid.backRightOutput = motors.velSlowBR;
    } else {
        pid.frontLeftOutput = motors.velGenFL;
        pid.backLeftOutput = motors.velGenBL;
        pid.frontRightOutput = motors.velGenFR;
        pid.backRightOutput = motors.velGenBR;
    }
    int angleDifference = pid.getAngleDifference(bno055.rawInput);
    pid.computeOutput(bno055.rawInput, bno055.lastInput);
    if(angleDifference > 0){ // MOVE TO THE LEFT
        pid.frontLeftOutput += pid.Output;
        pid.backLeftOutput += pid.Output;
        pid.frontRightOutput -= pid.Output;
        pid.backRightOutput -= pid.Output;     
    }
    else if(angleDifference < 0){ // MOVE TO THE RIGHT
        pid.frontLeftOutput -= pid.Output;
        pid.backLeftOutput -= pid.Output;
        pid.frontRightOutput += pid.Output;
        pid.backRightOutput += pid.Output;   
    }  
    pid.regulateOutputsFrontPID(); 
    motors.setMotor(0, 1, 0, 1, 0, 1, 0, 1);
    motors.setVelocity(pid.frontLeftOutput, pid.backLeftOutput, pid.frontRightOutput, pid.backRightOutput);
}

// TODO:
//Backwards with P correction for a specific time
void _Movements::backwardP_nTime(int time, bool goSlow) {
    double startTime = millis();
    while(millis() < startTime+time)
        backwardP(goSlow);
    motors.brake();
}

//Go backward the cm given in the parameter, Nestor style
// TODO:
void _Movements::backwardP_nCM(int cm, bool goSlow){
    encoder.encoderState = 1;
    //Counts of encoder to get to the objective
    int untilSteps = (encoder.encoder30Cm / 30) * cm;
    //Restart encoder counts
    encoder.steps = 0;
    //Move with p correction until the encoder read the cm
    while (encoder.steps < untilSteps)
        backwardP(goSlow);
    motors.brake();
}

// TODO:
//Go forward until finding a wall at a certain distance
void _Movements::backwardP_nWallCM(int cmDistance, bool checkColor){
    encoder.encoderState = 1;
    //Get Sharp Distance
    sharp.filtrateDistancesSharp(); 
    int actualDistance = sharp.sharpBack.kalmanDistance; //BackSharp
    bool ready = actualDistance == cmDistance ? true : false;
    bool goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
    int countCorrect = 0;
    int actualColor;
    //While not at ceratin distance from wall
    while (!ready){        
        ////////////////////////////MODIFIED////////////////////////////////   
        (checkColor)? actualColor=colorSensor.currentColor(): actualColor=10;
        if (actualDistance > cmDistance + 2){     //To far from wall
        if(actualColor == -1)                                                   //NOT DETECTING COLOR
            backwardP(true);
        else{
            if(actualColor == 3){                                                 //DETECTING COLOR BLACK
                backwardP_nTime(goSlow, 100); 
            }
            else if(actualColor == 1){                                            //DETECTING COLOR BLUE
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
            motors.brake();
            if (++countCorrect == 5) ready = true;
        }
        sharp.filtrateDistancesSharp(); 
        actualDistance = sharp.sharpBack.kalmanDistance;
        goSlow = abs(actualDistance - cmDistance) >= 15 ? false : true;
    }
    motors.brake();
}

// TODO:
void _Movements::spinP(int newAngle){
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
    pid.calculateNewSetpoint(newAngle);    
    pid.SetTunings(pid.turnKp, pid.turnKi, pid.turnKd);
    int x = 0;
    double startTime = millis();
    bno055.offsetAngle = bno055.offsetAngleTurn;  
    do {
        bno055.readBNO(pid.Setpoint);
        int angleDifference = pid.getAngleDifference(bno055.rawInput);
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
        motors.brake();
        delay(155);      
        if (++x == 3) break;
        }
    } while(millis() < startTime+5000);
    pid.SetTunings(pid.forwardKp, pid.forwardKi, pid.forwardKd);
    bno055.offsetAngle = bno055.offsetAngleForward;
}

// TODO:
void _Movements::turnP() {
    bno055.readBNO(pid.Setpoint);
    sharp.filtrateDistancesSharp(); 
    pid.frontLeftOutput = motors.velTurnFL;
    pid.backLeftOutput = motors.velTurnBL;
    pid.frontRightOutput = motors.velTurnFR;
    pid.backRightOutput = motors.velTurnBR;
    int angleDifference = pid.getAngleDifference(bno055.rawInput);
    pid.computeOutput(bno055.rawInput, bno055.lastInput);
    if(angleDifference > 1){ // MOVE TO THE LEFT
        pid.frontLeftOutput += pid.Output;
        pid.backLeftOutput += pid.Output;
        pid.frontRightOutput += pid.Output;
        pid.backRightOutput += pid.Output; 
        motors.setMotor(0, 1, 0, 1, 1, 0, 1, 0);            
    }
    else if(angleDifference < -1){ // MOVE TO THE RIGHT
        pid.frontLeftOutput += pid.Output;
        pid.backLeftOutput += pid.Output;
        pid.frontRightOutput += pid.Output;
        pid.backRightOutput += pid.Output;        
        motors.setMotor(1, 0, 1, 0, 0, 1, 0, 1);
    }         
    pid.regulateOutputsTurnPID();
    motors.setVelocity(pid.frontLeftOutput, pid.backLeftOutput, pid.frontRightOutput, pid.backRightOutput);          
}