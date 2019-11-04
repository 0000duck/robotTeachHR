#include "globalParameter.h"

using namespace std;
using namespace rclib;

static float s_vel;
void setGlobalVel(float vel){
    s_vel = vel;
}

float getGlobalVel(){
    return s_vel;
}

static bool s_chineseState = false;
void setProgramChinese(bool state){
    s_chineseState = state;
}

bool isProgramChinese(){
    return s_chineseState;
}
