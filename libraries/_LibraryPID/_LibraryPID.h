#ifndef _LibraryPID_h
#define _LibraryPID_h
#include <_ArduinoLIBRARY.h>
#include <math.h>

class _LibraryPID{
    public:
// TODO: METHODS
        void setupLibraryPID();
        void regulateOutputsPID(double, double);
        // Calculate Angle Output
        void calculateNewSetpoint(double);
        void setNewSetpoint(double);
        int getAngleDifference(double);
        int getAngleDerivate(double, double);
        void computeOutput_bno(double, double &);
        // Calculate TOF Output
        void computeOutput_tof(double);  
        // Calculate TCRT Output
        char computeOutput_tcrtVerticalLine(double, double, double, double);     
        // DEFAULT PID library functions 
        void Initialize(double, double &);
        void SetTunings(double, double, double);
        void SetSampleTime(int);
        void SetOutputLimits(double, double);
        void SetMode(int, double, double &);
        void SetControllerDirection(int);
// TODO: ATTRIBUTES 
        double Setpoint;
        double frontLeftOutput;
        double frontRightOutput;
        double backLeftOutput;
        double backRightOutput; 
        double Output;
        double OutputAlignMechanism;
        double OutputVerticalBlackLine;
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