#include <_LCD.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
uint8_t updateTimeMs = 50;
long lastUpdate = 0;

///////////////////// Local Variables ////////////////////////
const byte ledPlaca = 22;
// TODO:
void _LCD::setupLCD(){
    pinMode(ledPlaca, OUTPUT);
	Serial.println("LCD initializing...");
	lcd.begin();
	lcd.backlight();	
    Serial.println("LCD initialized");
	// lcd.backlight();    
}
// TODO:
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
// TODO:
void _LCD::clear() {
    lcd.clear();  
}
// TODO:
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
// TODO:
void _LCD::printAlertSec(String alert, int seconds){
    print(alert);
    delay(seconds*1000);
    clear();
}
// TODO:
void _LCD::onLed(){
    digitalWrite(ledPlaca, HIGH);
}
// TODO:
void _LCD::offLed(){
    digitalWrite(ledPlaca, LOW);
}

