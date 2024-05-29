// main.ino
#include <Pololu3piPlus32U4.h>
#include "LineFollower.h"
#include "UI.h"
#include "MazeSolver.h"

using namespace Pololu3piPlus32U4;

OLED display;
Buzzer buzzer;
ButtonA buttonA;
ButtonB buttonB;
ButtonC buttonC;
LineSensors lineSensors;
BumpSensors bumpSensors;
Motors motors;
Encoders encoders;
int flag = 0;
LineFollower lineFollower;
UI ui;
MazeSolver mazeSolver;

void setup() {
    ui.inicializarPantalla();
}

void loop() {
    if (flag == 0) {
        ui.mostrarCalibrando();
        lineFollower.calibrateSensors();
        flag = 1;
    }
    flag == mazeSolver.solveMaze();
    if (flag == 0){
      motors.setSpeeds(0, 0);
      if (buttonB.getSingleDebouncedPress()){
        mazeSolver.segundaVuelta();
      }
    }
}

