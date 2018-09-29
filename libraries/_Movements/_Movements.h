#ifndef _Movements_h
#define _Movements_h
#include <_BNO055.h>
#include <_ColorSensor.h>
#include <_Encoder.h>
#include <_LibraryPID.h>
#include <_Motors.h> 
#include <_Sharp.h>
#include <_TimeFlight.h>
#include <_TCRT5000.h>
#include <_LCD.h>

class _Movements{
    public:
// TODO: METHODS
        // Modular functions
        void updateSensors(bool, bool, bool, bool, bool, bool);
        void movePID(bool, char);
        void spinPID(bool, double);
        void turnPID(bool);        
        // Get Angle Outputs
        void setBaseVelocitiesByDirection(bool, char);
        void setBaseSpecificVelocities(double, double, double, double);
        void calculateAngleOutputsByDirection(bool, char);
        void verifyAndUploadOutputsByDirection(char);
        void verifySpecificAndUploadOutputs(double, double);
        // Move n Time/CM
        void movePID_nSec(double, bool, char);
        void movePID_nCM(double, bool, char);        
        // Align using TOF PID
        void align_tof();        
        void larc_alignToPickContainer(int);
        // Align using TCRT5000 PID
        void larc_moveBetweenVerticalBlackLine(bool, char);
        char oppositeDirection(char);
        void larc_moveAndAlignToShip();
        void larc_moveUntilBlackLine(bool, char, bool, bool, bool);
// TODO: ATTRIBUTES
        _BNO055 *bno055 = new _BNO055;
        _ColorSensor *colorSensor = new _ColorSensor;
        _Encoder *encoder = new _Encoder;
        _LibraryPID *pid = new _LibraryPID;
        _Motors *motors = new _Motors;
        _Sharp *sharp = new _Sharp;
        _LCD *lcd = new _LCD;      
        _TimeFlight *timeFlight = new _TimeFlight;
        _TCRT5000 *tcrt5000 = new _TCRT5000;
};

#endif