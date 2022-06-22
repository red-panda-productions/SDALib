#include <gtest/gtest.h>
#include "PythonDriver.inl"
#include "SDATypesConverter.h"
#include "Mocks/PointerManagerMock.h"
#include "Random.hpp"
#include "GeneratorUtils.h"
#include "SDASpeedDreams.h"

#define TEST_COUNT 10

/// @brief Checks all classes from SDATypes are callable
TEST(SDATypesConverterTests, SDATypesConverterInitTest)
{
    Py_Initialize();

    SDATypesConverter sdaTypesConverter = SDATypesConverter();

    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_sdaTypesClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_carClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_initCarClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_wheelSpecificationsClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_visualAttributesClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_publicCarClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_dynamicPointClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_trackLocationClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_trackSegmentClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_carRaceInfoClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_privCarClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_posDClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_collisionStateClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_carCtrlClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_carSetupClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_carSetupItemClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_carPitCmdClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_situationClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_raceInfoClass));
    ASSERT_TRUE(PyCallable_Check(sdaTypesConverter.m_vectorClass));
    Py_Finalize();
}

/// @brief checks the python vector object correspond to the original values
/// @param  p_x The original x value
/// @param  p_y The original y value
/// @param  p_z The original z value
/// @param  p_vecObject The vector data python object
void CheckVectorData(float p_x, float p_y, float p_z, PyObject* p_vecObject)
{
    PyObject* xAttr = PyUnicode_FromString("x");
    PyObject* xVal = PyObject_GetAttr(p_vecObject, xAttr);
    ASSERT_TRUE(static_cast<double>(p_x) == PyFloat_AsDouble(xVal));

    PyObject* yAttr = PyUnicode_FromString("y");
    PyObject* yVal = PyObject_GetAttr(p_vecObject, yAttr);
    ASSERT_TRUE(static_cast<double>(p_y) == PyFloat_AsDouble(yVal));

    PyObject* zAttr = PyUnicode_FromString("z");
    PyObject* zVal = PyObject_GetAttr(p_vecObject, zAttr);
    ASSERT_TRUE(static_cast<double>(p_z) == PyFloat_AsDouble(zVal));
}

/// @brief checks the collision state python object corresponds to the original collision state values
/// @param  p_collisionState The original collision state value
/// @param  p_collisionStateObject The python collision state object
void CheckCollisionStateObject(tCollisionState& p_collisionState, PyObject* p_collisionStateObject)
{
    PyObject* collisionCountAttr = PyUnicode_FromString("collisionCount");
    PyObject* collisionCountVal = PyObject_GetAttr(p_collisionStateObject, collisionCountAttr);
    ASSERT_TRUE(p_collisionState.collision_count == static_cast<int>(PyLong_AsLong(collisionCountVal)));

    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_collisionStateObject, posAttr);
    CheckVectorData(p_collisionState.pos[0], p_collisionState.pos[1], p_collisionState.pos[2], posVal);

    PyObject* forceAttr = PyUnicode_FromString("force");
    PyObject* forceVal = PyObject_GetAttr(p_collisionStateObject, forceAttr);
    CheckVectorData(p_collisionState.force[0], p_collisionState.force[1], p_collisionState.force[2], forceVal);
}

/// @brief checks the wheel python object corresponds to the original wheel values
/// @param  p_wheelSpec The original wheel value
/// @param  p_wheelObject The python wheel object
void CheckWheelData(tWheelSpec& p_wheelSpec, PyObject* p_wheelObject)
{
    PyObject* rimRadiusAttr = PyUnicode_FromString("rimRadius");
    PyObject* rimRadiusVal = PyObject_GetAttr(p_wheelObject, rimRadiusAttr);
    ASSERT_TRUE(static_cast<double>(p_wheelSpec.rimRadius) == PyFloat_AsDouble(rimRadiusVal));

    PyObject* tireHeightAttr = PyUnicode_FromString("tireHeight");
    PyObject* tireHeightVal = PyObject_GetAttr(p_wheelObject, tireHeightAttr);
    ASSERT_TRUE(static_cast<double>(p_wheelSpec.tireHeight) == PyFloat_AsDouble(tireHeightVal));

    PyObject* tireWidthAttr = PyUnicode_FromString("tireWidth");
    PyObject* tireWidthVal = PyObject_GetAttr(p_wheelObject, tireWidthAttr);
    ASSERT_TRUE(static_cast<double>(p_wheelSpec.tireWidth) == PyFloat_AsDouble(tireWidthVal));

    PyObject* brakeDiskRadiusAttr = PyUnicode_FromString("brakeDiskRadius");
    PyObject* brakeDiskRadiusVal = PyObject_GetAttr(p_wheelObject, brakeDiskRadiusAttr);
    ASSERT_TRUE(static_cast<double>(p_wheelSpec.brakeDiskRadius) == PyFloat_AsDouble(brakeDiskRadiusVal));

    PyObject* wheelRadiusAttr = PyUnicode_FromString("wheelRadius");
    PyObject* wheelRadiusVal = PyObject_GetAttr(p_wheelObject, wheelRadiusAttr);
    ASSERT_TRUE(static_cast<double>(p_wheelSpec.wheelRadius) == PyFloat_AsDouble(wheelRadiusVal));
}

/// @brief checks the visual attribute python object corresponds to the original visual attribute values
/// @param  p_visualAttributes The original visual attribute value
/// @param  p_visualAttributesObject The python visual attribute object
void CheckVisualAttrData(tVisualAttributes& p_visualAttributes, PyObject* p_visualAttributesObject)
{
    PyObject* exhaustNbAttr = PyUnicode_FromString("exhaustNb");
    PyObject* exhaustNbVal = PyObject_GetAttr(p_visualAttributesObject, exhaustNbAttr);
    ASSERT_TRUE(p_visualAttributes.exhaustNb == static_cast<int>(PyLong_AsLong(exhaustNbVal)));

    PyObject* exhaustPosAttr = PyUnicode_FromString("exhaustPos");
    PyObject* exhaustPosVal = PyList_AsTuple(PyObject_GetAttr(p_visualAttributesObject, exhaustPosAttr));
    CheckVectorData(p_visualAttributes.exhaustPos[0].x, p_visualAttributes.exhaustPos[0].y, p_visualAttributes.exhaustPos[0].z, PyTuple_GetItem(exhaustPosVal, 0));
    CheckVectorData(p_visualAttributes.exhaustPos[1].x, p_visualAttributes.exhaustPos[1].y, p_visualAttributes.exhaustPos[1].z, PyTuple_GetItem(exhaustPosVal, 1));

    PyObject* exhaustPowerAttr = PyUnicode_FromString("exhaustPower");
    PyObject* exhaustPowerVal = PyObject_GetAttr(p_visualAttributesObject, exhaustPowerAttr);
    ASSERT_TRUE(static_cast<double>(p_visualAttributes.exhaustPower) == PyFloat_AsDouble(exhaustPowerVal));
}

/// @brief checks the init car python object corresponds to the original init car values
/// @param  p_initCar The original init car value
/// @param  p_initCarObject The python init car object
void CheckInitCarData(tInitCar& p_initCar, PyObject* p_initCarObject)
{
    PyObject* nameAttr = PyUnicode_FromString("name");
    PyObject* nameVal = PyObject_GetAttr(p_initCarObject, nameAttr);
    ASSERT_TRUE(strcmp(p_initCar.name, PyUnicode_AsUTF8(nameVal)) == 0);

    PyObject* snameAttr = PyUnicode_FromString("sName");
    PyObject* snameVal = PyObject_GetAttr(p_initCarObject, snameAttr);
    ASSERT_TRUE(strcmp(p_initCar.sname, PyUnicode_AsUTF8(snameVal)) == 0);

    PyObject* codenameAttr = PyUnicode_FromString("codename");
    PyObject* codenameVal = PyObject_GetAttr(p_initCarObject, codenameAttr);
    ASSERT_TRUE(strcmp(p_initCar.codename, PyUnicode_AsUTF8(codenameVal)) == 0);

    PyObject* teamNameAttr = PyUnicode_FromString("teamName");
    PyObject* teamNameVal = PyObject_GetAttr(p_initCarObject, teamNameAttr);
    ASSERT_TRUE(strcmp(p_initCar.teamname, PyUnicode_AsUTF8(teamNameVal)) == 0);

    PyObject* carNameAttr = PyUnicode_FromString("carName");
    PyObject* carNameVal = PyObject_GetAttr(p_initCarObject, carNameAttr);
    ASSERT_TRUE(strcmp(p_initCar.carName, PyUnicode_AsUTF8(carNameVal)) == 0);

    PyObject* categoryAttr = PyUnicode_FromString("category");
    PyObject* categoryVal = PyObject_GetAttr(p_initCarObject, categoryAttr);
    ASSERT_TRUE(strcmp(p_initCar.category, PyUnicode_AsUTF8(categoryVal)) == 0);

    PyObject* raceNumberAttr = PyUnicode_FromString("raceNumber");
    PyObject* raceNumberVal = PyObject_GetAttr(p_initCarObject, raceNumberAttr);
    ASSERT_TRUE(p_initCar.raceNumber == static_cast<int>(PyLong_AsLong(raceNumberVal)));

    PyObject* startRankAttr = PyUnicode_FromString("startRank");
    PyObject* startRankVal = PyObject_GetAttr(p_initCarObject, startRankAttr);
    ASSERT_TRUE(p_initCar.startRank == static_cast<int>(PyLong_AsLong(startRankVal)));

    PyObject* driverTypeAttr = PyUnicode_FromString("driverType");
    PyObject* driverTypeVal = PyObject_GetAttr(p_initCarObject, driverTypeAttr);
    ASSERT_TRUE(p_initCar.driverType == static_cast<int>(PyLong_AsLong(driverTypeVal)));

    PyObject* networkPlayerAttr = PyUnicode_FromString("networkPlayer");
    PyObject* networkPlayerVal = PyObject_GetAttr(p_initCarObject, networkPlayerAttr);
    ASSERT_TRUE(p_initCar.networkplayer == static_cast<int>(PyLong_AsLong(networkPlayerVal)));

    PyObject* skillLevelAttr = PyUnicode_FromString("skillLevel");
    PyObject* skillLevelVal = PyObject_GetAttr(p_initCarObject, skillLevelAttr);
    ASSERT_TRUE(p_initCar.skillLevel == static_cast<int>(PyLong_AsLong(skillLevelVal)));

    PyObject* iconColorAttr = PyUnicode_FromString("iconColor");
    PyObject* iconColorVal = PyList_AsTuple(PyObject_GetAttr(p_initCarObject, iconColorAttr));
    ASSERT_TRUE(p_initCar.iconColor[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 0))));
    ASSERT_TRUE(p_initCar.iconColor[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 1))));
    ASSERT_TRUE(p_initCar.iconColor[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 2))));

    PyObject* dimensionAttr = PyUnicode_FromString("dimension");
    PyObject* dimensionVal = PyObject_GetAttr(p_initCarObject, dimensionAttr);
    CheckVectorData(p_initCar.dimension.x, p_initCar.dimension.y, p_initCar.dimension.z, dimensionVal);

    PyObject* drvPosAttr = PyUnicode_FromString("drvPos");
    PyObject* drvPosVal = PyObject_GetAttr(p_initCarObject, drvPosAttr);
    CheckVectorData(p_initCar.drvPos.x, p_initCar.drvPos.y, p_initCar.drvPos.z, drvPosVal);

    PyObject* bonnetPosAttr = PyUnicode_FromString("bonnetPos");
    PyObject* bonnetPosVal = PyObject_GetAttr(p_initCarObject, bonnetPosAttr);
    CheckVectorData(p_initCar.bonnetPos.x, p_initCar.bonnetPos.y, p_initCar.bonnetPos.z, bonnetPosVal);

    PyObject* tankAttr = PyUnicode_FromString("tank");
    PyObject* tankVal = PyObject_GetAttr(p_initCarObject, tankAttr);
    ASSERT_TRUE(static_cast<double>(p_initCar.tank) == PyFloat_AsDouble(tankVal));

    PyObject* steerLockAttr = PyUnicode_FromString("steerLock");
    PyObject* steerLockVal = PyObject_GetAttr(p_initCarObject, steerLockAttr);
    ASSERT_TRUE(static_cast<double>(p_initCar.steerLock) == PyFloat_AsDouble(steerLockVal));

    PyObject* statGCAttr = PyUnicode_FromString("statGC");
    PyObject* statGCVal = PyObject_GetAttr(p_initCarObject, statGCAttr);
    CheckVectorData(p_initCar.statGC.x, p_initCar.statGC.y, p_initCar.statGC.z, statGCVal);

    PyObject* wheelAttr = PyUnicode_FromString("wheel");
    PyObject* wheelVal = PyList_AsTuple(PyObject_GetAttr(p_initCarObject, wheelAttr));
    CheckWheelData(p_initCar.wheel[0], PyTuple_GetItem(wheelVal, 0));
    CheckWheelData(p_initCar.wheel[1], PyTuple_GetItem(wheelVal, 1));
    CheckWheelData(p_initCar.wheel[2], PyTuple_GetItem(wheelVal, 2));
    CheckWheelData(p_initCar.wheel[3], PyTuple_GetItem(wheelVal, 3));

    PyObject* visualAttrAttr = PyUnicode_FromString("visualAttr");
    PyObject* visualAttrVal = PyObject_GetAttr(p_initCarObject, visualAttrAttr);
    CheckVisualAttrData(p_initCar.visualAttr, visualAttrVal);

    PyObject* masterModelAttr = PyUnicode_FromString("masterModel");
    PyObject* masterModelVal = PyObject_GetAttr(p_initCarObject, masterModelAttr);
    ASSERT_TRUE(strcmp(p_initCar.masterModel, PyUnicode_AsUTF8(masterModelVal)) == 0);

    PyObject* skinNameAttr = PyUnicode_FromString("skinName");
    PyObject* skinNameVal = PyObject_GetAttr(p_initCarObject, skinNameAttr);
    ASSERT_TRUE(strcmp(p_initCar.skinName, PyUnicode_AsUTF8(skinNameVal)) == 0);

    PyObject* skinTargetsAttr = PyUnicode_FromString("skinTargets");
    PyObject* skinTargetsVal = PyObject_GetAttr(p_initCarObject, skinTargetsAttr);
    ASSERT_TRUE(p_initCar.skinTargets == static_cast<int>(PyLong_AsLong(skinTargetsVal)));
}

/// @brief checks the dynamic point python object corresponds to the original dynamic point values
/// @param  p_dynPt The original dynamic point value
/// @param  p_dynPtObject The python dynamic point object
void CheckDynamicPointData(tDynPt& p_dynPt, PyObject* p_dynPtObject)
{
    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_dynPtObject, posAttr);
    CheckVectorData(p_dynPt.pos.x, p_dynPt.pos.y, p_dynPt.pos.z, posVal);

    PyObject* velAttr = PyUnicode_FromString("vel");
    PyObject* velVal = PyObject_GetAttr(p_dynPtObject, velAttr);
    CheckVectorData(p_dynPt.vel.x, p_dynPt.vel.y, p_dynPt.vel.z, velVal);

    PyObject* accAttr = PyUnicode_FromString("acc");
    PyObject* accVal = PyObject_GetAttr(p_dynPtObject, accAttr);
    CheckVectorData(p_dynPt.acc.x, p_dynPt.acc.y, p_dynPt.acc.z, accVal);
}

/// @brief checks the matrix python object corresponds to the original matrix values
/// @param  p_posMat The original matrix value
/// @param  p_posMatObject The python matrix object
void CheckMatrixData(sgMat4& p_posMat, PyObject* p_posMatObject)
{
    PyObject* p_posMatTuple = PyList_AsTuple(p_posMatObject);
    PyObject* row1Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 0));
    PyObject* row2Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 1));
    PyObject* row3Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 2));
    PyObject* row4Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 3));

    ASSERT_TRUE(p_posMat[0][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 0))));
    ASSERT_TRUE(p_posMat[0][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 1))));
    ASSERT_TRUE(p_posMat[0][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 2))));
    ASSERT_TRUE(p_posMat[0][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 3))));

    ASSERT_TRUE(p_posMat[1][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 0))));
    ASSERT_TRUE(p_posMat[1][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 1))));
    ASSERT_TRUE(p_posMat[1][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 2))));
    ASSERT_TRUE(p_posMat[1][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 3))));

    ASSERT_TRUE(p_posMat[2][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 0))));
    ASSERT_TRUE(p_posMat[2][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 1))));
    ASSERT_TRUE(p_posMat[2][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 2))));
    ASSERT_TRUE(p_posMat[2][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 3))));

    ASSERT_TRUE(p_posMat[3][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 0))));
    ASSERT_TRUE(p_posMat[3][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 1))));
    ASSERT_TRUE(p_posMat[3][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 2))));
    ASSERT_TRUE(p_posMat[3][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 3))));
}

/// @brief checks the track segment python object corresponds to the original track segment values
/// @param  p_trackSegObject The original track segment value
/// @param  p_trackSeg The python track segment object
void CheckTrackSegmentData(tTrackSeg& p_trackSeg, PyObject* p_trackSegObject)
{
    // PyObject* idAttr = PyUnicode_FromString("trackId");
    // PyObject* idVal = PyObject_GetAttr(p_trackSegObject, idAttr);
    // ASSERT_TRUE(p_trackSeg.id == static_cast<int>(PyLong_AsLong(idVal)));

    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_trackSegObject, typeAttr);
    ASSERT_TRUE(p_trackSeg.type == static_cast<int>(PyLong_AsLong(typeVal)));

    PyObject* type2Attr = PyUnicode_FromString("type2");
    PyObject* type2Val = PyObject_GetAttr(p_trackSegObject, type2Attr);
    ASSERT_TRUE(p_trackSeg.type2 == static_cast<int>(PyLong_AsLong(type2Val)));

    PyObject* styleAttr = PyUnicode_FromString("style");
    PyObject* styleVal = PyObject_GetAttr(p_trackSegObject, styleAttr);
    ASSERT_TRUE(p_trackSeg.style == static_cast<int>(PyLong_AsLong(styleVal)));

    PyObject* lengthAttr = PyUnicode_FromString("length");
    PyObject* lengthVal = PyObject_GetAttr(p_trackSegObject, lengthAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.length) == PyFloat_AsDouble(lengthVal));

    PyObject* timeAttr = PyUnicode_FromString("time");
    PyObject* timeVal = PyObject_GetAttr(p_trackSegObject, timeAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.Time) == PyFloat_AsDouble(timeVal));

    PyObject* widthAttr = PyUnicode_FromString("width");
    PyObject* widthVal = PyObject_GetAttr(p_trackSegObject, widthAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.width) == PyFloat_AsDouble(widthVal));

    PyObject* startWidthAttr = PyUnicode_FromString("startWidth");
    PyObject* startWidthVal = PyObject_GetAttr(p_trackSegObject, startWidthAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.startWidth) == PyFloat_AsDouble(startWidthVal));

    PyObject* endWidthAttr = PyUnicode_FromString("endWidth");
    PyObject* endWidthVal = PyObject_GetAttr(p_trackSegObject, endWidthAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.endWidth) == PyFloat_AsDouble(endWidthVal));

    PyObject* lgFromStartAttr = PyUnicode_FromString("lgFromStart");
    PyObject* lgFromStartVal = PyObject_GetAttr(p_trackSegObject, lgFromStartAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.lgfromstart) == PyFloat_AsDouble(lgFromStartVal));

    PyObject* radiusAttr = PyUnicode_FromString("radius");
    PyObject* radiusVal = PyObject_GetAttr(p_trackSegObject, radiusAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.radius) == PyFloat_AsDouble(radiusVal));

    PyObject* radiusRAttr = PyUnicode_FromString("radiusR");
    PyObject* radiusRVal = PyObject_GetAttr(p_trackSegObject, radiusRAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.radiusr) == PyFloat_AsDouble(radiusRVal));

    PyObject* radiusLAttr = PyUnicode_FromString("radiusL");
    PyObject* radiusLVal = PyObject_GetAttr(p_trackSegObject, radiusLAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.radiusl) == PyFloat_AsDouble(radiusLVal));

    PyObject* arcAttr = PyUnicode_FromString("arc");
    PyObject* arcVal = PyObject_GetAttr(p_trackSegObject, arcAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.arc) == PyFloat_AsDouble(arcVal));

    PyObject* centerAttr = PyUnicode_FromString("center");
    PyObject* centerVal = PyObject_GetAttr(p_trackSegObject, centerAttr);
    CheckVectorData(p_trackSeg.center.x, p_trackSeg.center.y, p_trackSeg.center.z, centerVal);

    PyObject* vertexAttr = PyUnicode_FromString("vertex");
    PyObject* vertexVal = PyList_AsTuple(PyObject_GetAttr(p_trackSegObject, vertexAttr));
    CheckVectorData(p_trackSeg.vertex[0].x, p_trackSeg.vertex[0].y, p_trackSeg.vertex[0].z, PyTuple_GetItem(vertexVal, 0));
    CheckVectorData(p_trackSeg.vertex[1].x, p_trackSeg.vertex[1].y, p_trackSeg.vertex[1].z, PyTuple_GetItem(vertexVal, 1));
    CheckVectorData(p_trackSeg.vertex[2].x, p_trackSeg.vertex[2].y, p_trackSeg.vertex[2].z, PyTuple_GetItem(vertexVal, 2));
    CheckVectorData(p_trackSeg.vertex[3].x, p_trackSeg.vertex[3].y, p_trackSeg.vertex[3].z, PyTuple_GetItem(vertexVal, 3));

    PyObject* angleAttr = PyUnicode_FromString("angle");
    PyObject* angleVal = PyList_AsTuple(PyObject_GetAttr(p_trackSegObject, angleAttr));
    ASSERT_TRUE(p_trackSeg.angle[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 0))));
    ASSERT_TRUE(p_trackSeg.angle[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 1))));
    ASSERT_TRUE(p_trackSeg.angle[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 2))));
    ASSERT_TRUE(p_trackSeg.angle[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 3))));
    ASSERT_TRUE(p_trackSeg.angle[4] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 4))));
    ASSERT_TRUE(p_trackSeg.angle[5] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 5))));
    ASSERT_TRUE(p_trackSeg.angle[6] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 6))));

    PyObject* sinAttr = PyUnicode_FromString("sin");
    PyObject* sinVal = PyObject_GetAttr(p_trackSegObject, sinAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.sin) == PyFloat_AsDouble(sinVal));

    PyObject* cosAttr = PyUnicode_FromString("cos");
    PyObject* cosVal = PyObject_GetAttr(p_trackSegObject, cosAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.cos) == PyFloat_AsDouble(cosVal));

    PyObject* kzlAttr = PyUnicode_FromString("kzl");
    PyObject* kzlVal = PyObject_GetAttr(p_trackSegObject, kzlAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.Kzl) == PyFloat_AsDouble(kzlVal));

    PyObject* kzwAttr = PyUnicode_FromString("kzw");
    PyObject* kzwVal = PyObject_GetAttr(p_trackSegObject, kzwAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.Kzw) == PyFloat_AsDouble(kzwVal));

    PyObject* kylAttr = PyUnicode_FromString("kyl");
    PyObject* kylVal = PyObject_GetAttr(p_trackSegObject, kylAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.Kyl) == PyFloat_AsDouble(kylVal));

    PyObject* rgtSideNormalAttr = PyUnicode_FromString("rgtSideNormal");
    PyObject* rgtSideNormalVal = PyObject_GetAttr(p_trackSegObject, rgtSideNormalAttr);
    CheckVectorData(p_trackSeg.rgtSideNormal.x, p_trackSeg.rgtSideNormal.y, p_trackSeg.rgtSideNormal.z, rgtSideNormalVal);

    PyObject* envIndexAttr = PyUnicode_FromString("envIndex");
    PyObject* envIndexVal = PyObject_GetAttr(p_trackSegObject, envIndexAttr);
    ASSERT_TRUE(p_trackSeg.envIndex == static_cast<int>(PyLong_AsLong(envIndexVal)));

    PyObject* heightAttr = PyUnicode_FromString("height");
    PyObject* heightVal = PyObject_GetAttr(p_trackSegObject, heightAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.height) == PyFloat_AsDouble(heightVal));

    PyObject* raceInfoAttr = PyUnicode_FromString("raceInfo");
    PyObject* raceInfoVal = PyObject_GetAttr(p_trackSegObject, raceInfoAttr);
    ASSERT_TRUE(p_trackSeg.raceInfo == static_cast<int>(PyLong_AsLong(raceInfoVal)));

    PyObject* doVFactorAttr = PyUnicode_FromString("doVFactor");
    PyObject* doVFactorVal = PyObject_GetAttr(p_trackSegObject, doVFactorAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.DoVfactor) == PyFloat_AsDouble(doVFactorVal));

    PyObject* speedLimitAttr = PyUnicode_FromString("speedLimit");
    PyObject* speedLimitVal = PyObject_GetAttr(p_trackSegObject, speedLimitAttr);
    ASSERT_TRUE(static_cast<double>(p_trackSeg.SpeedLimit) == PyFloat_AsDouble(speedLimitVal));
}

