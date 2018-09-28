#ifndef _Logic_h
#define _Logic_h

#include <Arduino.h>

class _Traductor{

    public:

        static void horizontalLine(bool b);
        static void throughtHorizontal(int dir);
        static void throughtHorizontal2(int dir);
        static void girar(int grades);
        static void alineaPozo();

};

#endif