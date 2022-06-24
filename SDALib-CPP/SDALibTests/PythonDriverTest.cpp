#include <gtest/gtest.h>
#include "Mocks/SDADriverMock.h"
#include "PythonDriver.h"
#include "PythonDriver.inl"
#include "SDATypesConverter.h"
#include "Mocks/PointerManagerMock.h"
#include "Random.hpp"
#include "GeneratorUtils.h"
#include "SDASpeedDreams.h"

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
    sdaData.SimCar = GenerateSimCar(sdaData.Car);

    SDAAction action = pythonDriver.UpdateAI(sdaData);

    SDASpeedDreams sdaSpeedDreams;
    SDAAction testAction;
    testAction.Brake = 1;
    testAction.Steer = 1;
    testAction.Accel = 1;
    testAction.Gear = 1;
    SDAData newData = sdaSpeedDreams.UpdateSimulator(sdaData, testAction);

    ASSERT_EQ(action.Steer, sdaData.Car.pub.DynGC.vel.x);
    ASSERT_EQ(action.Accel, sdaData.Situation.deltaTime);
    ASSERT_EQ(action.Brake, sdaData.Situation.raceInfo.totTime);
    ASSERT_EQ(action.Gear, newData.TickCount);

    DestroySegments(segments);
    DestroyCar(sdaData.Car);
    DestroySituation(sdaData.Situation);

    Py_Finalize();
}

/// @brief tests whether the python function UpdateAI gives back an error
TEST(PythonDriverTests, PythonDriverCrashTest)
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
    sdaData.SimCar = GenerateSimCar(sdaData.Car);

    PyErr_BadInternalCall();
    SDAAction action = pythonDriver.UpdateAI(sdaData);

    ASSERT_EQ(action.Steer, 0);
    ASSERT_EQ(action.Accel, 0);
    ASSERT_EQ(action.Brake, 0);
    ASSERT_EQ(action.Gear, 0);

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

/// @brief tests whether the driver file name is correctly get and set
TEST(PythonDriverTests, SDASpeedDreamsTest)
{
    Random random;
    SDAData sdaData;
    TestSegments segments = GenerateSegments();
    sdaData.TickCount = random.NextUInt();
    sdaData.Car = GenerateCar(segments);
    sdaData.Situation = GenerateSituation();
    sdaData.SimCar = GenerateSimCar(sdaData.Car);

    SDAAction sdaAction;

    SDASpeedDreams sdaSpeedDreams;

    SDAData newData = sdaSpeedDreams.UpdateSimulator(sdaData, sdaAction);

    ASSERT_EQ(sdaData.TickCount, newData.TickCount);
}