/// @brief checks the track location python object corresponds to the original track location values
/// @param  p_trackLocObject The original track location value
/// @param  p_trackLoc The python track location object
void CheckTrackLocationData(tTrkLocPos& p_trackLoc, PyObject* p_trackLocObject)
{
    PyObject* segAttr = PyUnicode_FromString("seg");
    PyObject* segVal = PyObject_GetAttr(p_trackLocObject, segAttr);
    CheckTrackSegmentData(*p_trackLoc.seg, segVal);

    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_trackLocObject, typeAttr);
    ASSERT_TRUE(p_trackLoc.type == static_cast<int>(PyLong_AsLong(typeVal)));

    PyObject* toStartAttr = PyUnicode_FromString("toStart");
    PyObject* toStartVal = PyObject_GetAttr(p_trackLocObject, toStartAttr);
    ASSERT_TRUE(static_cast<double>(p_trackLoc.toStart) == PyFloat_AsDouble(toStartVal));

    PyObject* toRightAttr = PyUnicode_FromString("toRight");
    PyObject* toRightVal = PyObject_GetAttr(p_trackLocObject, toRightAttr);
    ASSERT_TRUE(static_cast<double>(p_trackLoc.toRight) == PyFloat_AsDouble(toRightVal));

    PyObject* toMiddleAttr = PyUnicode_FromString("toMiddle");
    PyObject* toMiddleVal = PyObject_GetAttr(p_trackLocObject, toMiddleAttr);
    ASSERT_TRUE(static_cast<double>(p_trackLoc.toMiddle) == PyFloat_AsDouble(toMiddleVal));

    PyObject* toLeftAttr = PyUnicode_FromString("toLeft");
    PyObject* toLeftVal = PyObject_GetAttr(p_trackLocObject, toLeftAttr);
    ASSERT_TRUE(static_cast<double>(p_trackLoc.toLeft) == PyFloat_AsDouble(toLeftVal));
}

/// @brief checks the public car python object corresponds to the original public car values
/// @param  p_publicCar The original public car value
/// @param  p_publicCarObject The python public car object
void CheckPublicCarData(tPublicCar& p_publicCar, PyObject* p_publicCarObject)
{
    PyObject* dynGCAttr = PyUnicode_FromString("dynGC");
    PyObject* dynGCVal = PyObject_GetAttr(p_publicCarObject, dynGCAttr);
    CheckDynamicPointData(p_publicCar.DynGC, dynGCVal);

    PyObject* dynGCgAttr = PyUnicode_FromString("dynGCg");
    PyObject* dynGCgVal = PyObject_GetAttr(p_publicCarObject, dynGCgAttr);
    CheckDynamicPointData(p_publicCar.DynGCg, dynGCgVal);

    PyObject* speedAttr = PyUnicode_FromString("speed");
    PyObject* speedVal = PyObject_GetAttr(p_publicCarObject, speedAttr);
    ASSERT_TRUE(static_cast<double>(p_publicCar.speed) == PyFloat_AsDouble(speedVal));

    PyObject* posMatAttr = PyUnicode_FromString("posMat");
    PyObject* posMatVal = PyObject_GetAttr(p_publicCarObject, posMatAttr);
    CheckMatrixData(p_publicCar.posMat, posMatVal);

    PyObject* trkPosAttr = PyUnicode_FromString("trkPos");
    PyObject* trkPosVal = PyObject_GetAttr(p_publicCarObject, trkPosAttr);
    CheckTrackLocationData(p_publicCar.trkPos, trkPosVal);

    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_publicCarObject, stateAttr);
    ASSERT_TRUE(p_publicCar.state == static_cast<int>(PyLong_AsLong(stateVal)));
}

/// @brief checks the car race info python object corresponds to the original car race info values
/// @param  p_carRaceInfo The original car race info value
/// @param  p_carRaceInfoObject The python car race info object
void CheckCarRaceInfoData(tCarRaceInfo& p_carRaceInfo, PyObject* p_carRaceInfoObject)
{
    PyObject* bestLapTimeAttr = PyUnicode_FromString("bestLapTime");
    PyObject* bestLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, bestLapTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.bestLapTime == static_cast<float>(PyFloat_AsDouble(bestLapTimeVal)));

    PyObject* commitBestLapTimeAttr = PyUnicode_FromString("commitBestLapTime");
    PyObject* commitBestLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, commitBestLapTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.commitBestLapTime == static_cast<bool>(PyLong_AsLong(commitBestLapTimeVal)));

    PyObject* deltaBestLapTimeAttr = PyUnicode_FromString("deltaBestLapTime");
    PyObject* deltaBestLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, deltaBestLapTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.deltaBestLapTime == static_cast<float>(PyFloat_AsDouble(deltaBestLapTimeVal)));

    PyObject* curLapTimeAttr = PyUnicode_FromString("curLapTime");
    PyObject* curLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, curLapTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.curLapTime == static_cast<float>(PyFloat_AsDouble(curLapTimeVal)));

    PyObject* lastLapTimeAttr = PyUnicode_FromString("lastLapTime");
    PyObject* lastLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, lastLapTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.lastLapTime == static_cast<float>(PyFloat_AsDouble(lastLapTimeVal)));

    PyObject* curTimeAttr = PyUnicode_FromString("curTime");
    PyObject* curTimeVal = PyObject_GetAttr(p_carRaceInfoObject, curTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.curTime == static_cast<float>(PyFloat_AsDouble(curTimeVal)));

    PyObject* topSpeedAttr = PyUnicode_FromString("topSpeed");
    PyObject* topSpeedVal = PyObject_GetAttr(p_carRaceInfoObject, topSpeedAttr);
    ASSERT_TRUE(p_carRaceInfo.topSpeed == static_cast<float>(PyFloat_AsDouble(topSpeedVal)));

    PyObject* currentMinSpeedForLapAttr = PyUnicode_FromString("currentMinSpeedForLap");
    PyObject* currentMinSpeedForLapVal = PyObject_GetAttr(p_carRaceInfoObject, currentMinSpeedForLapAttr);
    ASSERT_TRUE(p_carRaceInfo.currentMinSpeedForLap == static_cast<float>(PyFloat_AsDouble(currentMinSpeedForLapVal)));

    PyObject* lapsAttr = PyUnicode_FromString("laps");
    PyObject* lapsVal = PyObject_GetAttr(p_carRaceInfoObject, lapsAttr);
    ASSERT_TRUE(p_carRaceInfo.laps == static_cast<int>(PyLong_AsLong(lapsVal)));

    PyObject* bestLapAttr = PyUnicode_FromString("bestLap");
    PyObject* bestLapVal = PyObject_GetAttr(p_carRaceInfoObject, bestLapAttr);
    ASSERT_TRUE(p_carRaceInfo.bestLap == static_cast<int>(PyLong_AsLong(bestLapVal)));

    PyObject* nbPitStopsAttr = PyUnicode_FromString("nbPitStops");
    PyObject* nbPitStopsVal = PyObject_GetAttr(p_carRaceInfoObject, nbPitStopsAttr);
    ASSERT_TRUE(p_carRaceInfo.nbPitStops == static_cast<int>(PyLong_AsLong(nbPitStopsVal)));

    PyObject* remainingLapsAttr = PyUnicode_FromString("remainingLaps");
    PyObject* remainingLapsVal = PyObject_GetAttr(p_carRaceInfoObject, remainingLapsAttr);
    ASSERT_TRUE(p_carRaceInfo.remainingLaps == static_cast<int>(PyLong_AsLong(remainingLapsVal)));

    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_carRaceInfoObject, posAttr);
    ASSERT_TRUE(p_carRaceInfo.pos == static_cast<int>(PyLong_AsLong(posVal)));

    PyObject* timeBehindLeaderAttr = PyUnicode_FromString("timeBehindLeader");
    PyObject* timeBehindLeaderVal = PyObject_GetAttr(p_carRaceInfoObject, timeBehindLeaderAttr);
    ASSERT_TRUE(p_carRaceInfo.timeBehindLeader == static_cast<float>(PyFloat_AsDouble(timeBehindLeaderVal)));

    PyObject* lapsBehindLeaderAttr = PyUnicode_FromString("lapsBehindLeader");
    PyObject* lapsBehindLeaderVal = PyObject_GetAttr(p_carRaceInfoObject, lapsBehindLeaderAttr);
    ASSERT_TRUE(p_carRaceInfo.lapsBehindLeader == static_cast<int>(PyLong_AsLong(lapsBehindLeaderVal)));

    PyObject* timeBehindPrevAttr = PyUnicode_FromString("timeBehindPrev");
    PyObject* timeBehindPrevVal = PyObject_GetAttr(p_carRaceInfoObject, timeBehindPrevAttr);
    ASSERT_TRUE(p_carRaceInfo.timeBehindPrev == static_cast<float>(PyFloat_AsDouble(timeBehindPrevVal)));

    PyObject* timeBeforeNextAttr = PyUnicode_FromString("timeBeforeNext");
    PyObject* timeBeforeNextVal = PyObject_GetAttr(p_carRaceInfoObject, timeBeforeNextAttr);
    ASSERT_TRUE(p_carRaceInfo.timeBeforeNext == static_cast<float>(PyFloat_AsDouble(timeBeforeNextVal)));

    PyObject* distRacedAttr = PyUnicode_FromString("distRaced");
    PyObject* distRacedVal = PyObject_GetAttr(p_carRaceInfoObject, distRacedAttr);
    ASSERT_TRUE(p_carRaceInfo.distRaced == static_cast<float>(PyFloat_AsDouble(distRacedVal)));

    PyObject* distFromStartLineAttr = PyUnicode_FromString("distFromStartLine");
    PyObject* distFromStartLineVal = PyObject_GetAttr(p_carRaceInfoObject, distFromStartLineAttr);
    ASSERT_TRUE(p_carRaceInfo.distFromStartLine == static_cast<float>(PyFloat_AsDouble(distFromStartLineVal)));

    PyObject* currentSectorAttr = PyUnicode_FromString("currentSector");
    PyObject* currentSectorVal = PyObject_GetAttr(p_carRaceInfoObject, currentSectorAttr);
    ASSERT_TRUE(p_carRaceInfo.currentSector == static_cast<int>(PyLong_AsLong(currentSectorVal)));

    PyObject* nbSectorsAttr = PyUnicode_FromString("nbSectors");
    PyObject* nbSectorsVal = PyObject_GetAttr(p_carRaceInfoObject, nbSectorsAttr);
    ASSERT_TRUE(p_carRaceInfo.nbSectors == static_cast<int>(PyLong_AsLong(nbSectorsVal)));

    PyObject* trackLengthAttr = PyUnicode_FromString("trackLength");
    PyObject* trackLengthVal = PyObject_GetAttr(p_carRaceInfoObject, trackLengthAttr);
    ASSERT_TRUE(p_carRaceInfo.trackLength == static_cast<float>(PyFloat_AsDouble(trackLengthVal)));

    PyObject* scheduledEventTimeAttr = PyUnicode_FromString("scheduledEventTime");
    PyObject* scheduledEventTimeVal = PyObject_GetAttr(p_carRaceInfoObject, scheduledEventTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.scheduledEventTime == static_cast<float>(PyFloat_AsDouble(scheduledEventTimeVal)));

    PyObject* eventAttr = PyUnicode_FromString("event");
    PyObject* eventVal = PyObject_GetAttr(p_carRaceInfoObject, eventAttr);
    ASSERT_TRUE(p_carRaceInfo.event == static_cast<int>(PyLong_AsLong(eventVal)));

    PyObject* penaltyTimeAttr = PyUnicode_FromString("penaltyTime");
    PyObject* penaltyTimeVal = PyObject_GetAttr(p_carRaceInfoObject, penaltyTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.penaltyTime == static_cast<float>(PyFloat_AsDouble(penaltyTimeVal)));

    PyObject* prevFromStartLineAttr = PyUnicode_FromString("prevFromStartLine");
    PyObject* prevFromStartLineVal = PyObject_GetAttr(p_carRaceInfoObject, prevFromStartLineAttr);
    ASSERT_TRUE(p_carRaceInfo.prevFromStartLine == static_cast<float>(PyFloat_AsDouble(prevFromStartLineVal)));

    PyObject* wrongWayTimeAttr = PyUnicode_FromString("wrongWayTime");
    PyObject* wrongWayTimeVal = PyObject_GetAttr(p_carRaceInfoObject, wrongWayTimeAttr);
    ASSERT_TRUE(p_carRaceInfo.wrongWayTime == static_cast<float>(PyFloat_AsDouble(wrongWayTimeVal)));
}

/// @brief checks the posD python object corresponds to the original posD values
/// @param  p_posD The original posD value
/// @param  p_posD The python posD object
void CheckPosDObject(tPosd& p_posD, PyObject* p_posDObject)
{
    PyObject* xAttr = PyUnicode_FromString("x");
    PyObject* xVal = PyObject_GetAttr(p_posDObject, xAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.x) == PyFloat_AsDouble(xVal));

    PyObject* yAttr = PyUnicode_FromString("y");
    PyObject* yVal = PyObject_GetAttr(p_posDObject, yAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.y) == PyFloat_AsDouble(yVal));

    PyObject* zAttr = PyUnicode_FromString("z");
    PyObject* zVal = PyObject_GetAttr(p_posDObject, zAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.z) == PyFloat_AsDouble(zVal));

    PyObject* xyAttr = PyUnicode_FromString("xy");
    PyObject* xyVal = PyObject_GetAttr(p_posDObject, xyAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.xy) == PyFloat_AsDouble(xyVal));

    PyObject* axAttr = PyUnicode_FromString("ax");
    PyObject* axVal = PyObject_GetAttr(p_posDObject, axAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.ax) == PyFloat_AsDouble(axVal));

    PyObject* ayAttr = PyUnicode_FromString("ay");
    PyObject* ayVal = PyObject_GetAttr(p_posDObject, ayAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.ay) == PyFloat_AsDouble(ayVal));

    PyObject* azAttr = PyUnicode_FromString("az");
    PyObject* azVal = PyObject_GetAttr(p_posDObject, azAttr);
    ASSERT_TRUE(static_cast<double>(p_posD.az) == PyFloat_AsDouble(azVal));
}

