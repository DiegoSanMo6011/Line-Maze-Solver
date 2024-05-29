#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

#include <Pololu3piPlus32U4.h>

class LineFollower
{
public:
    LineFollower();
    void followSegment();
    void followSegment2();
    void calibrateSensors();
    
private:
    int lastProportional;
    long integral;
};

#endif
