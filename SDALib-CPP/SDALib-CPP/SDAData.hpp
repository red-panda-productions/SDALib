#pragma once

#include "SDStructs.hpp"

struct SDAData
{
public:
    SDAData(tCarElt *p_car, tSituation *p_situation, unsigned long p_tickCount);

    SDAData();

    tCar SimCar;
    tCarElt Car;
    tSituation Situation;
    unsigned long TickCount;
};
