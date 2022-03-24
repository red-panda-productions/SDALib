#pragma once
#include <iostream>

#include "SteerSampleAction.h"
#include "SteerSampleData.h"
#include "SDADriver.hpp"

/// @brief A sample driver that turns when its to far off the middle
class SteerSampleDriver : public SDADriver
{
public:

    SteerSampleDriver(float p_maxOff) : SDADriver()
    {
        m_maxOff = p_maxOff;
    }
protected:
    void InitAI() override
    {
        // do nothing
    }

    SDAAction UpdateAI(SDAData& p_data) override
    {
        SDAAction action;

        std::cout << p_data.ToLeft() << " " << p_data.ToMiddle() << " " << p_data.ToRight() << std::endl;

        if(p_data.ToMiddle() < -m_maxOff)
        {

            action.Steer = std::min(-p_data.ToMiddle() / 60.0f,1.0f);
            std::cout << "Steering left: " << action.Steer << std::endl;
        }
        if(p_data.ToMiddle() > m_maxOff)
        {
            action.Steer = std::max( - p_data.ToMiddle() / 60.0f,-1.0f);
            std::cout << "Steering right: " << action.Steer << std::endl;
        }

        return action;
    }

private:
    float m_maxOff;
};