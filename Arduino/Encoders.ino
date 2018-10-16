void encoderStep(){
    if (logic->traductor->movements->encoder->encoderState == 1)
      logic->traductor->movements->encoder->steps++;
}

void encoderStepMechanism(){
    if (logic->traductor->movements->encoder->encoderStateMechanism == 1)
      logic->traductor->movements->encoder->stepsMechanism++;
}
