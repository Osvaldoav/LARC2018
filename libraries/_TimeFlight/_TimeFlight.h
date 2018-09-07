#ifndef _TimeFlight_h
#define _TimeFlight_h
#include <_ArduinoLIBRARY.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Estadisticas.h>
#include <QuickSort.h>
struct TimeFlightKalman{
    TimeFlightKalman(){
        varSensor = 15e-6; //Variance of sensor. The LESS, the MORE it looks like the raw input. (40e-6)
        varProcess = 1e-7; 
        P = 1.0;
        Pc = 0.0;
        G = 0.0;
        Xp = 0.0;
        Zp = 0.0;
        kalmanDistance = 0.0;
        rawDistance = 0.0;
        medianDistance = 0.0;
        side = false;
    }
    float varSensor; //Variance of sensor. The LESS, the MORE it looks like the raw input
    float varProcess; // The greater the variance, faster the sensor response
    float P;
    float Pc;
    float G;
    float Xp;
    float Zp;
    float kalmanDistance;
    float rawDistance;
    float medianDistance;
    bool side;
};

class _TimeFlight{
    public:
// TODO: METHODS
        void setupTimeFlight();
        double getRawDistance(bool); 
        void calculateRawDistancesTimeFlight();
        // void calculateMedianDistancesSharp();
        void timeFlightKalmanFilter(TimeFlightKalman &);
        void filtrateDistancesTimeFlight();
        void timeFlight_RawKalman(TimeFlightKalman &);
// TODO: ATTRIBUTES
        TimeFlightKalman timeFlightLeft;
        TimeFlightKalman timeFlightRight;
};

#endif