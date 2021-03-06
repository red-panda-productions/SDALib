/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once

#include "SDStructs.hpp"

struct SDAData
{
public:
    SDAData(tCar* p_simCar, tCarElt* p_car, tSituation* p_situation, unsigned long p_tickCount);

    SDAData();

    tCar SimCar;
    tCarElt Car;
    tSituation Situation;
    unsigned long TickCount;
};
