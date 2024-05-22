#include <Pololu3piPlus32U4.h>
#include "MazeSolver.h"

using namespace Pololu3piPlus32U4;

extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;

MazeSolver::MazeSolver() : pathLength(0), solving(true), stepIndex(0), reversing(false) {}

void MazeSolver::turn(char dir)
{
    switch (dir)
    {
    case 'L':
        motors.setSpeeds(-80, 80);
        delay(200);
        break;
    case 'R':
        motors.setSpeeds(80, -80);
        delay(200);
        break;
    case 'B':
        motors.setSpeeds(80, -80);
        delay(400);
        break;
    case 'S':
        break;
    }
}

void MazeSolver::reverse()
{
    motors.setSpeeds(-50, -50);
    delay(500);
    reversing = true;
}

char MazeSolver::selectTurn(unsigned char foundLeft, unsigned char foundStraight, unsigned char foundRight)
{
    if (foundLeft)
        return 'L';
    else if (foundStraight)
        return 'S';
    else if (foundRight)
        return 'R';
    else
        return 'B';
}

void MazeSolver::solve()
{
    while (solving)
    {
        lineFollower.followSegment();
        motors.setSpeeds(50, 50);
        delay(50);

        unsigned char foundLeft = 0;
        unsigned char foundStraight = 0;
        unsigned char foundRight = 0;
        unsigned int sensors[5];

        // Primera lectura para izquierda y derecha
        lineSensors.readLineWhite(sensors);
        if (sensors[0] < 100) foundLeft = 1;
        if (sensors[4] < 100) foundRight = 1;

        // Pequeño avance y segunda lectura para dirección recta
        motors.setSpeeds(40, 40);
        delay(200);
        lineSensors.readLineWhite(sensors);
        if (sensors[1] < 200 || sensors[2] < 200 || sensors[3] < 200) foundStraight = 1;

        // Comprobación de intersección completa (meta)
        if (sensors[1] < 600 && sensors[2] < 600 && sensors[3] < 600) {
            solving = false;
            break;
        }

        // Detectar punto muerto (todos sensores negros)
        if (sensors[0] > 1000 && sensors[1] > 1000 && sensors[2] > 1000 && sensors[3] > 1000 && sensors[4] > 1000) {
            reverse();
            continue;
        }

        // Selección y ejecución del giro
        char dir = selectTurn(foundLeft, foundStraight, foundRight);
        turn(dir);

        // Almacena el giro en el camino recorrido
        path[pathLength] = dir;
        pathLength++;
    }
}

void MazeSolver::loopSolve()
{
    if (reversing) {
        reversing = false;
        // En caso de estar revirtiendo, descartamos la última dirección y elegimos otra
        if (pathLength > 0) {
            pathLength--;
            char lastDir = path[pathLength];
            // Basándonos en la prioridad, elegimos la siguiente opción
            if (lastDir == 'L') {
                // Intentar ir al frente o a la derecha
                solve();
            } else if (lastDir == 'S') {
                // Intentar girar a la derecha
                solve();
            } else if (lastDir == 'R') {
                // Intentar girar a la izquierda (se hace reversa)
                solve();
            }
        }
    } else if (solving) {
        solve();
    } else {
        if (stepIndex < pathLength) {
            lineFollower.followSegment();
            motors.setSpeeds(50, 50);
            delay(50);
            motors.setSpeeds(40, 40);
            delay(200);
            turn(path[stepIndex]);
            stepIndex++;
        } else {
            lineFollower.followSegment();
        }
    }
}
