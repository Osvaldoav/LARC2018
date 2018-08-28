#ifndef _Encoder_h
#define _Encoder_h
#include "Arduino.h"

class _Encoder{
    public:
// TODO: METHODS
        void setupEncoder();
// TODO: ATTRIBUTES
        volatile unsigned long steps;
        volatile byte encoderState;
        long encoder30Cm;
};

#endif