/// @brief checks the private car python object corresponds to the original private car values
/// @param  p_privCar The original private car value
/// @param  p_privCarObject The python private car object
void CheckPrivCarData(tPrivCar& p_privCar, PyObject* p_privCarObject)
{
    PyObject* driverIndexAttr = PyUnicode_FromString("driverIndex");
    PyObject* driverIndexVal = PyObject_GetAttr(p_privCarObject, driverIndexAttr);
    ASSERT_TRUE(p_privCar.driverIndex == static_cast<int>(PyLong_AsLong(driverIndexVal)));

    PyObject* moduleIndexAttr = PyUnicode_FromString("moduleIndex");
    PyObject* moduleIndexVal = PyObject_GetAttr(p_privCarObject, moduleIndexAttr);
    ASSERT_TRUE(p_privCar.moduleIndex == static_cast<int>(PyLong_AsLong(moduleIndexVal)));

    PyObject* modNameAttr = PyUnicode_FromString("modName");
    PyObject* modNameVal = PyObject_GetAttr(p_privCarObject, modNameAttr);
    ASSERT_TRUE(strcmp(p_privCar.modName, PyUnicode_AsUTF8(modNameVal)) == 0);

    PyObject* cornerAttr = PyUnicode_FromString("corner");
    PyObject* cornerVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, cornerAttr));
    CheckPosDObject(p_privCar.corner[0], PyTuple_GetItem(cornerVal, 0));
    CheckPosDObject(p_privCar.corner[1], PyTuple_GetItem(cornerVal, 1));
    CheckPosDObject(p_privCar.corner[2], PyTuple_GetItem(cornerVal, 2));
    CheckPosDObject(p_privCar.corner[3], PyTuple_GetItem(cornerVal, 3));

    PyObject* gearAttr = PyUnicode_FromString("gear");
    PyObject* gearVal = PyObject_GetAttr(p_privCarObject, gearAttr);
    ASSERT_TRUE(p_privCar.gear == static_cast<int>(PyLong_AsLong(gearVal)));

    PyObject* gearNextAttr = PyUnicode_FromString("gearNext");
    PyObject* gearNextVal = PyObject_GetAttr(p_privCarObject, gearNextAttr);
    ASSERT_TRUE(p_privCar.gearNext == static_cast<int>(PyLong_AsLong(gearNextVal)));

    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_privCarObject, fuelAttr);
    ASSERT_TRUE(p_privCar.fuel == static_cast<float>(PyFloat_AsDouble(fuelVal)));

    PyObject* fuelConsumptionTotalAttr = PyUnicode_FromString("fuelConsumptionTotal");
    PyObject* fuelConsumptionTotalVal = PyObject_GetAttr(p_privCarObject, fuelConsumptionTotalAttr);
    ASSERT_TRUE(p_privCar.fuel_consumption_total == static_cast<float>(PyFloat_AsDouble(fuelConsumptionTotalVal)));

    PyObject* fuelConsumptionInstantAttr = PyUnicode_FromString("fuelConsumptionInstant");
    PyObject* fuelConsumptionInstantVal = PyObject_GetAttr(p_privCarObject, fuelConsumptionInstantAttr);
    ASSERT_TRUE(p_privCar.fuel_consumption_instant == static_cast<float>(PyFloat_AsDouble(fuelConsumptionInstantVal)));

    PyObject* engineRPMAttr = PyUnicode_FromString("engineRPM");
    PyObject* engineRPMVal = PyObject_GetAttr(p_privCarObject, engineRPMAttr);
    ASSERT_TRUE(p_privCar.enginerpm == static_cast<float>(PyFloat_AsDouble(engineRPMVal)));

    PyObject* engineRPMRedLineAttr = PyUnicode_FromString("engineRPMRedLine");
    PyObject* engineRPMRedLineVal = PyObject_GetAttr(p_privCarObject, engineRPMRedLineAttr);
    ASSERT_TRUE(p_privCar.enginerpmRedLine == static_cast<float>(PyFloat_AsDouble(engineRPMRedLineVal)));

    PyObject* engineRPMMaxAttr = PyUnicode_FromString("engineRPMMax");
    PyObject* engineRPMMaxVal = PyObject_GetAttr(p_privCarObject, engineRPMMaxAttr);
    ASSERT_TRUE(p_privCar.enginerpmMax == static_cast<float>(PyFloat_AsDouble(engineRPMMaxVal)));

    PyObject* engineRPMMaxTqAttr = PyUnicode_FromString("engineRPMMaxTq");
    PyObject* engineRPMMaxTqVal = PyObject_GetAttr(p_privCarObject, engineRPMMaxTqAttr);
    ASSERT_TRUE(p_privCar.enginerpmMaxTq == static_cast<float>(PyFloat_AsDouble(engineRPMMaxTqVal)));

    PyObject* engineRPMMaxPwAttr = PyUnicode_FromString("engineRPMMaxPw");
    PyObject* engineRPMMaxPwVal = PyObject_GetAttr(p_privCarObject, engineRPMMaxPwAttr);
    ASSERT_TRUE(p_privCar.enginerpmMaxPw == static_cast<float>(PyFloat_AsDouble(engineRPMMaxPwVal)));

    PyObject* engineMaxTqAttr = PyUnicode_FromString("engineMaxTq");
    PyObject* engineMaxTqVal = PyObject_GetAttr(p_privCarObject, engineMaxTqAttr);
    ASSERT_TRUE(p_privCar.engineMaxTq == static_cast<float>(PyFloat_AsDouble(engineMaxTqVal)));

    PyObject* engineMaxPwAttr = PyUnicode_FromString("engineMaxPw");
    PyObject* engineMaxPwVal = PyObject_GetAttr(p_privCarObject, engineMaxPwAttr);
    ASSERT_TRUE(p_privCar.engineMaxPw == static_cast<float>(PyFloat_AsDouble(engineMaxPwVal)));

    PyObject* gearRatioAttr = PyUnicode_FromString("gearRatio");
    PyObject* gearRatioVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, gearRatioAttr));
    ASSERT_TRUE(p_privCar.gearRatio[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 0))));
    ASSERT_TRUE(p_privCar.gearRatio[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 1))));
    ASSERT_TRUE(p_privCar.gearRatio[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 2))));
    ASSERT_TRUE(p_privCar.gearRatio[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 3))));
    ASSERT_TRUE(p_privCar.gearRatio[4] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 4))));
    ASSERT_TRUE(p_privCar.gearRatio[5] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 5))));
    ASSERT_TRUE(p_privCar.gearRatio[6] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 6))));
    ASSERT_TRUE(p_privCar.gearRatio[7] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 7))));
    ASSERT_TRUE(p_privCar.gearRatio[8] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 8))));
    ASSERT_TRUE(p_privCar.gearRatio[9] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 9))));

    PyObject* gearNbAttr = PyUnicode_FromString("gearNb");
    PyObject* gearNbVal = PyObject_GetAttr(p_privCarObject, gearNbAttr);
    ASSERT_TRUE(p_privCar.gearNb == static_cast<int>(PyLong_AsLong(gearNbVal)));

    PyObject* gearOffsetAttr = PyUnicode_FromString("gearOffset");
    PyObject* gearOffsetVal = PyObject_GetAttr(p_privCarObject, gearOffsetAttr);
    ASSERT_TRUE(p_privCar.gearOffset == static_cast<int>(PyLong_AsLong(gearOffsetVal)));

    PyObject* skidAttr = PyUnicode_FromString("skid");
    PyObject* skidVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, skidAttr));
    ASSERT_TRUE(p_privCar.skid[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 0))));
    ASSERT_TRUE(p_privCar.skid[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 1))));
    ASSERT_TRUE(p_privCar.skid[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 2))));
    ASSERT_TRUE(p_privCar.skid[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 3))));

    PyObject* reactionAttr = PyUnicode_FromString("reaction");
    PyObject* reactionVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, reactionAttr));
    ASSERT_TRUE(p_privCar.reaction[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 0))));
    ASSERT_TRUE(p_privCar.reaction[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 1))));
    ASSERT_TRUE(p_privCar.reaction[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 2))));
    ASSERT_TRUE(p_privCar.reaction[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 3))));

    PyObject* collisionAttr = PyUnicode_FromString("collision");
    PyObject* collisionVal = PyObject_GetAttr(p_privCarObject, collisionAttr);
    ASSERT_TRUE(p_privCar.collision == static_cast<int>(PyLong_AsLong(collisionVal)));

    PyObject* simCollisionAttr = PyUnicode_FromString("simCollision");
    PyObject* simCollisionVal = PyObject_GetAttr(p_privCarObject, simCollisionAttr);
    ASSERT_TRUE(p_privCar.simcollision == static_cast<int>(PyLong_AsLong(simCollisionVal)));

    PyObject* smokeAttr = PyUnicode_FromString("smoke");
    PyObject* smokeVal = PyObject_GetAttr(p_privCarObject, smokeAttr);
    ASSERT_TRUE(p_privCar.smoke == static_cast<float>(PyFloat_AsDouble(smokeVal)));

    PyObject* normalAttr = PyUnicode_FromString("normal");
    PyObject* normalVal = PyObject_GetAttr(p_privCarObject, normalAttr);
    CheckVectorData(p_privCar.normal.x, p_privCar.normal.y, p_privCar.normal.z, normalVal);

    PyObject* collPosAttr = PyUnicode_FromString("collPos");
    PyObject* collPosVal = PyObject_GetAttr(p_privCarObject, collPosAttr);
    CheckVectorData(p_privCar.collpos.x, p_privCar.collpos.y, p_privCar.collpos.z, collPosVal);

    PyObject* damageAttr = PyUnicode_FromString("damage");
    PyObject* damageVal = PyObject_GetAttr(p_privCarObject, damageAttr);
    ASSERT_TRUE(p_privCar.dammage == static_cast<int>(PyLong_AsLong(damageVal)));

    PyObject* debugAttr = PyUnicode_FromString("debug");
    PyObject* debugVal = PyObject_GetAttr(p_privCarObject, debugAttr);
    ASSERT_TRUE(p_privCar.debug == static_cast<int>(PyLong_AsLong(debugVal)));

    PyObject* collisionStateAttr = PyUnicode_FromString("collisionState");
    PyObject* collisionStateVal = PyObject_GetAttr(p_privCarObject, collisionStateAttr);
    CheckCollisionStateObject(p_privCar.collision_state, collisionStateVal);

    PyObject* localPressureAttr = PyUnicode_FromString("localPressure");
    PyObject* localPressureVal = PyObject_GetAttr(p_privCarObject, localPressureAttr);
    ASSERT_TRUE(p_privCar.localPressure == static_cast<float>(PyFloat_AsDouble(localPressureVal)));

    PyObject* driveSkillAttr = PyUnicode_FromString("driveSkill");
    PyObject* driveSkillVal = PyObject_GetAttr(p_privCarObject, driveSkillAttr);
    ASSERT_TRUE(p_privCar.driveSkill == static_cast<float>(PyFloat_AsDouble(driveSkillVal)));

    PyObject* steerTqCenterAttr = PyUnicode_FromString("steerTqCenter");
    PyObject* steerTqCenterVal = PyObject_GetAttr(p_privCarObject, steerTqCenterAttr);
    ASSERT_TRUE(p_privCar.steerTqCenter == static_cast<float>(PyFloat_AsDouble(steerTqCenterVal)));

    PyObject* steerTqAlignAttr = PyUnicode_FromString("steerTqAlign");
    PyObject* steerTqAlignVal = PyObject_GetAttr(p_privCarObject, steerTqAlignAttr);
    ASSERT_TRUE(p_privCar.steerTqAlign == static_cast<float>(PyFloat_AsDouble(steerTqAlignVal)));

    PyObject* dashboardInstantNbAttr = PyUnicode_FromString("dashboardInstantNb");
    PyObject* dashboardInstantNbVal = PyObject_GetAttr(p_privCarObject, dashboardInstantNbAttr);
    ASSERT_TRUE(p_privCar.dashboardInstantNb == static_cast<int>(PyLong_AsLong(dashboardInstantNbVal)));

    PyObject* dashboardRequestNbAttr = PyUnicode_FromString("dashboardRequestNb");
    PyObject* dashboardRequestNbVal = PyObject_GetAttr(p_privCarObject, dashboardRequestNbAttr);
    ASSERT_TRUE(p_privCar.dashboardRequestNb == static_cast<int>(PyLong_AsLong(dashboardRequestNbVal)));

    PyObject* dashboardActiveItemAttr = PyUnicode_FromString("dashboardActiveItem");
    PyObject* dashboardActiveItemVal = PyObject_GetAttr(p_privCarObject, dashboardActiveItemAttr);
    ASSERT_TRUE(p_privCar.dashboardActiveItem == static_cast<int>(PyLong_AsLong(dashboardActiveItemVal)));
}

/// @brief checks the car control python object corresponds to the original car control values
/// @param  p_carCtrl The original car control value
/// @param  p_carCtrlObject The python car control object
void CheckCarCtrlData(tCarCtrl& p_carCtrl, PyObject* p_carCtrlObject)
{
    PyObject* steerAttr = PyUnicode_FromString("steer");
    PyObject* steerVal = PyObject_GetAttr(p_carCtrlObject, steerAttr);
    ASSERT_TRUE(p_carCtrl.steer == static_cast<float>(PyFloat_AsDouble(steerVal)));

    PyObject* accelCmdAttr = PyUnicode_FromString("accelCmd");
    PyObject* accelCmdVal = PyObject_GetAttr(p_carCtrlObject, accelCmdAttr);
    ASSERT_TRUE(p_carCtrl.accelCmd == static_cast<float>(PyFloat_AsDouble(accelCmdVal)));

    PyObject* brakeCmdAttr = PyUnicode_FromString("brakeCmd");
    PyObject* brakeCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeCmdAttr);
    ASSERT_TRUE(p_carCtrl.brakeCmd == static_cast<float>(PyFloat_AsDouble(brakeCmdVal)));

    PyObject* clutchCmdAttr = PyUnicode_FromString("clutchCmd");
    PyObject* clutchCmdVal = PyObject_GetAttr(p_carCtrlObject, clutchCmdAttr);
    ASSERT_TRUE(p_carCtrl.clutchCmd == static_cast<float>(PyFloat_AsDouble(clutchCmdVal)));

    PyObject* brakeFrontLeftCmdAttr = PyUnicode_FromString("brakeFrontLeftCmd");
    PyObject* brakeFrontLeftCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeFrontLeftCmdAttr);
    ASSERT_TRUE(p_carCtrl.brakeFrontLeftCmd == static_cast<float>(PyFloat_AsDouble(brakeFrontLeftCmdVal)));

    PyObject* brakeFrontRightCmdAttr = PyUnicode_FromString("brakeFrontRightCmd");
    PyObject* brakeFrontRightCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeFrontRightCmdAttr);
    ASSERT_TRUE(p_carCtrl.brakeFrontRightCmd == static_cast<float>(PyFloat_AsDouble(brakeFrontRightCmdVal)));

    PyObject* brakeRearLeftCmdAttr = PyUnicode_FromString("brakeRearLeftCmd");
    PyObject* brakeRearLeftCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeRearLeftCmdAttr);
    ASSERT_TRUE(p_carCtrl.brakeRearLeftCmd == static_cast<float>(PyFloat_AsDouble(brakeRearLeftCmdVal)));

    PyObject* brakeRearRightCmdAttr = PyUnicode_FromString("brakeRearRightCmd");
    PyObject* brakeRearRightCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeRearRightCmdAttr);
    ASSERT_TRUE(p_carCtrl.brakeRearRightCmd == static_cast<float>(PyFloat_AsDouble(brakeRearRightCmdVal)));

    PyObject* wingFrontCmdAttr = PyUnicode_FromString("wingFrontCmd");
    PyObject* wingFrontCmdVal = PyObject_GetAttr(p_carCtrlObject, wingFrontCmdAttr);
    ASSERT_TRUE(p_carCtrl.wingFrontCmd == static_cast<float>(PyFloat_AsDouble(wingFrontCmdVal)));

    PyObject* wingRearCmdAttr = PyUnicode_FromString("wingRearCmd");
    PyObject* wingRearCmdVal = PyObject_GetAttr(p_carCtrlObject, wingRearCmdAttr);
    ASSERT_TRUE(p_carCtrl.wingRearCmd == static_cast<float>(PyFloat_AsDouble(wingRearCmdVal)));

    PyObject* reserved1Attr = PyUnicode_FromString("reserved1");
    PyObject* reserved1Val = PyObject_GetAttr(p_carCtrlObject, reserved1Attr);
    ASSERT_TRUE(p_carCtrl.reserved1 == static_cast<float>(PyFloat_AsDouble(reserved1Val)));

    PyObject* reserved2Attr = PyUnicode_FromString("reserved2");
    PyObject* reserved2Val = PyObject_GetAttr(p_carCtrlObject, reserved2Attr);
    ASSERT_TRUE(p_carCtrl.reserved2 == static_cast<float>(PyFloat_AsDouble(reserved2Val)));

    PyObject* gearAttr = PyUnicode_FromString("gear");
    PyObject* gearVal = PyObject_GetAttr(p_carCtrlObject, gearAttr);
    ASSERT_TRUE(p_carCtrl.gear == static_cast<int>(PyLong_AsLong(gearVal)));

    PyObject* raceCmdAttr = PyUnicode_FromString("raceCmd");
    PyObject* raceCmdVal = PyObject_GetAttr(p_carCtrlObject, raceCmdAttr);
    ASSERT_TRUE(p_carCtrl.raceCmd == static_cast<int>(PyLong_AsLong(raceCmdVal)));

    PyObject* lightCmdAttr = PyUnicode_FromString("lightCmd");
    PyObject* lightCmdVal = PyObject_GetAttr(p_carCtrlObject, lightCmdAttr);
    ASSERT_TRUE(p_carCtrl.lightCmd == static_cast<int>(PyLong_AsLong(lightCmdVal)));

    PyObject* eBrakeCmdAttr = PyUnicode_FromString("eBrakeCmd");
    PyObject* eBrakeCmdVal = PyObject_GetAttr(p_carCtrlObject, eBrakeCmdAttr);
    ASSERT_TRUE(p_carCtrl.ebrakeCmd == static_cast<int>(PyLong_AsLong(eBrakeCmdVal)));

    PyObject* wingControlModeAttr = PyUnicode_FromString("wingControlMode");
    PyObject* wingControlModeVal = PyObject_GetAttr(p_carCtrlObject, wingControlModeAttr);
    ASSERT_TRUE(p_carCtrl.wingControlMode == static_cast<int>(PyLong_AsLong(wingControlModeVal)));

    PyObject* singleWheelBrakeModeAttr = PyUnicode_FromString("singleWheelBrakeMode");
    PyObject* singleWheelBrakeModeVal = PyObject_GetAttr(p_carCtrlObject, singleWheelBrakeModeAttr);
    ASSERT_TRUE(p_carCtrl.singleWheelBrakeMode == static_cast<int>(PyLong_AsLong(singleWheelBrakeModeVal)));

    PyObject* switch3Attr = PyUnicode_FromString("switch3");
    PyObject* switch3Val = PyObject_GetAttr(p_carCtrlObject, switch3Attr);
    ASSERT_TRUE(p_carCtrl.switch3 == static_cast<int>(PyLong_AsLong(switch3Val)));

    PyObject* telemetryModeAttr = PyUnicode_FromString("telemetryMode");
    PyObject* telemetryModeVal = PyObject_GetAttr(p_carCtrlObject, telemetryModeAttr);
    ASSERT_TRUE(p_carCtrl.telemetryMode == static_cast<int>(PyLong_AsLong(telemetryModeVal)));

    PyObject* msgAttr = PyUnicode_FromString("msg");
    PyObject* msgVal = PyList_AsTuple(PyObject_GetAttr(p_carCtrlObject, msgAttr));
    ASSERT_TRUE(strcmp(p_carCtrl.msg[0], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 0))) == 0);
    ASSERT_TRUE(strcmp(p_carCtrl.msg[1], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 1))) == 0);
    ASSERT_TRUE(strcmp(p_carCtrl.msg[2], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 2))) == 0);
    ASSERT_TRUE(strcmp(p_carCtrl.msg[3], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 3))) == 0);

    PyObject* msgColorAttr = PyUnicode_FromString("msgColor");
    PyObject* msgColorVal = PyList_AsTuple(PyObject_GetAttr(p_carCtrlObject, msgColorAttr));
    PyObject* test1 = PyTuple_GetItem(msgColorVal, 0);
    ASSERT_TRUE(p_carCtrl.msgColor[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 0))));
    ASSERT_TRUE(p_carCtrl.msgColor[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 1))));
    ASSERT_TRUE(p_carCtrl.msgColor[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 2))));
    ASSERT_TRUE(p_carCtrl.msgColor[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 3))));
}

/// @brief checks the car setup item python object corresponds to the original car setup item values
/// @param  p_carSetupItem The original car setup item value
/// @param  p_carSetupItemObject The python car setup item object
void CheckCarSetupItemData(tCarSetupItem& p_carSetupItem, PyObject* p_carSetupItemObject)
{
    PyObject* valueAttr = PyUnicode_FromString("value");
    PyObject* valueVal = PyObject_GetAttr(p_carSetupItemObject, valueAttr);
    ASSERT_TRUE(p_carSetupItem.value == static_cast<float>(PyFloat_AsDouble(valueVal)));

    PyObject* minAttr = PyUnicode_FromString("min");
    PyObject* minVal = PyObject_GetAttr(p_carSetupItemObject, minAttr);
    ASSERT_TRUE(p_carSetupItem.min == static_cast<float>(PyFloat_AsDouble(minVal)));

    PyObject* maxAttr = PyUnicode_FromString("max");
    PyObject* maxVal = PyObject_GetAttr(p_carSetupItemObject, maxAttr);
    ASSERT_TRUE(p_carSetupItem.max == static_cast<float>(PyFloat_AsDouble(maxVal)));

    PyObject* desiredValueAttr = PyUnicode_FromString("desiredValue");
    PyObject* desiredValueVal = PyObject_GetAttr(p_carSetupItemObject, desiredValueAttr);
    ASSERT_TRUE(p_carSetupItem.desired_value == static_cast<float>(PyFloat_AsDouble(desiredValueVal)));

    PyObject* stepSizeAttr = PyUnicode_FromString("stepSize");
    PyObject* stepSizeVal = PyObject_GetAttr(p_carSetupItemObject, stepSizeAttr);
    ASSERT_TRUE(p_carSetupItem.stepsize == static_cast<float>(PyFloat_AsDouble(stepSizeVal)));

    PyObject* changedAttr = PyUnicode_FromString("changed");
    PyObject* changedVal = PyObject_GetAttr(p_carSetupItemObject, changedAttr);
    ASSERT_TRUE(p_carSetupItem.changed == static_cast<bool>(PyLong_AsLong(changedVal)));
}

/// @brief checks the car pit cmd python object corresponds to the original car pit cmd values
/// @param  p_carPitCmd The original car pit cmd value
/// @param  p_carPitCmdObject The python car pit cmd object
void CheckCarPitCmdData(tCarPitCmd& p_carPitCmd, PyObject* p_carPitCmdObject)
{
    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_carPitCmdObject, fuelAttr);
    ASSERT_TRUE(p_carPitCmd.fuel == static_cast<float>(PyFloat_AsDouble(fuelVal)));

    PyObject* repairAttr = PyUnicode_FromString("repair");
    PyObject* repairVal = PyObject_GetAttr(p_carPitCmdObject, repairAttr);
    ASSERT_TRUE(p_carPitCmd.repair == static_cast<int>(PyFloat_AsDouble(repairVal)));

    PyObject* stopTypeAttr = PyUnicode_FromString("stopType");
    PyObject* stopTypeVal = PyObject_GetAttr(p_carPitCmdObject, stopTypeAttr);
    ASSERT_TRUE(p_carPitCmd.stopType == static_cast<int>(PyFloat_AsDouble(stopTypeVal)));

    PyObject* setupChangedAttr = PyUnicode_FromString("setupChanged");
    PyObject* setupChangedVal = PyObject_GetAttr(p_carPitCmdObject, setupChangedAttr);
    ASSERT_TRUE(p_carPitCmd.setupChanged == static_cast<bool>(PyLong_AsLong(setupChangedVal)));

    PyObject* tireChangeAttr = PyUnicode_FromString("tireChange");
    PyObject* tireChangeVal = PyObject_GetAttr(p_carPitCmdObject, tireChangeAttr);
    ASSERT_TRUE(p_carPitCmd.tireChange == static_cast<bool>(PyLong_AsLong(tireChangeVal)));
}

