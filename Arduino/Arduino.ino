 //////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LARC 2018 //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
#include <_Logic.h>
#include <Wire.h>
#include <_Serial.h>
_Logic *logic = new _Logic;

const byte pinEncoderFR = 18;
const byte pinEncoderBL = 19;
const byte pinEncoderMechanism = 2;

/////////////////////////////////////////////SETUP////////////////////////////////////////////////
void setup() {
    Wire.begin();
    Serial.begin(9600);    
    pinMode(30, INPUT);
    pinMode(22, OUTPUT);          

    digitalWrite(22, HIGH);
    while(digitalRead(30) == HIGH);//limitswitch
    delay(700);
    digitalWrite(22, LOW);      

    logic->traductor->movements->pid->setupLibraryPID(); 
    logic->traductor->movements->motors->setupMotors();
//    logic->traductor->movements->colorSensor->setupColorSensor();
    logic->traductor->movements->bno055->setupBNO055();
    logic->traductor->movements->timeFlight->setupTimeFlight();
    logic->traductor->movements->tcrt5000->setupTCRT5000();
    logic->traductor->movements->encoder->setupEncoder();
    logic->traductor->movements->servo->setupServo();
    pinMode(pinEncoderFR, INPUT_PULLUP);
    pinMode(pinEncoderBL, INPUT_PULLUP);
    pinMode(pinEncoderMechanism, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(pinEncoderFR), encoderStepFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinEncoderBL), encoderStepBL, CHANGE);      
    attachInterrupt(digitalPinToInterrupt(pinEncoderMechanism), encoderStepMechanism, CHANGE);    
    
    _Serial::send('Z');
    _Serial::read();     
    logic->traductor->movements->setInitialSetpoint();
    logic->traductor->movements->initMechanism();
    logic->initCommunication(); 

    //CALIBRATION   
//      logic->traductor->movements->timeFlight->calibTimeFlights(8);    
//      delay(5000);  
}

void testMovements(){ 
//    logic->traductor->movements->encoder->encoderStateFR = 1;
//    logic->traductor->movements->encoder->encoderStateBL = 1;
//    digitalWrite(3, HIGH);
//    digitalWrite(8, HIGH);
//    Serial.print(logic->traductor->movements->encoder->stepsFR);
//    Serial.print(" ");
//    Serial.println(logic->traductor->movements->encoder->stepsBL); 
//    logic->traductor->movements->crazyMode=true;    
    logic->traductor->movements->movePID(false, '6');
//    logic->traductor->movements->spinPID(true, -90);
//    delay(5000); 
//    logic->traductor->movements->movePID_nSec(1.5, false, '1');
//    delay(2000);
//    logic->traductor->movements->spinPID(false, -90);
//    logic->traductor->movements->motors->brake();
//    delay(2000);
//    logic->traductor->movements->movePID_nCM(37, false, '4');
//    delay(3000);
//    logic->traductor->movements->movePID_nCM(37, false, '6');
//    delay(3000);    
}
void aligningTofTest(){
//    logic->traductor->movements->movePID_alignToPickContainer(2);
//    while(1);
    logic->traductor->movements->align_tof();
//    Serial.print("///////////////////////////////////////////////////////////////////////");
//    logic->traductor->movements->motors->brake();
//    logic->traductor->movements->lcd->onLed('b');       
//    delay(6000);
//    logic->traductor->movements->lcd->offLed('b');  
}
void tof_vs_sharp(){
//    logic->traductor->movements->updateSensors(0,0,0,1,0,0);
//    Serial.print(logic->traductor->movements->timeFlight->timeFlightLeft.kalmanDistance);
//    Serial.print(" ");
//    Serial.print(logic->traductor->movements->timeFlight->timeFlightRight.kalmanDistance);
//    Serial.print(" ");
//    Serial.println((logic->traductor->movements->timeFlight->timeFlightLeft.kalmanDistance - logic->traductor->movements->timeFlight->timeFlightRight.kalmanDistance)*10);  
//
//    Serial.print("\t\t");
//
//    logic->traductor->movements->updateSensors(0,0,1,0,0,0);
//    Serial.print(logic->traductor->movements->sharp->sharpLeft.kalmanDistance);
//    Serial.print(" ");
//    Serial.print(logic->traductor->movements->sharp->sharpRight.kalmanDistance);
//    Serial.println(" ");
//    Serial.println((logic->traductor->movements->sharp->sharpLeft.kalmanDistance - logic->traductor->movements->sharp->sharpRight.kalmanDistance)*10);   
  
//     logic->traductor->movements->timeFlight->timeFlight_RawKalman(logic->traductor->movements->timeFlight->timeFlightLeft);
    // logic->traductor->movements->sharp->sharp_RawKalman(movements->sharp->sharpBL);

    double left=0, right=0, difference;
    int n=500;
    for(int i=0; i<n; i++){
        if(i>22 && i<478){
            left += logic->traductor->movements->timeFlight->getRawDistance(true);
            right += logic->traductor->movements->timeFlight->getRawDistance(false);
            delay(20);
        }
    }
    left/=n;
    right/=n;
    Serial.print(left);
    Serial.print(" ");
    Serial.println(right);       
}

