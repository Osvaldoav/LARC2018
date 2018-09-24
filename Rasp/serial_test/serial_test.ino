char dataString[50] = {0};
int a =0; 
char c;

void setup() {
Serial.begin(9600);              //Starting serial communication
}
  
void loop() {
	if(Serial.available() > 0){
		c = Serial.read();
		c += 1;
		Serial.write(c);
	}
}
