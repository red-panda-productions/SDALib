#pragma once

#include <fstream>
#include <iostream>
#include "ReplayAction.h"
#include "ReplayData.h"
#include "SDADriver.hpp"

#define SET_TICK_TO_ACT \
    READ_FILE           \
    m_tickToAct = std::stoul(m_reading);
#define READ_FILE m_replayFile >> m_reading;
#define READ_AND_SET_FLOAT(variable)                            \
    try                                                         \
    {                                                           \
        READ_FILE                                               \
        action.variable = std::stof(m_reading);                 \
    }                                                           \
    catch (std::exception & e)                                  \
    {                                                           \
        m_replayFile.close();                                   \
        throw std::exception("Cannot convert string to float"); \
    }

class ReplayDriver : public SDADriver
{
public:
    ReplayDriver(std::string& replayFile)
        : SDADriver()
    {
        m_replayFile.open(replayFile);
        if (!m_replayFile.good()) throw std::exception("Could not open replay file");
        SET_TICK_TO_ACT
    }

protected:
    void InitAI() override
    {
        // do nothing
    }

    SDAAction UpdateAI(SDAData& p_data) override
    {
        SDAAction action;

        if (m_replayFile.is_open() && m_replayFile.eof())
        {
            m_replayFile.close();
            return action;
        }

        if (m_tickToAct != p_data.TickCount) return action;

        READ_AND_SET_FLOAT(Brake)
        action.Brake = action.Brake > 1 || action.Brake < 0 ? 0 : action.Brake;

        READ_AND_SET_FLOAT(Steer)
        action.Steer = action.Steer > 1 || action.Steer < -1 ? 0 : action.Steer;

        std::cout << "TickCount: " << p_data.TickCount << "; Brake value: " << action.Brake << "; Steer value: " << action.Steer << std::endl;

        if (!m_replayFile.eof()) { SET_TICK_TO_ACT }

        return action;
    }

private:
    std::ifstream m_replayFile;
    std::string m_reading;
    unsigned long m_tickToAct;
};