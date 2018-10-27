#include <_TimeFlight.h>
#define GPIOLeft_SHUT 32
#define GPIORight_SHUT 31

/////////////////////// PINs DECLARATION ////////////////////////
VL53L0X timeFlightRightSensor;
VL53L0X timeFlightLeftSensor;
////////////////////////////// LOCAL VARAIBLES ///////////////////////////////
const double cantReads = 7;
double lastDistance; 
double leftCompensation=-1.87, rightCompensation=-1.6;//-1.87, -1.6

void _TimeFlight::calibTimeFlights(double target){
    leftCompensation=0,rightCompensation=0;
    double left=0, right=0, difference;
    int n=500;
    for(int i=0; i<n; i++){
        // Serial.println("START");
        left += getRawDistance(true);
        right += getRawDistance(false);
    }
    left/=n;
    right/=n;
    if(left>target)
        leftCompensation = -abs(left-target);
    else
        leftCompensation = abs(target-left);
    if(right>target)
        rightCompensation = -abs(right-target);
    else
        rightCompensation = abs(target-right);        
    // Serial.print(leftCompensation);
    // Serial.print(" ");
    // Serial.print(rightCompensation);
    // Serial.print("\t\t");
    // Serial.print(left+leftCompensation);
    // Serial.print(" ");
    // Serial.println(right+rightCompensation);    
}

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
    digitalWrite(GPIOLeft_SHUT, LOW);
    digitalWrite(GPIORight_SHUT, HIGH);    

    timeFlightRightSensor.init();
    timeFlightRightSensor.setAddress((uint8_t)30);
    timeFlightRightSensor.setMeasurementTimingBudget(20000);
    timeFlightRightSensor.startContinuous();

    // activating LOX1 and reseting LOX2
    digitalWrite(GPIOLeft_SHUT, HIGH);   

    timeFlightLeftSensor.init();
    timeFlightLeftSensor.setAddress((uint8_t)31);
    timeFlightLeftSensor.setMeasurementTimingBudget(20000);
    timeFlightLeftSensor.startContinuous();
}

// TODO:
double _TimeFlight::getRawDistance(bool leftTimeFlight){
    double distance;
    if(leftTimeFlight)
        distance = timeFlightLeftSensor.readRangeContinuousMillimeters()/10.0 + leftCompensation;
    else 
        distance = timeFlightRightSensor.readRangeContinuousMillimeters()/10.0 + rightCompensation;
    if(distance > 10)
        distance = 10;
    if(distance < 0)   
        distance *= -1; 
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
