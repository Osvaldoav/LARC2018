#include <_TCRT5000.h>

void _TCRT5000::setupTCRT5000(){

}

void _TCRT5000::readTCRT5000(){
    tcrtUpDistance = analogRead(tcrtUp);
    tcrtDownDistance = analogRead(tcrtDown);    
}