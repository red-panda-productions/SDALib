// This program has been developed by students from the bachelor Computer Science at
// Utrecht University within the Software Project course.
// © Copyright Utrecht University (Department of Information and Computing Sciences)

#pragma once

#include <fstream>
#include <iostream>
#include "ReplayAction.h"
#include "ReplayData.h"

#include "SDADriver.hpp"

#define MAX_ULONG 4294967295

#ifdef LOG_REPLAY

#define LOG_VALUES()                                   \
    std::cout << "\tSteer: " << action.Steer << "\n\t" \
              << "Brake: " << action.Brake << "\n\t"   \
              << "Accel: " << action.Accel << "\n\t"   \
              << "Gear: " << action.Gear << std::endl

#define LOG_TICK()                                          \
    std::cout << "TickCount: " << p_data.TickCount << "\n"; \
    << "Tick To Act: " << m_tickToAct << std::endl

#else

#define LOG_VALUES()
#define LOG_TICK()

#endif

template <typename TYPE>
struct Bits
{
    TYPE T;
};

template <typename TYPE>
static inline Bits<TYPE&> bits(TYPE& p_t)
{
    return Bits<TYPE&>{p_t};
}

template <typename TYPE>
static inline Bits<const TYPE&> bits(const TYPE& p_t)
{
    return Bits<const TYPE&>{p_t};
}

template <typename TYPE>
static inline std::istream& operator>>(std::istream& p_in, Bits<TYPE&> p_b)
{
    return p_in.read(reinterpret_cast<char*>(&p_b.T), sizeof(TYPE));
}

template <typename TYPE>
static inline std::ostream& operator<<(std::ostream& p_out, Bits<TYPE&> const p_b)
{
    return p_out.write(reinterpret_cast<const char*>(&(p_b.t)), sizeof(TYPE));
}

#define SET_TICK_TO_ACT m_replayFile >> bits(m_tickToAct);

#define READ_FLOAT(p_float) m_replayFile >> bits(p_float);

class ReplayDriver : public SDADriver
{
public:
    /// @brief Opens p_replayFile and sets first tickCount.
    /// @param p_replayFile File of recorded blackbox
    explicit ReplayDriver(std::string& p_replayFile)
        : SDADriver()
    {
        m_replayFile.open(p_replayFile, std::ios::binary);
        if (!m_replayFile.good())
        {
            SDA_THROW_EXCEPTION("Could not open replay file");
        }
        SET_TICK_TO_ACT
    }

protected:
    /// @brief Initialise the AI
    void InitAI() override
    {
        // do nothing
    }

    /// @brief Reads actions from recorded file in m_replayFile.
    ///        Sets the action if the read tick count is the same as the current tick count in p_data
    /// @param p_data Current simulation data from speed dreams
    /// @return Action to send to speed dreams
    SDAAction UpdateAI(SDAData& p_data) override
    {
        SDAAction action;

        LOG_TICK();

        if (!m_replayFile.is_open() || p_data.TickCount == MAX_ULONG) return action;

        if (m_tickToAct > p_data.TickCount)
            return m_prevAction;
        if (m_tickToAct < p_data.TickCount)
        {
            if (m_replayFile.is_open())
            {
                m_replayFile.close();
                std::cout << "Reached end of file." << std::endl;
            }
            return action;
        }

        float floatToRead = NAN;

        READ_FLOAT(floatToRead)
        action.Steer = floatToRead;

        READ_FLOAT(floatToRead)
        action.Accel = floatToRead;

        READ_FLOAT(floatToRead)
        action.Brake = floatToRead;

        READ_FLOAT(floatToRead)
        action.Gear = (int)floatToRead;

        LOG_VALUES();

        SET_TICK_TO_ACT

        m_prevAction = action;

        return action;
    }

private:
    std::ifstream m_replayFile;

    unsigned long m_tickToAct{};

    SDAAction m_prevAction;
};