void encoderStepFR(){
    if (logic->traductor->movements->encoder->encoderStateFR == 1)
      logic->traductor->movements->encoder->stepsFR++;
}

void encoderStepBL(){
    if (logic->traductor->movements->encoder->encoderStateBL == 1)
      logic->traductor->movements->encoder->stepsBL++;
}

void encoderStepMechanism(){
    if (logic->traductor->movements->encoder->encoderStateMechanism == 1)
      logic->traductor->movements->encoder->stepsMechanism++;
}
