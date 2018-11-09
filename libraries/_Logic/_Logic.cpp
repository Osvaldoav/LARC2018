#include <_Logic.h>

_Logic::_Logic(){
    traductor = new _Traductor;
    firstRed = -1;
    stacks = new int[8];
    for(int i = 0; i < 8; i++)
        stacks[i] = 4;
    blue_boxes = 0;
    green_boxes = 0;
    lastStack = 0;
    lastLevel = currentLevel = 4;
    lastColor = 'B';
    pinMode(limitSwitchPin, INPUT); 
    //needToAlign=false;
}

void _Logic::initCommunication(){
    // c_serial = _Serial::read();
    // _Serial::send('1');
    gotoFirst();
    _Serial::send('2');
    // delay(1000);
    // c_serial = _Serial::read();

    c_serial = _Serial::clean();
    // traductor->LcdPrint("c_serial", c_serial);
    // delay(3000);
    pickFirst(c_serial);
}

char _Logic::handleRed(){
    // traductor->LcdPrint("handleRed", "entro");
    // delay(3000);
    if(firstRed < 0)
        firstRed = lastStack < 4 ? 0 : 1;
    else
        firstRed = firstRed == 0 ? 1 : 0;
 
    return firstRed == 0 ? 'R' : 'S';
}

char _Logic::verifyColor(char c){
    char color = c > 98 ? 'R' : c > 65 ? 'G' : 'B';
    lastStack = color == 'R' ? c - 99 : color == 'G' ? c - 66 : c - 51;
    // verify and modify 'color' with sensor TCS3200
    
    // traductor->LcdPrint("lastStack", lastStack);
    // delay(3000);

    lastColor = color;
    stacks[lastStack]--;
    if(color == 'G')
        green_boxes++;
    else if(color == 'B')
        blue_boxes++;
    return color == 'R' ? handleRed() : color;
}

void _Logic::checkLimit(double steps){
    traductor->movements->encoder->encoderStateMechanism = 1;  
    traductor->movements->untilStepsMechanism = steps;
    traductor->movements->encoder->stepsMechanism = 0;
    do{
        traductor->movements->motors->moveMechanism(false); 
        limitIsPressed = digitalRead(limitSwitchPin);
    } while(traductor->movements->encoder->stepsMechanism<traductor->movements->untilStepsMechanism && !limitIsPressed);
    traductor->movements->motors->stopMechanism();
}

void _Logic::backUsingLimitSwitch(double steps){
    do{
        checkLimit(steps);
        if(limitIsPressed){
            traductor->moveMechanismForAligning(true, traductor->movements->encoder->stepsMechanism+300);
            traductor->waitForMechanism();                        
            traductor->movements->movePID_nCM(1, true, '4');
            traductor->moveMechanismForAligning(false, 300);
            traductor->waitForMechanism(); 
        } 
    }while(limitIsPressed); 
    traductor->moveMechanismForAligning(true, traductor->movements->encoder->stepsMechanism+300);
    traductor->waitForMechanism();    
    traductor->movements->movePID_nCM(0.5, true, '6');
    traductor->moveMechanismForAligning(false, 300);
    traductor->waitForMechanism();     
    checkLimit(steps);
    if(limitIsPressed){
        traductor->moveMechanismForAligning(true, traductor->movements->encoder->stepsMechanism+300);
        traductor->waitForMechanism();                      
        traductor->movements->movePID_nCM(1.2, true, '4');
        traductor->moveMechanismForAligning(false, 300);
        traductor->waitForMechanism();        
    }         
    else{
        traductor->moveMechanismForAligning(true, traductor->movements->encoder->stepsMechanism);
        traductor->waitForMechanism();
    }    
}

char _Logic::grabContainer(char c, bool isFirst){
    c = verifyColor(c);
    char ori = lastStack/2*2 != lastStack ? '2' : '8';  // '2'  :  '8'  

    traductor->centerContainer(ori); 

    backUsingLimitSwitch(2000);

    traductor->moveMechanismForAligning(false, traductor->stepsHigh);
    traductor->waitForMechanism();  

    traductor->movements->moveMechanism(5,4);
    traductor->grabContainer();

    traductor->moveMechanismForAligning(true, traductor->stepsHigh);
    traductor->waitForMechanism(); 

    _Serial::send(c);
}

void _Logic::gotoFirst(){
    traductor->gotoFirst();
}

void _Logic::gotoSecond(){
    traductor->gotoSecond();
    _Serial::send('1');
    c_serial = _Serial::clean();
    // traductor->LcdPrint("c_serial", c_serial);
    // delay(3000);    
    pickFirst(c_serial);
    currentLevel = 4;
}

void _Logic::pickFirst(char c){
    char color = c > 98 ? 'R' : c > 65 ? 'G' : 'B';
    lastStack = color == 'R' ? c - 99 : color == 'G' ? c - 66 : c - 51;
    // blink(lastStack);
    // traductor->LcdPrint("PF stack", lastStack);
    // delay(5000);
    traductor->pickFirst(lastStack);
    grabContainer(c, true);
}

