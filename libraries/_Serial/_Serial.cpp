#include <_Serial.h>
#include <_LCD.h>

void _Serial::send(char c){
    Serial.write(c);
}

char _Serial::read(){
    while(Serial.available() <= 0);
    return Serial.read();
}

char _Serial::clean(){
    char c;
    _LCD *lcd = new _LCD;    
    while(Serial.available() <= 0);

    int counter = 0;

    while(Serial.available() != 0){
        c = Serial.read();
        counter++;
        delay(3000);
    }
    lcd -> printInt("counter", counter);
    delay(3000);
    return c;
}