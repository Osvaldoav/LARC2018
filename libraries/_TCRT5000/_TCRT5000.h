#ifndef _TCRT5000_h
#define _TCRT5000_h
#include <_ArduinoLIBRARY.h>
#define tcrtUp A7
#define tcrtDown A6

class _TCRT5000{
    public:
// TODO: METHODS
        void setupTCRT5000();
        void readTCRT5000();
// TODO: ATTRIBUTES
    double tcrtUpDistance;
    double tcrtDownDistance;
};

#endif