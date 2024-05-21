#include <Pololu3piPlus32U4.h>
#include "LineFollower.h"
 
using namespace Pololu3piPlus32U4;
 
OLED display;
Buzzer buzzer;
ButtonA buttonA;
ButtonB buttonB;
ButtonC buttonC;
LineSensors lineSensors;
BumpSensors bumpSensors;
Motors motors;
Encoders encoders;
int flag = 0;
LineFollower lineFollower;

void setup() {

}

void loop() {
  if (flag == 0){
    lineFollower.calibrateSensors();
    flag =1;
  }
  lineFollower.followSegment();
}
