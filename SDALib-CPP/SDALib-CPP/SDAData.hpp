#pragma once
#include "SDStructs.hpp"

struct SDAData
{
public:
    SDAData(tCarElt* p_car, tSituation* p_situation, int p_tickCount) : Car(*p_car), Situation(*p_situation), TickCount(p_tickCount) {}
    SDAData()
    {
        TickCount = 0;
    }
    tCarElt Car;
    tSituation Situation;
    int TickCount;
};
