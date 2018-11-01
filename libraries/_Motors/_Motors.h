#ifndef _Motors_h
#define _Motors_h
#include <_ArduinoLIBRARY.h>
#include <math.h>

class _Motors{
    public:
//TODO: METHODS
        void setupMotors();
        double slowGo(double);
        void setMotor(bool, bool, bool, bool, bool, bool, bool, bool);
        void setVelocity(int, int);
        void setVelocity(int, int, int, int);
        void brake();
        void moveMechanism(bool);
        void stopMechanism();
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
        double velHorFL;
        double velHorBL;
        double velHorFR;
        double velHorBR;
        double INITIAL_VELOCITY;
        double FINAL_VELOCITY;
};
        
        
 
#endif