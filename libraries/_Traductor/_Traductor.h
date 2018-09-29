#ifndef _Logic_h
#define _Logic_h

#include <Arduino.h>
#include <_Movements.h>

class _Traductor{

    private:
        _Movements *movements = new _Movements;

    public:

        static void horizontalLine(bool b);
        static void throughtHorizontal(int dir);
        static void throughtHorizontal2(int dir);
        static void girar(int angle);
        static void avanzar(bool b);
        static void alineaPozo();
        static void alinearTren();
        static void mecanismo(u_int8_t s);
        static void moveAtras();
        static void moveToHorizontal(bool b);
        static void horizontal(int lines); //pendiente
        static void moveAtrasHorizontal();
        static void vertical(int lines);

};

#endif