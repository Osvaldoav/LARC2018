#include <_TimeFlight.h>

/////////////////////// PINs DECLARATION ////////////////////////
VL53L0X timeFlightLeftSensor;
VL53L0X timeFlightRightSensor;
////////////////////////////// LOCAL VARAIBLES ///////////////////////////////
const double cantReads = 7;

// TODO:
void _TimeFlight::setupTimeFlight(){
    timeFlightLeftSensor.init();
    timeFlightLeftSensor.setMeasurementTimingBudget(200000);
    // timeFlightLeft.setTimeout(500);
    timeFlightLeftSensor.startContinuous();
    timeFlightRightSensor.init();
    timeFlightRightSensor.setMeasurementTimingBudget(200000);
    // timeFlightRight.setTimeout(500);
    timeFlightRightSensor.startContinuous();
}

// TODO:
double _TimeFlight::getRawDistance(bool leftTimeFlight){
    if(leftTimeFlight)
        return timeFlightLeftSensor.readRangeContinuousMillimeters()/10.0;
    return timeFlightRightSensor.readRangeContinuousMillimeters()/10.0;
}

// TODO:
void _TimeFlight::calculateRawDistancesTimeFlight(){
    timeFlightLeft.rawDistance = getRawDistance(true);
    timeFlightRight.rawDistance = getRawDistance(false);
    delay(26);
}

// TODO:
void _TimeFlight::timeFlightKalmanFilter(TimeFlightKalman &timeFlight){
    timeFlight.Pc = timeFlight.P + timeFlight.varProcess;
    timeFlight.G = timeFlight.Pc/(timeFlight.Pc + timeFlight.varSensor);  //Kalman gain
    timeFlight.P = (1-timeFlight.G)*timeFlight.Pc;
    timeFlight.Xp = timeFlight.kalmanDistance;
    timeFlight.Zp = timeFlight.Xp;
    timeFlight.kalmanDistance = timeFlight.G*(timeFlight.rawDistance-timeFlight.Zp)+timeFlight.Xp;  //Estimates new filtered input
}

// TODO:
void _TimeFlight::filtrateDistancesTimeFlight(){
    // calculateMedianDistancesSharp();
    calculateRawDistancesTimeFlight(); 
    timeFlightKalmanFilter(timeFlightLeft);
    timeFlightKalmanFilter(timeFlightRight);
    // if(sharpFront.kalmanDistance <= 18 && sharpFront.kalmanDistance!=0) sharpFront.side = true;
    // else sharpFront.side = false;
    // if(sharpRight.kalmanDistance <= 13 && sharpRight.kalmanDistance != 0) sharpRight.side = true;
    // else sharpRight.side = false;
    // if(sharpLeft.kalmanDistance <= 13 && sharpLeft.kalmanDistance != 0) sharpLeft.side = true;
    // else sharpLeft.side = false;    
}

// TODO:
void _TimeFlight::timeFlight_RawKalman(TimeFlightKalman &timeFlight){
  calculateRawDistancesTimeFlight();
  Serial.print(timeFlight.rawDistance);
  Serial.print(",");
  timeFlightKalmanFilter(timeFlight);
  Serial.println(timeFlight.kalmanDistance);
}
