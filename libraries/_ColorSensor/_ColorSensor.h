#ifndef _ColorSensor_h
#define _ColorSensor_h
#include <_ArduinoLIBRARY.h>
#include <StackArray.h>
#include <math.h>
#include <String.h>

struct ColorKalman{
  ColorKalman(){
    varSensor = 2e-6; //Variance of sensor. The LESS, the MORE it looks like the raw input.    2e-6
    varProcess = 1e-7; 
    P = 1.0;
    Pc = 0.0;
    G = 0.0;
    Xp = 0.0;
    Zp = 0.0;
    kalmanColor = 0.0;
    rawColor = 0.0;
  }
  float varSensor; //Variance of sensor. The LESS, the MORE it looks like the raw input
  float varProcess; // The greater the variance, faster the sensor response
  float P;
  float Pc;
  float G;
  float Xp;
  float Zp;
  float rawColor;
  float kalmanColor;
};

class _ColorSensor{
    public: 
// TODO: METHODS
        void setupColorSensor();
        void regulateColorSignal(float &);
        void colorKalmanFilter(ColorKalman &);
        void readColor();
        void hardCodedCalibration();
        void colorCalibration(int);
        int currentColor();
        void updateColors(int);
        void showSelectedLed(int);
        void stopColor(int);
        bool colorDecision();
// TODO: ATTRIBUTES
        ColorKalman redColor;
        ColorKalman blueColor;
        ColorKalman greenColor;
};

#endif