void _Logic::blink(int times){
    for(int i = 0; i < times; i++){
        digitalWrite(22, HIGH);
        delay(500);
        digitalWrite(22, LOW);
        delay(500);
    }
}

void _Logic::stackToShip(){
    bool A = lastStack < 2 || (lastStack > 3 && lastStack < 6);
    bool B = lastColor != 'R';
    bool blueContainer = (lastColor == 'B');
    bool front;
    int dir;

    lastLevel = (lastStack+1)/2*2;
    if((lastColor != 'R' && lastLevel%4 != 0)||(lastColor == 'R' && lastLevel%4 == 0)) 
        lastLevel = stacks[lastStack/2*2] > stacks[lastStack/2*2+1] ? stacks[lastStack/2*2] : stacks[lastStack/2*2+1];
    else
        lastLevel = stacks[lastStack];
    lastLevel++;

    traductor->mecanismo(currentLevel, lastLevel);   // eleva el stack para no chocar con los demas
    traductor->horizontalLine(A == B); // Avanza de frente o de reversa hasta linea horizontal

    currentLevel = lastColor == 'R' ? 3 : lastColor == 'G' ? green_boxes%5 : blue_boxes%5;
    if((lastColor == 'B' && blue_boxes%5 == 0) || (lastColor == 'G' && green_boxes%5 == 0))
        currentLevel = 5;
    traductor->updateMechanismMovement(lastLevel, currentLevel, false);
 
    if(B){
        if ((lastColor == 'B' && blue_boxes < 6) || (lastColor == 'G' && green_boxes < 6)){
            dir = lastStack < 4 ? 2 : 1;
            dir *= lastStack > 5 - ((dir - 1)* 4) ? -1 : 1;
            dir = lastColor == 'G' ? abs(abs(dir) - 3) * -(dir/abs(dir)) : dir;
        }else if(lastStack < 2 || lastStack > 5){
            dir = 1;
        }else
            dir = -1;

        // 2 son dos lineas, 1 es una. negativo es derecha, positivo izquierda
        traductor->throughtHorizontal(dir); // Avanza por la linea horizontal a la izquierda o derecha cuando 'B' o 'G'
    }else{
        dir = lastStack < 4 ? 1 : 2;
        dir += firstRed == 1 ? 2 : 0;
        dir *= ((lastStack/2 + 1)%2 == 0) != firstRed ? 1 : -1;
        front = (lastStack/2*2)%4 == 0; // True if facing SOUTH, False if facing NORTH
        
        // Checar foto whatsapp para ver 1,2,3,4
        traductor->throughtHorizontal2(dir, front); // Avanza por la linea horizontal a la izquierda o derecha cuando 'R'
    }
    
    if ((lastColor == 'B' && blue_boxes > 5) || (lastColor == 'G' && green_boxes > 5)){
        bool b = ((lastStack/2 + 1)%2 == 1);
        if(((lastStack/2 + 1)%2 == 1) == (lastColor == 'B')){
            b = !b;
            traductor->girar(180); // gira 180 grados
        }
        bool thirdStack = (lastColor == 'B' && blue_boxes > 10) || (lastColor == 'G' && green_boxes > 10);
        traductor->avanzar(b, thirdStack); // avanza hasta que llegue a la altura de los barcos
    }else{
        // traductor->LcdPrint("Ship Color:", lastColor);
        dir = ((lastStack/2 + 1)%2 == 0) != B ? -90 : 90;
        bool tcrt = dir == 90? true: false;
        traductor->backUntilBlackLineSharps(tcrt);
        traductor->girar(dir); // giros de 90, + derecha, - izquierda
        // if(lastColor=='G' && dir==-90)
        //     traductor->fixContainerSteps(true);
        // else if(lastColor=='B' && dir==-90);
        //     traductor->fixContainerSteps(false);        
    }  
    traductor->waitForMechanism();  
    traductor->moveMechanismForAligning(false, traductor->stepsHigh);
    traductor->waitForMechanism();     
    bool firstContainer = (currentLevel < 2);   
    if (B){              
        if(!firstContainer)
            traductor->moveMechanismForAligning(true, 350);
        traductor->movements->movePID_nCM(5, false, '6');
        traductor->moveToShip(true);
        traductor->waitForMechanism();             //make sure mechanism is already (1/8) up 
        traductor->alignShip();
        if(firstContainer){
            traductor->movements->movePID_nCM(4.5, true, '4');
            traductor->moveToShip(false); 
            if((blueContainer&&blue_boxes==1) || (!blueContainer&&green_boxes==1))
                traductor->alignFirstShip();   
        }
        else{
            traductor->movements->movePID_nCM(4.5, true, '4');
            traductor->moveToShip(false);      
            if(lastColor == 'B' && blue_boxes > 5)
                traductor->centerContainer('2');
            else if(lastColor == 'G' && green_boxes > 5)
                traductor->centerContainer('8');
            else
                traductor->centerContainer(' ');
        }
        delay(400); 
        traductor->movements->movePID_nCM(1.1, true, '6');               
    }
    else{
        // traductor->setTrainLevel(true);
        // traductor->alinearTren();
        traductor->moveToTrain((firstRed == 1));
        traductor->waitForMechanism();  
    }

    delay(700);
    if(!firstContainer)
        traductor->moveMechanismForAligning(false, 350); //move mechanism a little down (1/8) of a level (back to normal)        
    traductor->waitForMechanism();       
    traductor->dropContainer();
    if(!firstContainer)
        traductor->moveMechanismForAligning(true, 350); //move mechanism a little down (1/8) of a level (back to normal)        
    traductor->waitForMechanism();       
    traductor->movements->movePID_nCM(5, true, '4');
    if(!firstContainer)
        traductor->moveMechanismForAligning(false, 350); //move mechanism a little down (1/8) of a level (back to normal)        
    traductor->waitForMechanism();             //make sure mechanism is already (1/8) down (normal)
}