/// @brief checks the car setup python object corresponds to the original car setup values
/// @param  p_carSetup The original car setup value
/// @param  p_carSetupObject The python car setup object
void CheckCarSetupData(tCarSetup& p_carSetup, PyObject* p_carSetupObject)
{
    PyObject* fRWeightRepAttr = PyUnicode_FromString("fRWeightRep");
    PyObject* fRWeightRepVal = PyObject_GetAttr(p_carSetupObject, fRWeightRepAttr);
    CheckCarSetupItemData(p_carSetup.FRWeightRep, fRWeightRepVal);

    PyObject* fRLWeightRepAttr = PyUnicode_FromString("fRLWeightRep");
    PyObject* fRLWeightRepVal = PyObject_GetAttr(p_carSetupObject, fRLWeightRepAttr);
    CheckCarSetupItemData(p_carSetup.FRLWeightRep, fRLWeightRepVal);

    PyObject* rRLWeightRepAttr = PyUnicode_FromString("rRLWeightRep");
    PyObject* rRLWeightRepVal = PyObject_GetAttr(p_carSetupObject, rRLWeightRepAttr);
    CheckCarSetupItemData(p_carSetup.RRLWeightRep, rRLWeightRepVal);

    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_carSetupObject, fuelAttr);
    CheckCarSetupItemData(p_carSetup.fuel, fuelVal);

    PyObject* wingAngleAttr = PyUnicode_FromString("wingAngle");
    PyObject* wingAngleVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, wingAngleAttr));
    PyObject* test = PyTuple_GetItem(wingAngleVal, 0);
    CheckCarSetupItemData(p_carSetup.wingAngle[0], PyTuple_GetItem(wingAngleVal, 0));
    CheckCarSetupItemData(p_carSetup.wingAngle[1], PyTuple_GetItem(wingAngleVal, 1));

    PyObject* revsLimiterAttr = PyUnicode_FromString("revsLimiter");
    PyObject* revsLimiterVal = PyObject_GetAttr(p_carSetupObject, revsLimiterAttr);
    CheckCarSetupItemData(p_carSetup.revsLimiter, revsLimiterVal);

    PyObject* gearRatioAttr = PyUnicode_FromString("gearRatio");
    PyObject* gearRatioVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, gearRatioAttr));
    CheckCarSetupItemData(p_carSetup.gearRatio[0], PyTuple_GetItem(gearRatioVal, 0));
    CheckCarSetupItemData(p_carSetup.gearRatio[1], PyTuple_GetItem(gearRatioVal, 1));
    CheckCarSetupItemData(p_carSetup.gearRatio[2], PyTuple_GetItem(gearRatioVal, 2));
    CheckCarSetupItemData(p_carSetup.gearRatio[3], PyTuple_GetItem(gearRatioVal, 3));
    CheckCarSetupItemData(p_carSetup.gearRatio[4], PyTuple_GetItem(gearRatioVal, 4));
    CheckCarSetupItemData(p_carSetup.gearRatio[5], PyTuple_GetItem(gearRatioVal, 5));
    CheckCarSetupItemData(p_carSetup.gearRatio[6], PyTuple_GetItem(gearRatioVal, 6));
    CheckCarSetupItemData(p_carSetup.gearRatio[7], PyTuple_GetItem(gearRatioVal, 7));
    CheckCarSetupItemData(p_carSetup.gearRatio[8], PyTuple_GetItem(gearRatioVal, 8));
    CheckCarSetupItemData(p_carSetup.gearRatio[9], PyTuple_GetItem(gearRatioVal, 9));

    PyObject* differentialTypeAttr = PyUnicode_FromString("differentialType");
    PyObject* differentialTypeVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialTypeAttr));
    ASSERT_EQ(p_carSetup.differentialType[0], static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(differentialTypeVal, 0))));
    ASSERT_EQ(p_carSetup.differentialType[1], static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(differentialTypeVal, 1))));
    ASSERT_EQ(p_carSetup.differentialType[2], static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(differentialTypeVal, 2))));

    PyObject* differentialRatioAttr = PyUnicode_FromString("differentialRatio");
    PyObject* differentialRatioVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialRatioAttr));
    CheckCarSetupItemData(p_carSetup.differentialRatio[0], PyTuple_GetItem(differentialRatioVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialRatio[1], PyTuple_GetItem(differentialRatioVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialRatio[2], PyTuple_GetItem(differentialRatioVal, 2));

    PyObject* differentialMinTqBiasAttr = PyUnicode_FromString("differentialMinTqBias");
    PyObject* differentialMinTqBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialMinTqBiasAttr));
    CheckCarSetupItemData(p_carSetup.differentialMinTqBias[0], PyTuple_GetItem(differentialMinTqBiasVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialMinTqBias[1], PyTuple_GetItem(differentialMinTqBiasVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialMinTqBias[2], PyTuple_GetItem(differentialMinTqBiasVal, 2));

    PyObject* differentialMaxTqBiasAttr = PyUnicode_FromString("differentialMaxTqBias");
    PyObject* differentialMaxTqBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialMaxTqBiasAttr));
    CheckCarSetupItemData(p_carSetup.differentialMaxTqBias[0], PyTuple_GetItem(differentialMaxTqBiasVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialMaxTqBias[1], PyTuple_GetItem(differentialMaxTqBiasVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialMaxTqBias[2], PyTuple_GetItem(differentialMaxTqBiasVal, 2));

    PyObject* differentialViscosityAttr = PyUnicode_FromString("differentialViscosity");
    PyObject* differentialViscosityVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialViscosityAttr));
    CheckCarSetupItemData(p_carSetup.differentialViscosity[0], PyTuple_GetItem(differentialViscosityVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialViscosity[1], PyTuple_GetItem(differentialViscosityVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialViscosity[2], PyTuple_GetItem(differentialViscosityVal, 2));

    PyObject* differentialLockingTqAttr = PyUnicode_FromString("differentialLockingTq");
    PyObject* differentialLockingTqVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialLockingTqAttr));
    CheckCarSetupItemData(p_carSetup.differentialLockingTq[0], PyTuple_GetItem(differentialLockingTqVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialLockingTq[1], PyTuple_GetItem(differentialLockingTqVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialLockingTq[2], PyTuple_GetItem(differentialLockingTqVal, 2));

    PyObject* differentialMaxSlipBiasAttr = PyUnicode_FromString("differentialMaxSlipBias");
    PyObject* differentialMaxSlipBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialMaxSlipBiasAttr));
    CheckCarSetupItemData(p_carSetup.differentialMaxSlipBias[0], PyTuple_GetItem(differentialMaxSlipBiasVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialMaxSlipBias[1], PyTuple_GetItem(differentialMaxSlipBiasVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialMaxSlipBias[2], PyTuple_GetItem(differentialMaxSlipBiasVal, 2));

    PyObject* differentialCoastMaxSlipBiasAttr = PyUnicode_FromString("differentialCoastMaxSlipBias");
    PyObject* differentialCoastMaxSlipBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialCoastMaxSlipBiasAttr));
    CheckCarSetupItemData(p_carSetup.differentialCoastMaxSlipBias[0], PyTuple_GetItem(differentialCoastMaxSlipBiasVal, 0));
    CheckCarSetupItemData(p_carSetup.differentialCoastMaxSlipBias[1], PyTuple_GetItem(differentialCoastMaxSlipBiasVal, 1));
    CheckCarSetupItemData(p_carSetup.differentialCoastMaxSlipBias[2], PyTuple_GetItem(differentialCoastMaxSlipBiasVal, 2));

    PyObject* steerLockAttr = PyUnicode_FromString("steerLock");
    PyObject* steerLockVal = PyObject_GetAttr(p_carSetupObject, steerLockAttr);
    CheckCarSetupItemData(p_carSetup.steerLock, steerLockVal);

    PyObject* brakeRepartitionAttr = PyUnicode_FromString("brakeRepartition");
    PyObject* brakeRepartitionVal = PyObject_GetAttr(p_carSetupObject, brakeRepartitionAttr);
    CheckCarSetupItemData(p_carSetup.brakeRepartition, brakeRepartitionVal);

    PyObject* brakePressureAttr = PyUnicode_FromString("brakePressure");
    PyObject* brakePressureVal = PyObject_GetAttr(p_carSetupObject, brakePressureAttr);
    CheckCarSetupItemData(p_carSetup.brakePressure, brakePressureVal);

    PyObject* rideHeightAttr = PyUnicode_FromString("rideHeight");
    PyObject* rideHeightVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, rideHeightAttr));
    CheckCarSetupItemData(p_carSetup.rideHeight[0], PyTuple_GetItem(rideHeightVal, 0));
    CheckCarSetupItemData(p_carSetup.rideHeight[1], PyTuple_GetItem(rideHeightVal, 1));
    CheckCarSetupItemData(p_carSetup.rideHeight[2], PyTuple_GetItem(rideHeightVal, 2));
    CheckCarSetupItemData(p_carSetup.rideHeight[3], PyTuple_GetItem(rideHeightVal, 3));

    PyObject* toeAttr = PyUnicode_FromString("toe");
    PyObject* toeVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, toeAttr));
    CheckCarSetupItemData(p_carSetup.toe[0], PyTuple_GetItem(toeVal, 0));
    CheckCarSetupItemData(p_carSetup.toe[1], PyTuple_GetItem(toeVal, 1));
    CheckCarSetupItemData(p_carSetup.toe[2], PyTuple_GetItem(toeVal, 2));
    CheckCarSetupItemData(p_carSetup.toe[3], PyTuple_GetItem(toeVal, 3));

    PyObject* camberAttr = PyUnicode_FromString("camber");
    PyObject* camberVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, camberAttr));
    CheckCarSetupItemData(p_carSetup.camber[0], PyTuple_GetItem(camberVal, 0));
    CheckCarSetupItemData(p_carSetup.camber[1], PyTuple_GetItem(camberVal, 1));
    CheckCarSetupItemData(p_carSetup.camber[2], PyTuple_GetItem(camberVal, 2));
    CheckCarSetupItemData(p_carSetup.camber[3], PyTuple_GetItem(camberVal, 3));

    PyObject* tirePressureAttr = PyUnicode_FromString("tirePressure");
    PyObject* tirePressureVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, tirePressureAttr));
    CheckCarSetupItemData(p_carSetup.tirePressure[0], PyTuple_GetItem(tirePressureVal, 0));
    CheckCarSetupItemData(p_carSetup.tirePressure[1], PyTuple_GetItem(tirePressureVal, 1));
    CheckCarSetupItemData(p_carSetup.tirePressure[2], PyTuple_GetItem(tirePressureVal, 2));
    CheckCarSetupItemData(p_carSetup.tirePressure[3], PyTuple_GetItem(tirePressureVal, 3));

    PyObject* tireOpLoadAttr = PyUnicode_FromString("tireOpLoad");
    PyObject* tireOpLoadVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, tireOpLoadAttr));
    CheckCarSetupItemData(p_carSetup.tireOpLoad[0], PyTuple_GetItem(tireOpLoadVal, 0));
    CheckCarSetupItemData(p_carSetup.tireOpLoad[1], PyTuple_GetItem(tireOpLoadVal, 1));
    CheckCarSetupItemData(p_carSetup.tireOpLoad[2], PyTuple_GetItem(tireOpLoadVal, 2));
    CheckCarSetupItemData(p_carSetup.tireOpLoad[3], PyTuple_GetItem(tireOpLoadVal, 3));

    PyObject* arbSpringAttr = PyUnicode_FromString("arbSpring");
    PyObject* arbSpringVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, arbSpringAttr));
    CheckCarSetupItemData(p_carSetup.arbSpring[0], PyTuple_GetItem(arbSpringVal, 0));
    CheckCarSetupItemData(p_carSetup.arbSpring[1], PyTuple_GetItem(arbSpringVal, 1));

    PyObject* arbBellCrankAttr = PyUnicode_FromString("arbBellCrank");
    PyObject* arbBellCrankVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, arbBellCrankAttr));
    CheckCarSetupItemData(p_carSetup.arbBellcrank[0], PyTuple_GetItem(arbBellCrankVal, 0));
    CheckCarSetupItemData(p_carSetup.arbBellcrank[1], PyTuple_GetItem(arbBellCrankVal, 1));

    PyObject* heaveSpringAttr = PyUnicode_FromString("heaveSpring");
    PyObject* heaveSpringVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveSpringAttr));
    CheckCarSetupItemData(p_carSetup.heaveSpring[0], PyTuple_GetItem(heaveSpringVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveSpring[1], PyTuple_GetItem(heaveSpringVal, 1));

    PyObject* heaveBellCrankAttr = PyUnicode_FromString("heaveBellCrank");
    PyObject* heaveBellCrankVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveBellCrankAttr));
    CheckCarSetupItemData(p_carSetup.heaveBellcrank[0], PyTuple_GetItem(heaveBellCrankVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveBellcrank[1], PyTuple_GetItem(heaveBellCrankVal, 1));

    PyObject* heaveInertanceAttr = PyUnicode_FromString("heaveInertance");
    PyObject* heaveInertanceVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveInertanceAttr));
    CheckCarSetupItemData(p_carSetup.heaveInertance[0], PyTuple_GetItem(heaveInertanceVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveInertance[1], PyTuple_GetItem(heaveInertanceVal, 1));

    PyObject* heaveFastBumpAttr = PyUnicode_FromString("heaveFastBump");
    PyObject* heaveFastBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveFastBumpAttr));
    CheckCarSetupItemData(p_carSetup.heaveFastBump[0], PyTuple_GetItem(heaveFastBumpVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveFastBump[1], PyTuple_GetItem(heaveFastBumpVal, 1));

    PyObject* heaveSlowBumpAttr = PyUnicode_FromString("heaveSlowBump");
    PyObject* heaveSlowBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveSlowBumpAttr));
    CheckCarSetupItemData(p_carSetup.heaveSlowBump[0], PyTuple_GetItem(heaveSlowBumpVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveSlowBump[1], PyTuple_GetItem(heaveSlowBumpVal, 1));

    PyObject* heaveBumpLevelAttr = PyUnicode_FromString("heaveBumpLevel");
    PyObject* heaveBumpLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveBumpLevelAttr));
    CheckCarSetupItemData(p_carSetup.heaveBumpLvel[0], PyTuple_GetItem(heaveBumpLevelVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveBumpLvel[1], PyTuple_GetItem(heaveBumpLevelVal, 1));

    PyObject* heaveFastReboundAttr = PyUnicode_FromString("heaveFastRebound");
    PyObject* heaveFastReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveFastReboundAttr));
    CheckCarSetupItemData(p_carSetup.heaveFastRebound[0], PyTuple_GetItem(heaveFastReboundVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveFastRebound[1], PyTuple_GetItem(heaveFastReboundVal, 1));

    PyObject* heaveSlowReboundAttr = PyUnicode_FromString("heaveSlowRebound");
    PyObject* heaveSlowReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveSlowReboundAttr));
    CheckCarSetupItemData(p_carSetup.heaveSlowRebound[0], PyTuple_GetItem(heaveSlowReboundVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveSlowRebound[1], PyTuple_GetItem(heaveSlowReboundVal, 1));

    PyObject* heaveReboundLevelAttr = PyUnicode_FromString("heaveReboundLevel");
    PyObject* heaveReboundLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveReboundLevelAttr));
    CheckCarSetupItemData(p_carSetup.heaveReboundLvel[0], PyTuple_GetItem(heaveReboundLevelVal, 0));
    CheckCarSetupItemData(p_carSetup.heaveReboundLvel[1], PyTuple_GetItem(heaveReboundLevelVal, 1));

    PyObject* suspSpringAttr = PyUnicode_FromString("suspSpring");
    PyObject* suspSpringVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspSpringAttr));
    CheckCarSetupItemData(p_carSetup.suspSpring[0], PyTuple_GetItem(suspSpringVal, 0));
    CheckCarSetupItemData(p_carSetup.suspSpring[1], PyTuple_GetItem(suspSpringVal, 1));
    CheckCarSetupItemData(p_carSetup.suspSpring[2], PyTuple_GetItem(suspSpringVal, 2));
    CheckCarSetupItemData(p_carSetup.suspSpring[3], PyTuple_GetItem(suspSpringVal, 3));

    PyObject* suspBellCrankAttr = PyUnicode_FromString("suspBellCrank");
    PyObject* suspBellCrankVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspBellCrankAttr));
    CheckCarSetupItemData(p_carSetup.suspBellcrank[0], PyTuple_GetItem(suspBellCrankVal, 0));
    CheckCarSetupItemData(p_carSetup.suspBellcrank[1], PyTuple_GetItem(suspBellCrankVal, 1));
    CheckCarSetupItemData(p_carSetup.suspBellcrank[2], PyTuple_GetItem(suspBellCrankVal, 2));
    CheckCarSetupItemData(p_carSetup.suspBellcrank[3], PyTuple_GetItem(suspBellCrankVal, 3));

    PyObject* suspInertanceAttr = PyUnicode_FromString("suspInertance");
    PyObject* suspInertanceVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspInertanceAttr));
    CheckCarSetupItemData(p_carSetup.suspInertance[0], PyTuple_GetItem(suspInertanceVal, 0));
    CheckCarSetupItemData(p_carSetup.suspInertance[1], PyTuple_GetItem(suspInertanceVal, 1));
    CheckCarSetupItemData(p_carSetup.suspInertance[2], PyTuple_GetItem(suspInertanceVal, 2));
    CheckCarSetupItemData(p_carSetup.suspInertance[3], PyTuple_GetItem(suspInertanceVal, 3));

    PyObject* suspCourseAttr = PyUnicode_FromString("suspCourse");
    PyObject* suspCourseVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspCourseAttr));
    CheckCarSetupItemData(p_carSetup.suspCourse[0], PyTuple_GetItem(suspCourseVal, 0));
    CheckCarSetupItemData(p_carSetup.suspCourse[1], PyTuple_GetItem(suspCourseVal, 1));
    CheckCarSetupItemData(p_carSetup.suspCourse[2], PyTuple_GetItem(suspCourseVal, 2));
    CheckCarSetupItemData(p_carSetup.suspCourse[3], PyTuple_GetItem(suspCourseVal, 3));

    PyObject* suspPackerAttr = PyUnicode_FromString("suspPacker");
    PyObject* suspPackerVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspPackerAttr));
    CheckCarSetupItemData(p_carSetup.suspPacker[0], PyTuple_GetItem(suspPackerVal, 0));
    CheckCarSetupItemData(p_carSetup.suspPacker[1], PyTuple_GetItem(suspPackerVal, 1));
    CheckCarSetupItemData(p_carSetup.suspPacker[2], PyTuple_GetItem(suspPackerVal, 2));
    CheckCarSetupItemData(p_carSetup.suspPacker[3], PyTuple_GetItem(suspPackerVal, 3));

    PyObject* suspFastBumpAttr = PyUnicode_FromString("suspFastBump");
    PyObject* suspFastBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspFastBumpAttr));
    CheckCarSetupItemData(p_carSetup.suspFastBump[0], PyTuple_GetItem(suspFastBumpVal, 0));
    CheckCarSetupItemData(p_carSetup.suspFastBump[1], PyTuple_GetItem(suspFastBumpVal, 1));
    CheckCarSetupItemData(p_carSetup.suspFastBump[2], PyTuple_GetItem(suspFastBumpVal, 2));
    CheckCarSetupItemData(p_carSetup.suspFastBump[3], PyTuple_GetItem(suspFastBumpVal, 3));

    PyObject* suspSlowBumpAttr = PyUnicode_FromString("suspSlowBump");
    PyObject* suspSlowBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspSlowBumpAttr));
    CheckCarSetupItemData(p_carSetup.suspSlowBump[0], PyTuple_GetItem(suspSlowBumpVal, 0));
    CheckCarSetupItemData(p_carSetup.suspSlowBump[1], PyTuple_GetItem(suspSlowBumpVal, 1));
    CheckCarSetupItemData(p_carSetup.suspSlowBump[2], PyTuple_GetItem(suspSlowBumpVal, 2));
    CheckCarSetupItemData(p_carSetup.suspSlowBump[3], PyTuple_GetItem(suspSlowBumpVal, 3));

    PyObject* suspBumpLevelAttr = PyUnicode_FromString("suspBumpLevel");
    PyObject* suspBumpLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspBumpLevelAttr));
    CheckCarSetupItemData(p_carSetup.suspBumpLvel[0], PyTuple_GetItem(suspBumpLevelVal, 0));
    CheckCarSetupItemData(p_carSetup.suspBumpLvel[1], PyTuple_GetItem(suspBumpLevelVal, 1));
    CheckCarSetupItemData(p_carSetup.suspBumpLvel[2], PyTuple_GetItem(suspBumpLevelVal, 2));
    CheckCarSetupItemData(p_carSetup.suspBumpLvel[3], PyTuple_GetItem(suspBumpLevelVal, 3));

    PyObject* suspFastReboundAttr = PyUnicode_FromString("suspFastRebound");
    PyObject* suspFastReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspFastReboundAttr));
    CheckCarSetupItemData(p_carSetup.suspFastRebound[0], PyTuple_GetItem(suspFastReboundVal, 0));
    CheckCarSetupItemData(p_carSetup.suspFastRebound[1], PyTuple_GetItem(suspFastReboundVal, 1));
    CheckCarSetupItemData(p_carSetup.suspFastRebound[2], PyTuple_GetItem(suspFastReboundVal, 2));
    CheckCarSetupItemData(p_carSetup.suspFastRebound[3], PyTuple_GetItem(suspFastReboundVal, 3));

    PyObject* suspSlowReboundAttr = PyUnicode_FromString("suspSlowRebound");
    PyObject* suspSlowReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspSlowReboundAttr));
    CheckCarSetupItemData(p_carSetup.suspSlowRebound[0], PyTuple_GetItem(suspSlowReboundVal, 0));
    CheckCarSetupItemData(p_carSetup.suspSlowRebound[1], PyTuple_GetItem(suspSlowReboundVal, 1));
    CheckCarSetupItemData(p_carSetup.suspSlowRebound[2], PyTuple_GetItem(suspSlowReboundVal, 2));
    CheckCarSetupItemData(p_carSetup.suspSlowRebound[3], PyTuple_GetItem(suspSlowReboundVal, 3));

    PyObject* suspReboundLevelAttr = PyUnicode_FromString("suspReboundLevel");
    PyObject* suspReboundLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspReboundLevelAttr));
    CheckCarSetupItemData(p_carSetup.suspReboundLvel[0], PyTuple_GetItem(suspReboundLevelVal, 0));
    CheckCarSetupItemData(p_carSetup.suspReboundLvel[1], PyTuple_GetItem(suspReboundLevelVal, 1));
    CheckCarSetupItemData(p_carSetup.suspReboundLvel[2], PyTuple_GetItem(suspReboundLevelVal, 2));
    CheckCarSetupItemData(p_carSetup.suspReboundLvel[3], PyTuple_GetItem(suspReboundLevelVal, 3));

    PyObject* reqRepairAttr = PyUnicode_FromString("reqRepair");
    PyObject* reqRepairVal = PyObject_GetAttr(p_carSetupObject, reqRepairAttr);
    CheckCarSetupItemData(p_carSetup.reqRepair, reqRepairVal);

    PyObject* reqTiresetAttr = PyUnicode_FromString("reqTireset");
    PyObject* reqTiresetVal = PyObject_GetAttr(p_carSetupObject, reqTiresetAttr);
    CheckCarSetupItemData(p_carSetup.reqTireset, reqTiresetVal);

    PyObject* reqPenaltyAttr = PyUnicode_FromString("reqPenalty");
    PyObject* reqPenaltyVal = PyObject_GetAttr(p_carSetupObject, reqPenaltyAttr);
    CheckCarSetupItemData(p_carSetup.reqPenalty, reqPenaltyVal);
}

