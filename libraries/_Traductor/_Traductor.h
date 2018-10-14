#ifndef _Traductor_h
#define _Traductor_h

#include <Arduino.h>
#include <_Movements.h>

class _Traductor{

    public:
        _Traductor();

        void horizontalLine(bool b);
        void throughtHorizontal(int dir);
        void throughtHorizontal2(int dir);
        void girar(int angle);
        void avanzar(bool b);
        void alinearPozo();
        void alinearTren();
        void mecanismo(uint8_t s);
        void moveAtras();
        void moveToHorizontal(bool b);
        void horizontal(int lines, bool tcrt);
        void moveAtrasHorizontal(); 
        void vertical(int lines);

        _Movements *movements;

};

#endif