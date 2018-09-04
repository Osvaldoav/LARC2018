#ifndef _Motors_h
#define _Motors_h
#include <_ArduinoLIBRARY.h>
#include <math.h>

class _Motors{
    public:
//TODO: METHODS
        void setupMotors();
        double slowGo(double);
        void stop(bool);
        void go();
        void back();
        void setMotor(bool, bool, bool, bool, bool, bool, bool, bool);
        void setVelocity(int, int);
        void setVelocity(int, int, int, int);
        void brake();
        void turnRight(int);
        void turnRight(int, int, int, int);
        void turnLeft(int);
        void turnLeft(int, int, int, int); 
// TODO: ATTRIBUTES
        double velSlowFL;
        double velSlowBL; 
        double velSlowFR;
        double velSlowBR;
        double velGenFL;
        double velGenBL;
        double velGenFR;
        double velGenBR;
        double velTurnFL;
        double velTurnBL;
        double velTurnFR;
        double velTurnBR;
};
 
#endif