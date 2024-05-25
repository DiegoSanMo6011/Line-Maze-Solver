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
  int flag = 0;
    while (flag==0) {
        followSegment();
        //delay(20);
        handleIntersection();
        
        flag= 1;   
    }
}

void MazeSolver::turnLeft()
{
    motors.setSpeeds(30, 30);
    delay(200);
    motors.setSpeeds(-60, 60);
    delay(400); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::turnRight()
{
    motors.setSpeeds(30, 30);
    delay(200);
    motors.setSpeeds(60, -60);
    delay(400); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::turnAround()
{
    motors.setSpeeds(30, 30);
    delay(200);
    motors.setSpeeds(60, -60);
    delay(700); // Ajustar el tiempo según sea necesario
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
    bool left =  sensors[0]  > 800;
    bool right = sensors[4]  > 800;
    bool centerleft =  sensors[1]  > 800;
    bool front = sensors[2]  > 800;
    bool centerright =  sensors[3]  > 800;

    // Decidir el giro en la intersección
    if (left && right) {
        turnRight();
    }else if (left && right && front && centerleft && centerright){
        motors.setSpeeds(0, 0);
        delay(20000);
    } else if (left && front){
        motors.setSpeeds(50, 50);
        delay(100);
    } else if (left) {
        turnLeft();
    } else if (right) {
        turnRight();
    } 
    else{
      turnAround();
    }
}
