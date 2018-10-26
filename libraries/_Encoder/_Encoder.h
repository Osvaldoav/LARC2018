#ifndef _Encoder_h
#define _Encoder_h
#include <_ArduinoLIBRARY.h>

class _Encoder{
    public:
// TODO: METHODS
        void setupEncoder();
// TODO: ATTRIBUTES
        volatile unsigned long stepsFR;
        volatile byte encoderStateFR;
        volatile unsigned long stepsBL;
        volatile byte encoderStateBL;        
        long encoder30Cm;
        volatile unsigned long stepsMechanism;
        volatile byte encoderStateMechanism;       
};

#endif