#pragma once
#include "SDADriver.hpp"
#include "SDADriver.inl"
#include "PythonDriver.h"
#include "SDADriverMock.h"
#include "PointerManagerMock.h"

/// @brief A python driver mock
class PythonDriverMock : public AIInterface<PointerManagerMock>
{
public:
    PythonDriverMock()
        : AIInterface<PointerManagerMock>()
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