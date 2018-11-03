#ifndef _BNO055_h
#define _BNO055_h
#include <_ArduinoLIBRARY.h>
#include <_LCD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

class _BNO055{
    private:
    long lastReadTime = 0;
    int readRateMs = 50;
    
    public:
// TODO: METHODS
        void setupBNO055();
        void setCal();
        void readCal();
        bool getCalStat();
        void calibrarBNO(double &);
        void readBNO(double &);
// TODO: ATTRIBUTES
        double rawInput;
        double lastInput;
        double offsetAngleForward;
        double offsetAngleTurn;
        double offsetAngle;
        _LCD *lcd;
};

#endif