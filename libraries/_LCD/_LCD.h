#ifndef _LCD_h
#define _LCD_h
#include <_ArduinoLIBRARY.h>
#include <LiquidCrystal_I2C.h> // Debe descargar la Libreria que controla el I2C
#include<Wire.h>

class _LCD{
    public:
// TODO: METHODS
    void print(String, String message2="");
    void print(String, int);
    void print(String, long);
    void print(String, char);  
    void print(String, double);  
    void printAlertSec(String, int);
    void clear();
    void onLed();
    void offLed();
    void setupLCD();
// TODO: ATTRIBUTES
};

#endif