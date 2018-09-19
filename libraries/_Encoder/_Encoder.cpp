#include <_Encoder.h>


//////////////////////// PINs DECLARATION ////////////////////////

//////////////////////// LOCAL VARIABLES /////////////////////////

// TODO:
void _Encoder::setupEncoder(){
    //Steps counted by the encoder
    steps = 0;
    //State of the encoder
    //0->Stop   1->Forward    2->Backwards
    encoderState = 0;
    //Const for encoder. Perfect at 60cm, 30cm fail by -1.5cm, 100cm by 2cm, 150cm by 3cm
    encoder30Cm = 3950L; // FIXME: Better it depends in the distance.
}