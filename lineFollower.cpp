#include <Pololu3piPlus32U4.h>
#include "LineFollower.h"
#include "UI.h"

// Declaración de los objetos globales
extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern UI ui;

// Constructor de la clase LineFollower
LineFollower::LineFollower() : lastProportional(0), integral(0) {}

// Función para calibrar los sensores de línea
void LineFollower::calibrateSensors()
{
    for (int i = 0; i < 80; i++) {
        // Mover el robot hacia adelante y hacia atrás para calibrar los sensores
        if (i < 20 || i >= 60)
            motors.setSpeeds(40, -40);
        else
            motors.setSpeeds(-40, 40);

        lineSensors.calibrate(); // Calibrar los sensores de línea
        delay(20);               // Esperar un poco entre calibraciones
    }
    motors.setSpeeds(0, 0); // Detener los motores al finalizar la calibración
}

// Función para seguir un segmento de línea
void LineFollower::followSegment()
{
    unsigned int sensors[5]; // Array para almacenar los valores de los sensores

    while (true) {
        unsigned int position = lineSensors.readLineWhite(sensors); // Leer la posición de la línea blanca
        ui.mostrarSensores(); // Mostrar los valores de los sensores en la pantalla

        // Calcular el error proporcional
        int proportional = ((int)position) - 2000;
        int derivative = proportional - lastProportional;
        integral += proportional;
        lastProportional = proportional;

        // Ajustar los valores de kp, ki y kd según sea necesario para mejorar la precisión
        float kp = 0.1;
        float ki = 0.0;
        float kd = 0.015;

        // Limitar la integral
        const int integralLimit = 1000;
        if (integral > integralLimit) integral = integralLimit;
        if (integral < -integralLimit) integral = -integralLimit;

        // Introducir un umbral muerto para evitar pequeñas oscilaciones
        const int deadband = 40;
        if (abs(proportional) < deadband) proportional = 0;

        // Calcular la diferencia de potencia para los motores
        int powerDifference = kp * proportional + ki * integral + kd * derivative;

        // Limitar la diferencia de potencia a la velocidad máxima del motor
        const int maxSpeed = 100; // Velocidad máxima del motor 100
        if (powerDifference > maxSpeed) powerDifference = maxSpeed;
        if (powerDifference < -maxSpeed) powerDifference = -maxSpeed;

        // Ajustar las velocidades de los motores en función de la diferencia de potencia
        if (powerDifference < 0)
            motors.setSpeeds(maxSpeed + powerDifference, maxSpeed);
        else
            motors.setSpeeds(maxSpeed, maxSpeed - powerDifference);

        // Condiciones para detenerse si se encuentra una intersección
        if (sensors[1] > 200 && sensors[2] > 200 && sensors[3] > 200) {
            return; // No hay lineas dead end detectado
        } else if (sensors[0] < 200 || sensors[4] < 200) {
            return; // Línea detectada en los bordes, intersección encontrada
        }
    }
}
