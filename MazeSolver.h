// MazeSolver.h
#ifndef MAZESOLVER_H
#define MAZESOLVER_H

class MazeSolver
{
public:
    MazeSolver();
    int solveMaze();
    void segundaVuelta();
    int i = 0;
    int j = 0;
    char instru;
    char Recorrido[50];

private:
    void turnLeft();
    void turnRight();
    void turnAround();
    void followSegment();
    void followSegment2();
    char handleIntersection();
    char Simplificar();
    void Reescribir();
    void simplePath();
    void guardarinstru();
    
};

#endif
