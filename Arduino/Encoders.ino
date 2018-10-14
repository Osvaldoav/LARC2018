void encoderStep(){
    if (logic->traductor->movements->encoder->encoderState == 1)
      logic->traductor->movements->encoder->steps++;
}
