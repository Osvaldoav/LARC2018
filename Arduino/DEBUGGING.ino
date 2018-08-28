// void myMillisDelay(double time){
//   double startTime = millis();
//   while (millis() < startTime + time){ 
    
//   }   
// }

// void ledsPID(){
//   if(abs(Setpoint) != 180){
//     (frontLeftOutput > 5)? digitalWrite(ledGreen, HIGH): digitalWrite(ledGreen, LOW); 
//     (frontRightOutput > 5)? digitalWrite(ledRed, HIGH): digitalWrite(ledRed, LOW);
//   }
// }

// void blinkingLEDS(){
//   for(int i=0; i<2; i++){
//     digitalWrite(ledGreen, HIGH); 
//     delay(20);
//     digitalWrite(ledGreen, LOW);
//     delay(20);
//   }
// }

// void turnOnRedLED(){
//   digitalWrite(ledGreen, LOW);
//   digitalWrite(ledRed, HIGH);  
//   digitalWrite(ledBlue, LOW); 
//   delay(3000);
//   turnOffLeds();
// }
// void turnOnGreenLED(){
//   digitalWrite(ledGreen, HIGH);
//   digitalWrite(ledRed, LOW);  
//   digitalWrite(ledBlue, LOW); 
//   delay(3000);
//   turnOffLeds();
// }
// void turnOnBlueLED(){
//   digitalWrite(ledGreen, LOW);
//   digitalWrite(ledRed, LOW);  
//   digitalWrite(ledBlue, HIGH); 
//   delay(3000);
//   turnOffLeds();
// }
// void turnOnPurpleLED(){
//   digitalWrite(ledGreen, LOW);
//   digitalWrite(ledRed, HIGH);  
//   digitalWrite(ledBlue, HIGH); 
//   delay(3000);
//   turnOffLeds();
// }
// void turnOnLeds(){
//   digitalWrite(ledGreen, HIGH);
//   digitalWrite(ledRed, HIGH);  
//   digitalWrite(ledBlue, LOW);
// }
// void turnOffLeds(){
//   digitalWrite(ledGreen, LOW);
//   digitalWrite(ledRed, LOW);
//   digitalWrite(ledBlue, LOW);
// }

// //void printBNO(sensors_event_t event){
// void printBNO(){
//   Serial.print("X: ");
//   Serial.print(event.orientation.x, 5);
//   Serial.print("\tY: ");
//   Serial.print(event.orientation.y, 5);
//   Serial.print("\t\tZ: ");
//   Serial.println(event.orientation.z, 5);
// }