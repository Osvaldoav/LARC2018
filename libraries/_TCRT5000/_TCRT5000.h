#ifndef _TCRT5000_h
#define _TCRT5000_h
#include <_ArduinoLIBRARY.h>

struct TCRT5000Kalman{
    TCRT5000Kalman(){
        varSensor = 1e-6; //Variance of sensor. The LESS, the MORE it looks like the raw input. (1e-6)
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

class _TCRT5000{
    public:
// TODO: METHODS
        void setupTCRT5000();
        void readTCRT5000();
        double getRawDistance(byte); 
        void calculateRawDistancesTCRT5000(bool);
        void tcrt5000KalmanFilter(TCRT5000Kalman &);
        void filtrateDistancesTCRT5000(bool);
        void tcrt5000_RawKalman(TCRT5000Kalman &);        
// TODO: ATTRIBUTES
    TCRT5000Kalman tcrtFrontLeft;
    TCRT5000Kalman tcrtDownLeft;
    TCRT5000Kalman tcrtFrontRight;       
    TCRT5000Kalman tcrtDownRight;
    TCRT5000Kalman tcrtMidFrontLeft;
    TCRT5000Kalman tcrtMidDownLeft;        
    TCRT5000Kalman tcrtMidFrontRight; 
    TCRT5000Kalman tcrtMidDownRight;       
};

#endif