void regulateColorSignal(float &colorSignal){
  if (colorSignal > 255)
    colorSignal = 255;
  else if (colorSignal < 0)
    colorSignal = 0;
}

void colorKalmanFilter(ColorKalman &color){
  color.Pc = color.P + color.varProcess;
  color.G = color.Pc/(color.Pc + color.varSensor);  //Kalman gain
  color.P = (1-color.G)*color.Pc;
  color.Xp = color.kalmanColor;
  color.Zp = color.Xp;
  color.kalmanColor = color.G*(color.rawColor-color.Zp)+color.Xp;  //Estimates new filtered input
}

void readColor(){
  digitalWrite(S2,LOW);   // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S3,LOW);
  delay(10);
  redColor.rawColor = pulseIn(sensorOut, LOW, 25000000);    //Reads output frecuency, if greater than (25000000), returns 0
  regulateColorSignal(redColor.rawColor);
//  Serial.print(redColor.rawColor);
//  Serial.print(" ");
  colorKalmanFilter(redColor);
//  Serial.println(redColor.kalmanColor);

//  Serial.print("\t\t");   
//  Serial.print(" "); 
  
  digitalWrite(S2,HIGH);  // GREEN (G)
  digitalWrite(S3,HIGH);
  delay(10);
  greenColor.rawColor = pulseIn(sensorOut, LOW, 25000000);    //Reads output frecuency, if greater than (25000000), returns 0
  regulateColorSignal(greenColor.rawColor);
//  Serial.print(greenColor.rawColor);
//  Serial.print(" ");    
  colorKalmanFilter(greenColor);
//  Serial.print(greenColor.kalmanColor);

//  Serial.print("\t\t");
//  Serial.print(" "); 
 
  digitalWrite(S2,LOW);  // BLUE (B)
  digitalWrite(S3,HIGH);
  delay(10);
  blueColor.rawColor = pulseIn(sensorOut, LOW, 25000000);   //Reads output frecuency, if greater than (25000000), returns 0
  regulateColorSignal(blueColor.rawColor);
//  Serial.print(blueColor.rawColor);   
//  Serial.print(" ");    
  colorKalmanFilter(blueColor);
//  Serial.println(blueColor.kalmanColor);  
}

void hardCodedCalibration(){
    color_position_arr[0].red = 23.74;
    color_position_arr[0].green = 97.43;
    color_position_arr[0].blue = 69.76;

    color_position_arr[1].red = 78.68;
    color_position_arr[1].green = 52.45;
    color_position_arr[1].blue = 25.55;

    color_position_arr[2].red = 43.86;
    color_position_arr[2].green = 19.30;
    color_position_arr[2].blue = 39.60;

    color_position_arr[3].red = 134.60;
    color_position_arr[3].green = 160.37;
    color_position_arr[3].blue = 124.38;
    
    color_position_arr[4].red = 16.81;
    color_position_arr[4].green = 18.94;
    color_position_arr[4].blue = 15.28; 
}

// 0 = AT (red, blue, green, black)
// 1 = LARC_fistChallenge (blue)
// 2 = LARC_secondChallenge (red, blue, green)
void colorCalibration(int challenge){
  StackArray <int> colorOptions;
  if(challenge == 0){
     colorOptions.push(4); colorOptions.push(3); colorOptions.push(2); colorOptions.push(1); colorOptions.push(0);
  }
  else if(challenge == 1){
    colorOptions.push(1);
  }
  while(!colorOptions.isEmpty()){
    int i = colorOptions.peek();
    Serial.println("La calibracion del color " + color_names[i] + " iniciara en 5 segundos.");        
    digitalWrite(ledRed, HIGH);
    delay(3000);
    digitalWrite(ledRed, LOW);
    Serial.println("La calibracion del color " + color_names[i] + " ha comenzado.");
    digitalWrite(ledGreen, HIGH);
    delay(800);
    double avr_red = 0, avr_green = 0, avr_blue = 0;
    color_position_arr[i].nombre = color_names[i];
    for(int cont = 0; cont < 500; cont++){
      readColor();
      avr_red += redColor.kalmanColor; avr_green += greenColor.kalmanColor; avr_blue += blueColor.kalmanColor;
    }
    color_position_arr[i].red = avr_red / 1000.0; color_position_arr[i].green = avr_green / 1000.0; color_position_arr[i].blue = avr_blue / 1000.0;
    Serial.println("Color = " + color_position_arr[i].nombre + " red = " + color_position_arr[i].red + " green = " + color_position_arr[i].green + " blue = " + color_position_arr[i].blue);
    Serial.println(color_position_arr[i].nombre + " ha sido calibrado");
    digitalWrite(ledGreen, LOW);
    colorOptions.pop();
  }
}

//int currentColor(double r, double g, double b){
int currentColor(){
  readColor();
//  Serial.print(r);
//  Serial.print("\t");
//  Serial.print(g);
//  Serial.print("\t");
//  Serial.println(b);
  int posicion = -1;
  for(int i = 0; i < num_col; i++){    
    if(redColor.kalmanColor <= color_position_arr[i].red + range && redColor.kalmanColor >= color_position_arr[i].red - range){
      if(greenColor.kalmanColor <= color_position_arr[i].green + range && greenColor.kalmanColor >= color_position_arr[i].green - range){
        if(blueColor.kalmanColor <= color_position_arr[i].blue + range && blueColor.kalmanColor >= color_position_arr[i].blue - range){         
          posicion = i;
          return posicion;
        }
      }
    }
  } 
  return posicion;
}

void updateColors(int posicion){
  if(posicion == 3) colorBlackDetected=true; 
  else if(posicion == 0) colorRedDetected = true;
  else if(posicion == 2) colorGreenDetected = true; 
}

void showSelectedLed(int color){
  if(color == 0){
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, LOW);  
  }
  else if(color == 1){
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, HIGH);  
  }
  else if(color == 2){
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, LOW);  
  }
}

void stopColor(int color){
  long int temp = millis();  
  brake();
  while(millis() < temp + 600){
    showSelectedLed(color);
  }   
  turnOffLeds();  
}

bool colorDecision(){
    return (currentColor() != -1); 
}

