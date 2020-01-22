char dataString[50] = {0};
int a =0; 
char c;

void setup() {
Serial.begin(9600);              //Starting serial communication

  char c = 'R';
  if(c < 83)
    Serial.println("Jala");
  else
    Serial.println("No jala");
}
  
void loop() {
//	if(Serial.available() > 0){
//		c = Serial.read();
//		c += 1;
//		Serial.write(c);
//	}
}
