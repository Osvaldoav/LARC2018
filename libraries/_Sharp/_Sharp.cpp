#include <_Sharp.h>

/////////////////////// PINs DECLARATION ////////////////////////
const byte pinSharpFR = A4;
const byte pinSharpFL = A3;
const byte pinSharpBR = A1;
const byte pinSharpBL = A2;
const byte pinSharpBack = A5; 
const byte pinSharpFront = A0;
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
    sharpFR.rawDistance = getRawDistance(pinSharpFR);
    sharpFL.rawDistance = getRawDistance(pinSharpFL); 
    sharpBR.rawDistance = getRawDistance(pinSharpBR);
    sharpBL.rawDistance = getRawDistance(pinSharpBL);
    sharpBack.rawDistance = getRawDistance(pinSharpBack);
    sharpFront.rawDistance = getRawDistance(pinSharpFront);  
    delay(26);
}

// TODO:
// void _Sharp::calculateMedianDistancesSharp(){
//     int readsFront[7], readsRight[7], readsLeft[7];
//     for (int j = 0; j < cantReads; j++){
//       float voltsFront = analogRead(sharpFrontPin) * 0.0048828125;    //Raw data
//       if (voltsFront == 0)     readsFront[j] = 30000;
//       else                     readsFront[j] = round(13.0 / voltsFront); 
//       float voltsRight = analogRead(sharpRightPin) * 0.0048828125;    //Raw data
//       if (voltsRight == 0)     readsRight[j] = 30000;
//       else                     readsRight[j] = round(13.0 / voltsRight); 
//       float voltsLeft = analogRead(sharpLeftPin) * 0.0048828125;    //Raw data
//       if (voltsLeft == 0)     readsLeft[j] = 30000;
//       else                     readsLeft[j] = round(13.0 / voltsLeft);         
//       delay(26);
//     }
//     sharpFront.medianDistance = Estadisticas<int>::createNewMediana(readsFront, cantReads).getMediana();
//     sharpRight.medianDistance = Estadisticas<int>::createNewMediana(readsRight, cantReads).getMediana();
//     sharpLeft.medianDistance = Estadisticas<int>::createNewMediana(readsLeft, cantReads).getMediana();
// }

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
    // calculateMedianDistancesSharp();
    calculateRawDistancesSharp(); 
    sharpKalmanFilter(sharpFR);
    sharpKalmanFilter(sharpFL);
    sharpKalmanFilter(sharpBR);
    sharpKalmanFilter(sharpBL);
    sharpKalmanFilter(sharpBack);
    sharpKalmanFilter(sharpFront);
    // if(sharpFront.kalmanDistance <= 18 && sharpFront.kalmanDistance!=0) sharpFront.side = true;
    // else sharpFront.side = false;
    // if(sharpRight.kalmanDistance <= 13 && sharpRight.kalmanDistance != 0) sharpRight.side = true;
    // else sharpRight.side = false;
    // if(sharpLeft.kalmanDistance <= 13 && sharpLeft.kalmanDistance != 0) sharpLeft.side = true;
    // else sharpLeft.side = false;    
}

// TODO:
void _Sharp::sharp_RawKalman(SharpKalman &sharp){
  calculateRawDistancesSharp();
  Serial.print(sharp.rawDistance);
  Serial.print(",");
  sharpKalmanFilter(sharp);
  Serial.println(sharp.kalmanDistance);
}
