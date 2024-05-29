// MazeSolver.cpp
#include <Pololu3piPlus32U4.h>
#include "MazeSolver.h"
#include "LineFollower.h"
#include "UI.h"

extern Pololu3piPlus32U4::Motors motors;
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern LineFollower lineFollower;
extern Pololu3piPlus32U4::OLED display;
extern UI ui;

MazeSolver::MazeSolver() {}

int MazeSolver::solveMaze()
{
  followSegment();
  unsigned int sensors[5];
  lineSensors.read(sensors);

  // Verificar si hay una intersección
  bool left =  sensors[0]  < 1200;
  bool right = sensors[4]  < 1200;
  bool centerleft =  sensors[1]  < 900;
  bool front = sensors[2]  < 1000;
  bool centerright =  sensors[3]  < 900;

  // Decidir el giro en la intersección
  if (left && right && front && centerleft && centerright){
    motors.setSpeeds(0, 0);
    return 0;
  }
  Recorrido[i] = handleIntersection();   
  i++;
}




void MazeSolver::turnLeft()
{
    motors.setSpeeds(30, 30);
    delay(120);
    motors.setSpeeds(-60, 60);
    delay(300); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::turnRight()
{
    motors.setSpeeds(30, 30);
    delay(120);
    motors.setSpeeds(60, -60);
    delay(300); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::turnAround()
{
    motors.setSpeeds(30, 30);
    delay(120);
    motors.setSpeeds(60, -60);
    delay(700); // Ajustar el tiempo según sea necesario
    motors.setSpeeds(0, 0);
}

void MazeSolver::followSegment()
{
    lineFollower.followSegment();
}

void MazeSolver::followSegment2()
{
    lineFollower.followSegment2();
}

char MazeSolver::handleIntersection()
{
    unsigned int sensors[5];
    lineSensors.read(sensors);

    // Verificar si hay una intersección
    bool left =  sensors[0]  < 1200;
    bool right = sensors[4]  < 1200;
    bool centerleft =  sensors[1]  < 900;
    bool front = sensors[2]  < 1000;
    bool centerright =  sensors[3]  < 900;

    // Decidir el giro en la intersección
    if (left && right && front && centerleft && centerright){
        motors.setSpeeds(0, 0);
  
    }
    else if (left && right) {
        turnRight();
        return 'R';
    }  else if (left && front){
        motors.setSpeeds(30, 30);
        delay(100);
        return 'S';
    } else if (left) {
        turnLeft();
    } else if (right) {
        turnRight();
    } 
    else{
      turnAround();
      return 'U';
    }
}


char MazeSolver::Simplificar(){
  int length = strlen(Recorrido);
  for (j = 0; j < length; j++){
    if (Recorrido[j]=='U'){
      char Antes = Recorrido[j-1];
      char VueltaU = Recorrido[j];
      char Despues = Recorrido[j+1];
      if ((Antes == 'R') && (Despues == 'R')){
        return 'S';
      }else if ((Antes == 'R') && (Despues == 'S')){
        return 'L';
      }else if ((Antes == 'L') && (Despues == 'R')){
        return 'U';
      }else if ((Antes == 'S') && (Despues == 'R')){
        return 'L';
      }else if ((Antes == 'S') && (Despues == 'S')){
        return 'U';
      }
    }
  }
}

void MazeSolver::Reescribir(){
  int length = strlen(Recorrido);
  char RecTemp[length-2];
  for (j = 0; j < length; j++){
      if (Recorrido[j]=='U'){
        break;
      }
  }
  int k;
  for ( k = 0; k < j - 1; k++){
        RecTemp[k] = Recorrido[k];
  }
  RecTemp[k] = Simplificar();
  for (int l = j ; l < length - 2 ; l++){
        RecTemp[l] = Recorrido[l+2];
  }
  int templength = strlen(RecTemp);
  for (int p = 0 ; p < templength; p++){
        Recorrido[p] = RecTemp[p] ;
  }
  return;
}

void  MazeSolver::simplePath(){

  int flag=1;
  while (flag==1)
  {
    int length = strlen(Recorrido);
    for (j = 0; j < length; j++){
      if (Recorrido[j]=='U'){
        Reescribir();
        flag=1;
      }
      else {
        flag=0;
      }
    }
  }
  return;
}


void MazeSolver::segundaVuelta(){
    simplePath();

   int lenght = strlen(Recorrido);
   for (int f=0;f<lenght;f++){
    followSegment2();
    if (Recorrido [i] == 'S') {
      motors.setSpeeds(30, 30);
        delay(100);  

    } else if (Recorrido[i] == 'L') {
      turnLeft();

    }else if (Recorrido[i] == 'R') {
      turnRight();
    }
  }
    motors.setSpeeds(0, 0);

    return ;
}

