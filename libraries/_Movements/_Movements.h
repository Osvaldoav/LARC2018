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
        void updateSensors(bool, bool, bool, bool, bool);
        void setBaseVelocitiesByDirection(bool, char);
        void setBaseSpecificVelocities(double, double, double, double);
        void calculateAngleOutputsByDirection(bool, char);
        void verifyAndUploadOutputsByDirection(char);
        void verifySpecificAndUploadOutputs(double, double);
        void movePID(bool, char);
        void movePID_nTime(int, bool, char);
        void movePID_nCM(int, bool, char);
        void alignMechanism();        
        void movePID_nWallCM(int, char);
        void movePID_alignWall(double, bool, bool, int);
        void spinPID(bool, int);
        void turnPID(bool);
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