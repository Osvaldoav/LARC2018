#include <_Serial.h>

void _Serial::send(char c){
    Serial.write(c);
}

char _Serial::read(){
    while(Serial.available() <= 0);
    return Serial.read();
}