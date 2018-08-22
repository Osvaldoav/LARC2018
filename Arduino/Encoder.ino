//Interrup funcitons, add or substract to steps
void encoderStep(){
  //If we are moving forward
  if (encoderState == 1)
    steps++;
  //else if we are moving backwards
  else if (encoderState == 2)
    steps--;
}