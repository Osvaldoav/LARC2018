#include <_LCD.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
uint8_t updateTimeMs = 50;
long lastUpdate = 0;

///////////////////// Local Variables
const byte ledRed = 38;
const byte ledGreen = 36;
const byte ledBlue = 35;

void _LCD::setupLCD(){
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledBlue, OUTPUT);
	Serial.println("LCD initializing...");
	lcd.begin();
	lcd.backlight();	
    Serial.println("LCD initialized");
	// lcd.backlight();    
}

void _LCD::print(String message1, String message2) {
    // if(millis() - lastUpdate > updateTimeMs){
        lcd.setCursor(0, 0);    
        lcd.clear();
        lcd.print(message1);
        lcd.setCursor(0, 1);
        lcd.print(message2);
    // }
    // lastUpdate = millis();    
}

void _LCD::clear() {
        lcd.clear();  
}

void _LCD::printInt(String message1, int message2) {
    if(millis() - lastUpdate > updateTimeMs){
        lcd.setCursor(0, 0);    
        lcd.clear();
        lcd.print(message1);
        lcd.setCursor(0, 1);
        lcd.print(message2);
    }
    lastUpdate = millis();    
}

void _LCD::printAlertSec(String alert, int seconds){
    print(alert);
    delay(seconds*1000);
    clear();
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

