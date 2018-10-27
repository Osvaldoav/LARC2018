#include <_Serial.h>

void _Serial::send(char c){
    Serial.write(c);
}

char _Serial::read(){
    while(Serial.available() <= 0);
    return Serial.read();
}

char _Serial::clean(){
    char c;
    while(Serial.available() <= 0);

    counter = 0;

    while(Serial.available() != 0){
        c = Serial.read();
        counter++;
        delay(500);
    }
    return c;
}