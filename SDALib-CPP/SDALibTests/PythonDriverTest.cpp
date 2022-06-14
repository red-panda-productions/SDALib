#include <gtest/gtest.h>
#include "Mocks/SDADriverMock.h"
#include "PythonDriver.h"
#include "PythonDriver.inl"
#include "SDATypesConverter.h"
#include "Mocks/PointerManagerMock.h"
#include "Random.hpp"
#include "GeneratorUtils.h"

CREATE_PYTHON_DRIVER_IMPLEMENTATION(PointerManagerMock)

#define TPythonDriver PythonDriver<PointerManagerMock>

/// @brief tests whether the python function UpdateAI give back the correct c++ data
TEST(PythonDriverTests, PythonDriverUpdateAITest)
{
    Py_Initialize();

    Random random;

    TPythonDriver pythonDriver = TPythonDriver();
    pythonDriver.SetPythonDriverFileName("DriverTest");
    pythonDriver.InitAI();

    SDAData sdaData;
    TestSegments segments = GenerateSegments();
    sdaData.TickCount = random.NextUInt();
    sdaData.Car = GenerateCar(segments);
    sdaData.Situation = GenerateSituation();

    SDAAction action = pythonDriver.UpdateAI(sdaData);

    ASSERT_EQ(action.Steer, sdaData.Car.pub.DynGC.vel.x);
    ASSERT_EQ(action.Accel, sdaData.Situation.deltaTime);
    ASSERT_EQ(action.Brake, sdaData.Situation.raceInfo.totTime);
    ASSERT_EQ(action.Gear, sdaData.Situation.raceInfo.totTime);

    DestroySegments(segments);
    DestroyCar(sdaData.Car);
    DestroySituation(sdaData.Situation);

    Py_Finalize();
}

/// @brief tests whether the driver file name is correctly get and set
TEST(PythonDriverTests, PythonDriverPythonDriverFileNameTest)
{
    Py_Initialize();
    TPythonDriver pythonDriver = TPythonDriver();

    pythonDriver.SetPythonDriverFileName("DriverTest");
    ASSERT_EQ(pythonDriver.GetPythonDriverFileName(), "DriverTest");

    Py_Finalize();
}