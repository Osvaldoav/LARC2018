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
        double velFordFL;
        double velFordBL;
        double velFordFR;
        double velFordBR;
        double velSlowFordFL;
        double velSlowFordBL; 
        double velSlowFordFR;
        double velSlowFordBR;        
        double velTurnFL;
        double velTurnBL;
        double velTurnFR;
        double velTurnBR;
        double velSlowTurnFL;
        double velSlowTurnBL;
        double velSlowTurnFR;
        double velSlowTurnBR;
};
        
        
 
#endif