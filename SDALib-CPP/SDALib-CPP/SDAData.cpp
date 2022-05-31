#include "SDAData.hpp"

SDAData::SDAData(tCarElt* p_car, tSituation* p_situation, unsigned long p_tickCount)
    : Car(*p_car), Situation(*p_situation), TickCount(p_tickCount) {}

SDAData::SDAData()
    : Car(), Situation(), TickCount(0) {}