#include <Pololu3piPlus32U4.h>
#include "LineFollower.h"

extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;

LineFollower::LineFollower() : lastProportional(0), integral(0) {}

void LineFollower::calibrateSensors()
{
    for (int i = 0; i < 80; i++) {
        if (i < 20 || i >= 60)
            motors.setSpeeds(40, -40);
        else
            motors.setSpeeds(-40, 40);

        lineSensors.calibrate();
        delay(20);
    }
    motors.setSpeeds(0, 0);
}

void LineFollower::followSegment()
{
    unsigned int sensors[5];

    while (true) {
        unsigned int position = lineSensors.readLineBlack(sensors);
        int proportional = ((int)position) - 2000;
        int derivative = proportional - lastProportional;
        integral += proportional;
        lastProportional = proportional;

        // Ajusta los valores de kp, ki y kd según sea necesario para mejorar la precisión
        float kp = 0.1;
        float ki = 0.0;
        float kd = 0.015;

        // Limitar la integral
        const int integralLimit = 1000;
        if (integral > integralLimit) integral = integralLimit;
        if (integral < -integralLimit) integral = -integralLimit;

        // Introducir un umbral muerto
        const int deadband = 40;
        if (abs(proportional) < deadband) proportional = 0;

        int powerDifference = kp * proportional + ki * integral + kd * derivative;

        const int maxSpeed = 60; // Velocidad máxima del motor
        if (powerDifference > maxSpeed) powerDifference = maxSpeed;
        if (powerDifference < -maxSpeed) powerDifference = -maxSpeed;

        if (powerDifference < 0)
            motors.setSpeeds(maxSpeed + powerDifference, maxSpeed);
        else
            motors.setSpeeds(maxSpeed, maxSpeed - powerDifference);
    }
}