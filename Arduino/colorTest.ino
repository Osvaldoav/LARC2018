//#include <_Logic.h>
//
//#define S0 36
//#define S1 37
//#define S2 33
//#define S3 34
//#define sensorOut 35
//
//int frequency = 0;
//
//void setup(){
//  pinMode(S0, OUTPUT);
//  pinMode(S1, OUTPUT);
//  pinMode(S2, OUTPUT);
//  pinMode(S3, OUTPUT);
//  pinMode(sensorOut, INPUT);
//  
//  // Setting frequency-scaling to 20%
//  digitalWrite(S0,LOW);
//  digitalWrite(S1,HIGH);
//  
//  Serial.begin(9600);
//}
//
//void loop(){
//  // Setting red filtered photodiodes to be read
//  digitalWrite(S2,LOW);
//  digitalWrite(S3,LOW);
//  // Reading the output frequency
//  frequency = pulseIn(sensorOut, LOW);
//  // Printing the value on the serial monitor
//  Serial.print("R= ");//printing name
//  Serial.print(frequency);//printing RED color frequency
//  Serial.print("  ");
//  delay(100);
//  // Setting Green filtered photodiodes to be read
//  digitalWrite(S2,HIGH);
//  digitalWrite(S3,HIGH);
//  // Reading the output frequency
//  frequency = pulseIn(sensorOut, LOW);
//  // Printing the value on the serial monitor
//  Serial.print("G= ");//printing name
//  Serial.print(frequency);//printing RED color frequency
//  Serial.print("  ");
//  delay(100);
//  // Setting Blue filtered photodiodes to be read
//  digitalWrite(S2,LOW);
//  digitalWrite(S3,HIGH);
//  // Reading the output frequency
//  frequency = pulseIn(sensorOut, LOW);
//  // Printing the value on the serial monitor
//  Serial.print("B= ");//printing name
//  Serial.print(frequency);//printing RED color frequency
//  Serial.println("  ");
//  delay(100);
//}
