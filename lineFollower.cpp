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
        unsigned int position = lineSensors.readLineWhite(sensors);
        int proportional = ((int)position) - 2000;
        int derivative = proportional - lastProportional;
        integral += proportional;
        lastProportional = proportional;
        int powerDifference = proportional / 20 + integral / 10000 + derivative * 3 / 2;

        const int max = 60;
        if (powerDifference > max) powerDifference = max;
        if (powerDifference < -max) powerDifference = -max;

        if (powerDifference < 0)
            motors.setSpeeds(max + powerDifference, max);
        else
            motors.setSpeeds(max, max - powerDifference);

        if (sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100) {
            return;
        } else if (sensors[0] > 200 || sensors[4] > 200) {
            return;
        }
    }
}