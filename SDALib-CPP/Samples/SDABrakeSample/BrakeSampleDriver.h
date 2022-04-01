#pragma once
#include <iostream>

#include "BrakeSampleAction.h"
#include "SDADriver.hpp"

/// @brief A sample driver that brakes when driving above set max speed
class BrakeSampleDriver : public SDADriver
{
public:

    BrakeSampleDriver(float p_maxSpeed) : SDADriver()
    {
        m_maxSpeed = p_maxSpeed;
    }
protected:
    void InitAI() override
    {
        // do nothing
    }

    SDAAction UpdateAI(SDAData& p_data) override
    {
        SDAAction action;

        //full brake when going 20km/h above max speed
        action.Brake = std::max(0.0f,(p_data.Car.pub.DynGC.vel.x - m_maxSpeed) / 20);
        std::cout << p_data.Car.pub.DynGC.vel.x << " " << action.Brake << std::endl;

        return action;
    }

private:
    float m_maxSpeed;
};