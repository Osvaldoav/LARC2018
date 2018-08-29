void encoderStep(){
    //If we are moving forward
    if (movements->encoder->encoderState == 1)
      movements->encoder->steps++;
    //else if we are moving backwards
    else if (movements->encoder->encoderState == 2)
      movements->encoder->steps--;
}
