#pragma once
#include "SDADriver.hpp"
#include "SDADriver.inl"
#include "PointerManagerMock.h"
#include "PythonDriver.h"

/// @brief A driver mock
class SDADriverMock : public AIInterface<PointerManagerMock>
{
public:
    SDADriverMock()
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

/// @brief A driver mock
class PythonDriverMock : public SDADriverMock
{
public:
    PythonDriverMock()
        : SDADriverMock()
    {
        // do nothing
    }

    PythonDriver pythonDriverFunctions;


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