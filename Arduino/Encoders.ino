void encoderStep(){
    if (movements->encoder->encoderState == 1)
      movements->encoder->steps++;
}
