#ifndef _BNO055_h
#define _BNO055_h
#include "Arduino.h"
#include <math.h>

class _BNO055{
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
};

#endif