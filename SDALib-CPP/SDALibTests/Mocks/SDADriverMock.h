#pragma once
#include "SDADataMock.h"
#include "SDAActionMock.h"
#include "SDADriver.hpp"


/// @brief A driver mock
class SDADriverMock : public SDADriver
{
public:

	SDADriverMock() : SDADriver()
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
		action.Steer = 1;
		action.Brake = 2;
		return action;
	}
};