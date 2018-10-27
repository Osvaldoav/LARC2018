#ifndef _LCD_h
#define _LCD_h
#include <_ArduinoLIBRARY.h>
#include <LiquidCrystal_I2C.h> // Debe descargar la Libreria que controla el I2C
#include<Wire.h>

class _LCD{
    public:
// TODO: METHODS
    void print(String, String message2="");
    void printInt(String, int);
    void printAlertSec(String, int);
    void clear();
    void onLed();
    void offLed();
    void setupLCD();
// TODO: ATTRIBUTES
};

#endif