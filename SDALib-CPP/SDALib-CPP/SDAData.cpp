/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#include "SDAData.hpp"

SDAData::SDAData(tCar* p_simCar, tCarElt* p_car, tSituation* p_situation, unsigned long p_tickCount)
    : SimCar(*p_simCar), Car(*p_car), Situation(*p_situation), TickCount(p_tickCount) {}

SDAData::SDAData()
    : Car(), Situation(), TickCount(0) {}