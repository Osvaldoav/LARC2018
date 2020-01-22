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
#include <_Servo.h>

class _Movements{
    public:
        _Movements();
// TODO: METHODS
        // Modular functions
        void setInitialSetpoint();
        void updateSensors(bool, bool, bool, bool, bool, bool);
        void movePID(bool, char);
        void movePID(bool, char, double, double);
        void crazyMove(char);
        void spinPID(bool, double);
        void turnPID(bool);        
        // Get Angle Outputs
        double evaluatePWMFunctionByType(double, double, double);
        void setBaseVelocitiesByDirection(bool, char, double, double);
        void setBaseSpecificVelocities(double, double, double, double);
        void calculateAngleOutputsByDirection(bool, char);
        void verifyAndUploadOutputsByDirection(char);
        void verifySpecificAndUploadOutputs(double, double);
        // Move n Time/CM
        void movePID_nSec(double, bool, char);
        void movePID_nCM(double, bool, char);        
        // Align using TOF PID
        void align_tof();        
        void getCloseToStack();
        // Align using TCRT5000 PID
        void larc_moveBetweenVerticalBlackLine(bool, char, bool);
        char oppositeDirection(char);
        void moveToShip(bool);
        void alignShip();
        void alignFirstShip();
        void centerContainer(char);
        void larc_moveUntilBlackLine(bool, char, bool, bool, bool, bool, bool);
        void moveMechanism(int, int);
        void asyncMovement(int, char, int, int);
        void initMechanism();
        void alignLine();
        void moveMechanismForAligning(bool, double);
        void moveUntilThreshold();
        void moveToTrain(bool);
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
        _Servo *servo;
        double BLACKLINE_TRIGGER;//300
        double BLACKLINE_TRIGGER_SHIP;
        long untilStepsMechanism;
        bool crazyMode;
        bool crazyRight;
        long moveCalled;        
};

#endif