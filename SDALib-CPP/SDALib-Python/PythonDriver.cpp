#include "PythonDriver.h"
#include "PythonDriver.inl"

CREATE_PYTHON_DRIVER_IMPLEMENTATION(IPCPointerManager)

#ifndef SDA_LIBRARY
/// @brief  Main entry point of the driver
int main()
{
    Py_Initialize();
    PythonDriver<IPCPointerManager> driver;
    driver.Run();
    Py_Finalize();
    return 0;
}
#endif