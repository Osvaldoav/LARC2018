#include <_Sharp.h>

/////////////////////// PINs DECLARATION ////////////////////////
const byte pinSharpRight = A0;
const byte pinSharpLeft = A1;
////////////////////////////// LOCAL VARAIBLES ///////////////////////////////
const double cantReads = 7;

// TODO:
double _Sharp::getRawDistance(byte sharp){
    float distance;
    float volts = analogRead(sharp) * 0.0048828125;
    if (volts == 0)                                    //If the raw data gives zero is really far
      distance = 30000;
    else                                               //The distance is acceptable save the formula
      distance = round(13.0 / volts);   
    if(distance > 65)
      return 65;
    return distance;
}

// TODO:
void _Sharp::calculateRawDistancesSharp(){
    sharpRight.rawDistance = getRawDistance(pinSharpRight);
    sharpLeft.rawDistance = getRawDistance(pinSharpLeft); 
    delay(20);
}

// TODO:
void _Sharp::sharpKalmanFilter(SharpKalman &sharp){
    sharp.Pc = sharp.P + sharp.varProcess;
    sharp.G = sharp.Pc/(sharp.Pc + sharp.varSensor);  //Kalman gain
    sharp.P = (1-sharp.G)*sharp.Pc;
    sharp.Xp = sharp.kalmanDistance;
    sharp.Zp = sharp.Xp;
    sharp.kalmanDistance = sharp.G*(sharp.rawDistance-sharp.Zp)+sharp.Xp;  //Estimates new filtered input
}

// TODO:
void _Sharp::filtrateDistancesSharp(){
    calculateRawDistancesSharp(); 
    sharpKalmanFilter(sharpRight);
    sharpKalmanFilter(sharpLeft); 
}

// TODO:
void _Sharp::sharp_RawKalman(SharpKalman &sharp){
  calculateRawDistancesSharp();
  Serial.print(sharp.rawDistance);
  Serial.print(",");
  sharpKalmanFilter(sharp);
  Serial.println(sharp.kalmanDistance);
}
