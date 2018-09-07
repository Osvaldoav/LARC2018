#include <_TimeFlight.h>

/////////////////////// PINs DECLARATION ////////////////////////
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define GPIOLeft_SHUT 33
#define GPIORight_SHUT 32

// objects for the vl53l0x
Adafruit_VL53L0X timeFlightLeftSensor = Adafruit_VL53L0X();
Adafruit_VL53L0X timeFlightRightSensor = Adafruit_VL53L0X();
// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
////////////////////////////// LOCAL VARAIBLES ///////////////////////////////
const double cantReads = 7;
double lastDistance;

// TODO:
void _TimeFlight::setupTimeFlight(){
    pinMode(GPIOLeft_SHUT, OUTPUT);
    pinMode(GPIORight_SHUT, OUTPUT);
    // all reset
    digitalWrite(GPIOLeft_SHUT, LOW);    
    digitalWrite(GPIORight_SHUT, LOW);
    delay(10);    
    // all unreset
    digitalWrite(GPIOLeft_SHUT, HIGH);
    digitalWrite(GPIORight_SHUT, HIGH);
    delay(10);    
    // activating LOX1 and reseting LOX2
    digitalWrite(GPIOLeft_SHUT, HIGH);
    digitalWrite(GPIORight_SHUT, LOW);  
    if(!timeFlightLeftSensor.begin(LOX1_ADDRESS)) {
        Serial.println(F("Failed to boot first VL53L0X"));
        while(1);
    }
    delay(10);
    // activating LOX2
    digitalWrite(GPIORight_SHUT, HIGH);
    delay(10);

    if(!timeFlightRightSensor.begin(LOX2_ADDRESS)) {
      Serial.println(F("Failed to boot second VL53L0X"));
      while(1);
    }
    delay(10);
}

// TODO:
double _TimeFlight::getRawDistance(bool leftTimeFlight){
    timeFlightLeftSensor.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    timeFlightRightSensor.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!    
    double distance;
    if(leftTimeFlight){
        if(measure1.RangeStatus != 4)
            distance = measure1.RangeMilliMeter/10.0;
        else
            distance = lastDistance;
    }
    else{
        if(measure2.RangeStatus != 4)
            distance = measure2.RangeMilliMeter/10.0;
        else
            distance = lastDistance;
    }
    if(distance > 25)
        distance = 25;
    // if(abs(lastDistance - distance) > 8)
        // distance = lastDistance;
    lastDistance = distance;
    return distance;
}

// TODO:
void _TimeFlight::calculateRawDistancesTimeFlight(){
    timeFlightLeft.rawDistance = getRawDistance(true);
    timeFlightRight.rawDistance = getRawDistance(false);  
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
    // double sumaLeft=0, sumaRight=0;
    // for(int i=0; i<15; i++){
    //     calculateRawDistancesTimeFlight(); 
    //     timeFlightKalmanFilter(timeFlightLeft);
    //     timeFlightKalmanFilter(timeFlightRight);          
    //     sumaLeft += timeFlightLeft.kalmanDistance;
    //     sumaRight += timeFlightRight.kalmanDistance;
    // }
    // timeFlightLeft.kalmanDistance = sumaLeft/15;
    // timeFlightRight.kalmanDistance = sumaRight/15;
    calculateRawDistancesTimeFlight(); 
    timeFlightKalmanFilter(timeFlightLeft);
    timeFlightKalmanFilter(timeFlightRight);      
    
}

// TODO:
void _TimeFlight::timeFlight_RawKalman(TimeFlightKalman &timeFlight){
  calculateRawDistancesTimeFlight();
  Serial.print(timeFlight.rawDistance);
  Serial.print(",");
  timeFlightKalmanFilter(timeFlight);
  Serial.println(timeFlight.kalmanDistance);
}