/// @brief checks the race info python object corresponds to the original race info values
/// @param  p_raceInfo The original race info value
/// @param  p_raceInfoObject The python race info object
void CheckRaceInfoData(tRaceAdmInfo& p_raceInfo, PyObject* p_raceInfoObject)
{
    PyObject* ncarsAttr = PyUnicode_FromString("ncars");
    PyObject* ncarsVal = PyObject_GetAttr(p_raceInfoObject, ncarsAttr);
    ASSERT_TRUE(p_raceInfo.ncars == static_cast<int>(PyLong_AsLong(ncarsVal)));

    PyObject* totLapsAttr = PyUnicode_FromString("totLaps");
    PyObject* totLapsVal = PyObject_GetAttr(p_raceInfoObject, totLapsAttr);
    ASSERT_TRUE(p_raceInfo.totLaps == static_cast<int>(PyLong_AsLong(totLapsVal)));

    PyObject* extraLapsAttr = PyUnicode_FromString("extraLaps");
    PyObject* extraLapsVal = PyObject_GetAttr(p_raceInfoObject, extraLapsAttr);
    ASSERT_TRUE(p_raceInfo.extraLaps == static_cast<int>(PyLong_AsLong(extraLapsVal)));

    PyObject* totTimeAttr = PyUnicode_FromString("totTime");
    PyObject* totTimeVal = PyObject_GetAttr(p_raceInfoObject, totTimeAttr);
    ASSERT_TRUE(p_raceInfo.totTime == static_cast<float>(PyFloat_AsDouble(totTimeVal)));

    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_raceInfoObject, stateAttr);
    ASSERT_TRUE(p_raceInfo.state == static_cast<int>(PyLong_AsLong(stateVal)));

    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_raceInfoObject, typeAttr);
    ASSERT_TRUE(p_raceInfo.type == static_cast<int>(PyLong_AsLong(typeVal)));

    PyObject* maxDamageAttr = PyUnicode_FromString("maxDamage");
    PyObject* maxDamageVal = PyObject_GetAttr(p_raceInfoObject, maxDamageAttr);
    ASSERT_TRUE(p_raceInfo.maxDammage == static_cast<int>(PyLong_AsLong(maxDamageVal)));

    PyObject* fpsAttr = PyUnicode_FromString("fps");
    PyObject* fpsVal = PyObject_GetAttr(p_raceInfoObject, fpsAttr);
    ASSERT_TRUE(p_raceInfo.fps == static_cast<int>(PyLong_AsLong(fpsVal)));

    PyObject* featuresAttr = PyUnicode_FromString("features");
    PyObject* featuresVal = PyObject_GetAttr(p_raceInfoObject, featuresAttr);
    ASSERT_TRUE(p_raceInfo.features == static_cast<int>(PyLong_AsLong(featuresVal)));
}

/// @brief checks the situation python object corresponds to the original situation values
/// @param  p_situation The original situation value
/// @param  p_situationObject The python situation object
void CheckSituationData(tSituation& p_situation, PyObject* p_situationObject)
{
    PyObject* raceInfoAttr = PyUnicode_FromString("raceInfo");
    PyObject* raceInfoVal = PyObject_GetAttr(p_situationObject, raceInfoAttr);
    CheckRaceInfoData(p_situation.raceInfo, raceInfoVal);

    PyObject* deltaTimeAttr = PyUnicode_FromString("deltaTime");
    PyObject* deltaTimeVal = PyObject_GetAttr(p_situationObject, deltaTimeAttr);
    ASSERT_TRUE(p_situation.deltaTime == static_cast<float>(PyFloat_AsDouble(deltaTimeVal)));

    PyObject* currentTimeAttr = PyUnicode_FromString("currentTime");
    PyObject* currentTimeVal = PyObject_GetAttr(p_situationObject, currentTimeAttr);
    ASSERT_TRUE(p_situation.currentTime == static_cast<float>(PyFloat_AsDouble(currentTimeVal)));

    PyObject* accelTimeAttr = PyUnicode_FromString("accelTime");
    PyObject* accelTimeVal = PyObject_GetAttr(p_situationObject, accelTimeAttr);
    ASSERT_TRUE(p_situation.accelTime == static_cast<float>(PyFloat_AsDouble(accelTimeVal)));

    PyObject* nbPlayersAttr = PyUnicode_FromString("nbPlayers");
    PyObject* nbPlayersVal = PyObject_GetAttr(p_situationObject, nbPlayersAttr);
    ASSERT_TRUE(p_situation.nbPlayers == static_cast<int>(PyLong_AsLong(nbPlayersVal)));
}

/// @brief checks the car python object corresponds to the original car values
/// @param  p_car The original car value
/// @param  p_carObject The python car object
void CheckCarData(tCarElt& p_car, PyObject* p_carObject)
{
    PyObject* indexAttr = PyUnicode_FromString("index");
    PyObject* indexVal = PyObject_GetAttr(p_carObject, indexAttr);
    ASSERT_TRUE(p_car.index == static_cast<int>(PyLong_AsLong(indexVal)));

    PyObject* infoAttr = PyUnicode_FromString("info");
    PyObject* infoVal = PyObject_GetAttr(p_carObject, infoAttr);
    CheckInitCarData(p_car.info, infoVal);

    PyObject* pubAttr = PyUnicode_FromString("pub");
    PyObject* pubVal = PyObject_GetAttr(p_carObject, pubAttr);
    CheckPublicCarData(p_car.pub, pubVal);

    PyObject* raceAttr = PyUnicode_FromString("race");
    PyObject* raceVal = PyObject_GetAttr(p_carObject, raceAttr);
    CheckCarRaceInfoData(p_car.race, raceVal);

    PyObject* privAttr = PyUnicode_FromString("priv");
    PyObject* privVal = PyObject_GetAttr(p_carObject, privAttr);
    CheckPrivCarData(p_car.priv, privVal);

    PyObject* ctrlAttr = PyUnicode_FromString("ctrl");
    PyObject* ctrlVal = PyObject_GetAttr(p_carObject, ctrlAttr);
    CheckCarCtrlData(p_car.ctrl, ctrlVal);

    PyObject* setupAttr = PyUnicode_FromString("setup");
    PyObject* setupVal = PyObject_GetAttr(p_carObject, setupAttr);
    CheckCarSetupData(p_car.setup, setupVal);

    PyObject* pitcmdAttr = PyUnicode_FromString("pitcmd");
    PyObject* pitcmdVal = PyObject_GetAttr(p_carObject, pitcmdAttr);
    CheckCarPitCmdData(p_car.pitcmd, pitcmdVal);
}

/// @brief checks the aero python object corresponds to the original aero values
/// @param  p_aero The original aero value
/// @param  p_aeroObject The python aero object
void CheckAeroData(tAero p_aero, PyObject* p_aeroObject)
{
    PyObject* dragAttr = PyUnicode_FromString("drag");
    PyObject* dragVal = PyObject_GetAttr(p_aeroObject, dragAttr);
    ASSERT_EQ(p_aero.drag, static_cast<float>(PyFloat_AsDouble(dragVal)));

    PyObject* liftAttr = PyUnicode_FromString("lift");
    PyObject* liftVal = PyList_AsTuple(PyObject_GetAttr(p_aeroObject, liftAttr));
    ASSERT_EQ(p_aero.lift[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(liftVal, 0))));
    ASSERT_EQ(p_aero.lift[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(liftVal, 1))));

    PyObject* CliftAttr = PyUnicode_FromString("Clift");
    PyObject* CliftVal = PyList_AsTuple(PyObject_GetAttr(p_aeroObject, CliftAttr));
    ASSERT_EQ(p_aero.Clift[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(CliftVal, 0))));
    ASSERT_EQ(p_aero.Clift[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(CliftVal, 1))));

    PyObject* CdAttr = PyUnicode_FromString("Cd");
    PyObject* CdVal = PyObject_GetAttr(p_aeroObject, CdAttr);
    ASSERT_EQ(p_aero.Cd, static_cast<float>(PyFloat_AsDouble(CdVal)));

    PyObject* CdBodyAttr = PyUnicode_FromString("CdBody");
    PyObject* CdBodyVal = PyObject_GetAttr(p_aeroObject, CdBodyAttr);
    ASSERT_EQ(p_aero.CdBody, static_cast<float>(PyFloat_AsDouble(CdBodyVal)));
}

/// @brief checks the wing python object corresponds to the original wing values
/// @param  p_data The original wing value
/// @param  p_dataObject The python wing object
void CheckWingData(tWing p_wing, PyObject* p_wingObject)
{
    PyObject* forcesAttr = PyUnicode_FromString("forces");
    PyObject* forcesVal = PyObject_GetAttr(p_wingObject, forcesAttr);
    CheckVectorData(p_wing.forces.x, p_wing.forces.y, p_wing.forces.z, forcesVal);

    PyObject* KxAttr = PyUnicode_FromString("Kx");
    PyObject* KxVal = PyObject_GetAttr(p_wingObject, KxAttr);
    ASSERT_EQ(p_wing.Kx, static_cast<float>(PyFloat_AsDouble(KxVal)));

    PyObject* KzAttr = PyUnicode_FromString("Kz");
    PyObject* KzVal = PyObject_GetAttr(p_wingObject, KzAttr);
    ASSERT_EQ(p_wing.Kz, static_cast<float>(PyFloat_AsDouble(KzVal)));

    PyObject* Kz_orgAttr = PyUnicode_FromString("Kz_org");
    PyObject* Kz_orgVal = PyObject_GetAttr(p_wingObject, Kz_orgAttr);
    ASSERT_EQ(p_wing.Kz_org, static_cast<float>(PyFloat_AsDouble(Kz_orgVal)));

    PyObject* angleAttr = PyUnicode_FromString("angleMax");
    PyObject* angleVal = PyObject_GetAttr(p_wingObject, angleAttr);
    ASSERT_EQ(p_wing.angle, static_cast<float>(PyFloat_AsDouble(angleVal)));

    PyObject* staticPosAttr = PyUnicode_FromString("staticPos");
    PyObject* staticPosVal = PyObject_GetAttr(p_wingObject, staticPosAttr);
    CheckVectorData(p_wing.staticPos.x, p_wing.staticPos.y, p_wing.staticPos.z, staticPosVal);

    PyObject* AoAatMaxAttr = PyUnicode_FromString("AoAatMax");
    PyObject* AoAatMaxVal = PyObject_GetAttr(p_wingObject, AoAatMaxAttr);
    ASSERT_EQ(p_wing.AoAatMax, static_cast<float>(PyFloat_AsDouble(AoAatMaxVal)));

    PyObject* AoAatZeroAttr = PyUnicode_FromString("AoAatZero");
    PyObject* AoAatZeroVal = PyObject_GetAttr(p_wingObject, AoAatZeroAttr);
    ASSERT_EQ(p_wing.AoAatZero, static_cast<float>(PyFloat_AsDouble(AoAatZeroVal)));

    PyObject* AoAatZRadAttr = PyUnicode_FromString("AoAatZRad");
    PyObject* AoAatZRadVal = PyObject_GetAttr(p_wingObject, AoAatZRadAttr);
    ASSERT_EQ(p_wing.AoAatZRad, static_cast<float>(PyFloat_AsDouble(AoAatZRadVal)));

    PyObject* AoAOffsetAttr = PyUnicode_FromString("AoAOffset");
    PyObject* AoAOffsetVal = PyObject_GetAttr(p_wingObject, AoAOffsetAttr);
    ASSERT_EQ(p_wing.AoAOffset, static_cast<float>(PyFloat_AsDouble(AoAOffsetVal)));

    PyObject* CliftMaxAttr = PyUnicode_FromString("CliftMax");
    PyObject* CliftMaxVal = PyObject_GetAttr(p_wingObject, CliftMaxAttr);
    ASSERT_EQ(p_wing.CliftMax, static_cast<float>(PyFloat_AsDouble(CliftMaxVal)));

    PyObject* CliftZeroAttr = PyUnicode_FromString("CliftZero");
    PyObject* CliftZeroVal = PyObject_GetAttr(p_wingObject, CliftZeroAttr);
    ASSERT_EQ(p_wing.CliftZero, static_cast<float>(PyFloat_AsDouble(CliftZeroVal)));

    PyObject* CliftAsympAttr = PyUnicode_FromString("CliftAsymp");
    PyObject* CliftAsympVal = PyObject_GetAttr(p_wingObject, CliftAsympAttr);
    ASSERT_EQ(p_wing.CliftAsymp, static_cast<float>(PyFloat_AsDouble(CliftAsympVal)));

    PyObject* aAttr = PyUnicode_FromString("a");
    PyObject* aVal = PyObject_GetAttr(p_wingObject, aAttr);
    ASSERT_EQ(p_wing.a, static_cast<float>(PyFloat_AsDouble(aVal)));

    PyObject* bAttr = PyUnicode_FromString("b");
    PyObject* bVal = PyObject_GetAttr(p_wingObject, bAttr);
    ASSERT_EQ(p_wing.b, static_cast<float>(PyFloat_AsDouble(bVal)));

    PyObject* cAttr = PyUnicode_FromString("c");
    PyObject* cVal = PyObject_GetAttr(p_wingObject, cAttr);
    ASSERT_EQ(p_wing.c, static_cast<float>(PyFloat_AsDouble(cVal)));

    PyObject* dAttr = PyUnicode_FromString("d");
    PyObject* dVal = PyObject_GetAttr(p_wingObject, dAttr);
    ASSERT_EQ(p_wing.d, static_cast<float>(PyFloat_AsDouble(dVal)));

    PyObject* fAttr = PyUnicode_FromString("f");
    PyObject* fVal = PyObject_GetAttr(p_wingObject, fAttr);
    ASSERT_EQ(p_wing.f, static_cast<float>(PyFloat_AsDouble(fVal)));

    PyObject* AoStallAttr = PyUnicode_FromString("AoStall");
    PyObject* AoStallVal = PyObject_GetAttr(p_wingObject, AoStallAttr);
    ASSERT_EQ(p_wing.AoStall, static_cast<float>(PyFloat_AsDouble(AoStallVal)));

    PyObject* StallwAttr = PyUnicode_FromString("Stallw");
    PyObject* StallwVal = PyObject_GetAttr(p_wingObject, StallwAttr);
    ASSERT_EQ(p_wing.Stallw, static_cast<float>(PyFloat_AsDouble(StallwVal)));

    PyObject* ARAttr = PyUnicode_FromString("AR");
    PyObject* ARVal = PyObject_GetAttr(p_wingObject, ARAttr);
    ASSERT_EQ(p_wing.AR, static_cast<float>(PyFloat_AsDouble(ARVal)));

    PyObject* Kx1Attr = PyUnicode_FromString("Kx1");
    PyObject* Kx1Val = PyObject_GetAttr(p_wingObject, Kx1Attr);
    ASSERT_EQ(p_wing.Kx1, static_cast<float>(PyFloat_AsDouble(Kx1Val)));

    PyObject* Kx2Attr = PyUnicode_FromString("Kx2");
    PyObject* Kx2Val = PyObject_GetAttr(p_wingObject, Kx2Attr);
    ASSERT_EQ(p_wing.Kx2, static_cast<float>(PyFloat_AsDouble(Kx2Val)));

    PyObject* Kx3Attr = PyUnicode_FromString("Kx3");
    PyObject* Kx3Val = PyObject_GetAttr(p_wingObject, Kx3Attr);
    ASSERT_EQ(p_wing.Kx3, static_cast<float>(PyFloat_AsDouble(Kx3Val)));

    PyObject* Kx4Attr = PyUnicode_FromString("Kx4");
    PyObject* Kx4Val = PyObject_GetAttr(p_wingObject, Kx4Attr);
    ASSERT_EQ(p_wing.Kx4, static_cast<float>(PyFloat_AsDouble(Kx4Val)));

    PyObject* Kz1Attr = PyUnicode_FromString("Kz1");
    PyObject* Kz1Val = PyObject_GetAttr(p_wingObject, Kz1Attr);
    ASSERT_EQ(p_wing.Kz1, static_cast<float>(PyFloat_AsDouble(Kz1Val)));

    PyObject* Kz2Attr = PyUnicode_FromString("Kz2");
    PyObject* Kz2Val = PyObject_GetAttr(p_wingObject, Kz2Attr);
    ASSERT_EQ(p_wing.Kz2, static_cast<float>(PyFloat_AsDouble(Kz2Val)));

    PyObject* Kz3Attr = PyUnicode_FromString("Kz3");
    PyObject* Kz3Val = PyObject_GetAttr(p_wingObject, Kz3Attr);
    ASSERT_EQ(p_wing.Kz3, static_cast<float>(PyFloat_AsDouble(Kz3Val)));

    PyObject* WingTypeAttr = PyUnicode_FromString("WingType");
    PyObject* WingTypeVal = PyObject_GetAttr(p_wingObject, WingTypeAttr);
    ASSERT_EQ(p_wing.WingType, static_cast<int>(PyLong_AsLong(WingTypeVal)));
}

/// @brief checks the DemperDef python object corresponds to the original DemperDef values
/// @param  p_demperDef The original DemperDef value
/// @param  p_demperDefObject The python DemperDef object
void CheckDemperDefData(tDamperDef p_demperDef, PyObject* p_demperDefObject)
{
    PyObject* C1Attr = PyUnicode_FromString("C1");
    PyObject* C1Val = PyObject_GetAttr(p_demperDefObject, C1Attr);
    ASSERT_EQ(p_demperDef.C1, static_cast<float>(PyFloat_AsDouble(C1Val)));

    PyObject* b1Attr = PyUnicode_FromString("b1");
    PyObject* b1Val = PyObject_GetAttr(p_demperDefObject, b1Attr);
    ASSERT_EQ(p_demperDef.b1, static_cast<float>(PyFloat_AsDouble(b1Val)));

    PyObject* v1Attr = PyUnicode_FromString("v1");
    PyObject* v1Val = PyObject_GetAttr(p_demperDefObject, v1Attr);
    ASSERT_EQ(p_demperDef.C2, static_cast<float>(PyFloat_AsDouble(v1Val)));

    PyObject* C2Attr = PyUnicode_FromString("C2");
    PyObject* C2Val = PyObject_GetAttr(p_demperDefObject, C2Attr);
    ASSERT_EQ(p_demperDef.C2, static_cast<float>(PyFloat_AsDouble(C2Val)));

    PyObject* b2Attr = PyUnicode_FromString("b2");
    PyObject* b2Val = PyObject_GetAttr(p_demperDefObject, b2Attr);
    ASSERT_EQ(p_demperDef.b2, static_cast<float>(PyFloat_AsDouble(b2Val)));
}

/// @brief checks the Demper python object corresponds to the original Demper values
/// @param  p_demper The original Demper value
/// @param  p_demperObject The python Demper object
void CheckDemperData(tDamper p_demper, PyObject* p_demperObject)
{
    PyObject* bumpAttr = PyUnicode_FromString("bump");
    PyObject* bumpVal = PyObject_GetAttr(p_demperObject, bumpAttr);
    CheckDemperDefData(p_demper.bump, bumpVal);

    PyObject* reboundAttr = PyUnicode_FromString("rebound");
    PyObject* reboundVal = PyObject_GetAttr(p_demperObject, reboundAttr);
    CheckDemperDefData(p_demper.rebound, reboundVal);
}

/// @brief checks the Spring python object corresponds to the original Spring values
/// @param  p_spring The original Spring value
/// @param  p_springObject The python Spring object
void CheckSpringData(tSpring p_spring, PyObject* p_springObject)
{
    PyObject* KAttr = PyUnicode_FromString("K");
    PyObject* KVal = PyObject_GetAttr(p_springObject, KAttr);
    ASSERT_EQ(p_spring.K, static_cast<float>(PyFloat_AsDouble(KVal)));

    PyObject* F0Attr = PyUnicode_FromString("F0");
    PyObject* F0Val = PyObject_GetAttr(p_springObject, F0Attr);
    ASSERT_EQ(p_spring.F0, static_cast<float>(PyFloat_AsDouble(F0Val)));

    PyObject* x0Attr = PyUnicode_FromString("x0");
    PyObject* x0Val = PyObject_GetAttr(p_springObject, x0Attr);
    ASSERT_EQ(p_spring.x0, static_cast<float>(PyFloat_AsDouble(x0Val)));

    PyObject* xMaxAttr = PyUnicode_FromString("xMax");
    PyObject* xMaxVal = PyObject_GetAttr(p_springObject, xMaxAttr);
    ASSERT_EQ(p_spring.xMax, static_cast<float>(PyFloat_AsDouble(xMaxVal)));

    PyObject* bellcrankAttr = PyUnicode_FromString("bellcrank");
    PyObject* bellcrankVal = PyObject_GetAttr(p_springObject, bellcrankAttr);
    ASSERT_EQ(p_spring.bellcrank, static_cast<float>(PyFloat_AsDouble(bellcrankVal)));

    PyObject* packersAttr = PyUnicode_FromString("packers");
    PyObject* packersVal = PyObject_GetAttr(p_springObject, packersAttr);
    ASSERT_EQ(p_spring.packers, static_cast<float>(PyFloat_AsDouble(packersVal)));
}

/// @brief checks the Suspension python object corresponds to the original Suspension values
/// @param  p_suspension The original Suspension value
/// @param  p_suspensionObject The python Suspension object
void CheckSuspensionData(tSuspension p_suspension, PyObject* p_suspensionObject)
{
    PyObject* springAttr = PyUnicode_FromString("spring");
    PyObject* springVal = PyObject_GetAttr(p_suspensionObject, springAttr);
    CheckSpringData(p_suspension.spring, springVal);

    PyObject* damperAttr = PyUnicode_FromString("damper");
    PyObject* damperVal = PyObject_GetAttr(p_suspensionObject, damperAttr);
    CheckDemperData(p_suspension.damper, damperVal);

    PyObject* inertanceAttr = PyUnicode_FromString("inertance");
    PyObject* inertanceVal = PyObject_GetAttr(p_suspensionObject, inertanceAttr);
    ASSERT_EQ(p_suspension.inertance, static_cast<float>(PyFloat_AsDouble(inertanceVal)));

    PyObject* xAttr = PyUnicode_FromString("x");
    PyObject* xVal = PyObject_GetAttr(p_suspensionObject, xAttr);
    ASSERT_EQ(p_suspension.x, static_cast<float>(PyFloat_AsDouble(xVal)));

    PyObject* vAttr = PyUnicode_FromString("v");
    PyObject* vVal = PyObject_GetAttr(p_suspensionObject, vAttr);
    ASSERT_EQ(p_suspension.v, static_cast<float>(PyFloat_AsDouble(vVal)));

    PyObject* aAttr = PyUnicode_FromString("a");
    PyObject* aVal = PyObject_GetAttr(p_suspensionObject, aAttr);
    ASSERT_EQ(p_suspension.a, static_cast<float>(PyFloat_AsDouble(aVal)));

    PyObject* forceAttr = PyUnicode_FromString("force");
    PyObject* forceVal = PyObject_GetAttr(p_suspensionObject, forceAttr);
    ASSERT_EQ(p_suspension.force, static_cast<float>(PyFloat_AsDouble(forceVal)));

    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_suspensionObject, stateAttr);
    ASSERT_EQ(p_suspension.state, static_cast<int>(PyLong_AsLong(stateVal)));
}

