#ifndef _Traductor_h
#define _Traductor_h

#include <Arduino.h>
#include <_Movements.h>

class _Traductor{

    public:
        _Traductor();

        void horizontalLine(bool b);
        void throughtHorizontal(int, int, int);
        void throughtHorizontal2(int, int, int);
        void girar(int angle);
        void avanzar(bool b);
        void alinearPozo();
        void alinearTren();
        void mecanismo(uint8_t newStack, uint8_t currentStack);
        void grabContainer();
        void dropContainer();
        void moveAtras();
        void moveToHorizontal(bool b);
        void horizontal(int lines, bool tcrt, int, int);
        void backUntilBlackLineSharps(bool tcrt);
        void moveAtrasHorizontal(); 
        void vertical(int lines);
        void alinearStack();
        void updateMechanismMovement(int, int);

        void pickFirst(int stack);
        void gotoFirst();

        _Movements *movements;

};

#endif