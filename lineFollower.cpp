#include <Pololu3piPlus32U4.h>
#include "LineFollower.h"
#include "UI.h"

extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern UI ui;

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
        unsigned int position = lineSensors.readLineWhite(sensors);
        ui.mostrarSensores();
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

        const int maxSpeed = 100; // Velocidad máxima del motor 100
        if (powerDifference > maxSpeed) powerDifference = maxSpeed;
        if (powerDifference < -maxSpeed) powerDifference = -maxSpeed;

        if (powerDifference < 0)
            motors.setSpeeds(maxSpeed + powerDifference, maxSpeed);
        else
            motors.setSpeeds(maxSpeed, maxSpeed - powerDifference);
            
        if(sensors[1] > 200 && sensors[2] > 200 && sensors[3] > 200)
        {
            return;
        }
        else if(sensors[0] < 200 || sensors[4] < 200)
        {
          //Found an intersection.
          return;
        }
    }

}

void LineFollower::followSegment2()
{
    unsigned int sensors[5];

    while (true) {
        unsigned int position = lineSensors.readLineWhite(sensors);
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

        const int maxSpeed = 100; // Velocidad máxima del motor 100
        if (powerDifference > maxSpeed) powerDifference = maxSpeed;
        if (powerDifference < -maxSpeed) powerDifference = -maxSpeed;

        if (powerDifference < 0)
            motors.setSpeeds(maxSpeed + powerDifference, maxSpeed);
        else
            motors.setSpeeds(maxSpeed, maxSpeed - powerDifference);

        if(sensors[0] < 200 && sensors[2] < 200 && sensors[4] < 200)//todos
        {
            return;

        }
        
        else if(sensors[0] < 200 && sensors[4] < 200)//Izq & Derecha
        {
          return;
        }
        else if (sensors[0] < 200 && sensors[2] < 100){  //iz & enfrente
          return;
        }
        else if (sensors[4] < 200 && sensors[2] < 100){ // derecha & enfrente
          return;
        }

        else if (sensors[4]< 200){
          motors.setSpeeds(30, 30);
          delay(120);
          motors.setSpeeds(60, -60);
          delay(300); // Ajustar el tiempo según sea necesario
          motors.setSpeeds(0, 0);

        }
        
        
        
        else if (sensors[0] < 200 ){

          motors.setSpeeds(30, 30);
          delay(120);
          motors.setSpeeds(-60, 60);
          delay(300); // Ajustar el tiempo según sea necesario
          motors.setSpeeds(0, 0);

        }
        else if(sensors[0] < 200 && sensors[2] < 200 && sensors[4] < 200 && sensors[3] < 200 && sensors[1] < 200)//todos
        {
            return;
        }
    }

}