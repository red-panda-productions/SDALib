#pragma once
#define SDA_STEER_ACTION
#define SDA_BRAKE_ACTION
#define SDA_GEAR_ACTION
#define SDA_ACCEL_ACTION
#include "SDAAction.hpp"
#include "SDADriver.hpp"
#include "SDAData.hpp"
#include "Python.h"

class PythonDriver : public SDADriver
{
public:
    PythonDriver();

protected:
    void InitAI() override;

    SDAAction UpdateAI(SDAData& p_data) override;

private:
    //object m_main;
    //object m_globals;
    //object m_driverType;
    //object m_pythonDriver;

    //object m_updateFunction;
    //object m_initFunction;
};
