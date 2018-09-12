#include <_LCD.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

///////////////////// Local Variables
const byte ledRed = 38;
const byte ledGreen = 36;
const byte ledBlue = 35;

void _LCD::setupLCD(){
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledBlue, OUTPUT);
}

void _LCD::onLed(char led){
    switch(led){
        case 'r':
            digitalWrite(ledRed, HIGH);
            break;
        case 'g':
            digitalWrite(ledGreen, HIGH);
            break;
        case 'b':
            digitalWrite(ledBlue, HIGH);
            break;
    }
}
void _LCD::offLed(char led){
    switch(led){
        case 'r':
            digitalWrite(ledRed, LOW);
            break;
        case 'g':
            digitalWrite(ledGreen, LOW);
            break;
        case 'b':
            digitalWrite(ledBlue, LOW);
            break;
    }
}

