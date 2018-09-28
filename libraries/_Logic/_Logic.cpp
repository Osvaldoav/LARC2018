#include <_Logic.h>

_Logic::_Logic(){
    firstRed = -1;
    stacks = new int[8];
    for(int i = 0; i < 8; i++)
        stacks[i] = 3;
}

char _Logic::handleRed(){
    if(firstRed > -1)
        firstRed = lastStack < 4 ? 0 : 1;
    else
        firstRed = firstRed == 0 ? 1 : 0;

    return firstRed == 0 ? 'R' : 'S';
}

char _Logic::verifyColor(char c){
    char color = c > 98 ? 'R' : c > 65 ? 'G' : 'B';
    lastStack = color == 'R' ? c - 51 : color == 'G' ? c - 18 : c - 3;
    // verify and modify 'color' with sensor TCS3200
    lastColor = color;
    stacks[lastStack]--;
    if(color == 'G')
        green_boxes++;
    if(color == 'B')
        blue_boxes++;
    return color == 'R' ? handleRed() : color;
}

char _Logic::grabContainer(char c){
    // move forward
    // grab container
    return verifyColor(c);
}

void _Logic::stackToShip(){
    bool A = lastStack < 2 || (lastStack > 3 && lastStack < 6);
    bool B = lastColor != 'R';

    horizontalline(A == B); // Avanza de frente o de reversa hasta linea horizontal
    int dir;

    if(B){
        if ((lastColor == 'B' && blue_boxes < 4) || (lastColor == 'G' && green_boxes < 4)){
            dir = lastStack < 4 ? 2 : 1;
            dir *= lastStack > 5 - ((dir - 1)* 4) ? -1 : 1;
            dir = lastColor == 'G' ? abs(abs(dir) - 3) * -(dir/abs(dir)) : dir;
        }else if(lastStack < 2 || lastStack > 5){
            dir = 1;
        }else
            dir = -1;

        // 2 son dos lineas, 1 es una. negativo es derecha, positivo izquierda
        throughtHorizontal(dir); // Avanza por la linea horizontal a la izquierda o derecha cuando 'B' o 'G'
    }else{
        dir = lastStack < 4 ? 1 : 2;
        dir += firstRed ? 2 : 0;
        dir *= ((lastStack/2 + 1)%2 == 0) != firstRed ? 1 : -1;
    
        // Checar foto whatsapp para ver 1,2,3,4
        throughtHorizontal2(dir); // Avanza por la linea horizontal a la izquierda o derecha cuando 'R'
    }
    
    if ((lastColor == 'B' && blue_boxes > 3) || (lastColor == 'G' && green_boxes > 3)){
        if(((lastStack/2 + 1)%2 == 0) == (lastColor == 'B'))
            girar(180); // gira 180 grados
        avanzar() // avanza hasta que llegue a la altura de los barcos
    }else{
        dir = ((lastStack/2 + 1)%2 == 0) != B ? 1 : 0;
        girar(dir); // giros de 90, 1 -> izq, 0 -> der
    }
    alinearPozo();
}

void _Logic::shipToStack(char c){
    char color = c > 98 ? 'R' : c > 65 ? 'G' : 'B';
    stack = color == 'R' ? c - 51 : color == 'G' ? c - 18 : c - 3;
    bool dir;
    int lines;

    mecanismo(stacks[stack]); // nivela el mecanismo al nivel adecuado

    if((lastColor == 'B' && blue_boxes < 3) || (lastColor == 'G' && green_boxes < 3)){
        moveAtras(); // Se mueve poquito hacia atras
        if((lastColor == 'B' && (stack/2 + 1) % 2 == 1) || (lastColor == 'G' && (stack/2 + 1) % 2 == 0))
            girar(180);
        dir = (stack/2 + 1) % 2 == 0;
        moveToHorizontal(dir); // frente o reversa hasta topar linea horizontal. True es frente, False es atras
        lines = stack < 2 || stack > 5 ? -1 : 1;
        // 2 son dos lineas, 1 es una. negativo es derecha, positivo izquierda
        horizontal(lines); // Avanza por la linea horizontal a la izquierda o derecha
    }else{
        moveAtrasHorizontal(); // izquierda hasta topar linea horizontal
        dir = (stack/2 + 1)%2 != 0;
        dir = lastColor == 'R' ? !dir : dir;
        girar(dir); // True es vuelta de 90 hacia la derecha, False es 90 hacia izquierda
        
        if(lastColor != 'R'){
            lines = (stack < 4 && lastColor == 'B') || (stack > 3 && lastColor == 'G') ? 2 : 1;
            lines *= ((stack/2 + 1) % 2 == 1 && lastColor == 'B') || ((stack/2 + 1) % 2 == 0 && lastColor = 'G') ? -1 : 1;
        }else{
            lines = (stack < 4 && !firstRed) || (stack > 3 && firstRed) ? 1 : 2;
            lines *= ((stack/2 + 1) % 2 == 1 && !firstRed) || ((stack/2 + 1) % 2 == 0 && firstRed) ? 1 : -1;
        }
        // 2 son dos lineas, 1 es una. negativo es derecha, positivo izquierda
        horizontal(lines); // Avanza por la linea horizontal a la izquierda o derecha
    }

    if(lastColor != 'R'){
        lines = (stack % 4) % 3 == 0 ? 1 : 2;
        lines *= stack % 4 > 1 ? -1 : 1;
    }else{
        lines = (stack % 4) % 3 == 0 ? 2 : 1;
        lines *= stack % 4 > 1 ? 1 : -1;
    }
    // 2 son dos stacks, 1 es uno. negativo es derecha, positivo izquierda
    vertical(lines); // Avanza por la linea vertical frente o reversa
}