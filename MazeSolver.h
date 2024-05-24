// MazeSolver.h
#ifndef MAZESOLVER_H
#define MAZESOLVER_H

class MazeSolver
{
public:
    MazeSolver();
    void solveMaze();

private:
    void turnLeft();
    void turnRight();
    void turnAround();
    void followSegment();
    void handleIntersection();

};

#endif