/// @brief checks the Brake python object corresponds to the original Brake values
/// @param  p_brake The original Brake value
/// @param  p_brakeObject The python Brake object
void CheckBrakeData(tBrake p_brake, PyObject* p_brakeObject)
{
    PyObject* pressureAttr = PyUnicode_FromString("pressure");
    PyObject* pressureVal = PyObject_GetAttr(p_brakeObject, pressureAttr);
    ASSERT_EQ(p_brake.pressure, static_cast<float>(PyFloat_AsDouble(pressureVal)));

    PyObject* TqAttr = PyUnicode_FromString("Tq");
    PyObject* TqVal = PyObject_GetAttr(p_brakeObject, TqAttr);
    ASSERT_EQ(p_brake.Tq, static_cast<float>(PyFloat_AsDouble(TqVal)));

    PyObject* coeffAttr = PyUnicode_FromString("coeff");
    PyObject* coeffVal = PyObject_GetAttr(p_brakeObject, coeffAttr);
    ASSERT_EQ(p_brake.coeff, static_cast<float>(PyFloat_AsDouble(coeffVal)));

    PyObject* IAttr = PyUnicode_FromString("I");
    PyObject* IVal = PyObject_GetAttr(p_brakeObject, IAttr);
    ASSERT_EQ(p_brake.I, static_cast<float>(PyFloat_AsDouble(IVal)));

    PyObject* radiusAttr = PyUnicode_FromString("radius");
    PyObject* radiusVal = PyObject_GetAttr(p_brakeObject, radiusAttr);
    ASSERT_EQ(p_brake.radius, static_cast<float>(PyFloat_AsDouble(radiusVal)));

    PyObject* tempAttr = PyUnicode_FromString("temp");
    PyObject* tempVal = PyObject_GetAttr(p_brakeObject, tempAttr);
    ASSERT_EQ(p_brake.temp, static_cast<float>(PyFloat_AsDouble(tempVal)));

    PyObject* TCLAttr = PyUnicode_FromString("TCL");
    PyObject* TCLVal = PyObject_GetAttr(p_brakeObject, TCLAttr);
    ASSERT_EQ(p_brake.TCL, static_cast<float>(PyFloat_AsDouble(TCLVal)));

    PyObject* ABSAttr = PyUnicode_FromString("ABS");
    PyObject* ABSVal = PyObject_GetAttr(p_brakeObject, ABSAttr);
    ASSERT_EQ(p_brake.ABS, static_cast<float>(PyFloat_AsDouble(ABSVal)));

    PyObject* EnableABSAttr = PyUnicode_FromString("EnableABS");
    PyObject* EnableABSVal = PyObject_GetAttr(p_brakeObject, EnableABSAttr);
    ASSERT_EQ(p_brake.EnableABS, static_cast<bool>(EnableABSVal));
}

/// @brief checks the BrakeSyst python object corresponds to the original BrakeSyst values
/// @param  p_brakeSyst The original BrakeSyst value
/// @param  p_brakeSystObject The python Brake object
void CheckBrakeSystData(tBrakeSyst p_brakeSyst, PyObject* p_brakeSystObject)
{
    PyObject* repAttr = PyUnicode_FromString("rep");
    PyObject* repVal = PyObject_GetAttr(p_brakeSystObject, repAttr);
    ASSERT_EQ(p_brakeSyst.rep, static_cast<float>(PyFloat_AsDouble(repVal)));

    PyObject* coeffAttr = PyUnicode_FromString("coeff");
    PyObject* coeffVal = PyObject_GetAttr(p_brakeSystObject, coeffAttr);
    ASSERT_EQ(p_brakeSyst.coeff, static_cast<float>(PyFloat_AsDouble(coeffVal)));

    PyObject* ebrake_pressureAttr = PyUnicode_FromString("ebrake_pressure");
    PyObject* ebrake_pressureVal = PyObject_GetAttr(p_brakeSystObject, ebrake_pressureAttr);
    ASSERT_EQ(p_brakeSyst.ebrake_pressure, static_cast<float>(PyFloat_AsDouble(ebrake_pressureVal)));
}

/// @brief checks the DynAxis python object corresponds to the original DynAxis values
/// @param  p_dynAxis The original DynAxis value
/// @param  p_dynAxisObject The python DynAxis object
void CheckDynAxisData(tDynAxis p_dynAxis, PyObject* p_dynAxisObject)
{
    PyObject* spinVelAttr = PyUnicode_FromString("spinVel");
    PyObject* spinVelVal = PyObject_GetAttr(p_dynAxisObject, spinVelAttr);
    ASSERT_EQ(p_dynAxis.spinVel, static_cast<float>(PyFloat_AsDouble(spinVelVal)));

    PyObject* TqAttr = PyUnicode_FromString("Tq");
    PyObject* TqVal = PyObject_GetAttr(p_dynAxisObject, TqAttr);
    ASSERT_EQ(p_dynAxis.Tq, static_cast<float>(PyFloat_AsDouble(TqVal)));

    PyObject* brkTqAttr = PyUnicode_FromString("brkTq");
    PyObject* brkTqVal = PyObject_GetAttr(p_dynAxisObject, brkTqAttr);
    ASSERT_EQ(p_dynAxis.brkTq, static_cast<float>(PyFloat_AsDouble(brkTqVal)));

    PyObject* IAttr = PyUnicode_FromString("I");
    PyObject* IVal = PyObject_GetAttr(p_dynAxisObject, IAttr);
    ASSERT_EQ(p_dynAxis.I, static_cast<float>(PyFloat_AsDouble(IVal)));
}

/// @brief checks the Differential python object corresponds to the original Differential values
/// @param  p_differential The original Differential value
/// @param  p_differentialObject The python Differential object
void CheckDifferentialData(tDifferential p_differential, PyObject* p_differentialObject)
{
    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_differentialObject, typeAttr);
    ASSERT_EQ(p_differential.type, static_cast<float>(PyFloat_AsDouble(typeVal)));

    PyObject* ratioAttr = PyUnicode_FromString("ratio");
    PyObject* ratioVal = PyObject_GetAttr(p_differentialObject, ratioAttr);
    ASSERT_EQ(p_differential.ratio, static_cast<float>(PyFloat_AsDouble(ratioVal)));

    PyObject* IAttr = PyUnicode_FromString("I");
    PyObject* IVal = PyObject_GetAttr(p_differentialObject, IAttr);
    ASSERT_EQ(p_differential.I, static_cast<float>(PyFloat_AsDouble(IVal)));

    PyObject* efficiencyAttr = PyUnicode_FromString("efficiency");
    PyObject* efficiencyVal = PyObject_GetAttr(p_differentialObject, efficiencyAttr);
    ASSERT_EQ(p_differential.efficiency, static_cast<float>(PyFloat_AsDouble(efficiencyVal)));

    PyObject* biasAttr = PyUnicode_FromString("bias");
    PyObject* biasVal = PyObject_GetAttr(p_differentialObject, biasAttr);
    ASSERT_EQ(p_differential.bias, static_cast<float>(PyFloat_AsDouble(biasVal)));

    PyObject* dTqMinAttr = PyUnicode_FromString("dTqMin");
    PyObject* dTqMinVal = PyObject_GetAttr(p_differentialObject, dTqMinAttr);
    ASSERT_EQ(p_differential.dTqMin, static_cast<float>(PyFloat_AsDouble(dTqMinVal)));

    PyObject* dTqMaxAttr = PyUnicode_FromString("dTqMax");
    PyObject* dTqMaxVal = PyObject_GetAttr(p_differentialObject, dTqMaxAttr);
    ASSERT_EQ(p_differential.dTqMax, static_cast<float>(PyFloat_AsDouble(dTqMaxVal)));

    PyObject* dSlipMaxAttr = PyUnicode_FromString("dSlipMax");
    PyObject* dSlipMaxVal = PyObject_GetAttr(p_differentialObject, dSlipMaxAttr);
    ASSERT_EQ(p_differential.dSlipMax, static_cast<float>(PyFloat_AsDouble(dSlipMaxVal)));

    PyObject* dCoastSlipMaxAttr = PyUnicode_FromString("dCoastSlipMax");
    PyObject* dCoastSlipMaxVal = PyObject_GetAttr(p_differentialObject, dCoastSlipMaxAttr);
    ASSERT_EQ(p_differential.dCoastSlipMax, static_cast<float>(PyFloat_AsDouble(dCoastSlipMaxVal)));

    PyObject* lockInputTqAttr = PyUnicode_FromString("lockInputTq");
    PyObject* lockInputTqVal = PyObject_GetAttr(p_differentialObject, lockInputTqAttr);
    ASSERT_EQ(p_differential.lockInputTq, static_cast<float>(PyFloat_AsDouble(lockInputTqVal)));

    PyObject* viscosityAttr = PyUnicode_FromString("viscosity");
    PyObject* viscosityVal = PyObject_GetAttr(p_differentialObject, viscosityAttr);
    ASSERT_EQ(p_differential.viscosity, static_cast<float>(PyFloat_AsDouble(viscosityVal)));

    PyObject* viscomaxAttr = PyUnicode_FromString("viscomax");
    PyObject* viscomaxVal = PyObject_GetAttr(p_differentialObject, viscomaxAttr);
    ASSERT_EQ(p_differential.viscomax, static_cast<float>(PyFloat_AsDouble(viscomaxVal)));

    PyObject* invarAttr = PyUnicode_FromString("invar");
    PyObject* invarVal = PyObject_GetAttr(p_differentialObject, invarAttr);
    CheckDynAxisData(p_differential.feedBack, invarVal);

    PyObject* feedbackAttr = PyUnicode_FromString("feedback");
    PyObject* feedbackVal = PyObject_GetAttr(p_differentialObject, feedbackAttr);
    CheckDynAxisData(p_differential.feedBack, feedbackVal);
}

/// @brief checks the Axle python object corresponds to the original Axle values
/// @param  p_axle The original Axle value
/// @param  p_axleObject The python Axle object
void CheckAxleData(tAxle p_axle, PyObject* p_axleObject)
{
    PyObject* xposAttr = PyUnicode_FromString("xpos");
    PyObject* xposVal = PyObject_GetAttr(p_axleObject, xposAttr);
    ASSERT_EQ(p_axle.xpos, static_cast<float>(PyFloat_AsDouble(xposVal)));

    PyObject* arbSuspAttr = PyUnicode_FromString("arbSusparbSusp");
    PyObject* arbSuspVal = PyObject_GetAttr(p_axleObject, arbSuspAttr);
    CheckSuspensionData(p_axle.heaveSusp, arbSuspVal);

    PyObject* heaveSuspAttr = PyUnicode_FromString("heaveSusp");
    PyObject* heaveSuspVal = PyObject_GetAttr(p_axleObject, heaveSuspAttr);
    CheckSuspensionData(p_axle.heaveSusp, heaveSuspVal);

    PyObject* wheight0Attr = PyUnicode_FromString("wheight0");
    PyObject* wheight0Val = PyObject_GetAttr(p_axleObject, wheight0Attr);
    ASSERT_EQ(p_axle.wheight0, static_cast<float>(PyFloat_AsDouble(wheight0Val)));

    PyObject* forceAttr = PyUnicode_FromString("force");
    PyObject* forceVal = PyList_AsTuple(PyObject_GetAttr(p_axleObject, forceAttr));
    ASSERT_EQ(p_axle.force[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(forceVal, 0))));
    ASSERT_EQ(p_axle.force[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(forceVal, 1))));

    PyObject* IAttr = PyUnicode_FromString("I");
    PyObject* IVal = PyObject_GetAttr(p_axleObject, IAttr);
    ASSERT_EQ(p_axle.I, static_cast<float>(PyFloat_AsDouble(IVal)));
}

/// @brief checks the Steer python object corresponds to the original Steer values
/// @param  p_steer The original Steer value
/// @param  p_steerObject The python Steer object
void CheckSteerData(tSteer p_steer, PyObject* p_steerObject)
{
    PyObject* steerLockAttr = PyUnicode_FromString("steerLock");
    PyObject* steerLockVal = PyObject_GetAttr(p_steerObject, steerLockAttr);
    ASSERT_EQ(p_steer.steerLock, static_cast<float>(PyFloat_AsDouble(steerLockVal)));

    PyObject* maxSpeedAttr = PyUnicode_FromString("maxSpeed");
    PyObject* maxSpeedVal = PyObject_GetAttr(p_steerObject, maxSpeedAttr);
    ASSERT_EQ(p_steer.maxSpeed, static_cast<float>(PyFloat_AsDouble(maxSpeedVal)));

    PyObject* steerAttr = PyUnicode_FromString("steer");
    PyObject* steerVal = PyObject_GetAttr(p_steerObject, steerAttr);
    ASSERT_EQ(p_steer.steer, static_cast<float>(PyFloat_AsDouble(steerVal)));
}

void CheckWheelSystData(tWheel p_wheelSyst, PyObject* p_wheelSystObject)
{
    PyObject* suspAttr = PyUnicode_FromString("susp");
    PyObject* suspVal = PyObject_GetAttr(p_wheelSystObject, suspAttr);
    CheckSuspensionData(p_wheelSyst.susp, suspVal);

    PyObject* brakeAttr = PyUnicode_FromString("brake");
    PyObject* brakeVal = PyObject_GetAttr(p_wheelSystObject, brakeAttr);
    CheckBrakeData(p_wheelSyst.brake, brakeVal);

    PyObject* forcesAttr = PyUnicode_FromString("forces");
    PyObject* forcesVal = PyObject_GetAttr(p_wheelSystObject, forcesAttr);
    CheckVectorData(p_wheelSyst.forces.x, p_wheelSyst.forces.y, p_wheelSyst.forces.z, forcesVal);

    PyObject* torquesAttr = PyUnicode_FromString("torques");
    PyObject* torquesVal = PyObject_GetAttr(p_wheelSystObject, torquesAttr);
    CheckVectorData(p_wheelSyst.torques.x, p_wheelSyst.torques.y, p_wheelSyst.torques.z, torquesVal);

    PyObject* torqueAlignAttr = PyUnicode_FromString("torqueAlign");
    PyObject* torqueAlignVal = PyObject_GetAttr(p_wheelSystObject, torqueAlignAttr);
    ASSERT_EQ(p_wheelSyst.torqueAlign, static_cast<float>(PyFloat_AsDouble(torqueAlignVal)));

    PyObject* roolResAttr = PyUnicode_FromString("roolRes");
    PyObject* roolResVal = PyObject_GetAttr(p_wheelSystObject, roolResAttr);
    ASSERT_EQ(p_wheelSyst.rollRes, static_cast<float>(PyFloat_AsDouble(roolResVal)));

    PyObject* rideHeightAttr = PyUnicode_FromString("rideHeight");
    PyObject* rideHeightVal = PyObject_GetAttr(p_wheelSystObject, rideHeightAttr);
    ASSERT_EQ(p_wheelSyst.rideHeight, static_cast<float>(PyFloat_AsDouble(rideHeightVal)));

    PyObject* zRoadAttr = PyUnicode_FromString("zRoad");
    PyObject* zRoadVal = PyObject_GetAttr(p_wheelSystObject, zRoadAttr);
    ASSERT_EQ(p_wheelSyst.zRoad, static_cast<float>(PyFloat_AsDouble(zRoadVal)));

    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_wheelSystObject, posAttr);
    CheckVectorData(p_wheelSyst.pos.x, p_wheelSyst.pos.y, p_wheelSyst.pos.z, posVal);

    PyObject* bodyVelAttr = PyUnicode_FromString("bodyVel");
    PyObject* bodyVelVal = PyObject_GetAttr(p_wheelSystObject, bodyVelAttr);
    CheckVectorData(p_wheelSyst.bodyVel.x, p_wheelSyst.bodyVel.y, p_wheelSyst.bodyVel.z, bodyVelVal);

    PyObject* driveTqAttr = PyUnicode_FromString("driveTq");
    PyObject* driveTqVal = PyObject_GetAttr(p_wheelSystObject, driveTqAttr);
    ASSERT_EQ(p_wheelSyst.driveTq, static_cast<float>(PyFloat_AsDouble(driveTqVal)));

    PyObject* vtAttr = PyUnicode_FromString("vt");
    PyObject* vtVal = PyObject_GetAttr(p_wheelSystObject, vtAttr);
    ASSERT_EQ(p_wheelSyst.vt, static_cast<float>(PyFloat_AsDouble(vtVal)));

    PyObject* spinTqAttr = PyUnicode_FromString("spinTq");
    PyObject* spinTqVal = PyObject_GetAttr(p_wheelSystObject, spinTqAttr);
    ASSERT_EQ(p_wheelSyst.spinTq, static_cast<float>(PyFloat_AsDouble(spinTqVal)));

    PyObject* spinVelAttr = PyUnicode_FromString("spinVel");
    PyObject* spinVelVal = PyObject_GetAttr(p_wheelSystObject, spinVelAttr);
    ASSERT_EQ(p_wheelSyst.spinVel, static_cast<float>(PyFloat_AsDouble(spinVelVal)));

    PyObject* prespinVelAttr = PyUnicode_FromString("prespinVel");
    PyObject* prespinVelVal = PyObject_GetAttr(p_wheelSystObject, prespinVelAttr);
    ASSERT_EQ(p_wheelSyst.prespinVel, static_cast<float>(PyFloat_AsDouble(prespinVelVal)));

    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_wheelSystObject, stateAttr);
    ASSERT_EQ(p_wheelSyst.state, static_cast<int>(PyLong_AsLong(stateVal)));

    PyObject* axleFzAttr = PyUnicode_FromString("axleFz");
    PyObject* axleFzVal = PyObject_GetAttr(p_wheelSystObject, axleFzAttr);
    ASSERT_EQ(p_wheelSyst.axleFz, static_cast<float>(PyFloat_AsDouble(axleFzVal)));

    PyObject* axleFz3rdAttr = PyUnicode_FromString("axleFz3rd");
    PyObject* axleFz3rdVal = PyObject_GetAttr(p_wheelSystObject, axleFz3rdAttr);
    ASSERT_EQ(p_wheelSyst.axleFz3rd, static_cast<float>(PyFloat_AsDouble(axleFz3rdVal)));

    PyObject* trkPosAttr = PyUnicode_FromString("trkPos");
    PyObject* trkPosVal = PyObject_GetAttr(p_wheelSystObject, trkPosAttr);
    CheckTrackLocationData(p_wheelSyst.trkPos, trkPosVal);

    PyObject* relPosAttr = PyUnicode_FromString("relPos");
    PyObject* relPosVal = PyObject_GetAttr(p_wheelSystObject, relPosAttr);
    CheckPosDObject(p_wheelSyst.relPos, relPosVal);

    PyObject* saAttr = PyUnicode_FromString("sa");
    PyObject* saVal = PyObject_GetAttr(p_wheelSystObject, saAttr);
    ASSERT_EQ(p_wheelSyst.sa, static_cast<float>(PyFloat_AsDouble(saVal)));

    PyObject* sxAttr = PyUnicode_FromString("sx");
    PyObject* sxVal = PyObject_GetAttr(p_wheelSystObject, sxAttr);
    ASSERT_EQ(p_wheelSyst.sx, static_cast<float>(PyFloat_AsDouble(sxVal)));

    PyObject* steerAttr = PyUnicode_FromString("steer");
    PyObject* steerVal = PyObject_GetAttr(p_wheelSystObject, steerAttr);
    ASSERT_EQ(p_wheelSyst.steer, static_cast<float>(PyFloat_AsDouble(steerVal)));

    PyObject* staticPosAttr = PyUnicode_FromString("staticPos");
    PyObject* staticPosVal = PyObject_GetAttr(p_wheelSystObject, staticPosAttr);
    CheckPosDObject(p_wheelSyst.staticPos, staticPosVal);

    PyObject* cosaxAttr = PyUnicode_FromString("cosax");
    PyObject* cosaxVal = PyObject_GetAttr(p_wheelSystObject, cosaxAttr);
    ASSERT_EQ(p_wheelSyst.cosax, static_cast<float>(PyFloat_AsDouble(cosaxVal)));

    PyObject* sinaxAttr = PyUnicode_FromString("sinax");
    PyObject* sinaxVal = PyObject_GetAttr(p_wheelSystObject, sinaxAttr);
    ASSERT_EQ(p_wheelSyst.sinax, static_cast<float>(PyFloat_AsDouble(sinaxVal)));

    PyObject* weight0Attr = PyUnicode_FromString("weight0");
    PyObject* weight0Val = PyObject_GetAttr(p_wheelSystObject, weight0Attr);
    ASSERT_EQ(p_wheelSyst.weight0, static_cast<float>(PyFloat_AsDouble(weight0Val)));

    PyObject* tireSpringRateAttr = PyUnicode_FromString("tireSpringRate");
    PyObject* tireSpringRateVal = PyObject_GetAttr(p_wheelSystObject, tireSpringRateAttr);
    ASSERT_EQ(p_wheelSyst.tireSpringRate, static_cast<float>(PyFloat_AsDouble(tireSpringRateVal)));

    PyObject* radiusAttr = PyUnicode_FromString("radius");
    PyObject* radiusVal = PyObject_GetAttr(p_wheelSystObject, radiusAttr);
    ASSERT_EQ(p_wheelSyst.radius, static_cast<float>(PyFloat_AsDouble(radiusVal)));

    PyObject* muAttr = PyUnicode_FromString("mu");
    PyObject* muVal = PyObject_GetAttr(p_wheelSystObject, muAttr);
    ASSERT_EQ(p_wheelSyst.mu, static_cast<float>(PyFloat_AsDouble(muVal)));

    PyObject* IAttr = PyUnicode_FromString("I");
    PyObject* IVal = PyObject_GetAttr(p_wheelSystObject, IAttr);
    ASSERT_EQ(p_wheelSyst.I, static_cast<float>(PyFloat_AsDouble(IVal)));

    PyObject* curIAttr = PyUnicode_FromString("curI");
    PyObject* curIVal = PyObject_GetAttr(p_wheelSystObject, curIAttr);
    ASSERT_EQ(p_wheelSyst.curI, static_cast<float>(PyFloat_AsDouble(curIVal)));

    PyObject* mfCAttr = PyUnicode_FromString("mfC");
    PyObject* mfCVal = PyObject_GetAttr(p_wheelSystObject, mfCAttr);
    ASSERT_EQ(p_wheelSyst.mfC, static_cast<float>(PyFloat_AsDouble(mfCVal)));

    PyObject* mfBAttr = PyUnicode_FromString("mfB");
    PyObject* mfBVal = PyObject_GetAttr(p_wheelSystObject, mfBAttr);
    ASSERT_EQ(p_wheelSyst.mfB, static_cast<float>(PyFloat_AsDouble(mfBVal)));

    PyObject* mfEAttr = PyUnicode_FromString("mfE");
    PyObject* mfEVal = PyObject_GetAttr(p_wheelSystObject, mfEAttr);
    ASSERT_EQ(p_wheelSyst.mfE, static_cast<float>(PyFloat_AsDouble(mfEVal)));

    PyObject* lfMaxAttr = PyUnicode_FromString("lfMax");
    PyObject* lfMaxVal = PyObject_GetAttr(p_wheelSystObject, lfMaxAttr);
    ASSERT_EQ(p_wheelSyst.lfMax, static_cast<float>(PyFloat_AsDouble(lfMaxVal)));

    PyObject* lfMinAttr = PyUnicode_FromString("lfMin");
    PyObject* lfMinVal = PyObject_GetAttr(p_wheelSystObject, lfMinAttr);
    ASSERT_EQ(p_wheelSyst.lfMin, static_cast<float>(PyFloat_AsDouble(lfMinVal)));

    PyObject* lfKAttr = PyUnicode_FromString("lfK");
    PyObject* lfKVal = PyObject_GetAttr(p_wheelSystObject, lfKAttr);
    ASSERT_EQ(p_wheelSyst.lfK, static_cast<float>(PyFloat_AsDouble(lfKVal)));

    PyObject* opLoadAttr = PyUnicode_FromString("opLoad");
    PyObject* opLoadVal = PyObject_GetAttr(p_wheelSystObject, opLoadAttr);
    ASSERT_EQ(p_wheelSyst.opLoad, static_cast<float>(PyFloat_AsDouble(opLoadVal)));

    PyObject* AlignTqFactorAttr = PyUnicode_FromString("AlignTqFactor");
    PyObject* AlignTqFactorVal = PyObject_GetAttr(p_wheelSystObject, AlignTqFactorAttr);
    ASSERT_EQ(p_wheelSyst.AlignTqFactor, static_cast<float>(PyFloat_AsDouble(AlignTqFactorVal)));

    PyObject* massAttr = PyUnicode_FromString("mass");
    PyObject* massVal = PyObject_GetAttr(p_wheelSystObject, massAttr);
    ASSERT_EQ(p_wheelSyst.mass, static_cast<float>(PyFloat_AsDouble(massVal)));

    PyObject* camberAttr = PyUnicode_FromString("camber");
    PyObject* camberVal = PyObject_GetAttr(p_wheelSystObject, camberAttr);
    ASSERT_EQ(p_wheelSyst.camber, static_cast<float>(PyFloat_AsDouble(camberVal)));

    PyObject* pressureAttr = PyUnicode_FromString("pressure");
    PyObject* pressureVal = PyObject_GetAttr(p_wheelSystObject, pressureAttr);
    ASSERT_EQ(p_wheelSyst.pressure, static_cast<float>(PyFloat_AsDouble(pressureVal)));

    PyObject* TtireAttr = PyUnicode_FromString("Ttire");
    PyObject* TtireVal = PyObject_GetAttr(p_wheelSystObject, TtireAttr);
    ASSERT_EQ(p_wheelSyst.Ttire, static_cast<float>(PyFloat_AsDouble(TtireVal)));

    PyObject* ToptAttr = PyUnicode_FromString("Topt");
    PyObject* ToptVal = PyObject_GetAttr(p_wheelSystObject, ToptAttr);
    ASSERT_EQ(p_wheelSyst.Topt, static_cast<float>(PyFloat_AsDouble(ToptVal)));

    PyObject* TinitAttr = PyUnicode_FromString("Tinit");
    PyObject* TinitVal = PyObject_GetAttr(p_wheelSystObject, TinitAttr);
    ASSERT_EQ(p_wheelSyst.Tinit, static_cast<float>(PyFloat_AsDouble(TinitVal)));

    PyObject* muTmultAttr = PyUnicode_FromString("muTmult");
    PyObject* muTmultVal = PyObject_GetAttr(p_wheelSystObject, muTmultAttr);
    ASSERT_EQ(p_wheelSyst.muTmult, static_cast<float>(PyFloat_AsDouble(muTmultVal)));

    PyObject* heatingmAttr = PyUnicode_FromString("heatingm");
    PyObject* heatingmVal = PyObject_GetAttr(p_wheelSystObject, heatingmAttr);
    ASSERT_EQ(p_wheelSyst.heatingm, static_cast<float>(PyFloat_AsDouble(heatingmVal)));

    PyObject* aircoolmAttr = PyUnicode_FromString("aircoolm");
    PyObject* aircoolmVal = PyObject_GetAttr(p_wheelSystObject, aircoolmAttr);
    ASSERT_EQ(p_wheelSyst.aircoolm, static_cast<float>(PyFloat_AsDouble(aircoolmVal)));

    PyObject* speedcoolmAttr = PyUnicode_FromString("speedcoolm");
    PyObject* speedcoolmVal = PyObject_GetAttr(p_wheelSystObject, speedcoolmAttr);
    ASSERT_EQ(p_wheelSyst.speedcoolm, static_cast<float>(PyFloat_AsDouble(speedcoolmVal)));

    PyObject* wearrateAttr = PyUnicode_FromString("wearrate");
    PyObject* wearrateVal = PyObject_GetAttr(p_wheelSystObject, wearrateAttr);
    ASSERT_EQ(p_wheelSyst.wearrate, static_cast<float>(PyFloat_AsDouble(wearrateVal)));

    PyObject* treadDepthAttr = PyUnicode_FromString("treadDepth");
    PyObject* treadDepthVal = PyObject_GetAttr(p_wheelSystObject, treadDepthAttr);
    ASSERT_EQ(p_wheelSyst.treadDepth, static_cast<float>(PyFloat_AsDouble(treadDepthVal)));

    PyObject* critTreadDepthAttr = PyUnicode_FromString("critTreadDepth");
    PyObject* critTreadDepthVal = PyObject_GetAttr(p_wheelSystObject, critTreadDepthAttr);
    ASSERT_EQ(p_wheelSyst.critTreadDepth, static_cast<float>(PyFloat_AsDouble(critTreadDepthVal)));

    PyObject* invarAttr = PyUnicode_FromString("invar");
    PyObject* invarVal = PyObject_GetAttr(p_wheelSystObject, invarAttr);
    CheckDynAxisData(p_wheelSyst.in, invarVal);

    PyObject* muTDmultAttr = PyUnicode_FromString("muTDmult");
    PyObject* muTDmultVal = PyList_AsTuple(PyObject_GetAttr(p_wheelSystObject, muTDmultAttr));
    ASSERT_EQ(p_wheelSyst.muTDmult[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDmultVal, 0))));
    ASSERT_EQ(p_wheelSyst.muTDmult[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDmultVal, 1))));

    PyObject* muTDoffsetAttr = PyUnicode_FromString("muTDoffset");
    PyObject* muTDoffsetVal = PyList_AsTuple(PyObject_GetAttr(p_wheelSystObject, muTDoffsetAttr));
    ASSERT_EQ(p_wheelSyst.muTDoffset[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDoffsetVal, 0))));
    ASSERT_EQ(p_wheelSyst.muTDoffset[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDoffsetVal, 1))));

    PyObject* feedBackAttr = PyUnicode_FromString("feedBack");
    PyObject* feedBackVal = PyObject_GetAttr(p_wheelSystObject, feedBackAttr);
    CheckDynAxisData(p_wheelSyst.feedBack, feedBackVal);

    PyObject* preFnAttr = PyUnicode_FromString("preFn");
    PyObject* preFnVal = PyObject_GetAttr(p_wheelSystObject, preFnAttr);
    ASSERT_EQ(p_wheelSyst.preFn, static_cast<float>(PyFloat_AsDouble(preFnVal)));

    PyObject* preFtAttr = PyUnicode_FromString("preFt");
    PyObject* preFtVal = PyObject_GetAttr(p_wheelSystObject, preFtAttr);
    ASSERT_EQ(p_wheelSyst.preFt, static_cast<float>(PyFloat_AsDouble(preFtVal)));
}

