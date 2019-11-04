#ifndef GLOBALPARAMETER_H
#define GLOBALPARAMETER_H

#include <iostream>
#include <string>

#include "../robotServer/RobotStructure/robotStructure.h"

void setGlobalVel(float vel);
float getGlobalVel();

void setProgramChinese(bool);
bool isProgramChinese();

#endif
