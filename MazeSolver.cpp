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
  bool detener = false;
  while (detener == false){
    unsigned int sensors[5];
    followSegment();
    
    lineSensors.read(sensors);

    // Verificar si hay una intersección
    bool left =  sensors[0]  < 900;
    bool right = sensors[4]  < 900;
    bool centerleft =  sensors[1]  < 900;
    bool front = sensors[2]  < 900;
    bool centerright =  sensors[3]  < 900;
    instru = handleIntersection(); 
    guardarinstru();
    // SI llega al final
    if (left && right && front && centerleft && centerright){
      detener = true;
    }
    
    
  }
  return 1;
}

void MazeSolver::guardarinstru()
{
    if (instru == 'R'){
      Recorrido[i] = instru;
      i++;
    }else if (instru == 'L'){
      Recorrido[i] = instru;
      i++;
    }else if (instru == 'S'){
      Recorrido[i] = instru;
      i++;
    }
    else if (instru == 'U'){
      Recorrido[i] = instru;
      i++;
    }else{
      return;
    }
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
    bool front = sensors[2]  < 885;
    bool centerright =  sensors[3]  < 900;

    
    if (left && right && front && centerleft && centerright){
      motors.setSpeeds(30, 30);
      return 'A';
    }else if (left && right) {
        turnRight();
        return 'R';
    }else if (left && front){
        motors.setSpeeds(30, 30);
        delay(100);
        return 'S';
    }else if (right && front){
        turnRight();
        return 'R';
    }else if (right && left && front) {
        turnRight();
        return 'R';
    }else if (left) {
        turnLeft();
        return 'A';
    }else if (right) {
        turnRight();
        return 'A';
    } 
    else {
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

   
  
  for   (int r=0; r < length; r++){
    
    if (Recorrido[r] == 'U'){
      j = r;
      break;
    }
  }


  RecTemp[j-1] = Simplificar();

  

  for ( int k = 0; k < j - 1; k++){
        RecTemp[k] = Recorrido[k];
      
  }
  
  
  for (int l = j ; l < length - 2 ; l++){
        RecTemp[l] = Recorrido[l+2];
  }

  for (int p = 0 ; p < length; p++){
        Recorrido[p] = 0;
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
   
    for ( j = 0; j < length; j++){
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
   int length = strlen(Recorrido);
  display.noAutoDisplay();
    display.clear();
    display.gotoXY(0, 0);
    display.print(length);
    display.display();
    delay(500);
    for ( j = 0; j < length; j++){
      display.noAutoDisplay();
      display.clear();
      display.gotoXY(0, 0);
      display.print(Recorrido[j]);
      display.display();
      delay(1000);
      display.noAutoDisplay();
      display.clear();
      display.gotoXY(0, 0);
      display.print(j);
      display.display();
      delay(1000);
    }

  simplePath();
  
   length = strlen(Recorrido);
     display.noAutoDisplay();
    display.clear();
    display.gotoXY(0, 0);
    display.print(length);
    display.display();
    delay(500);
    for ( j = 0; j < length; j++){
      display.noAutoDisplay();
      display.clear();
      display.gotoXY(0, 0);
      display.print(Recorrido[j]);
      display.display();
      delay(1000);
      display.noAutoDisplay();
      display.clear();
      display.gotoXY(0, 0);
      display.print(j);
      display.display();
      delay(1000);
    }
  for (int f=0;f<length;f++){
    followSegment2();
    motors.setSpeeds(0, 0);
    ui.mostrarSensores();
    delay(2000);
    if (Recorrido [f] == 'S') {
      motors.setSpeeds(30, 30);
      delay(100);  

    } else if (Recorrido[f] == 'L') {
      turnLeft();

    }else if (Recorrido[f] == 'R') {
      turnRight();
    }
  }
  followSegment2();
  motors.setSpeeds(0, 0);

    return ;
  
}