/// @brief checks the Gearbox python object corresponds to the original Gearbox values
/// @param  p_gearbox The original Gearbox value
/// @param  p_gearboxObject The python Gearbox object
void CheckGearboxData(tGearbox p_gearbox, PyObject* p_gearboxObject)
{
    PyObject* gearAttr = PyUnicode_FromString("gear");
    PyObject* gearVal = PyObject_GetAttr(p_gearboxObject, gearAttr);
    ASSERT_EQ(p_gearbox.gear, static_cast<int>(PyLong_AsLong(gearVal)));

    PyObject* gearMinAttr = PyUnicode_FromString("gearMin");
    PyObject* gearMinVal = PyObject_GetAttr(p_gearboxObject, gearMinAttr);
    ASSERT_EQ(p_gearbox.gearMin, static_cast<int>(PyLong_AsLong(gearMinVal)));

    PyObject* gearMaxAttr = PyUnicode_FromString("gearMax");
    PyObject* gearMaxVal = PyObject_GetAttr(p_gearboxObject, gearMaxAttr);
    ASSERT_EQ(p_gearbox.gearMax, static_cast<int>(PyLong_AsLong(gearMaxVal)));

    PyObject* gearNextAttr = PyUnicode_FromString("gearNext");
    PyObject* gearNextVal = PyObject_GetAttr(p_gearboxObject, gearNextAttr);
    ASSERT_EQ(p_gearbox.gearNext, static_cast<int>(PyLong_AsLong(gearNextVal)));

    PyObject* shiftTimeAttr = PyUnicode_FromString("shiftTime");
    PyObject* shiftTimeVal = PyObject_GetAttr(p_gearboxObject, shiftTimeAttr);
    ASSERT_EQ(p_gearbox.shiftTime, static_cast<float>(PyFloat_AsDouble(shiftTimeVal)));

    PyObject* timeToEngageAttr = PyUnicode_FromString("timeToEngage");
    PyObject* timeToEngageVal = PyObject_GetAttr(p_gearboxObject, timeToEngageAttr);
    ASSERT_EQ(p_gearbox.timeToEngage, static_cast<float>(PyFloat_AsDouble(timeToEngageVal)));
}

/// @brief checks the clutch python object corresponds to the original clutch values
/// @param  p_clutch The original Clutch value
/// @param  p_clutchObject The python Clutch object
void CheckClutchData(tClutch p_clutch, PyObject* p_clutchObject)
{
    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_clutchObject, stateAttr);
    ASSERT_EQ(p_clutch.state, static_cast<int>(PyLong_AsLong(stateVal)));

    PyObject* modeAttr = PyUnicode_FromString("mode");
    PyObject* modeVal = PyObject_GetAttr(p_clutchObject, modeAttr);
    ASSERT_EQ(p_clutch.mode, static_cast<int>(PyLong_AsLong(modeVal)));

    PyObject* timeToReleaseAttr = PyUnicode_FromString("timeToRelease");
    PyObject* timeToReleaseVal = PyObject_GetAttr(p_clutchObject, timeToReleaseAttr);
    ASSERT_EQ(p_clutch.timeToRelease, static_cast<float>(PyFloat_AsDouble(timeToReleaseVal)));

    PyObject* releaseTimeAttr = PyUnicode_FromString("releaseTime");
    PyObject* releaseTimeVal = PyObject_GetAttr(p_clutchObject, releaseTimeAttr);
    ASSERT_EQ(p_clutch.releaseTime, static_cast<float>(PyFloat_AsDouble(releaseTimeVal)));

    PyObject* transferValueAttr = PyUnicode_FromString("transferValue");
    PyObject* transferValueVal = PyObject_GetAttr(p_clutchObject, transferValueAttr);
    ASSERT_EQ(p_clutch.transferValue, static_cast<float>(PyFloat_AsDouble(transferValueVal)));
}

/// @brief checks the transmission python object corresponds to the original clutch values
/// @param  p_transmission The original Transmission value
/// @param  p_transmissionObject The python Transmission object
void CheckTransmissionData(tTransmission p_transmission, PyObject* p_transmissionObject)
{
    PyObject* gearboxAttr = PyUnicode_FromString("gearbox");
    PyObject* gearboxVal = PyObject_GetAttr(p_transmissionObject, gearboxAttr);
    CheckGearboxData(p_transmission.gearbox, gearboxVal);

    PyObject* clutchAttr = PyUnicode_FromString("clutch");
    PyObject* clutchVal = PyObject_GetAttr(p_transmissionObject, clutchAttr);
    CheckClutchData(p_transmission.clutch, clutchVal);

    PyObject* overallRatioAttr = PyUnicode_FromString("overallRatio");
    PyObject* overallRatioVal = PyList_AsTuple(PyObject_GetAttr(p_transmissionObject, overallRatioAttr));
    ASSERT_EQ(p_transmission.overallRatio[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 0))));
    ASSERT_EQ(p_transmission.overallRatio[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 1))));
    ASSERT_EQ(p_transmission.overallRatio[2], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 2))));
    ASSERT_EQ(p_transmission.overallRatio[3], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 3))));
    ASSERT_EQ(p_transmission.overallRatio[4], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 4))));
    ASSERT_EQ(p_transmission.overallRatio[5], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 5))));
    ASSERT_EQ(p_transmission.overallRatio[6], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 6))));
    ASSERT_EQ(p_transmission.overallRatio[7], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 7))));
    ASSERT_EQ(p_transmission.overallRatio[8], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 8))));
    ASSERT_EQ(p_transmission.overallRatio[9], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(overallRatioVal, 9))));

    PyObject* gearIAttr = PyUnicode_FromString("gearI");
    PyObject* gearIVal = PyList_AsTuple(PyObject_GetAttr(p_transmissionObject, gearIAttr));
    ASSERT_EQ(p_transmission.gearI[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 0))));
    ASSERT_EQ(p_transmission.gearI[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 1))));
    ASSERT_EQ(p_transmission.gearI[2], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 2))));
    ASSERT_EQ(p_transmission.gearI[3], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 3))));
    ASSERT_EQ(p_transmission.gearI[4], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 4))));
    ASSERT_EQ(p_transmission.gearI[5], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 5))));
    ASSERT_EQ(p_transmission.gearI[6], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 6))));
    ASSERT_EQ(p_transmission.gearI[7], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 7))));
    ASSERT_EQ(p_transmission.gearI[8], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 8))));
    ASSERT_EQ(p_transmission.gearI[9], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearIVal, 9))));

    PyObject* driveIAttr = PyUnicode_FromString("driveI");
    PyObject* driveIVal = PyList_AsTuple(PyObject_GetAttr(p_transmissionObject, driveIAttr));
    ASSERT_EQ(p_transmission.driveI[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 0))));
    ASSERT_EQ(p_transmission.driveI[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 1))));
    ASSERT_EQ(p_transmission.driveI[2], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 2))));
    ASSERT_EQ(p_transmission.driveI[3], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 3))));
    ASSERT_EQ(p_transmission.driveI[4], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 4))));
    ASSERT_EQ(p_transmission.driveI[5], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 5))));
    ASSERT_EQ(p_transmission.driveI[6], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 6))));
    ASSERT_EQ(p_transmission.driveI[7], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 7))));
    ASSERT_EQ(p_transmission.driveI[8], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 8))));
    ASSERT_EQ(p_transmission.driveI[9], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(driveIVal, 9))));

    PyObject* freeIAttr = PyUnicode_FromString("freeI");
    PyObject* freeIVal = PyList_AsTuple(PyObject_GetAttr(p_transmissionObject, freeIAttr));
    ASSERT_EQ(p_transmission.freeI[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 0))));
    ASSERT_EQ(p_transmission.freeI[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 1))));
    ASSERT_EQ(p_transmission.freeI[2], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 2))));
    ASSERT_EQ(p_transmission.freeI[3], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 3))));
    ASSERT_EQ(p_transmission.freeI[4], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 4))));
    ASSERT_EQ(p_transmission.freeI[5], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 5))));
    ASSERT_EQ(p_transmission.freeI[6], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 6))));
    ASSERT_EQ(p_transmission.freeI[7], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 7))));
    ASSERT_EQ(p_transmission.freeI[8], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 8))));
    ASSERT_EQ(p_transmission.freeI[9], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(freeIVal, 9))));

    PyObject* gearEffAttr = PyUnicode_FromString("gearEff");
    PyObject* gearEffVal = PyList_AsTuple(PyObject_GetAttr(p_transmissionObject, gearEffAttr));
    ASSERT_EQ(p_transmission.gearEff[0], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 0))));
    ASSERT_EQ(p_transmission.gearEff[1], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 1))));
    ASSERT_EQ(p_transmission.gearEff[2], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 2))));
    ASSERT_EQ(p_transmission.gearEff[3], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 3))));
    ASSERT_EQ(p_transmission.gearEff[4], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 4))));
    ASSERT_EQ(p_transmission.gearEff[5], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 5))));
    ASSERT_EQ(p_transmission.gearEff[6], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 6))));
    ASSERT_EQ(p_transmission.gearEff[7], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 7))));
    ASSERT_EQ(p_transmission.gearEff[8], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 8))));
    ASSERT_EQ(p_transmission.gearEff[9], static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearEffVal, 9))));

    PyObject* currIAttr = PyUnicode_FromString("currI");
    PyObject* currIVal = PyObject_GetAttr(p_transmissionObject, currIAttr);
    ASSERT_EQ(p_transmission.curI, static_cast<float>(PyFloat_AsDouble(currIVal)));

    PyObject* differentialAttr = PyUnicode_FromString("differential");
    PyObject* differentialVal = PyList_AsTuple(PyObject_GetAttr(p_transmissionObject, differentialAttr));
    CheckDifferentialData(p_transmission.differential[0], PyTuple_GetItem(differentialVal, 0));
    CheckDifferentialData(p_transmission.differential[1], PyTuple_GetItem(differentialVal, 1));
    CheckDifferentialData(p_transmission.differential[2], PyTuple_GetItem(differentialVal, 2));
    CheckDifferentialData(p_transmission.differential[3], PyTuple_GetItem(differentialVal, 3));
}

/// @brief checks the engineCurveElem python object corresponds to the original clutch values
/// @param  p_engineCurveElem The original EngineCurveElem value
/// @param  p_engineCurveElemObject The python EngineCurveElem object
void CheckEngineCurveElem(tEngineCurveElem p_engineCurveElem, PyObject* p_engineCurveElemObject)
{
    PyObject* radsAttr = PyUnicode_FromString("rads");
    PyObject* radsVal = PyObject_GetAttr(p_engineCurveElemObject, radsAttr);
    ASSERT_EQ(p_engineCurveElem.rads, static_cast<float>(PyFloat_AsDouble(radsVal)));

    PyObject* aAttr = PyUnicode_FromString("a");
    PyObject* aVal = PyObject_GetAttr(p_engineCurveElemObject, aAttr);
    ASSERT_EQ(p_engineCurveElem.a, static_cast<float>(PyFloat_AsDouble(aVal)));

    PyObject* bAttr = PyUnicode_FromString("b");
    PyObject* bVal = PyObject_GetAttr(p_engineCurveElemObject, bAttr);
    ASSERT_EQ(p_engineCurveElem.b, static_cast<float>(PyFloat_AsDouble(bVal)));
}

/// @brief checks the engineCurve python object corresponds to the original clutch values
/// @param  p_engineCurve The original engineCurve value
/// @param  p_engineCurveObject The python engineCurve object
void CheckEngineCurveData(tEngineCurve p_engineCurve, PyObject* p_engineCurveObject)
{
    PyObject* maxTqAttr = PyUnicode_FromString("maxTq");
    PyObject* maxTqVal = PyObject_GetAttr(p_engineCurveObject, maxTqAttr);
    ASSERT_EQ(p_engineCurve.maxTq, static_cast<float>(PyFloat_AsDouble(maxTqVal)));

    PyObject* maxPwAttr = PyUnicode_FromString("maxPw");
    PyObject* maxPwVal = PyObject_GetAttr(p_engineCurveObject, maxPwAttr);
    ASSERT_EQ(p_engineCurve.maxPw, static_cast<float>(PyFloat_AsDouble(maxPwVal)));

    PyObject* rpmMaxPwAttr = PyUnicode_FromString("rpmMaxPw");
    PyObject* rpmMaxPwVal = PyObject_GetAttr(p_engineCurveObject, rpmMaxPwAttr);
    ASSERT_EQ(p_engineCurve.rpmMaxPw, static_cast<float>(PyFloat_AsDouble(rpmMaxPwVal)));

    PyObject* TqAtMaxPwAttr = PyUnicode_FromString("TqAtMaxPw");
    PyObject* TqAtMaxPwVal = PyObject_GetAttr(p_engineCurveObject, TqAtMaxPwAttr);
    ASSERT_EQ(p_engineCurve.TqAtMaxPw, static_cast<float>(PyFloat_AsDouble(TqAtMaxPwVal)));

    PyObject* rpmMaxTqAttr = PyUnicode_FromString("rpmMaxTq");
    PyObject* rpmMaxTqVal = PyObject_GetAttr(p_engineCurveObject, rpmMaxTqAttr);
    ASSERT_EQ(p_engineCurve.rpmMaxTq, static_cast<float>(PyFloat_AsDouble(rpmMaxTqVal)));

    PyObject* nbPtsAttr = PyUnicode_FromString("nbPts");
    PyObject* nbPtsVal = PyObject_GetAttr(p_engineCurveObject, nbPtsAttr);
    ASSERT_EQ(p_engineCurve.nbPts, static_cast<int>(PyLong_AsLong(nbPtsVal)));
}

