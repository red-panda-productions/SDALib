// This program has been developed by students from the bachelor Computer Science at
// Utrecht University within the Software Project course.
// © Copyright Utrecht University (Department of Information and Computing Sciences)

#pragma once

#include "SDStructs.hpp"

struct SDAData
{
public:
    SDAData(tCarElt *p_car, tSituation *p_situation, unsigned long p_tickCount);

    SDAData();

    tCarElt Car;
    tSituation Situation;
    unsigned long TickCount;
};
