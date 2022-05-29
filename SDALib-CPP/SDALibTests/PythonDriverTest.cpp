#include "PythonDriver.h"
#include "Mocks/SDADriverMock.h"
#include <gtest/gtest.h>
#include <vector>
#include "Utils.h"

// check all classes are callable
TEST(PythonDriverTests, PythonDriverInitTest)
{
    PythonDriver pythonDriver = PythonDriver();

    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_sdaTypesClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_initCarClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_wheelSpecificationsClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_visualAttributesClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_publicCarClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_dynamicPointClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_trackLocationClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_trackSegmentClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carRaceInfoClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_privCarClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_posDClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_collisionStateClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carCtrlClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carSetupClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carSetupItemClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carPitCmdClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_situationClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_raceInfoClass));
    //ASSERT_TRUE(PyCallable_Check(pythonDriver.m_vectorClass));

}
