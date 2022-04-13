#pragma once
#include "SDADriver.hpp"
#include "SDADriver.inl"
#include "PointerManagerMock.h"



/// @brief A driver mock
class SDADriverMock : public AIInterface<PointerManagerMock>
{
public:

	SDADriverMock() : AIInterface<PointerManagerMock>()
	{
		// do nothing
	}
protected:
	void InitAI() override
	{
		// do nothing
	}

	SDAAction UpdateAI(SDAData& p_data) override
	{
		SDAAction action;
		action.Steer = 0.75;
		action.Brake = 2;
		action.Gear = 1;
		action.Accel = 0.5;
		return action;
	}
};