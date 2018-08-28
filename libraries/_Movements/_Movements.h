#ifndef _Movimiento_h
#define _Movimiento_h
#include <_BNO055.h>
#include <_ColorSensor.h>
#include <_Encoder.h>
#include <_LibraryPID.h>
#include <_Motors.h>
#include <_Sharp.h>
#include <_LCD.h>

class _Movements{
    public:
// TODO: METHODS
        void forwardP(bool);
        void forwardP_nTime(int, bool);
        void forwardP_nCM(int, bool);
        void forwardP_nWallCM(int, bool);
        void forwardP_alignWall(double, bool, bool);
        void backwardP(bool);
        void backwardP_nTime(int, bool);
        void backwardP_nCM(int, bool);
        void backwardP_nWallCM(int, bool);
        void spinP(int);
        void turnP();
// TODO: ATTRIBUTES
        _BNO055 bno055;
        _ColorSensor colorSensor;
        _Encoder encoder;
        _LibraryPID pid;
        _Motors motors;
        _Sharp sharp;
        _LCD lcd;
};

#endif