/// @brief checks the engine python object corresponds to the original clutch values
/// @param  p_engine The original engine value
/// @param  p_engineObject The python engine object
void CheckEngineData(tEngine p_engine, PyObject* p_engineObject)
{
    PyObject* curveAttr = PyUnicode_FromString("curve");
    PyObject* curveVal = PyObject_GetAttr(p_engineObject, curveAttr);
    CheckEngineCurveData(p_engine.curve, curveVal);

    PyObject* revsLimiterAttr = PyUnicode_FromString("revsLimiter");
    PyObject* revsLimiterVal = PyObject_GetAttr(p_engineObject, revsLimiterAttr);
    ASSERT_EQ(p_engine.revsLimiter, static_cast<float>(PyFloat_AsDouble(revsLimiterVal)));

    PyObject* revsMaxAttr = PyUnicode_FromString("revsMax");
    PyObject* revsMaxVal = PyObject_GetAttr(p_engineObject, revsMaxAttr);
    ASSERT_EQ(p_engine.revsMax, static_cast<float>(PyFloat_AsDouble(revsMaxVal)));

    PyObject* tickoverAttr = PyUnicode_FromString("tickover");
    PyObject* tickoverVal = PyObject_GetAttr(p_engineObject, tickoverAttr);
    ASSERT_EQ(p_engine.tickover, static_cast<float>(PyFloat_AsDouble(tickoverVal)));

    PyObject* IAttr = PyUnicode_FromString("I");
    PyObject* IVal = PyObject_GetAttr(p_engineObject, IAttr);
    ASSERT_EQ(p_engine.I, static_cast<float>(PyFloat_AsDouble(IVal)));

    PyObject* radsAttr = PyUnicode_FromString("rads");
    PyObject* radsVal = PyObject_GetAttr(p_engineObject, radsAttr);
    ASSERT_EQ(p_engine.rads, static_cast<float>(PyFloat_AsDouble(radsVal)));

    PyObject* TqAttr = PyUnicode_FromString("Tq");
    PyObject* TqVal = PyObject_GetAttr(p_engineObject, TqAttr);
    ASSERT_EQ(p_engine.Tq, static_cast<float>(PyFloat_AsDouble(TqVal)));

    PyObject* Tq_responseAttr = PyUnicode_FromString("Tq_response");
    PyObject* Tq_responseVal = PyObject_GetAttr(p_engineObject, Tq_responseAttr);
    ASSERT_EQ(p_engine.Tq_response, static_cast<float>(PyFloat_AsDouble(Tq_responseVal)));

    PyObject* I_jointAttr = PyUnicode_FromString("I_joint");
    PyObject* I_jointVal = PyObject_GetAttr(p_engineObject, I_jointAttr);
    ASSERT_EQ(p_engine.I_joint, static_cast<float>(PyFloat_AsDouble(I_jointVal)));

    PyObject* fuelconsAttr = PyUnicode_FromString("fuelcons");
    PyObject* fuelconsVal = PyObject_GetAttr(p_engineObject, fuelconsAttr);
    ASSERT_EQ(p_engine.fuelcons, static_cast<float>(PyFloat_AsDouble(fuelconsVal)));

    PyObject* brakeCoeffAttr = PyUnicode_FromString("brakeCoeff");
    PyObject* brakeCoeffVal = PyObject_GetAttr(p_engineObject, brakeCoeffAttr);
    ASSERT_EQ(p_engine.brakeCoeff, static_cast<float>(PyFloat_AsDouble(brakeCoeffVal)));

    PyObject* brakeLinCoeffAttr = PyUnicode_FromString("brakeLinCoeff");
    PyObject* brakeLinCoeffVal = PyObject_GetAttr(p_engineObject, brakeLinCoeffAttr);
    ASSERT_EQ(p_engine.brakeLinCoeff, static_cast<float>(PyFloat_AsDouble(brakeLinCoeffVal)));

    PyObject* pressureAttr = PyUnicode_FromString("pressure");
    PyObject* pressureVal = PyObject_GetAttr(p_engineObject, pressureAttr);
    ASSERT_EQ(p_engine.pressure, static_cast<float>(PyFloat_AsDouble(pressureVal)));

    PyObject* exhaust_presureAttr = PyUnicode_FromString("exhaust_pressure");
    PyObject* exhaust_presureVal = PyObject_GetAttr(p_engineObject, exhaust_presureAttr);
    ASSERT_EQ(p_engine.exhaust_pressure, static_cast<float>(PyFloat_AsDouble(exhaust_presureVal)));

    PyObject* exhaust_refractAttr = PyUnicode_FromString("exhaust_refract");
    PyObject* exhaust_refractVal = PyObject_GetAttr(p_engineObject, exhaust_refractAttr);
    ASSERT_EQ(p_engine.exhaust_refract, static_cast<float>(PyFloat_AsDouble(exhaust_refractVal)));

    PyObject* timeInLimiterAttr = PyUnicode_FromString("timeInLimiter");
    PyObject* timeInLimiterVal = PyObject_GetAttr(p_engineObject, timeInLimiterAttr);
    ASSERT_EQ(p_engine.timeInLimiter, static_cast<float>(PyFloat_AsDouble(timeInLimiterVal)));

    PyObject* TCLAttr = PyUnicode_FromString("TCL");
    PyObject* TCLVal = PyObject_GetAttr(p_engineObject, TCLAttr);
    ASSERT_EQ(p_engine.TCL, static_cast<float>(PyFloat_AsDouble(TCLVal)));

    PyObject* EnableTCLAttr = PyUnicode_FromString("EnableTCL");
    PyObject* EnableTCLVal = PyObject_GetAttr(p_engineObject, EnableTCLAttr);
    ASSERT_TRUE(p_engine.EnableTCL == static_cast<bool>(PyLong_AsLong(EnableTCLVal)));
}

/// @brief checks the car python object corresponds to the original clutch values
/// @param  p_engine The original car value
/// @param  p_engineObject The python car object
void CheckSimCarData(tCar p_car, PyObject* p_carObject)
{
    PyObject* preCtrlAttr = PyUnicode_FromString("preCtrl");
    PyObject* preCtrlVal = PyObject_GetAttr(p_carObject, preCtrlAttr);
    CheckCarCtrlData(p_car.preCtrl, preCtrlVal);

    PyObject* axleAttr = PyUnicode_FromString("axle");
    PyObject* axleVal = PyList_AsTuple(PyObject_GetAttr(p_carObject, axleAttr));
    CheckAxleData(p_car.axle[0], PyTuple_GetItem(axleVal, 0));
    CheckAxleData(p_car.axle[1], PyTuple_GetItem(axleVal, 1));

    PyObject* wheelAttr = PyUnicode_FromString("wheel");
    PyObject* wheelVal = PyList_AsTuple(PyObject_GetAttr(p_carObject, wheelAttr));
    CheckWheelSystData(p_car.wheel[0], PyTuple_GetItem(wheelVal, 0));
    CheckWheelSystData(p_car.wheel[1], PyTuple_GetItem(wheelVal, 1));
    CheckWheelSystData(p_car.wheel[2], PyTuple_GetItem(wheelVal, 2));
    CheckWheelSystData(p_car.wheel[3], PyTuple_GetItem(wheelVal, 3));

    PyObject* steerAttr = PyUnicode_FromString("steer");
    PyObject* steerVal = PyObject_GetAttr(p_carObject, steerAttr);
    CheckSteerData(p_car.steer, steerVal);

    PyObject* brkSystAttr = PyUnicode_FromString("brkSyst");
    PyObject* brkSystVal = PyObject_GetAttr(p_carObject, brkSystAttr);
    CheckBrakeSystData(p_car.brkSyst, brkSystVal);

    PyObject* aeroAttr = PyUnicode_FromString("aero");
    PyObject* aeroVal = PyObject_GetAttr(p_carObject, aeroAttr);
    CheckAeroData(p_car.aero, aeroVal);

    PyObject* wingAttr = PyUnicode_FromString("wing");
    PyObject* wingVal = PyList_AsTuple(PyObject_GetAttr(p_carObject, wingAttr));
    CheckWingData(p_car.wing[0], PyTuple_GetItem(wingVal, 0));
    CheckWingData(p_car.wing[1], PyTuple_GetItem(wingVal, 1));

    PyObject* transmissionAttr = PyUnicode_FromString("transmission");
    PyObject* transmissionVal = PyObject_GetAttr(p_carObject, transmissionAttr);
    CheckTransmissionData(p_car.transmission, transmissionVal);

    PyObject* engineAttr = PyUnicode_FromString("engine");
    PyObject* engineVal = PyObject_GetAttr(p_carObject, engineAttr);
    CheckEngineData(p_car.engine, engineVal);

    PyObject* dimensionAttr = PyUnicode_FromString("dimension");
    PyObject* dimensionVal = PyObject_GetAttr(p_carObject, dimensionAttr);
    CheckVectorData(p_car.dimension.x, p_car.dimension.y, p_car.dimension.z, dimensionVal);

    PyObject* massAttr = PyUnicode_FromString("mass");
    PyObject* massVal = PyObject_GetAttr(p_carObject, massAttr);
    ASSERT_EQ(p_car.mass, static_cast<float>(PyFloat_AsDouble(massVal)));

    PyObject* MinvAttr = PyUnicode_FromString("Minv");
    PyObject* MinvVal = PyObject_GetAttr(p_carObject, MinvAttr);
    ASSERT_EQ(p_car.Minv, static_cast<float>(PyFloat_AsDouble(MinvVal)));

    PyObject* tankAttr = PyUnicode_FromString("tank");
    PyObject* tankVal = PyObject_GetAttr(p_carObject, tankAttr);
    ASSERT_EQ(p_car.tank, static_cast<float>(PyFloat_AsDouble(tankVal)));

    PyObject* statGCAttr = PyUnicode_FromString("statGC");
    PyObject* statGCVal = PyObject_GetAttr(p_carObject, statGCAttr);
    CheckVectorData(p_car.statGC.x, p_car.statGC.y, p_car.statGC.z, statGCVal);

    PyObject* IinvAttr = PyUnicode_FromString("Iinv");
    PyObject* IinvVal = PyObject_GetAttr(p_carObject, IinvAttr);
    CheckVectorData(p_car.Iinv.x, p_car.Iinv.y, p_car.Iinv.z, IinvVal);

    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_carObject, fuelAttr);
    ASSERT_EQ(p_car.fuel, static_cast<float>(PyFloat_AsDouble(fuelVal)));

    PyObject* fuel_consumptionAttr = PyUnicode_FromString("fuel_consumption");
    PyObject* fuel_consumptionVal = PyObject_GetAttr(p_carObject, fuel_consumptionAttr);
    ASSERT_EQ(p_car.fuel_consumption, static_cast<float>(PyFloat_AsDouble(fuel_consumptionVal)));

    PyObject* fuel_prevAttr = PyUnicode_FromString("fuel_prev");
    PyObject* fuel_prevVal = PyObject_GetAttr(p_carObject, fuel_prevAttr);
    ASSERT_EQ(p_car.fuel_prev, static_cast<float>(PyFloat_AsDouble(fuel_prevVal)));

    PyObject* fuel_timeAttr = PyUnicode_FromString("fuel_time");
    PyObject* fuel_timeVal = PyObject_GetAttr(p_carObject, fuel_timeAttr);
    ASSERT_EQ(p_car.fuel_time, static_cast<float>(PyFloat_AsDouble(fuel_timeVal)));

    PyObject* DynGCAttr = PyUnicode_FromString("DynGC");
    PyObject* DynGCVal = PyObject_GetAttr(p_carObject, DynGCAttr);
    CheckDynamicPointData(p_car.DynGC, DynGCVal);

    PyObject* DynGCgAttr = PyUnicode_FromString("DynGCg");
    PyObject* DynGCgVal = PyObject_GetAttr(p_carObject, DynGCgAttr);
    CheckDynamicPointData(p_car.DynGCg, DynGCgVal);

    PyObject* VelCollAttr = PyUnicode_FromString("VelColl");
    PyObject* VelCollVal = PyObject_GetAttr(p_carObject, VelCollAttr);
    CheckPosDObject(p_car.VelColl, VelCollVal);

    PyObject* preDynGCAttr = PyUnicode_FromString("preDynGC");
    PyObject* preDynGCVal = PyObject_GetAttr(p_carObject, preDynGCAttr);
    CheckDynamicPointData(p_car.preDynGC, preDynGCVal);

    PyObject* trkPosAttr = PyUnicode_FromString("trkPos");
    PyObject* trkPosVal = PyObject_GetAttr(p_carObject, trkPosAttr);
    CheckTrackLocationData(p_car.trkPos, trkPosVal);

    PyObject* airSpeed2Attr = PyUnicode_FromString("airSpeed2");
    PyObject* airSpeed2Val = PyObject_GetAttr(p_carObject, airSpeed2Attr);
    ASSERT_EQ(p_car.airSpeed2, static_cast<float>(PyFloat_AsDouble(airSpeed2Val)));

    PyObject* CoszAttr = PyUnicode_FromString("Cosz");
    PyObject* CoszVal = PyObject_GetAttr(p_carObject, CoszAttr);
    ASSERT_EQ(p_car.Cosz, static_cast<float>(PyFloat_AsDouble(CoszVal)));

    PyObject* SinzAttr = PyUnicode_FromString("Sinz");
    PyObject* SinzVal = PyObject_GetAttr(p_carObject, SinzAttr);
    ASSERT_EQ(p_car.Sinz, static_cast<float>(PyFloat_AsDouble(SinzVal)));

    PyObject* cornerAttr = PyUnicode_FromString("corner");
    PyObject* cornerVal = PyList_AsTuple(PyObject_GetAttr(p_carObject, cornerAttr));
    CheckDynamicPointData(p_car.corner[0], PyTuple_GetItem(cornerVal, 0));
    CheckDynamicPointData(p_car.corner[1], PyTuple_GetItem(cornerVal, 1));
    CheckDynamicPointData(p_car.corner[2], PyTuple_GetItem(cornerVal, 2));
    CheckDynamicPointData(p_car.corner[3], PyTuple_GetItem(cornerVal, 3));

    PyObject* collisionAttr = PyUnicode_FromString("collision");
    PyObject* collisionVal = PyObject_GetAttr(p_carObject, collisionAttr);
    ASSERT_EQ(p_car.collision, static_cast<int>(PyLong_AsLong(collisionVal)));

    PyObject* normalAttr = PyUnicode_FromString("normal");
    PyObject* normalVal = PyObject_GetAttr(p_carObject, normalAttr);
    CheckVectorData(p_car.normal.x, p_car.normal.y, p_car.normal.z, normalVal);

    PyObject* collposAttr = PyUnicode_FromString("collpos");
    PyObject* collposVal = PyObject_GetAttr(p_carObject, collposAttr);
    CheckVectorData(p_car.collpos.x, p_car.collpos.y, p_car.collpos.z, collposVal);

    PyObject* wheelbaseAttr = PyUnicode_FromString("wheelbase");
    PyObject* wheelbaseVal = PyObject_GetAttr(p_carObject, wheelbaseAttr);
    ASSERT_EQ(p_car.wheelbase, static_cast<float>(PyFloat_AsDouble(wheelbaseVal)));

    PyObject* wheeltrackAttr = PyUnicode_FromString("wheeltrack");
    PyObject* wheeltrackVal = PyObject_GetAttr(p_carObject, wheeltrackAttr);
    ASSERT_EQ(p_car.wheeltrack, static_cast<float>(PyFloat_AsDouble(wheeltrackVal)));

    PyObject* posMatAttr = PyUnicode_FromString("posMat");
    PyObject* posMatVal = PyObject_GetAttr(p_carObject, posMatAttr);
    CheckMatrixData(p_car.posMat, posMatVal);

    PyObject* blockedAttr = PyUnicode_FromString("blocked");
    PyObject* blockedVal = PyObject_GetAttr(p_carObject, blockedAttr);
    ASSERT_EQ(p_car.blocked, static_cast<int>(PyLong_AsLong(blockedVal)));

    PyObject* dammageAttr = PyUnicode_FromString("dammage");
    PyObject* dammageVal = PyObject_GetAttr(p_carObject, dammageAttr);
    ASSERT_EQ(p_car.dammage, static_cast<int>(PyLong_AsLong(dammageVal)));

    PyObject* featuresAttr = PyUnicode_FromString("features");
    PyObject* featuresVal = PyObject_GetAttr(p_carObject, featuresAttr);
    ASSERT_EQ(p_car.features, static_cast<int>(PyLong_AsLong(featuresVal)));

    PyObject* restPosAttr = PyUnicode_FromString("restPos");
    PyObject* restPosVal = PyObject_GetAttr(p_carObject, restPosAttr);
    CheckDynamicPointData(p_car.restPos, restPosVal);

    PyObject* collisionAwareAttr = PyUnicode_FromString("collisionAware");
    PyObject* collisionAwareVal = PyObject_GetAttr(p_carObject, collisionAwareAttr);
    ASSERT_EQ(p_car.collisionAware, static_cast<float>(PyFloat_AsDouble(collisionAwareVal)));
}

/// @brief checks the sda data python object corresponds to the original sda data values
/// @param  p_data The original sda data value
/// @param  p_dataObject The python sda data object
void CheckSDAData(SDAData p_data, PyObject* p_dataObject)
{
    PyObject* tickAttr = PyUnicode_FromString("tickCount");
    PyObject* tickVal = PyObject_GetAttr(p_dataObject, tickAttr);
    ASSERT_TRUE(p_data.TickCount == static_cast<unsigned long>(PyLong_AsLong(tickVal)));

    PyObject* carAttr = PyUnicode_FromString("car");
    PyObject* carVal = PyObject_GetAttr(p_dataObject, carAttr);
    CheckCarData(p_data.Car, carVal);

    PyObject* situationAttr = PyUnicode_FromString("situation");
    PyObject* situationVal = PyObject_GetAttr(p_dataObject, situationAttr);
    CheckSituationData(p_data.Situation, situationVal);

    PyObject* simCarAttr = PyUnicode_FromString("simCar");
    PyObject* simCarVal = PyObject_GetAttr(p_dataObject, simCarAttr);
    CheckSimCarData(p_data.SimCar, simCarVal);
}

/// @brief tests the sda data c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetSDATypeObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    Random random;
    for (int i = 0; i < TEST_COUNT; i++)
    {
        SDAData sdaData;
        TestSegments segments = GenerateSegments();
        sdaData.TickCount = random.NextUInt();
        sdaData.Car = GenerateCar(segments);
        sdaData.SimCar = GenerateSimCar(sdaData.Car);
        sdaData.Situation = GenerateSituation();

        // checks the data is correctly translated from SDAData to PyObject
        PyObject* sdaDataObject = converter.GetPythonSDATypeObject(sdaData);
        CheckSDAData(sdaData, sdaDataObject);

        // checks the data is correctly translated from PyObject to SDAData
        CheckSDAData(converter.GetCppSDAData(sdaDataObject), sdaDataObject);

        // checks the data is correctly set from PyObject to PyObject
        SDAData newSDAData;
        TestSegments newSegments = GenerateSegments();
        newSDAData.TickCount = random.NextUInt();
        newSDAData.Car = GenerateCar(newSegments);
        newSDAData.SimCar = GenerateSimCar(newSDAData.Car);
        newSDAData.Situation = GenerateSituation();

        PyObject* newSDADataObject = converter.GetPythonSDATypeObject(newSDAData);
        converter.SetPythonSDATypeObject(newSDADataObject, sdaDataObject);

        CheckSDAData(sdaData, newSDADataObject);

        DestroySegments(segments);
        DestroyCar(sdaData.Car);
        DestroySituation(sdaData.Situation);

        DestroySegments(newSegments);
        DestroyCar(newSDAData.Car);
        DestroySituation(newSDAData.Situation);
    }

    Py_Finalize();
}

/// @brief tests the car c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carObject = converter.GetPythonCarObject(carData);

        CheckCarData(carData, carObject);

        tCarElt convertedCarData = converter.GetCppCarObject(carObject);
        CheckCarData(convertedCarData, carObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the init car c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarInitObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carInitObject = converter.GetPythonCarInitObject(carData.info);

        CheckInitCarData(carData.info, carInitObject);

        tInitCar convertedCarData = converter.GetCppCarInitObject(carInitObject);
        CheckInitCarData(convertedCarData, carInitObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the public car c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarPublicObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carPublicObject = converter.GetPythonCarPublicObject(carData.pub);

        CheckPublicCarData(carData.pub, carPublicObject);

        tPublicCar convertedCarData = converter.GetCppCarPublicObject(carPublicObject);
        CheckPublicCarData(convertedCarData, carPublicObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the car race info c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarRaceInfoObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carRaceInfoObject = converter.GetPythonCarRaceInfoObject(carData.race);

        CheckCarRaceInfoData(carData.race, carRaceInfoObject);

        tCarRaceInfo convertedCarData = converter.GetCppCarRaceInfoObject(carRaceInfoObject);
        CheckCarRaceInfoData(convertedCarData, carRaceInfoObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the private car c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarPrivObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carPrivObject = converter.GetPythonCarPrivObject(carData.priv);

        CheckPrivCarData(carData.priv, carPrivObject);

        tPrivCar convertedCarData = converter.GetCppCarPrivObject(carPrivObject);
        CheckPrivCarData(convertedCarData, carPrivObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the car control c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarCtrlObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carCtrlObject = converter.GetPythonCarCtrlObject(carData.ctrl);
        CheckCarCtrlData(carData.ctrl, carCtrlObject);

        tCarCtrl convertedCarData = converter.GetCppCarCtrlObject(carCtrlObject);
        CheckCarCtrlData(convertedCarData, carCtrlObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the car setup c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarSetupObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carSetupObject = converter.GetPythonCarSetupObject(carData.setup);
        CheckCarSetupData(carData.setup, carSetupObject);

        tCarSetup convertedCarData = converter.GetCppCarSetupObject(carSetupObject);
        CheckCarSetupData(convertedCarData, carSetupObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the car pit cmd c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetCarPitCmdObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carPitCmdObject = converter.GetPythonCarPitCmdObject(carData.pitcmd);
        CheckCarPitCmdData(carData.pitcmd, carPitCmdObject);

        tCarPitCmd convertedCarData = converter.GetCppCarPitCmdObject(carPitCmdObject);
        CheckCarPitCmdData(convertedCarData, carPitCmdObject);

        DestroySegments(testSegments);
        DestroyCar(carData);
    }

    Py_Finalize();
}

/// @brief tests the situation c++ data correctly translated to python and back
TEST(PythonConverterTests, PythonDriverGetSituationObjectTest)
{
    Py_Initialize();
    SDATypesConverter converter = SDATypesConverter();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        tSituation situationData = GenerateSituation();

        PyObject* situationObject = converter.GetPythonSituationObject(situationData);
        CheckSituationData(situationData, situationObject);

        tSituation convertedSituationData = converter.GetCppSituationObject(situationObject);
        CheckSituationData(convertedSituationData, situationObject);

        DestroySituation(situationData);
    }

    Py_Finalize();
}