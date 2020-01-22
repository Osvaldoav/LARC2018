#ifndef _Sharp_h
#define _Sharp_h
#include <_ArduinoLIBRARY.h>
#include <Estadisticas.h>
#include <QuickSort.h>

struct SharpKalman{
    SharpKalman(){
        varSensor = 4e-6; //Variance of sensor. The LESS, the MORE it looks like the raw input. (1e-6)
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

class _Sharp{ 
    public:
// TODO: METHODS
        double getRawDistance(byte); 
        void calculateRawDistancesSharp();
        void sharpKalmanFilter(SharpKalman &);
        void filtrateDistancesSharp();
        void sharp_RawKalman(SharpKalman &);
// TODO: ATTRIBUTES
        SharpKalman sharpRight;
        SharpKalman sharpLeft;
};

#endif