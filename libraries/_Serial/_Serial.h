#ifndef _Serial_h
#define _Serial_h

#include <Arduino.h>

class _Serial{
    public:
        static void send(char c);
        static char read();
};

#endif