#ifndef _Traductor_h
#define _Traductor_h

#include <Arduino.h>
#include <_Movements.h>

class _Traductor{

    public:
        _Traductor();

        void horizontalLine(bool b);
        void throughtHorizontal(int);
        void throughtHorizontal2(int, bool);
        void girar(int angle);
        void avanzar(bool b);
        void moveToShip(bool);
        void alignShip();
        void alignFirstShip();
        void alinearTren();
        void mecanismo(uint8_t newStack, uint8_t currentStack);
        void grabContainer();
        void dropContainer();
        void moveAtras();
        void moveToHorizontal(bool b);
        void horizontal(int lines, bool tcrt);
        void backUntilBlackLineSharps(bool tcrt);
        void moveAtrasHorizontal(); 
        void vertical(int lines);
        void alinearStack(bool);
        void updateMechanismMovement(int, int, bool);
        void setTrainLevel(bool);
        void waitForMechanism();
        void fixContainerSteps(char);
        void centerContainer(char);
        void moveMechanismForAligning(bool, double);
        void moveToTrain(bool);

        void LcdPrint(String name, char value);
        void LcdPrint(String name, int value);
        void LcdPrint(String name, String value);
        void LcdPrint(String name, double value);

        void pickFirst(int stack);
        void gotoFirst();
        void gotoSecond();

        _Movements *movements;

};

#endif