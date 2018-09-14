#ifndef _LibraryPID_h
#define _LibraryPID_h
#include <_ArduinoLIBRARY.h>
#include <math.h>

class _LibraryPID{
    public:
// TODO: METHODS
        void calculateNewSetpoint(int);
        void setNewSetpoint(double);
        int getAngleDifference(double);
        int getAngleDerivate(double, double);
        void computeOutput(double, double &);
        void computeOutputAlignMechanism(double);
        void regulateOutputsPID(double, double);
        void Initialize(double, double &);
        void SetTunings(double, double, double);
        void SetSampleTime(int);
        void SetOutputLimits(double, double);
        void SetMode(int, double, double &);
        void SetControllerDirection(int);
        void setupLibraryPID();
// TODO: ATTRIBUTES 
        double Setpoint;
        double frontLeftOutput;
        double frontRightOutput;
        double backLeftOutput;
        double backRightOutput; 
        double Output;
        double OutputAlignMechanism;
        /////// PID CONSTANTS ///////
        double alignKp;
        double alignKi;
        double alignKd;
        double turnKp;
        double turnKi;
        double turnKd;
        double forwardKp;
        double forwardKi;
        double forwardKd;
        /////// Velocities ///////
        double maxTurnVel;      
        double minTurnVel;
        double maxForwardVel;
        double minForwardVel;          
};

#endif