void readTCRT5000(){
     logic->traductor->movements->updateSensors(0,0,0,0,1,1);
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMechaLeft.kalmanDistance);
     Serial.print(" ");
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMechaRight.kalmanDistance);     
     Serial.print(" ");
     Serial.print(logic->traductor->movements->tcrt5000->tcrtSharpLeft.kalmanDistance);
     Serial.print(" ");     
     Serial.print(logic->traductor->movements->tcrt5000->tcrtSharpRight.kalmanDistance);
     
     Serial.print("\t\t");     
     
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMidFrontLeft.kalmanDistance);
     Serial.print(" ");
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMidFrontRight.kalmanDistance);
     Serial.print(" ");     
     Serial.print(logic->traductor->movements->tcrt5000->tcrtMidDownLeft.kalmanDistance);
     Serial.print(" ");
     Serial.println(logic->traductor->movements->tcrt5000->tcrtMidDownRight.kalmanDistance);               
//    logic->traductor->movements->tcrt5000->tcrt5000_RawKalman(logic->traductor->movements->tcrt5000->tcrtFrontRight);  
}

void aligningTcrtTest(){
//  logic->traductor->movements->larc_moveAndAlignToShip(false);
  delay(4000);
  while(1);
//      logic->traductor->movements->larc_alignBetweenVerticalBlackLine(true, '8');
}

void larc(){
//  logic->traductor->moveAtrasHorizontal();
//  logic->traductor->girar(-90);
//  logic->traductor->movements->movePID_nCM(4, false, '8');
//  logic->traductor->movements->larc_moveUntilBlackLine(false, '6', true, true, true, false);

  logic->traductor->movements->movePID_nCM(5, false, '6');
  logic->traductor->moveToShip(true);
  logic->traductor->alignShip();
  logic->traductor->movements->movePID_nCM(4.5, true, '4');
  logic->traductor->moveToShip(false);  
//  logic->traductor->alignFirstShip();
//  logic->traductor->centerContainer();

//  logic->traductor->movements->moveToTrain();
  
//  do{
//      movements->movePID(false, '8');
//      movements->updateSensors(0,0,0,0,1,1);        
//      if(tcrt && movements->tcrt5000->tcrtSharpLeft.kalmanDistance>movements->BLACKLINE_TRIGGER)
//          break;  
//      else if(!tcrt && movements->tcrt5000->tcrtSharpRight.kalmanDistance>movements->BLACKLINE_TRIGGER)
//          break;              
//  } while(1);    
//  movements->motors->brake();
  while(1); 
}
void colorSensor(){
  logic->traductor->movements->colorSensor->readColor();
}
void logicTest(){  
//    logic->traductor->movements->lcd->print("STACK", "HOLA");
//    logic->traductor->mecanismo(3, 4);  
//    logic->shipToStack();                            
//    logic->stackToShip();
//    logic->shipToStack();
//    logic->stackToShip();
//    logic->shipToStack();
//    logic->stackToShip();
//    logic->shipToStack();
//    logic->stackToShip();
//    logic->shipToStack();
//    logic->stackToShip();
//    logic->traductor->dropContainer();    
//    logic->traductor->mecanismo(1,3);
//    logic->shipToStack('6');
//    logic->stackToShip();  
//    logic->traductor->mecanismo(3, 2); // nivela el mecanismo al nivel adecuado       
//    logic->traductor->dropContainer();  
    logic->backUsingLimitSwitch(850); // nivela el mecanismo al nivel adecuado      
    while(1);
}
void mechanism(){
//    logic->traductor->movements->servo->pickContainer();
//    delay(1000);
//    logic->traductor->movements->servo->dropContainer();
//    delay(1000);
////    logic->traductor->movements->encoder->encoderStateMechanism=1;
//    Serial.println(logic->traductor->movements->encoder->stepsMechanism);
//    logic->traductor->movements->motors->moveMechanism(false);
//    Serial.println("DOWN");
//    logic->traductor->movements->initMechanism();
//    delay(2000);
//    logic->traductor->movements->motors->moveMechanism(false);
//    digitalWrite(10,HIGH);
//    digitalWrite(11,LOW);
//    logic->traductor->movements->motors->stopMechanism();
//    delay(2000);
//    logic->traductor->mecanismo(5,1);
//    delay(1000);    
    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->movements->moveMechanismForAligning(true);
//    logic->traductor->waitForMechanism();
    logic->traductor->mecanismo(5,1);    
//    delay(5000);
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(5,4);    
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(4,3);
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(3,2);
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(2,1); 
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(1,2);
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(2,3);
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(3,4);
//    while(digitalRead(30) == HIGH);//limitswitch
//    logic->traductor->mecanismo(4,5);  
    while(1);  
}
void alignLine(){
    logic->traductor->movements->alignLine();
//    while(1);
}

void loop(){
//  logic->traductor->dropContainer();
//  logic->traductor->grabContainer();
//  aligningTofTest();
//  tof_vs_sharp();
//  testMovements(); 
//  testSteps();


//    logic->traductor->movements->updateSensors(1,0,0,0,0,0);
//    Serial.println(logic->traductor->movements->bno055->rawInput);

//  readTCRT5000();
//  aligningTcrtTest();
//  larc(); 
//  logicTest();
//  Serial.println(digitalRead(35));
//  colorSensor();
//  logic->traductor->gotoFirst();
//  mechanism();

//  Serial.println(digitalRead(35));

//  logic->backUsingLimitSwitch();

  logic->stackToShip();
  logic->shipToStack();
//  alignLine();

//  while(1);  
}
