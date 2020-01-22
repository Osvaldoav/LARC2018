#include <_Encoder.h>


//////////////////////// PINs DECLARATION ////////////////////////

//////////////////////// LOCAL VARIABLES /////////////////////////

// TODO:
void _Encoder::setupEncoder(){
    encoder30Cm = 3165L; // FIXME: Better it depends in the distance.
    stepsMechanism = 0;
    //0->off   1->on    
    encoderStateMechanism = 0;
    stepsFR = 0;
    //0->off   1->on 
    encoderStateFR = 0;    
    stepsBL = 0;
    //0->off   1->on     
    encoderStateBL = 0;    
}