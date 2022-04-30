#pragma once

#include "ReplayAction.h"
#include "ReplayData.h"
#include "SDADriver.hpp"
#include <fstream>
#include <iostream>

template <typename TYPE> struct Bits { TYPE T; };

template <typename TYPE> static inline Bits<TYPE&> bits(TYPE& p_t)
{
  return Bits<TYPE&>{p_t};
}

template <typename TYPE>
static inline Bits<const TYPE&> bits(const TYPE& p_t)
{
  return Bits<const TYPE&>{p_t};
}

template <typename TYPE>
static inline std::istream &operator>>(std::istream& p_in, Bits<TYPE&> p_b)
{
  std::cout << "Reading " << sizeof(TYPE) << " bits" << std::endl;
  return p_in.read(reinterpret_cast<char *>(&p_b.T), sizeof(TYPE));
}

template <typename TYPE>
static inline std::ostream &operator<<(std::ostream& p_out,Bits<TYPE&> const p_b)
{
  // reinterpret_cast is for pointer conversion
  // static_cast is for compatible pointer conversion
  return p_out.write(reinterpret_cast<const char *>(&(p_b.t)), sizeof(TYPE));
}

#define SET_TICK_TO_ACT m_replayFile >> bits(m_tickToAct);

#define READ_FLOAT(p_float)                                                    \
  try {                                                                        \
    m_replayFile >> bits(p_float);                                             \
  } catch (std::exception & e) {                                               \
    (p_float) = NAN;                                                           \
    std::cerr << "Could convert string to float" << std::endl;                 \
  }

class ReplayDriver : public SDADriver {
public:
  ReplayDriver(std::string& replayFile) : SDADriver() {
    m_replayFile.open(replayFile, std::ios::binary);
    if (!m_replayFile.good())
      throw std::exception("Could not open replay file");

    SET_TICK_TO_ACT
  }

protected:
  void InitAI() override {
    // do nothing
  }

  SDAAction UpdateAI(SDAData& p_data) override {
    SDAAction action;

    if (!m_replayFile.is_open())
      return action;

    std::cout << "TickCount: " << p_data.TickCount << "\n"
              << "Tick To Act: " << m_tickToAct << std::endl;

    if (m_tickToAct != p_data.TickCount)
      return action;

    float floatToRead = NAN;

    READ_FLOAT(floatToRead)
    action.Steer = floatToRead > 1 || floatToRead < -1 ? 0 : floatToRead;

    READ_FLOAT(floatToRead)
    action.Brake = floatToRead > 1 || floatToRead < 0 ? 0 : floatToRead;

    READ_FLOAT(floatToRead)
    action.Accel = floatToRead > 1 || floatToRead < 0 ? 0 : floatToRead;

    READ_FLOAT(floatToRead)
    action.Gear = (int)floatToRead < -1 || (int)floatToRead > 1 ? 0 : (int)floatToRead;

    std::cout << "Steer: " << action.Steer << "\n\t"
              << "Brake: " << action.Brake << "\n\t"
              << "Accel: " << action.Accel << "\n\t"
              << "Gear: " << action.Gear << std::endl;

    if (m_replayFile.eof()) {
      m_replayFile.close();
      std::cout << "Reached end of file." << std::endl;
    } else {
      SET_TICK_TO_ACT
    }

    return action;
  }

private:
  std::ifstream m_replayFile;
  unsigned long m_tickToAct;
};