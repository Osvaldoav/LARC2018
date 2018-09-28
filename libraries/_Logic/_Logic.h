#ifndef _Logic_h
#define _Logic_h

#include <Arduino.h>

class _Logic{
    private:
        int *stacks;
        u_int8_t blue_boxes, green_boxes;
        u_int8_t lastStack,lastColor;
        u_int8_t firstRed;
    
    public:
        _Logic();

        char verifyColor(char c);
        char handleRed();
        char grabContainer(char c);


        void stackToShip();
        void shipToStack(char c);
        
};

#endif