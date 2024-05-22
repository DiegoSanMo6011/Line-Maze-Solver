#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "LineFollower.h"

class MazeSolver
{
public:
    MazeSolver();
    void solve();
    void loopSolve();  // Function to be called from Arduino's loop()

private:
    void turn(char dir);
    char selectTurn(unsigned char foundLeft, unsigned char foundStraight, unsigned char foundRight);
    void reverse();

    LineFollower lineFollower;
    char path[100];
    unsigned char pathLength;
    bool solving;
    int stepIndex;
    bool reversing;
};

#endif
