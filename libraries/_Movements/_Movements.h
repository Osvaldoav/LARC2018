#ifndef _Movimiento_h
#define _Movimiento_h
#include <_BNO055.h>
#include <_ColorSensor.h>
#include <_Encoder.h>
#include <_LibraryPID.h>
#include <_Motors.h>
#include <_Sharp.h>
#include <_LCD.h>
#include <_TimeFlight.h>
#include <_TCRT5000.h>

class _Movements{
    public:
// TODO: METHODS
        void setupMovements();
        void setWheelsOutputsByMovement(bool, int);
        void movePID(bool, int);
        void movePID_nTime(int, bool, int);
        void movePID_nCM(int, bool, int);
        void movePID_nWallCM(int, int);
        void movePID_alignWall(double, bool, bool, int);
        void spinPID(int);
        void turnPID();
        void alignMechanism();
// TODO: ATTRIBUTES
        _BNO055 *bno055;
        _ColorSensor *colorSensor;
        _Encoder *encoder;
        _LibraryPID *pid;
        _Motors *motors;
        _Sharp *sharp;
        _LCD *lcd;
        _TimeFlight *timeFlight;
        _TCRT5000 *tcrt5000;
};

#endif