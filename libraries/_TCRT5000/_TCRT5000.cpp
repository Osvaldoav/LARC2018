#include <_TCRT5000.h>
// FIXME:
/* TCRT positions following Vertical black line
        ||  A2  ||     ||  A3  ||          || midFL ||    || midFR ||
        ||      ||     ||      ||    =>    ||       ||    ||       ||
        ||  A0  ||     ||  A1  ||          || midBL ||    || midBR ||
*/
/* TCRT positions aligning to drop Container
        ||      ||     ||  A7  ||          || FL ||    || FR ||
        ||  A6  ||     ||      ||          || BL ||    || BR ||      
        ||  A4  ||     ||      ||          || BL ||    || BR ||        
        ||      ||     ||  A5  ||          || BL ||    || BR ||
*/
const byte  tcrtMidFrontLeftSensor = A8;
const byte  tcrtMidDownLeftSensor = A3;
const byte  tcrtMidFrontRightSensor = A9;
const byte  tcrtMidDownRightSensor = A2;
const byte  tcrtMechaLeftSensor = A7;
const byte  tcrtMechaRightSensor = A6;
const byte  tcrtSharpRightSensor = A5;
const byte  tcrtSharpLeftSensor = A4;

// TODO:
void _TCRT5000::setupTCRT5000(){
    leftMechanism = 0;
    rightMechanism = 0;    
    for(int i=0; i<100; i++){
        filtrateDistancesTCRT5000(true);
        leftMechanism += tcrtMechaLeft.kalmanDistance;
        rightMechanism += tcrtMechaRight.kalmanDistance;
    }
    leftMechanism /= 100.0;
    rightMechanism /= 100.0;
}
// TODO:
double _TCRT5000::getRawDistance(byte tcrt){
    return analogRead(tcrt);;
}
// TODO:
void _TCRT5000::calculateRawDistancesTCRT5000(bool dropContainer){
    if(dropContainer){
        tcrtMechaLeft.rawDistance = getRawDistance(tcrtMechaLeftSensor);
        tcrtSharpRight.rawDistance = getRawDistance(tcrtMechaRightSensor); 
        tcrtMechaRight.rawDistance = getRawDistance(tcrtSharpRightSensor);
        tcrtSharpLeft.rawDistance = getRawDistance(tcrtSharpLeftSensor);            
    }
    else{ 
        tcrtMidFrontLeft.rawDistance = getRawDistance(tcrtMidFrontLeftSensor);
        tcrtMidDownLeft.rawDistance = getRawDistance(tcrtMidDownLeftSensor); 
        tcrtMidFrontRight.rawDistance = getRawDistance(tcrtMidFrontRightSensor);
        tcrtMidDownRight.rawDistance = getRawDistance(tcrtMidDownRightSensor);  
    }   
}
// TODO:
void _TCRT5000::tcrt5000KalmanFilter(TCRT5000Kalman &tcrt){
    tcrt.Pc = tcrt.P + tcrt.varProcess;
    tcrt.G = tcrt.Pc/(tcrt.Pc + tcrt.varSensor);  //Kalman gain
    tcrt.P = (1-tcrt.G)*tcrt.Pc;
    tcrt.Xp = tcrt.kalmanDistance;
    tcrt.Zp = tcrt.Xp;
    tcrt.kalmanDistance = tcrt.G*(tcrt.rawDistance-tcrt.Zp)+tcrt.Xp;  //Estimates new filtered input
}
// TODO:
void _TCRT5000::filtrateDistancesTCRT5000(bool dropContainer){
    calculateRawDistancesTCRT5000(dropContainer); 
    if(dropContainer){
        tcrtMechaLeft.kalmanDistance = tcrtMechaLeft.rawDistance;
        tcrtMechaRight.kalmanDistance = tcrtMechaRight.rawDistance;
        tcrtSharpLeft.kalmanDistance = tcrtSharpLeft.rawDistance;
        tcrtSharpRight.kalmanDistance = tcrtSharpRight.rawDistance;         
    } 
    else{
        tcrtMidFrontLeft.kalmanDistance = tcrtMidFrontLeft.rawDistance;
        tcrtMidDownLeft.kalmanDistance = tcrtMidDownLeft.rawDistance;
        tcrtMidFrontRight.kalmanDistance = tcrtMidFrontRight.rawDistance;
        tcrtMidDownRight.kalmanDistance = tcrtMidDownRight.rawDistance;    
    }    
}
// TODO:
void _TCRT5000::tcrt5000_RawKalman(TCRT5000Kalman &tcrt){
  calculateRawDistancesTCRT5000(true);
  calculateRawDistancesTCRT5000(false);  
  Serial.print(tcrt.rawDistance);
  Serial.print(",");
  tcrt5000KalmanFilter(tcrt);
  Serial.println(tcrt.kalmanDistance);
}
// TODO:
