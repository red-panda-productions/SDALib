#include "PythonDriver.h"


PythonDriver::PythonDriver()
    :m_main(import("__main__"))
    ,m_globals(m_main.attr("__dict__"))
{
    exec("exec(open(\"Driver.py\").read())", m_globals);
    m_SDADriverType = m_globals["SDADriver"];
    m_pythonDriver = m_SDADriverType();
    m_updateFunction = m_pythonDriver.attr("UpdateAI");
    m_initFunction = m_pythonDriver.attr("InitAI");
}

void PythonDriver::InitAI()
{
    try
    {
        m_initFunction();
    }
    catch (error_already_set&)
    {
        PyErr_Print();
    }
}

SDAAction PythonDriver::UpdateAI(SDAData& p_data)
{
    SDAAction action;
    try
    {
        tuple result = extract<tuple>(m_updateFunction()); // needs input data
        action.Steer = extract<float>(result[0]);
        action.Accel = extract<float>(result[1]);
        action.Brake = extract<float>(result[2]);
        action.Gear  = extract<int>(result[3]);
    }
    catch (error_already_set&)
    {
        PyErr_Print();
    }

    return action;
}


int main()
{
    Py_Initialize();

    PythonDriver driver;
    driver.Run();

    Py_Exit(0);
    return 0;
}