#include <Pololu3piPlus32U4.h>
#include "MazeSolver.h"
#include "LineFollower.h"
#include "UI.h"

// Declaración de objetos externos para motores, sensores de línea, seguidor de líneas y UI
extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern LineFollower lineFollower;
extern UI ui;

// Constructor de la clase MazeSolver
MazeSolver::MazeSolver() {}

// Función principal para resolver el laberinto
void MazeSolver::solveMaze()
{
    int flag = 0; // Bandera para controlar el bucle
    while (flag == 0) {
        followSegment();      // Sigue un segmento de la línea
        handleIntersection(); // Maneja las intersecciones encontradas

        flag = 1; // Se establece la bandera para salir del bucle
    }
}

// Función para girar a la izquierda
void MazeSolver::turnLeft()
{
    motors.setSpeeds(30, 30);  // Avanza lentamente
    delay(120);                // Espera un corto período
    motors.setSpeeds(-60, 60); // Gira a la izquierda
    delay(300);                // Espera a que complete el giro
    motors.setSpeeds(0, 0);    // Detiene los motores
}

// Función para girar a la derecha
void MazeSolver::turnRight()
{
    motors.setSpeeds(30, 30);  // Avanza lentamente
    delay(120);                // Espera un corto período
    motors.setSpeeds(60, -60); // Gira a la derecha
    delay(300);                // Espera a que complete el giro
    motors.setSpeeds(0, 0);    // Detiene los motores
}

// Función para dar media vuelta
void MazeSolver::turnAround()
{
    motors.setSpeeds(30, 30);  // Avanza lentamente
    delay(120);                // Espera un corto período
    motors.setSpeeds(60, -60); // Gira 180 grados
    delay(700);                // Espera a que complete el giro
    motors.setSpeeds(0, 0);    // Detiene los motores
}

// Función para seguir un segmento de la línea
void MazeSolver::followSegment()
{
    lineFollower.followSegment(); // Usa el seguidor de líneas para seguir el segmento
}

// Función para manejar las intersecciones en el laberinto
void MazeSolver::handleIntersection()
{
    unsigned int sensors[5]; // Array para almacenar las lecturas de los sensores de línea
    lineSensors.read(sensors); // Lee los valores de los sensores

    // Determina la presencia de línea en cada sensor
    bool left = sensors[0] < 1200;
    bool right = sensors[4] < 1200;
    bool centerleft = sensors[1] < 1200;
    bool front = sensors[2] < 1000;
    bool centerright = sensors[3] < 1200;

    // Decide el giro en la intersección basado en los sensores
    if (left && right) {
        turnRight(); // Gira a la derecha si hay línea a ambos lados
    } else if (left && right && front && centerleft && centerright) {
        motors.setSpeeds(0, 0); // Detiene el robot si todos los sensores detectan línea
        delay(20000);           // Espera un largo período
    } else if (left && front) {
        motors.setSpeeds(30, 30); // Avanza si hay línea a la izquierda y al frente
        delay(100);
    } else if (left) {
        turnLeft(); // Gira a la izquierda si solo hay línea a la izquierda
    } else if (right) {
        turnRight(); // Gira a la derecha si solo hay línea a la derecha
    } else {
        turnAround(); // Da media vuelta si no hay línea a los lados
    }
}

