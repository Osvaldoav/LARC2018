#include <_TCRT5000.h>
const byte  tcrtRightSensor = A7;
const byte tcrtLeftSensor = A6;

void _TCRT5000::setupTCRT5000(){

}

// TODO:
double _TCRT5000::getRawDistance(byte tcrt){
    return analogRead(tcrt);;
}

// TODO:
void _TCRT5000::calculateRawDistancesTCRT5000(){
    tcrtRight.rawDistance = getRawDistance(tcrtRightSensor);
    tcrtLeft.rawDistance = getRawDistance(tcrtLeftSensor); 
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
void _TCRT5000::filtrateDistancesTCRT5000(){
    calculateRawDistancesTCRT5000(); 
    tcrt5000KalmanFilter(tcrtRight);
    tcrt5000KalmanFilter(tcrtLeft);
}

// TODO:
void _TCRT5000::tcrt5000_RawKalman(TCRT5000Kalman &tcrt){
  calculateRawDistancesTCRT5000();
  Serial.print(tcrt.rawDistance);
  Serial.print(",");
  tcrt5000KalmanFilter(tcrt);
  Serial.println(tcrt.kalmanDistance);
}