void _Logic::shipToStack(){
    char c = _Serial::read();
    if(c == 'S'){
        gotoSecond();
        return;
    }

    traductor->moveMechanismForAligning(true, traductor->stepsHigh);
    traductor->waitForMechanism(); 
    
    char color = c > 98 ? 'R' : c > 65 ? 'G' : 'B';
    int stack = color == 'R' ? c - 99 : color == 'G' ? c - 66 : c - 51;
    bool dir, tcrt;
    int lines, angle;

    lastLevel = (stack+1)/2*2;
    if((lastColor != 'R' && lastLevel%4 != 0)||(lastColor == 'R' && lastLevel%4 == 0)) 
        lastLevel = stacks[stack/2*2] > stacks[stack/2*2+1] ? stacks[stack/2*2] : stacks[stack/2*2+1];
    else
        lastLevel = stacks[stack];

    lastLevel++;
    
    if((lastColor == 'B' && blue_boxes > 5) || (lastColor == 'G' && green_boxes > 5)){
        traductor->moveAtras(); // Se mueve poquito hacia atras
        traductor->updateMechanismMovement(currentLevel, lastLevel, false); // eleva el stack para no chocar con los demas
        if((lastColor == 'B' && (stack/2 + 1) % 2 == 1) || (lastColor == 'G' && (stack/2 + 1) % 2 == 0))
            traductor->girar(180);
        dir = (stack/2 + 1) % 2 == 0;
        traductor->moveToHorizontal(dir); // frente o reversa hasta topar linea horizontal. True es frente, False es atras
        lines = stack < 2 || stack > 5 ? -1 : 1;
        // 2 son dos lineas, 1 es una. negativo es derecha, positivo izquierda        
        traductor->horizontal(lines, dir); // Avanza por la linea horizontal a la izquierda o derecha        
        currentLevel = stacks[stack]; 
    }else{
        if(lastColor == 'R')
            traductor->setTrainLevel(false);
        traductor->moveAtrasHorizontal(); // izquierda hasta topar linea horizontal
        traductor->waitForMechanism();
        traductor->updateMechanismMovement(currentLevel, lastLevel, false);  // eleva el stack para no chocar con los demas
        currentLevel = stacks[stack];
        angle = (stack/2 + 1)%2 != 0 ? 90 : -90;
        angle *= lastColor == 'R' ? -1 : 1;
        traductor->girar(angle); // 90 es vuelta de 90 hacia la derecha, -90 es 90 hacia izquierda
        tcrt = angle < 0;
        
        if(lastColor != 'R'){
            lines = (stack < 4 && lastColor == 'B') || (stack > 3 && lastColor == 'G') ? 2 : 1;
            lines *= ((stack/2 + 1) % 2 == 1 && lastColor == 'B') || ((stack/2 + 1) % 2 == 0 && lastColor == 'G') ? -1 : 1;
        }else{
            lines = (stack < 4 && !firstRed) || (stack > 3 && firstRed) ? 1 : 2;
            lines *= ((stack/2 + 1) % 2 == 1 && !firstRed) || ((stack/2 + 1) % 2 == 0 && firstRed) ? 1 : -1;
        }
        // 2 son dos lineas, 1 es una. negativo es derecha, positivo izquierda
        traductor->horizontal(lines, tcrt); // Avanza por la linea horizontal a la izquierda o derecha
    }

    if(lastColor != 'R'){
        lines = (stack % 4) % 3 == 0 ? 1 : 2;
        lines *= stack % 4 > 1 ? -1 : 1;
    }else{
        lines = (stack % 4) % 3 == 0 ? 2 : 1;
        lines *= stack % 4 > 1 ? 1 : -1;
    }
    // 2 son dos stacks, 1 es uno. negativo es frente, positivo reversa
    traductor->waitForMechanism();  
    traductor->vertical(lines); // Avanza por la linea vertical frente o reversa
    traductor->mecanismo(lastLevel, currentLevel+1);
    grabContainer(c, false);
}