#include <_ColorSensor.h>

/////////////////// PINs DECLARATION /////////////////////
const byte sensorOut = 43;
const byte S2 = 42;
const byte S3 = 41;
const byte S1 = 40;
const byte S0 = 39;
////////////////////////// STRUCTS /////////////////////
struct color{
  String nombre;
  double red;
  double green;
  double blue;
};
////////////////////////// LOCAL VARIABLES  /////////////////////////
int counter=0;
double r = 0, g = 0, b = 0;
const int num_col = 5;
int range = 10;
int color_position;     //  0        1       2       3        4
String color_names[5] = {"rojo", "azul", "verde", "negro", "blanco"};
bool switchColor=false; 
int initialColors[3];
bool colorRedDetected=false, colorGreenDetected=false, colorBlackDetected=false;
color color_position_arr[num_col];

// TODO:
void _ColorSensor::setupColorSensor(){
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW);   
    //  colorCalibration(1);
    //  hardCodedCalibration();
}

// TODO:
void _ColorSensor::regulateColorSignal(float &colorSignal){
    if (colorSignal > 255)
      colorSignal = 255;
    else if (colorSignal < 0)
      colorSignal = 0;
}

// TODO:
void _ColorSensor::colorKalmanFilter(ColorKalman &color){
    color.Pc = color.P + color.varProcess;
    color.G = color.Pc/(color.Pc + color.varSensor);  //Kalman gain
    color.P = (1-color.G)*color.Pc;
    color.Xp = color.kalmanColor;
    color.Zp = color.Xp;
    color.kalmanColor = color.G*(color.rawColor-color.Zp)+color.Xp;  //Estimates new filtered input
}

// TODO:
void _ColorSensor::readColor(){
    delay(10);
    redColor.rawColor = pulseIn(sensorOut, LOW, 25000000);    //Reads output frecuency, if greater than (25000000), returns 0
    regulateColorSignal(redColor.rawColor);
    colorKalmanFilter(redColor);
    
    digitalWrite(S2,HIGH);  // GREEN (G)
    digitalWrite(S3,HIGH);
    delay(10);
    greenColor.rawColor = pulseIn(sensorOut, LOW, 25000000);    //Reads output frecuency, if greater than (25000000), returns 0
    regulateColorSignal(greenColor.rawColor);
    colorKalmanFilter(greenColor);
  
    digitalWrite(S2,LOW);  // BLUE (B)
    digitalWrite(S3,HIGH);
    delay(10);
    blueColor.rawColor = pulseIn(sensorOut, LOW, 25000000);   //Reads output frecuency, if greater than (25000000), returns 0
    regulateColorSignal(blueColor.rawColor);
    colorKalmanFilter(blueColor);
}

// TODO:
void _ColorSensor::hardCodedCalibration(){
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

// TODO:
// 0 = AT (red, blue, green, black)
// 1 = LARC_fistChallenge (blue)
// 2 = LARC_secondChallenge (red, blue, green)
void _ColorSensor::colorCalibration(int challenge){
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
      // digitalWrite(ledRed, HIGH);
      delay(3000);
      // digitalWrite(ledRed, LOW);
      Serial.println("La calibracion del color " + color_names[i] + " ha comenzado.");
      // digitalWrite(ledGreen, HIGH);
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
      // digitalWrite(ledGreen, LOW);
      colorOptions.pop();
    }
}

// TODO:
//int currentColor(double r, double g, double b){
int _ColorSensor::currentColor(){
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

// TODO:
void _ColorSensor::updateColors(int posicion){
    if(posicion == 3) colorBlackDetected=true; 
    else if(posicion == 0) colorRedDetected = true;
    else if(posicion == 2) colorGreenDetected = true; 
}

// TODO:
void _ColorSensor::showSelectedLed(int color){
    if(color == 0){
      // digitalWrite(ledRed, HIGH);
      // digitalWrite(ledGreen, LOW);
      // digitalWrite(ledBlue, LOW);  
    }
    else if(color == 1){
      // digitalWrite(ledRed, LOW);
      // digitalWrite(ledGreen, LOW);
      // digitalWrite(ledBlue, HIGH);  
    }
    else if(color == 2){
      // digitalWrite(ledRed, LOW);
      // digitalWrite(ledGreen, HIGH);
      // digitalWrite(ledBlue, LOW);  
    }
}

// TODO:
void _ColorSensor::stopColor(int color){
    long int temp = millis();  
    // brake();
    while(millis() < temp + 600){
      showSelectedLed(color);
    }   
}

// TODO:
bool _ColorSensor::colorDecision(){
    return (currentColor() != -1); 
}

