#pragma once

#include "SDAData.hpp"
#include "SDAAction.hpp"
#include "sim.h"

tCar CarConstructor(const tCar& p_other, tCarElt* p_carElt);
SDAData SDASpeedDreams(const SDAData& p_data, SDAAction& p_action);
