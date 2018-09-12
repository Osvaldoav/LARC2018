#ifndef _LCD_h
#define _LCD_h
#include <_ArduinoLIBRARY.h>
#include <LiquidCrystal_I2C.h>

class _LCD{
    public:
// TODO: METHODS
    void onLed(char);
    void offLed(char);
    void setupLCD();
// TODO: ATTRIBUTES
};

#endif