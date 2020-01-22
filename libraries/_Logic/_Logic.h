#ifndef _Logic_h
#define _Logic_h
#include <_Traductor.h>
#include <Arduino.h>
#include <_Serial.h>

class _Logic{
    private:
        int *stacks;
        uint8_t blue_boxes, green_boxes;
        uint8_t lastStack, currentLevel, lastLevel;
        int firstRed;      
        char lastColor, c_serial;
        const int limitSwitchPin = 35;
        int limitIsPressed;
        bool needToAlign;  
    
    public:
        _Logic();
 
        char verifyColor(char c);
        char handleRed();
        char grabContainer(char c, bool b);
        void stackToShip();
        void shipToStack();
        void pickFirst(char c); // Elige 6 o 7 despues de la primera foto
        void gotoFirst();
        void gotoSecond();
        void initCommunication();
        void backUsingLimitSwitch(double);
        void checkLimit(double);

        void blink(int times);

        _Traductor *traductor;  
        
};

#endif