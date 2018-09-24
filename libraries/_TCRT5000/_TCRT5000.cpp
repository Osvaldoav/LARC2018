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
const byte  tcrtMidFrontLeftSensor = A1;
const byte  tcrtMidDownLeftSensor = A3;
const byte  tcrtMidFrontRightSensor = A0;
const byte  tcrtMidDownRightSensor = A2;
const byte  tcrtFrontLeftSensor = A7;
const byte  tcrtDownLeftSensor = A6;
const byte  tcrtFrontRightSensor = A5;
const byte  tcrtDownRightSensor = A4;

// TODO:
void _TCRT5000::setupTCRT5000(){

}
// TODO:
double _TCRT5000::getRawDistance(byte tcrt){
    return analogRead(tcrt);;
}
// TODO:
void _TCRT5000::calculateRawDistancesTCRT5000(bool dropContainer){
    if(dropContainer){
        tcrtFrontLeft.rawDistance = getRawDistance(tcrtFrontLeftSensor);
        tcrtDownLeft.rawDistance = getRawDistance(tcrtDownLeftSensor); 
        tcrtFrontRight.rawDistance = getRawDistance(tcrtFrontRightSensor);
        tcrtDownRight.rawDistance = getRawDistance(tcrtDownRightSensor);    
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
        tcrt5000KalmanFilter(tcrtFrontLeft);
        tcrt5000KalmanFilter(tcrtDownLeft);
        tcrt5000KalmanFilter(tcrtFrontRight);
        tcrt5000KalmanFilter(tcrtDownRight); 
    } 
    else{
        tcrt5000KalmanFilter(tcrtMidFrontLeft);
        tcrt5000KalmanFilter(tcrtMidDownLeft);
        tcrt5000KalmanFilter(tcrtMidFrontRight);
        tcrt5000KalmanFilter(tcrtMidDownRight);    
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
