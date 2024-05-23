// MazeSolver.cpp
#include <Pololu3piPlus32U4.h>
#include "MazeSolver.h"
#include "LineFollower.h"
#include "UI.h"

extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern LineFollower lineFollower;
extern UI ui;

MazeSolver::MazeSolver() {}

void MazeSolver::solveMaze()
{
    while (true) {
        ui.mostrarSensores();
        handleIntersection();
        followSegment();
    }
}

void MazeSolver::turnLeft()
{
    motors.setSpeeds(-60, 60);
    delay(200); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::turnRight()
{
    motors.setSpeeds(60, -60);
    delay(200); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::turnAround()
{
    motors.setSpeeds(60, -60);
    delay(400); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::followSegment()
{
    lineFollower.followSegment();
}

void MazeSolver::handleIntersection()
{
    unsigned int sensors[5];
    lineSensors.read(sensors);

    // Verificar si hay una intersección
    bool left = sensors[0] > 500;
    bool right = sensors[4] > 500;

    // Decidir el giro en la intersección
    if (left && right) {
        turnLeft();
    } else if (left) {
        turnLeft();
    } else if (right) {
        turnRight();
    } else {
        // Si no hay intersección, gira alrededor
        turnAround();
    }
}
