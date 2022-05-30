#include "PythonDriver.h"
#include "Mocks/SDADriverMock.h"
#include <gtest/gtest.h>
#include <vector>
#include "Utils.h"
#include "Random.hpp"
#include "GeneratorUtils.h"

#define TEST_COUNT 1

// check all classes are callable
TEST(PythonDriverTests, PythonDriverInitTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_sdaTypesClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_initCarClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_wheelSpecificationsClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_visualAttributesClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_publicCarClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_dynamicPointClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_trackLocationClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_trackSegmentClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carRaceInfoClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_privCarClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_posDClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_collisionStateClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carCtrlClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carSetupClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carSetupItemClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_carPitCmdClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_situationClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_raceInfoClass));
    ASSERT_TRUE(PyCallable_Check(pythonDriver.m_vectorClass));
    Py_Finalize();
}

SDAData CreateSDAData()
{
    SDAData data;
    Random random;
    TestSegments testSegments = GenerateSegments();
    data.TickCount = random.NextUInt();
    data.Car = GenerateCar(testSegments);
    data.Situation = GenerateSituation();
    return data;
}

bool CheckVectorData(float p_x, float p_y, float p_z, PyObject* p_vecObject)
{
    PyObject* xAttr = PyUnicode_FromString("x");
    PyObject* xVal = PyObject_GetAttr(p_vecObject, xAttr);
    double xValTest = PyFloat_AsDouble(xVal);
    bool xCorrect = static_cast<double>(p_x) == PyFloat_AsDouble(xVal);

    PyObject* yAttr = PyUnicode_FromString("y");
    PyObject* yVal = PyObject_GetAttr(p_vecObject, yAttr);
    double yValTest = PyFloat_AsDouble(yVal);
    bool yCorrect = static_cast<double>(p_y) == PyFloat_AsDouble(yVal);

    PyObject* zAttr = PyUnicode_FromString("z");
    PyObject* zVal = PyObject_GetAttr(p_vecObject, zAttr);
    double zValTest = PyFloat_AsDouble(zVal);
    bool zCorrect = static_cast<double>(p_z) == PyFloat_AsDouble(zVal);

    return xCorrect && yCorrect && zCorrect;
}

bool CheckCollisionStateObject(tCollisionState& p_collisionState, PyObject* p_collisionStateObject)
{
    PyObject* collisionCountAttr = PyUnicode_FromString("collisionCount");
    PyObject* collisionCountVal = PyObject_GetAttr(p_collisionStateObject, collisionCountAttr);
    bool collisionCountCorrect = p_collisionState.collision_count == static_cast<int>(PyLong_AsLong(collisionCountVal));

    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_collisionStateObject, posAttr);
    bool posCorrect = CheckVectorData(p_collisionState.pos[0], p_collisionState.pos[1], p_collisionState.pos[2], posVal);

    PyObject* forceAttr = PyUnicode_FromString("force");
    PyObject* forceVal = PyObject_GetAttr(p_collisionStateObject, forceAttr);
    bool forceCorrect = CheckVectorData(p_collisionState.force[0], p_collisionState.force[1], p_collisionState.force[2], forceVal);

    return collisionCountCorrect && posCorrect && forceCorrect;
}

bool CheckWheelData(tWheelSpec& p_wheelSpec, PyObject* p_wheelObject)
{
    PyObject* rimRadiusAttr = PyUnicode_FromString("rimRadius");
    PyObject* rimRadiusVal = PyObject_GetAttr(p_wheelObject, rimRadiusAttr);
    bool rimRadiusCorrect = static_cast<double>(p_wheelSpec.rimRadius) == PyFloat_AsDouble(rimRadiusVal);

    PyObject* tireHeightAttr = PyUnicode_FromString("tireHeight");
    PyObject* tireHeightVal = PyObject_GetAttr(p_wheelObject, tireHeightAttr);
    bool tireHeightCorrect = static_cast<double>(p_wheelSpec.tireHeight) == PyFloat_AsDouble(tireHeightVal);

    PyObject* tireWidthAttr = PyUnicode_FromString("tireWidth");
    PyObject* tireWidthVal = PyObject_GetAttr(p_wheelObject, tireWidthAttr);
    bool tireWidthCorrect = static_cast<double>(p_wheelSpec.tireWidth) == PyFloat_AsDouble(tireWidthVal);

    PyObject* brakeDiskRadiusAttr = PyUnicode_FromString("brakeDiskRadius");
    PyObject* brakeDiskRadiusVal = PyObject_GetAttr(p_wheelObject, brakeDiskRadiusAttr);
    bool brakeDiskRadiusCorrect = static_cast<double>(p_wheelSpec.brakeDiskRadius) == PyFloat_AsDouble(brakeDiskRadiusVal);

    PyObject* wheelRadiusAttr = PyUnicode_FromString("wheelRadius");
    PyObject* wheelRadiusVal = PyObject_GetAttr(p_wheelObject, wheelRadiusAttr);
    bool wheelRadiusCorrect = static_cast<double>(p_wheelSpec.wheelRadius) == PyFloat_AsDouble(wheelRadiusVal);

    return rimRadiusCorrect && tireHeightCorrect && tireWidthCorrect && brakeDiskRadiusCorrect && wheelRadiusCorrect;
}

bool CheckVisualAttrData(tVisualAttributes& p_visualAttributes, PyObject* p_visualAttributesObject)
{
    PyObject* exhaustNbAttr = PyUnicode_FromString("exhaustNb");
    PyObject* exhaustNbVal = PyObject_GetAttr(p_visualAttributesObject, exhaustNbAttr);
    bool exhaustNbCorrect = p_visualAttributes.exhaustNb == static_cast<int>(PyLong_AsLong(exhaustNbVal));

    PyObject* exhaustPosAttr = PyUnicode_FromString("exhaustPos");
    PyObject* exhaustPosVal = PyList_AsTuple(PyObject_GetAttr(p_visualAttributesObject, exhaustPosAttr));
    bool exhaustPosCorrect = CheckVectorData(p_visualAttributes.exhaustPos[0].x, p_visualAttributes.exhaustPos[0].y, p_visualAttributes.exhaustPos[0].z, PyTuple_GetItem(exhaustPosVal, 0)) &&
                             CheckVectorData(p_visualAttributes.exhaustPos[1].x, p_visualAttributes.exhaustPos[1].y, p_visualAttributes.exhaustPos[1].z, PyTuple_GetItem(exhaustPosVal, 1));

    PyObject* exhaustPowerAttr = PyUnicode_FromString("exhaustPower");
    PyObject* exhaustPowerVal = PyObject_GetAttr(p_visualAttributesObject, exhaustPowerAttr);
    bool exhaustPowerCorrect = static_cast<double>(p_visualAttributes.exhaustPower) == PyFloat_AsDouble(exhaustPowerVal);

    return exhaustNbCorrect && exhaustPosCorrect && exhaustPowerCorrect;
}

bool CheckInitCarData(tInitCar& p_initCar, PyObject* p_initCarObject)
{
    PyObject* nameAttr = PyUnicode_FromString("name");
    PyObject* nameVal = PyObject_GetAttr(p_initCarObject, nameAttr);
    bool nameCorrect = strcmp(p_initCar.name, PyUnicode_AsUTF8(nameVal)) == 0;

    PyObject* snameAttr = PyUnicode_FromString("sName");
    PyObject* snameVal = PyObject_GetAttr(p_initCarObject, snameAttr);
    bool snameCorrect = strcmp(p_initCar.sname, PyUnicode_AsUTF8(snameVal)) == 0;

    PyObject* codenameAttr = PyUnicode_FromString("codename");
    PyObject* codenameVal = PyObject_GetAttr(p_initCarObject, codenameAttr);
    bool codenameCorrect = strcmp(p_initCar.codename, PyUnicode_AsUTF8(codenameVal)) == 0;

    PyObject* teamNameAttr = PyUnicode_FromString("teamName");
    PyObject* teamNameVal = PyObject_GetAttr(p_initCarObject, teamNameAttr);
    bool teamNameCorrect = strcmp(p_initCar.teamname, PyUnicode_AsUTF8(teamNameVal)) == 0;

    PyObject* carNameAttr = PyUnicode_FromString("carName");
    PyObject* carNameVal = PyObject_GetAttr(p_initCarObject, carNameAttr);
    bool carNameCorrect = strcmp(p_initCar.carName, PyUnicode_AsUTF8(carNameVal)) == 0;

    PyObject* categoryAttr = PyUnicode_FromString("category");
    PyObject* categoryVal = PyObject_GetAttr(p_initCarObject, categoryAttr);
    bool categoryCorrect = strcmp(p_initCar.category, PyUnicode_AsUTF8(categoryVal)) == 0;

    PyObject* raceNumberAttr = PyUnicode_FromString("raceNumber");
    PyObject* raceNumberVal = PyObject_GetAttr(p_initCarObject, raceNumberAttr);
    bool raceNumberCorrect = p_initCar.raceNumber == static_cast<int>(PyLong_AsLong(raceNumberVal));

    PyObject* startRankAttr = PyUnicode_FromString("startRank");
    PyObject* startRankVal = PyObject_GetAttr(p_initCarObject, startRankAttr);
    bool startRankCorrect = p_initCar.startRank == static_cast<int>(PyLong_AsLong(startRankVal));

    PyObject* driverTypeAttr = PyUnicode_FromString("driverType");
    PyObject* driverTypeVal = PyObject_GetAttr(p_initCarObject, driverTypeAttr);
    bool driverTypeCorrect = p_initCar.driverType == static_cast<int>(PyLong_AsLong(driverTypeVal));

    PyObject* networkPlayerAttr = PyUnicode_FromString("networkPlayer");
    PyObject* networkPlayerVal = PyObject_GetAttr(p_initCarObject, networkPlayerAttr);
    bool networkPlayerCorrect = p_initCar.networkplayer == static_cast<int>(PyLong_AsLong(networkPlayerVal));

    PyObject* skillLevelAttr = PyUnicode_FromString("skillLevel");
    PyObject* skillLevelVal = PyObject_GetAttr(p_initCarObject, skillLevelAttr);
    bool skillLevelCorrect = p_initCar.skillLevel == static_cast<int>(PyLong_AsLong(skillLevelVal));

    PyObject* iconColorAttr = PyUnicode_FromString("iconColor");
    PyObject* iconColorVal = PyList_AsTuple(PyObject_GetAttr(p_initCarObject, iconColorAttr));
    bool iconColorCorrect = p_initCar.iconColor[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 0))) &&
                            p_initCar.iconColor[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 1))) &&
                            p_initCar.iconColor[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 2)));

    PyObject* dimensionAttr = PyUnicode_FromString("dimension");
    PyObject* dimensionVal = PyObject_GetAttr(p_initCarObject, dimensionAttr);
    bool dimensionCorrect = CheckVectorData(p_initCar.dimension.x, p_initCar.dimension.y, p_initCar.dimension.z, dimensionVal);

    PyObject* drvPosAttr = PyUnicode_FromString("drvPos");
    PyObject* drvPosVal = PyObject_GetAttr(p_initCarObject, drvPosAttr);
    bool drvPosCorrect = CheckVectorData(p_initCar.drvPos.x, p_initCar.drvPos.y, p_initCar.drvPos.z, drvPosVal);

    PyObject* bonnetPosAttr = PyUnicode_FromString("bonnetPos");
    PyObject* bonnetPosVal = PyObject_GetAttr(p_initCarObject, bonnetPosAttr);
    bool bonnetPosCorrect = CheckVectorData(p_initCar.bonnetPos.x, p_initCar.bonnetPos.y, p_initCar.bonnetPos.z, bonnetPosVal);

    PyObject* tankAttr = PyUnicode_FromString("tank");
    PyObject* tankVal = PyObject_GetAttr(p_initCarObject, tankAttr);
    bool tankCorrect = static_cast<double>(p_initCar.tank) == PyFloat_AsDouble(tankVal);

    PyObject* steerLockAttr = PyUnicode_FromString("steerLock");
    PyObject* steerLockVal = PyObject_GetAttr(p_initCarObject, steerLockAttr);
    bool steerLockCorrect = static_cast<double>(p_initCar.steerLock) == PyFloat_AsDouble(steerLockVal);

    PyObject* statGCAttr = PyUnicode_FromString("statGC");
    PyObject* statGCVal = PyObject_GetAttr(p_initCarObject, statGCAttr);
    bool statGCCorrect = CheckVectorData(p_initCar.statGC.x, p_initCar.statGC.y, p_initCar.statGC.z, statGCVal);

    PyObject* wheelAttr = PyUnicode_FromString("wheel");
    PyObject* wheelVal = PyList_AsTuple(PyObject_GetAttr(p_initCarObject, wheelAttr));
    bool wheelCorrect = CheckWheelData(p_initCar.wheel[0], PyTuple_GetItem(wheelVal, 0)) &&
                        CheckWheelData(p_initCar.wheel[1], PyTuple_GetItem(wheelVal, 1)) &&
                        CheckWheelData(p_initCar.wheel[2], PyTuple_GetItem(wheelVal, 2)) &&
                        CheckWheelData(p_initCar.wheel[3], PyTuple_GetItem(wheelVal, 3));

    PyObject* visualAttrAttr = PyUnicode_FromString("visualAttr");
    PyObject* visualAttrVal = PyObject_GetAttr(p_initCarObject, visualAttrAttr);
    bool visualAttrCorrect = CheckVisualAttrData(p_initCar.visualAttr, visualAttrVal);

    PyObject* masterModelAttr = PyUnicode_FromString("masterModel");
    PyObject* masterModelVal = PyObject_GetAttr(p_initCarObject, masterModelAttr);
    bool masterModelCorrect = strcmp(p_initCar.masterModel, PyUnicode_AsUTF8(masterModelVal)) == 0;

    PyObject* skinNameAttr = PyUnicode_FromString("skinName");
    PyObject* skinNameVal = PyObject_GetAttr(p_initCarObject, skinNameAttr);
    bool skinNameCorrect = strcmp(p_initCar.skinName, PyUnicode_AsUTF8(skinNameVal)) == 0;

    PyObject* skinTargetsAttr = PyUnicode_FromString("skinTargets");
    PyObject* skinTargetsVal = PyObject_GetAttr(p_initCarObject, skinTargetsAttr);
    bool skinTargetsCorrect = p_initCar.skinTargets == static_cast<int>(PyLong_AsLong(skinTargetsVal));

    bool result = nameCorrect && snameCorrect && codenameCorrect && teamNameCorrect && carNameCorrect &&
                  categoryCorrect && raceNumberCorrect && startRankCorrect && driverTypeCorrect &&
                  networkPlayerCorrect && skillLevelCorrect && iconColorCorrect && dimensionCorrect &&
                  drvPosCorrect && bonnetPosCorrect && tankCorrect && steerLockCorrect && statGCCorrect &&
                  wheelCorrect && visualAttrCorrect && masterModelCorrect && skinNameCorrect && skinTargetsCorrect;
    return result;
}

bool CheckDynamicPointData(tDynPt& p_dynPt, PyObject* p_dynPtObject)
{
    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_dynPtObject, posAttr);
    bool posCorrect = CheckVectorData(p_dynPt.pos.x, p_dynPt.pos.y, p_dynPt.pos.z, posVal);

    PyObject* velAttr = PyUnicode_FromString("vel");
    PyObject* velVal = PyObject_GetAttr(p_dynPtObject, velAttr);
    bool velCorrect = CheckVectorData(p_dynPt.vel.x, p_dynPt.vel.y, p_dynPt.vel.z, velVal);

    PyObject* accAttr = PyUnicode_FromString("acc");
    PyObject* accVal = PyObject_GetAttr(p_dynPtObject, accAttr);
    bool accCorrect = CheckVectorData(p_dynPt.acc.x, p_dynPt.acc.y, p_dynPt.acc.z, accVal);

    return posCorrect && velCorrect && accCorrect;
}

bool CheckMatrixData(sgMat4& p_posMat, PyObject* p_posMatObject)
{
    PyObject* p_posMatTuple = PyList_AsTuple(p_posMatObject);
    PyObject* row1Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 0));
    PyObject* row2Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 1));
    PyObject* row3Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 2));
    PyObject* row4Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 3));

    bool row1Correct = p_posMat[0][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 0))) &&
                       p_posMat[0][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 1))) &&
                       p_posMat[0][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 2))) &&
                       p_posMat[0][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 3)));
    bool row2Correct = p_posMat[1][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 0))) &&
                       p_posMat[1][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 1))) &&
                       p_posMat[1][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 2))) &&
                       p_posMat[1][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 3)));
    bool row3Correct = p_posMat[2][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 0))) &&
                       p_posMat[2][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 1))) &&
                       p_posMat[2][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 2))) &&
                       p_posMat[2][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 3)));
    bool row4Correct = p_posMat[3][0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 0))) &&
                       p_posMat[3][1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 1))) &&
                       p_posMat[3][2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 2))) &&
                       p_posMat[3][3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 3)));

    return row1Correct && row2Correct && row3Correct && row4Correct;
}

bool CheckTrackSegmentData(tTrackSeg& p_trackSeg, PyObject* p_trackSegObject)
{
    PyObject* nameAttr = PyUnicode_FromString("name");
    PyObject* nameVal = PyObject_GetAttr(p_trackSegObject, nameAttr);
    bool nameCorrect = strcmp(p_trackSeg.name, PyUnicode_AsUTF8(nameVal)) == 0;

    PyObject* idAttr = PyUnicode_FromString("id");
    PyObject* idVal = PyObject_GetAttr(p_trackSegObject, idAttr);
    bool idCorrect = p_trackSeg.id == static_cast<int>(PyLong_AsLong(idVal));

    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_trackSegObject, typeAttr);
    bool typeCorrect = p_trackSeg.type == static_cast<int>(PyLong_AsLong(typeVal));

    PyObject* type2Attr = PyUnicode_FromString("type2");
    PyObject* type2Val = PyObject_GetAttr(p_trackSegObject, type2Attr);
    bool type2Correct = p_trackSeg.type2 == static_cast<int>(PyLong_AsLong(type2Val));

    PyObject* styleAttr = PyUnicode_FromString("style");
    PyObject* styleVal = PyObject_GetAttr(p_trackSegObject, styleAttr);
    bool styleCorrect = p_trackSeg.style == static_cast<int>(PyLong_AsLong(styleVal));

    PyObject* lengthAttr = PyUnicode_FromString("length");
    PyObject* lengthVal = PyObject_GetAttr(p_trackSegObject, lengthAttr);
    bool lengthCorrect = static_cast<double>(p_trackSeg.length) == PyFloat_AsDouble(lengthVal);

    PyObject* widthAttr = PyUnicode_FromString("width");
    PyObject* widthVal = PyObject_GetAttr(p_trackSegObject, widthAttr);
    bool widthCorrect = static_cast<double>(p_trackSeg.width) == PyFloat_AsDouble(widthVal);

    PyObject* startWidthAttr = PyUnicode_FromString("startWidth");
    PyObject* startWidthVal = PyObject_GetAttr(p_trackSegObject, startWidthAttr);
    bool startWidthCorrect = static_cast<double>(p_trackSeg.startWidth) == PyFloat_AsDouble(startWidthVal);

    PyObject* endWidthAttr = PyUnicode_FromString("endWidth");
    PyObject* endWidthVal = PyObject_GetAttr(p_trackSegObject, endWidthAttr);
    bool endWidthCorrect = static_cast<double>(p_trackSeg.endWidth) == PyFloat_AsDouble(endWidthVal);

    PyObject* lgFromStartAttr = PyUnicode_FromString("lgFromStart");
    PyObject* lgFromStartVal = PyObject_GetAttr(p_trackSegObject, lgFromStartAttr);
    bool lgFromStartCorrect = static_cast<double>(p_trackSeg.lgfromstart) == PyFloat_AsDouble(lgFromStartVal);

    PyObject* radiusAttr = PyUnicode_FromString("radius");
    PyObject* radiusVal = PyObject_GetAttr(p_trackSegObject, radiusAttr);
    bool radiusCorrect = static_cast<double>(p_trackSeg.radius) == PyFloat_AsDouble(radiusVal);

    PyObject* radiusRAttr = PyUnicode_FromString("radiusR");
    PyObject* radiusRVal = PyObject_GetAttr(p_trackSegObject, radiusRAttr);
    bool radiusRCorrect = static_cast<double>(p_trackSeg.radiusr) == PyFloat_AsDouble(radiusRVal);

    PyObject* radiusLAttr = PyUnicode_FromString("radiusL");
    PyObject* radiusLVal = PyObject_GetAttr(p_trackSegObject, radiusLAttr);
    bool radiusLCorrect = static_cast<double>(p_trackSeg.radiusl) == PyFloat_AsDouble(radiusLVal);

    PyObject* arcAttr = PyUnicode_FromString("arc");
    PyObject* arcVal = PyObject_GetAttr(p_trackSegObject, arcAttr);
    bool arcCorrect = static_cast<double>(p_trackSeg.arc) == PyFloat_AsDouble(arcVal);

    PyObject* centerAttr = PyUnicode_FromString("center");
    PyObject* centerVal = PyObject_GetAttr(p_trackSegObject, centerAttr);
    bool centerCorrect = CheckVectorData(p_trackSeg.center.x, p_trackSeg.center.y, p_trackSeg.center.z, centerVal);

    PyObject* vertexAttr = PyUnicode_FromString("vertex");
    PyObject* vertexVal = PyList_AsTuple(PyObject_GetAttr(p_trackSegObject, vertexAttr));
    bool vertexCorrect = CheckVectorData(p_trackSeg.vertex[0].x, p_trackSeg.vertex[0].y, p_trackSeg.vertex[0].z, PyTuple_GetItem(vertexVal, 0)) &&
                         CheckVectorData(p_trackSeg.vertex[1].x, p_trackSeg.vertex[1].y, p_trackSeg.vertex[1].z, PyTuple_GetItem(vertexVal, 1)) &&
                         CheckVectorData(p_trackSeg.vertex[2].x, p_trackSeg.vertex[2].y, p_trackSeg.vertex[2].z, PyTuple_GetItem(vertexVal, 2)) &&
                         CheckVectorData(p_trackSeg.vertex[3].x, p_trackSeg.vertex[3].y, p_trackSeg.vertex[3].z, PyTuple_GetItem(vertexVal, 3));

    PyObject* angleAttr = PyUnicode_FromString("angle");
    PyObject* angleVal = PyList_AsTuple(PyObject_GetAttr(p_trackSegObject, angleAttr));
    bool angleCorrect = p_trackSeg.angle[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 0))) &&
                        p_trackSeg.angle[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 1))) &&
                        p_trackSeg.angle[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 2))) &&
                        p_trackSeg.angle[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 3))) &&
                        p_trackSeg.angle[4] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 4))) &&
                        p_trackSeg.angle[5] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 5))) &&
                        p_trackSeg.angle[6] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleVal, 6)));

    PyObject* sinAttr = PyUnicode_FromString("sin");
    PyObject* sinVal = PyObject_GetAttr(p_trackSegObject, sinAttr);
    bool sinCorrect = static_cast<double>(p_trackSeg.sin) == PyFloat_AsDouble(sinVal);

    PyObject* cosAttr = PyUnicode_FromString("cos");
    PyObject* cosVal = PyObject_GetAttr(p_trackSegObject, cosAttr);
    bool cosCorrect = static_cast<double>(p_trackSeg.cos) == PyFloat_AsDouble(cosVal);

    PyObject* kzlAttr = PyUnicode_FromString("kzl");
    PyObject* kzlVal = PyObject_GetAttr(p_trackSegObject, kzlAttr);
    bool kzlCorrect = static_cast<double>(p_trackSeg.Kzl) == PyFloat_AsDouble(kzlVal);

    PyObject* kzwAttr = PyUnicode_FromString("kzw");
    PyObject* kzwVal = PyObject_GetAttr(p_trackSegObject, kzwAttr);
    bool kzwCorrect = static_cast<double>(p_trackSeg.Kzw) == PyFloat_AsDouble(kzwVal);

    PyObject* kylAttr = PyUnicode_FromString("kyl");
    PyObject* kylVal = PyObject_GetAttr(p_trackSegObject, kylAttr);
    bool kylCorrect = static_cast<double>(p_trackSeg.Kyl) == PyFloat_AsDouble(kylVal);

    PyObject* rgtSideNormalAttr = PyUnicode_FromString("rgtSideNormal");
    PyObject* rgtSideNormalVal = PyObject_GetAttr(p_trackSegObject, rgtSideNormalAttr);
    bool rgtSideNormalCorrect = CheckVectorData(p_trackSeg.rgtSideNormal.x, p_trackSeg.rgtSideNormal.y, p_trackSeg.rgtSideNormal.z, rgtSideNormalVal);

    PyObject* envIndexAttr = PyUnicode_FromString("envIndex");
    PyObject* envIndexVal = PyObject_GetAttr(p_trackSegObject, envIndexAttr);
    bool envIndexCorrect = p_trackSeg.envIndex == static_cast<int>(PyLong_AsLong(envIndexVal));

    PyObject* heightAttr = PyUnicode_FromString("height");
    PyObject* heightVal = PyObject_GetAttr(p_trackSegObject, heightAttr);
    bool heightCorrect = static_cast<double>(p_trackSeg.height) == PyFloat_AsDouble(heightVal);

    PyObject* raceInfoAttr = PyUnicode_FromString("raceInfo");
    PyObject* raceInfoVal = PyObject_GetAttr(p_trackSegObject, raceInfoAttr);
    bool raceInfoCorrect = p_trackSeg.raceInfo == static_cast<int>(PyLong_AsLong(raceInfoVal));

    PyObject* doVFactorAttr = PyUnicode_FromString("doVFactor");
    PyObject* doVFactorVal = PyObject_GetAttr(p_trackSegObject, doVFactorAttr);
    bool doVFactorCorrect = static_cast<double>(p_trackSeg.DoVfactor) == PyFloat_AsDouble(doVFactorVal);

    // TODO: PyObject* speedLimitAttr = PyUnicode_FromString("speedLimit");
    // PyObject* speedLimitVal = PyObject_GetAttr(p_trackSegObject, speedLimitAttr);
    // bool speedLimitCorrect = static_cast<double>(p_trackSeg.SpeedLimit) == PyFloat_AsDouble(speedLimitVal);

    return nameCorrect && idCorrect && typeCorrect && type2Correct && styleCorrect && lengthCorrect &&
           widthCorrect && startWidthCorrect && endWidthCorrect && lgFromStartCorrect && radiusCorrect &&
           radiusRCorrect && radiusLCorrect && arcCorrect && centerCorrect && vertexCorrect && angleCorrect &&
           sinCorrect && cosCorrect && kzlCorrect && kzwCorrect && kylCorrect && rgtSideNormalCorrect &&
           envIndexCorrect && heightCorrect && raceInfoCorrect && doVFactorCorrect;  // TODO: && speedLimitCorrect;
}

bool CheckTrackLocationData(tTrkLocPos& p_trackLoc, PyObject* p_trackLocObject)
{
    PyObject* segAttr = PyUnicode_FromString("seg");
    PyObject* segVal = PyObject_GetAttr(p_trackLocObject, segAttr);
    bool segCorrect = CheckTrackSegmentData(*p_trackLoc.seg, segVal);

    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_trackLocObject, typeAttr);
    bool typeCorrect = p_trackLoc.type == static_cast<int>(PyLong_AsLong(typeVal));

    PyObject* toStartAttr = PyUnicode_FromString("toStart");
    PyObject* toStartVal = PyObject_GetAttr(p_trackLocObject, toStartAttr);
    bool toStartCorrect = static_cast<double>(p_trackLoc.toStart) == PyFloat_AsDouble(toStartVal);

    PyObject* toRightAttr = PyUnicode_FromString("toRight");
    PyObject* toRightVal = PyObject_GetAttr(p_trackLocObject, toRightAttr);
    bool toRightCorrect = static_cast<double>(p_trackLoc.toRight) == PyFloat_AsDouble(toRightVal);

    PyObject* toMiddleAttr = PyUnicode_FromString("toMiddle");
    PyObject* toMiddleVal = PyObject_GetAttr(p_trackLocObject, toMiddleAttr);
    bool toMiddleCorrect = static_cast<double>(p_trackLoc.toMiddle) == PyFloat_AsDouble(toMiddleVal);

    PyObject* toLeftAttr = PyUnicode_FromString("toLeft");
    PyObject* toLeftVal = PyObject_GetAttr(p_trackLocObject, toLeftAttr);
    bool toLeftCorrect = static_cast<double>(p_trackLoc.toLeft) == PyFloat_AsDouble(toLeftVal);

    return segCorrect && typeCorrect && toStartCorrect && toRightCorrect && toMiddleCorrect && toLeftCorrect;
}

bool CheckPublicCarData(tPublicCar& p_publicCar, PyObject* p_publicCarObject)
{
    PyObject* dynGCAttr = PyUnicode_FromString("dynGC");
    PyObject* dynGCVal = PyObject_GetAttr(p_publicCarObject, dynGCAttr);
    bool dynGCCorrect = CheckDynamicPointData(p_publicCar.DynGC, dynGCVal);

    PyObject* dynGCgAttr = PyUnicode_FromString("dynGCg");
    PyObject* dynGCgVal = PyObject_GetAttr(p_publicCarObject, dynGCgAttr);
    bool dynGCgCorrect = CheckDynamicPointData(p_publicCar.DynGCg, dynGCgVal);

    PyObject* speedAttr = PyUnicode_FromString("speed");
    PyObject* speedVal = PyObject_GetAttr(p_publicCarObject, speedAttr);
    bool speedCorrect = static_cast<double>(p_publicCar.speed) == PyFloat_AsDouble(speedVal);

    PyObject* posMatAttr = PyUnicode_FromString("posMat");
    PyObject* posMatVal = PyObject_GetAttr(p_publicCarObject, posMatAttr);
    bool posMatCorrect = CheckMatrixData(p_publicCar.posMat, posMatVal);

    PyObject* trkPosAttr = PyUnicode_FromString("trkPos");
    PyObject* trkPosVal = PyObject_GetAttr(p_publicCarObject, trkPosAttr);
    bool trkPosCorrect = CheckTrackLocationData(p_publicCar.trkPos, trkPosVal);

    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_publicCarObject, stateAttr);
    bool stateCorrect = p_publicCar.state == static_cast<int>(PyLong_AsLong(stateVal));

    return dynGCCorrect && dynGCgCorrect && speedCorrect && posMatCorrect && trkPosCorrect && stateCorrect;
}

bool CheckCarRaceInfoData(tCarRaceInfo& p_carRaceInfo, PyObject* p_carRaceInfoObject)
{
    PyObject* bestLapTimeAttr = PyUnicode_FromString("bestLapTime");
    PyObject* bestLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, bestLapTimeAttr);
    bool bestLapTimeCorrect = p_carRaceInfo.bestLapTime == static_cast<float>(PyFloat_AsDouble(bestLapTimeVal));

    PyObject* commitBestLapTimeAttr = PyUnicode_FromString("commitBestLapTime");
    PyObject* commitBestLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, commitBestLapTimeAttr);
    bool commitBestLapTimeCorrect = p_carRaceInfo.commitBestLapTime == static_cast<bool>(PyLong_AsLong(commitBestLapTimeVal));

    PyObject* deltaBestLapTimeAttr = PyUnicode_FromString("deltaBestLapTime");
    PyObject* deltaBestLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, deltaBestLapTimeAttr);
    bool deltaBestLapTimeCorrect = p_carRaceInfo.deltaBestLapTime == static_cast<float>(PyFloat_AsDouble(deltaBestLapTimeVal));

    PyObject* curLapTimeAttr = PyUnicode_FromString("curLapTime");
    PyObject* curLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, curLapTimeAttr);
    bool curLapTimeCorrect = p_carRaceInfo.curLapTime == static_cast<float>(PyFloat_AsDouble(curLapTimeVal));

    PyObject* lastLapTimeAttr = PyUnicode_FromString("lastLapTime");
    PyObject* lastLapTimeVal = PyObject_GetAttr(p_carRaceInfoObject, lastLapTimeAttr);
    bool lastLapTimeCorrect = p_carRaceInfo.lastLapTime == static_cast<float>(PyFloat_AsDouble(lastLapTimeVal));

    PyObject* curTimeAttr = PyUnicode_FromString("curTime");
    PyObject* curTimeVal = PyObject_GetAttr(p_carRaceInfoObject, curTimeAttr);
    bool curTimeCorrect = p_carRaceInfo.curTime == static_cast<float>(PyFloat_AsDouble(curTimeVal));

    PyObject* topSpeedAttr = PyUnicode_FromString("topSpeed");
    PyObject* topSpeedVal = PyObject_GetAttr(p_carRaceInfoObject, topSpeedAttr);
    bool topSpeedCorrect = p_carRaceInfo.topSpeed == static_cast<float>(PyFloat_AsDouble(topSpeedVal));

    PyObject* currentMinSpeedForLapAttr = PyUnicode_FromString("currentMinSpeedForLap");
    PyObject* currentMinSpeedForLapVal = PyObject_GetAttr(p_carRaceInfoObject, currentMinSpeedForLapAttr);
    bool currentMinSpeedForLapCorrect = p_carRaceInfo.currentMinSpeedForLap == static_cast<float>(PyFloat_AsDouble(currentMinSpeedForLapVal));

    PyObject* lapsAttr = PyUnicode_FromString("laps");
    PyObject* lapsVal = PyObject_GetAttr(p_carRaceInfoObject, lapsAttr);
    bool lapsCorrect = p_carRaceInfo.laps == static_cast<int>(PyLong_AsLong(lapsVal));

    PyObject* bestLapAttr = PyUnicode_FromString("bestLap");
    PyObject* bestLapVal = PyObject_GetAttr(p_carRaceInfoObject, bestLapAttr);
    bool bestLapCorrect = p_carRaceInfo.bestLap == static_cast<int>(PyLong_AsLong(bestLapVal));

    PyObject* nbPitStopsAttr = PyUnicode_FromString("nbPitStops");
    PyObject* nbPitStopsVal = PyObject_GetAttr(p_carRaceInfoObject, nbPitStopsAttr);
    bool nbPitStopsCorrect = p_carRaceInfo.nbPitStops == static_cast<int>(PyLong_AsLong(nbPitStopsVal));

    PyObject* remainingLapsAttr = PyUnicode_FromString("remainingLaps");
    PyObject* remainingLapsVal = PyObject_GetAttr(p_carRaceInfoObject, remainingLapsAttr);
    bool remainingLapsCorrect = p_carRaceInfo.remainingLaps == static_cast<int>(PyLong_AsLong(remainingLapsVal));

    PyObject* posAttr = PyUnicode_FromString("pos");
    PyObject* posVal = PyObject_GetAttr(p_carRaceInfoObject, posAttr);
    bool posCorrect = p_carRaceInfo.pos == static_cast<int>(PyLong_AsLong(posVal));

    PyObject* timeBehindLeaderAttr = PyUnicode_FromString("timeBehindLeader");
    PyObject* timeBehindLeaderVal = PyObject_GetAttr(p_carRaceInfoObject, timeBehindLeaderAttr);
    bool timeBehindLeaderCorrect = p_carRaceInfo.timeBehindLeader == static_cast<float>(PyFloat_AsDouble(timeBehindLeaderVal));

    PyObject* lapsBehindLeaderAttr = PyUnicode_FromString("lapsBehindLeader");
    PyObject* lapsBehindLeaderVal = PyObject_GetAttr(p_carRaceInfoObject, lapsBehindLeaderAttr);
    bool lapsBehindLeaderCorrect = p_carRaceInfo.lapsBehindLeader == static_cast<int>(PyLong_AsLong(lapsBehindLeaderVal));

    PyObject* timeBehindPrevAttr = PyUnicode_FromString("timeBehindPrev");
    PyObject* timeBehindPrevVal = PyObject_GetAttr(p_carRaceInfoObject, timeBehindPrevAttr);
    bool timeBehindPrevCorrect = p_carRaceInfo.timeBehindPrev == static_cast<float>(PyFloat_AsDouble(timeBehindPrevVal));

    PyObject* timeBeforeNextAttr = PyUnicode_FromString("timeBeforeNext");
    PyObject* timeBeforeNextVal = PyObject_GetAttr(p_carRaceInfoObject, timeBeforeNextAttr);
    bool timeBeforeNextCorrect = p_carRaceInfo.timeBeforeNext == static_cast<float>(PyFloat_AsDouble(timeBeforeNextVal));

    PyObject* distRacedAttr = PyUnicode_FromString("distRaced");
    PyObject* distRacedVal = PyObject_GetAttr(p_carRaceInfoObject, distRacedAttr);
    bool distRacedCorrect = p_carRaceInfo.distRaced == static_cast<float>(PyFloat_AsDouble(distRacedVal));

    PyObject* distFromStartLineAttr = PyUnicode_FromString("distFromStartLine");
    PyObject* distFromStartLineVal = PyObject_GetAttr(p_carRaceInfoObject, distFromStartLineAttr);
    bool distFromStartLineCorrect = p_carRaceInfo.distFromStartLine == static_cast<float>(PyFloat_AsDouble(distFromStartLineVal));

    PyObject* currentSectorAttr = PyUnicode_FromString("currentSector");
    PyObject* currentSectorVal = PyObject_GetAttr(p_carRaceInfoObject, currentSectorAttr);
    bool currentSectorCorrect = p_carRaceInfo.currentSector == static_cast<int>(PyLong_AsLong(currentSectorVal));

    PyObject* nbSectorsAttr = PyUnicode_FromString("nbSectors");
    PyObject* nbSectorsVal = PyObject_GetAttr(p_carRaceInfoObject, nbSectorsAttr);
    bool nbSectorsCorrect = p_carRaceInfo.nbSectors == static_cast<int>(PyLong_AsLong(nbSectorsVal));

    PyObject* trackLengthAttr = PyUnicode_FromString("trackLength");
    PyObject* trackLengthVal = PyObject_GetAttr(p_carRaceInfoObject, trackLengthAttr);
    bool trackLengthCorrect = p_carRaceInfo.trackLength == static_cast<float>(PyFloat_AsDouble(trackLengthVal));

    PyObject* scheduledEventTimeAttr = PyUnicode_FromString("scheduledEventTime");
    PyObject* scheduledEventTimeVal = PyObject_GetAttr(p_carRaceInfoObject, scheduledEventTimeAttr);
    bool scheduledEventTimeCorrect = p_carRaceInfo.scheduledEventTime == static_cast<float>(PyFloat_AsDouble(scheduledEventTimeVal));

    PyObject* eventAttr = PyUnicode_FromString("event");
    PyObject* eventVal = PyObject_GetAttr(p_carRaceInfoObject, eventAttr);
    bool eventCorrect = p_carRaceInfo.event == static_cast<int>(PyLong_AsLong(eventVal));

    PyObject* penaltyTimeAttr = PyUnicode_FromString("penaltyTime");
    PyObject* penaltyTimeVal = PyObject_GetAttr(p_carRaceInfoObject, penaltyTimeAttr);
    bool penaltyTimeCorrect = p_carRaceInfo.penaltyTime == static_cast<float>(PyFloat_AsDouble(penaltyTimeVal));

    PyObject* prevFromStartLineAttr = PyUnicode_FromString("prevFromStartLine");
    PyObject* prevFromStartLineVal = PyObject_GetAttr(p_carRaceInfoObject, prevFromStartLineAttr);
    bool prevFromStartLineCorrect = p_carRaceInfo.prevFromStartLine == static_cast<float>(PyFloat_AsDouble(prevFromStartLineVal));

    PyObject* wrongWayTimeAttr = PyUnicode_FromString("wrongWayTime");
    PyObject* wrongWayTimeVal = PyObject_GetAttr(p_carRaceInfoObject, wrongWayTimeAttr);
    bool wrongWayTimeCorrect = p_carRaceInfo.wrongWayTime == static_cast<float>(PyFloat_AsDouble(wrongWayTimeVal));

    return bestLapTimeCorrect && commitBestLapTimeCorrect && deltaBestLapTimeCorrect && curLapTimeCorrect && lastLapTimeCorrect &&
           curTimeCorrect && topSpeedCorrect && currentMinSpeedForLapCorrect && lapsCorrect && bestLapCorrect && nbPitStopsCorrect &&
           remainingLapsCorrect && posCorrect && timeBehindLeaderCorrect && lapsBehindLeaderCorrect && timeBehindPrevCorrect &&
           timeBeforeNextCorrect && distRacedCorrect && distFromStartLineCorrect && currentSectorCorrect && nbSectorsCorrect &&
           trackLengthCorrect && scheduledEventTimeCorrect && eventCorrect && penaltyTimeCorrect && prevFromStartLineCorrect && wrongWayTimeCorrect;
}

bool CheckPosDObject(tPosd& p_posD, PyObject* p_trackSegObject)
{
    PyObject* xAttr = PyUnicode_FromString("x");
    PyObject* xVal = PyObject_GetAttr(p_trackSegObject, xAttr);
    bool xCorrect = static_cast<double>(p_posD.x) == PyFloat_AsDouble(xVal);

    PyObject* yAttr = PyUnicode_FromString("y");
    PyObject* yVal = PyObject_GetAttr(p_trackSegObject, yAttr);
    bool yCorrect = static_cast<double>(p_posD.y) == PyFloat_AsDouble(yVal);

    PyObject* zAttr = PyUnicode_FromString("z");
    PyObject* zVal = PyObject_GetAttr(p_trackSegObject, zAttr);
    bool zCorrect = static_cast<double>(p_posD.z) == PyFloat_AsDouble(zVal);

    PyObject* xyAttr = PyUnicode_FromString("xy");
    PyObject* xyVal = PyObject_GetAttr(p_trackSegObject, xyAttr);
    bool xyCorrect = static_cast<double>(p_posD.xy) == PyFloat_AsDouble(xyVal);

    PyObject* axAttr = PyUnicode_FromString("ax");
    PyObject* axVal = PyObject_GetAttr(p_trackSegObject, axAttr);
    bool axCorrect = static_cast<double>(p_posD.ax) == PyFloat_AsDouble(axVal);

    PyObject* ayAttr = PyUnicode_FromString("ay");
    PyObject* ayVal = PyObject_GetAttr(p_trackSegObject, ayAttr);
    bool ayCorrect = static_cast<double>(p_posD.ay) == PyFloat_AsDouble(ayVal);

    PyObject* azAttr = PyUnicode_FromString("az");
    PyObject* azVal = PyObject_GetAttr(p_trackSegObject, azAttr);
    bool azCorrect = static_cast<double>(p_posD.az) == PyFloat_AsDouble(azVal);

    return xCorrect && yCorrect && zCorrect && xyCorrect && axCorrect && ayCorrect && azCorrect;
}

bool CheckPrivCarData(tPrivCar& p_privCar, PyObject* p_privCarObject)
{
    PyObject* driverIndexAttr = PyUnicode_FromString("driverIndex");
    PyObject* driverIndexVal = PyObject_GetAttr(p_privCarObject, driverIndexAttr);
    bool driverIndexCorrect = p_privCar.driverIndex == static_cast<int>(PyLong_AsLong(driverIndexVal));

    PyObject* moduleIndexAttr = PyUnicode_FromString("moduleIndex");
    PyObject* moduleIndexVal = PyObject_GetAttr(p_privCarObject, moduleIndexAttr);
    bool moduleIndexCorrect = p_privCar.moduleIndex == static_cast<int>(PyLong_AsLong(moduleIndexVal));

    PyObject* modNameAttr = PyUnicode_FromString("modName");
    PyObject* modNameVal = PyObject_GetAttr(p_privCarObject, modNameAttr);
    bool modNameCorrect = strcmp(p_privCar.modName, PyUnicode_AsUTF8(modNameVal)) == 0;

    PyObject* cornerAttr = PyUnicode_FromString("corner");
    PyObject* cornerVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, cornerAttr));
    bool cornerCorrect = CheckPosDObject(p_privCar.corner[0], PyTuple_GetItem(cornerVal, 0)) &&
                         CheckPosDObject(p_privCar.corner[1], PyTuple_GetItem(cornerVal, 1)) &&
                         CheckPosDObject(p_privCar.corner[2], PyTuple_GetItem(cornerVal, 2)) &&
                         CheckPosDObject(p_privCar.corner[3], PyTuple_GetItem(cornerVal, 3));

    PyObject* gearAttr = PyUnicode_FromString("gear");
    PyObject* gearVal = PyObject_GetAttr(p_privCarObject, gearAttr);
    bool gearCorrect = p_privCar.gear == static_cast<int>(PyLong_AsLong(gearVal));

    PyObject* gearNextAttr = PyUnicode_FromString("gearNext");
    PyObject* gearNextVal = PyObject_GetAttr(p_privCarObject, gearNextAttr);
    bool gearNextCorrect = p_privCar.gearNext == static_cast<int>(PyLong_AsLong(gearNextVal));

    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_privCarObject, fuelAttr);
    bool fuelCorrect = p_privCar.fuel == static_cast<float>(PyFloat_AsDouble(fuelVal));

    PyObject* fuelConsumptionTotalAttr = PyUnicode_FromString("fuelConsumptionTotal");
    PyObject* fuelConsumptionTotalVal = PyObject_GetAttr(p_privCarObject, fuelConsumptionTotalAttr);
    bool fuelConsumptionTotalCorrect = p_privCar.fuel_consumption_total == static_cast<float>(PyFloat_AsDouble(fuelConsumptionTotalVal));

    PyObject* fuelConsumptionInstantAttr = PyUnicode_FromString("fuelConsumptionInstant");
    PyObject* fuelConsumptionInstantVal = PyObject_GetAttr(p_privCarObject, fuelConsumptionInstantAttr);
    bool fuelConsumptionInstantCorrect = p_privCar.fuel_consumption_instant == static_cast<float>(PyFloat_AsDouble(fuelConsumptionInstantVal));

    PyObject* engineRPMAttr = PyUnicode_FromString("engineRPM");
    PyObject* engineRPMVal = PyObject_GetAttr(p_privCarObject, engineRPMAttr);
    bool engineRPMCorrect = p_privCar.enginerpm == static_cast<float>(PyFloat_AsDouble(engineRPMVal));

    PyObject* engineRPMRedLineAttr = PyUnicode_FromString("engineRPMRedLine");
    PyObject* engineRPMRedLineVal = PyObject_GetAttr(p_privCarObject, engineRPMRedLineAttr);
    bool engineRPMRedLineCorrect = p_privCar.enginerpmRedLine == static_cast<float>(PyFloat_AsDouble(engineRPMRedLineVal));

    PyObject* engineRPMMaxAttr = PyUnicode_FromString("engineRPMMax");
    PyObject* engineRPMMaxVal = PyObject_GetAttr(p_privCarObject, engineRPMMaxAttr);
    bool engineRPMMaxCorrect = p_privCar.enginerpmMax == static_cast<float>(PyFloat_AsDouble(engineRPMMaxVal));

    PyObject* engineRPMMaxTqAttr = PyUnicode_FromString("engineRPMMaxTq");
    PyObject* engineRPMMaxTqVal = PyObject_GetAttr(p_privCarObject, engineRPMMaxTqAttr);
    bool engineRPMMaxTqCorrect = p_privCar.enginerpmMaxTq == static_cast<float>(PyFloat_AsDouble(engineRPMMaxTqVal));

    PyObject* engineRPMMaxPwAttr = PyUnicode_FromString("engineRPMMaxPw");
    PyObject* engineRPMMaxPwVal = PyObject_GetAttr(p_privCarObject, engineRPMMaxPwAttr);
    bool engineRPMMaxPwCorrect = p_privCar.enginerpmMaxPw == static_cast<float>(PyFloat_AsDouble(engineRPMMaxPwVal));

    PyObject* engineMaxTqAttr = PyUnicode_FromString("engineMaxTq");
    PyObject* engineMaxTqVal = PyObject_GetAttr(p_privCarObject, engineMaxTqAttr);
    bool engineMaxTqCorrect = p_privCar.engineMaxTq == static_cast<float>(PyFloat_AsDouble(engineMaxTqVal));

    PyObject* engineMaxPwAttr = PyUnicode_FromString("engineMaxPw");
    PyObject* engineMaxPwVal = PyObject_GetAttr(p_privCarObject, engineMaxPwAttr);
    bool engineMaxPwCorrect = p_privCar.engineMaxPw == static_cast<float>(PyFloat_AsDouble(engineMaxPwVal));

    PyObject* gearRatioAttr = PyUnicode_FromString("gearRatio");
    PyObject* gearRatioVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, gearRatioAttr));
    bool gearRatioCorrect = p_privCar.gearRatio[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 0))) &&
                            p_privCar.gearRatio[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 1))) &&
                            p_privCar.gearRatio[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 2))) &&
                            p_privCar.gearRatio[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 3))) &&
                            p_privCar.gearRatio[4] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 4))) &&
                            p_privCar.gearRatio[5] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 5))) &&
                            p_privCar.gearRatio[6] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 6))) &&
                            p_privCar.gearRatio[7] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 7))) &&
                            p_privCar.gearRatio[8] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 8))) &&
                            p_privCar.gearRatio[9] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearRatioVal, 9)));

    PyObject* gearNbAttr = PyUnicode_FromString("gearNb");
    PyObject* gearNbVal = PyObject_GetAttr(p_privCarObject, gearNbAttr);
    bool gearNbCorrect = p_privCar.gearNb == static_cast<int>(PyLong_AsLong(gearNbVal));

    PyObject* gearOffsetAttr = PyUnicode_FromString("gearOffset");
    PyObject* gearOffsetVal = PyObject_GetAttr(p_privCarObject, gearOffsetAttr);
    bool gearOffsetCorrect = p_privCar.gearOffset == static_cast<int>(PyLong_AsLong(gearOffsetVal));

    PyObject* skidAttr = PyUnicode_FromString("skid");
    PyObject* skidVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, skidAttr));
    bool skidCorrect = p_privCar.skid[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 0))) &&
                       p_privCar.skid[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 1))) &&
                       p_privCar.skid[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 2))) &&
                       p_privCar.skid[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidVal, 3)));

    PyObject* reactionAttr = PyUnicode_FromString("reaction");
    PyObject* reactionVal = PyList_AsTuple(PyObject_GetAttr(p_privCarObject, reactionAttr));
    bool reactionCorrect = p_privCar.reaction[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 0))) &&
                           p_privCar.reaction[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 1))) &&
                           p_privCar.reaction[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 2))) &&
                           p_privCar.reaction[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionVal, 3)));

    PyObject* collisionAttr = PyUnicode_FromString("collision");
    PyObject* collisionVal = PyObject_GetAttr(p_privCarObject, collisionAttr);
    bool collisionCorrect = p_privCar.collision == static_cast<int>(PyLong_AsLong(collisionVal));

    PyObject* simCollisionAttr = PyUnicode_FromString("simCollision");
    PyObject* simCollisionVal = PyObject_GetAttr(p_privCarObject, simCollisionAttr);
    bool simCollisionCorrect = p_privCar.simcollision == static_cast<int>(PyLong_AsLong(simCollisionVal));

    PyObject* smokeAttr = PyUnicode_FromString("smoke");
    PyObject* smokeVal = PyObject_GetAttr(p_privCarObject, smokeAttr);
    bool smokeCorrect = p_privCar.smoke == static_cast<float>(PyFloat_AsDouble(smokeVal));

    PyObject* normalAttr = PyUnicode_FromString("normal");
    PyObject* normalVal = PyObject_GetAttr(p_privCarObject, normalAttr);
    bool normalCorrect = CheckVectorData(p_privCar.normal.x, p_privCar.normal.y, p_privCar.normal.z, normalVal);

    PyObject* collPosAttr = PyUnicode_FromString("collPos");
    PyObject* collPosVal = PyObject_GetAttr(p_privCarObject, collPosAttr);
    bool collPosCorrect = CheckVectorData(p_privCar.collpos.x, p_privCar.collpos.y, p_privCar.collpos.z, collPosVal);

    PyObject* damageAttr = PyUnicode_FromString("damage");
    PyObject* damageVal = PyObject_GetAttr(p_privCarObject, damageAttr);
    bool damageCorrect = p_privCar.dammage == static_cast<int>(PyLong_AsLong(damageVal));

    PyObject* debugAttr = PyUnicode_FromString("debug");
    PyObject* debugVal = PyObject_GetAttr(p_privCarObject, debugAttr);
    bool debugCorrect = p_privCar.debug == static_cast<int>(PyLong_AsLong(debugVal));

    PyObject* collisionStateAttr = PyUnicode_FromString("collisionState");
    PyObject* collisionStateVal = PyObject_GetAttr(p_privCarObject, collisionStateAttr);
    bool collisionStateCorrect = CheckCollisionStateObject(p_privCar.collision_state, collisionStateVal);

    PyObject* localPressureAttr = PyUnicode_FromString("localPressure");
    PyObject* localPressureVal = PyObject_GetAttr(p_privCarObject, localPressureAttr);
    bool localPressureCorrect = p_privCar.localPressure == static_cast<float>(PyFloat_AsDouble(localPressureVal));

    PyObject* driveSkillAttr = PyUnicode_FromString("driveSkill");
    PyObject* driveSkillVal = PyObject_GetAttr(p_privCarObject, driveSkillAttr);
    bool driveSkillCorrect = p_privCar.driveSkill == static_cast<float>(PyFloat_AsDouble(driveSkillVal));

    PyObject* steerTqCenterAttr = PyUnicode_FromString("steerTqCenter");
    PyObject* steerTqCenterVal = PyObject_GetAttr(p_privCarObject, steerTqCenterAttr);
    bool steerTqCenterCorrect = p_privCar.steerTqCenter == static_cast<float>(PyFloat_AsDouble(steerTqCenterVal));

    PyObject* steerTqAlignAttr = PyUnicode_FromString("steerTqAlign");
    PyObject* steerTqAlignVal = PyObject_GetAttr(p_privCarObject, steerTqAlignAttr);
    bool steerTqAlignCorrect = p_privCar.steerTqAlign == static_cast<float>(PyFloat_AsDouble(steerTqAlignVal));

    PyObject* dashboardInstantNbAttr = PyUnicode_FromString("dashboardInstantNb");
    PyObject* dashboardInstantNbVal = PyObject_GetAttr(p_privCarObject, dashboardInstantNbAttr);
    bool dashboardInstantNbCorrect = p_privCar.dashboardInstantNb == static_cast<int>(PyLong_AsLong(dashboardInstantNbVal));

    PyObject* dashboardRequestNbAttr = PyUnicode_FromString("dashboardRequestNb");
    PyObject* dashboardRequestNbVal = PyObject_GetAttr(p_privCarObject, dashboardRequestNbAttr);
    bool dashboardRequestNbCorrect = p_privCar.dashboardRequestNb == static_cast<int>(PyLong_AsLong(dashboardRequestNbVal));

    PyObject* dashboardActiveItemAttr = PyUnicode_FromString("dashboardActiveItem");
    PyObject* dashboardActiveItemVal = PyObject_GetAttr(p_privCarObject, dashboardActiveItemAttr);
    bool dashboardActiveItemCorrect = p_privCar.dashboardActiveItem == static_cast<int>(PyLong_AsLong(dashboardActiveItemVal));

    return driverIndexCorrect && moduleIndexCorrect && modNameCorrect && cornerCorrect && gearCorrect && gearNextCorrect &&
           fuelCorrect && fuelConsumptionTotalCorrect && fuelConsumptionInstantCorrect && engineRPMCorrect && engineRPMRedLineCorrect &&
           engineRPMMaxCorrect && engineRPMMaxTqCorrect && engineRPMMaxPwCorrect && engineMaxTqCorrect && engineMaxPwCorrect &&
           gearRatioCorrect && gearNbCorrect && gearOffsetCorrect && skidCorrect && reactionCorrect && collisionCorrect &&
           simCollisionCorrect && smokeCorrect && normalCorrect && collPosCorrect && damageCorrect && debugCorrect && collisionStateCorrect &&
           localPressureCorrect && driveSkillCorrect && steerTqCenterCorrect && steerTqAlignCorrect && dashboardInstantNbCorrect &&
           dashboardRequestNbCorrect && dashboardActiveItemCorrect;
}

bool CheckCarCtrlData(tCarCtrl& p_carCtrl, PyObject* p_carCtrlObject)
{
    PyObject* steerAttr = PyUnicode_FromString("steer");
    PyObject* steerVal = PyObject_GetAttr(p_carCtrlObject, steerAttr);
    bool steerCorrect = p_carCtrl.steer == static_cast<float>(PyFloat_AsDouble(steerVal));

    PyObject* accelCmdAttr = PyUnicode_FromString("accelCmd");
    PyObject* accelCmdVal = PyObject_GetAttr(p_carCtrlObject, accelCmdAttr);
    bool accelCmdCorrect = p_carCtrl.accelCmd == static_cast<float>(PyFloat_AsDouble(accelCmdVal));

    PyObject* brakeCmdAttr = PyUnicode_FromString("brakeCmd");
    PyObject* brakeCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeCmdAttr);
    bool brakeCmdCorrect = p_carCtrl.brakeCmd == static_cast<float>(PyFloat_AsDouble(brakeCmdVal));

    PyObject* clutchCmdAttr = PyUnicode_FromString("clutchCmd");
    PyObject* clutchCmdVal = PyObject_GetAttr(p_carCtrlObject, clutchCmdAttr);
    bool clutchCmdCorrect = p_carCtrl.clutchCmd == static_cast<float>(PyFloat_AsDouble(clutchCmdVal));

    PyObject* brakeFrontLeftCmdAttr = PyUnicode_FromString("brakeFrontLeftCmd");
    PyObject* brakeFrontLeftCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeFrontLeftCmdAttr);
    bool brakeFrontLeftCmdCorrect = p_carCtrl.brakeFrontLeftCmd == static_cast<float>(PyFloat_AsDouble(brakeFrontLeftCmdVal));

    PyObject* brakeFrontRightCmdAttr = PyUnicode_FromString("brakeFrontRightCmd");
    PyObject* brakeFrontRightCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeFrontRightCmdAttr);
    bool brakeFrontRightCmdCorrect = p_carCtrl.brakeFrontRightCmd == static_cast<float>(PyFloat_AsDouble(brakeFrontRightCmdVal));

    PyObject* brakeRearLeftCmdAttr = PyUnicode_FromString("brakeRearLeftCmd");
    PyObject* brakeRearLeftCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeRearLeftCmdAttr);
    bool brakeRearLeftCmdCorrect = p_carCtrl.brakeRearLeftCmd == static_cast<float>(PyFloat_AsDouble(brakeRearLeftCmdVal));

    PyObject* brakeRearRightCmdAttr = PyUnicode_FromString("brakeRearRightCmd");
    PyObject* brakeRearRightCmdVal = PyObject_GetAttr(p_carCtrlObject, brakeRearRightCmdAttr);
    bool brakeRearRightCmdCorrect = p_carCtrl.brakeRearRightCmd == static_cast<float>(PyFloat_AsDouble(brakeRearRightCmdVal));

    PyObject* wingFrontCmdAttr = PyUnicode_FromString("wingFrontCmd");
    PyObject* wingFrontCmdVal = PyObject_GetAttr(p_carCtrlObject, wingFrontCmdAttr);
    bool wingFrontCmdCorrect = p_carCtrl.wingFrontCmd == static_cast<float>(PyFloat_AsDouble(wingFrontCmdVal));

    PyObject* wingRearCmdAttr = PyUnicode_FromString("wingRearCmd");
    PyObject* wingRearCmdVal = PyObject_GetAttr(p_carCtrlObject, wingRearCmdAttr);
    bool wingRearCmdCorrect = p_carCtrl.wingRearCmd == static_cast<float>(PyFloat_AsDouble(wingRearCmdVal));

    PyObject* reserved1Attr = PyUnicode_FromString("reserved1");
    PyObject* reserved1Val = PyObject_GetAttr(p_carCtrlObject, reserved1Attr);
    bool reserved1Correct = p_carCtrl.reserved1 == static_cast<float>(PyFloat_AsDouble(reserved1Val));

    PyObject* reserved2Attr = PyUnicode_FromString("reserved2");
    PyObject* reserved2Val = PyObject_GetAttr(p_carCtrlObject, reserved2Attr);
    bool reserved2Correct = p_carCtrl.reserved2 == static_cast<float>(PyFloat_AsDouble(reserved2Val));

    PyObject* gearAttr = PyUnicode_FromString("gear");
    PyObject* gearVal = PyObject_GetAttr(p_carCtrlObject, gearAttr);
    bool gearCorrect = p_carCtrl.gear == static_cast<int>(PyLong_AsLong(gearVal));

    PyObject* raceCmdAttr = PyUnicode_FromString("raceCmd");
    PyObject* raceCmdVal = PyObject_GetAttr(p_carCtrlObject, raceCmdAttr);
    bool raceCmdCorrect = p_carCtrl.raceCmd == static_cast<int>(PyLong_AsLong(raceCmdVal));

    PyObject* lightCmdAttr = PyUnicode_FromString("lightCmd");
    PyObject* lightCmdVal = PyObject_GetAttr(p_carCtrlObject, lightCmdAttr);
    bool lightCmdCorrect = p_carCtrl.lightCmd == static_cast<int>(PyLong_AsLong(lightCmdVal));

    PyObject* eBrakeCmdAttr = PyUnicode_FromString("eBrakeCmd");
    PyObject* eBrakeCmdVal = PyObject_GetAttr(p_carCtrlObject, eBrakeCmdAttr);
    bool eBrakeCmdCorrect = p_carCtrl.ebrakeCmd == static_cast<int>(PyLong_AsLong(eBrakeCmdVal));

    PyObject* wingControlModeAttr = PyUnicode_FromString("wingControlMode");
    PyObject* wingControlModeVal = PyObject_GetAttr(p_carCtrlObject, wingControlModeAttr);
    bool wingControlModeCorrect = p_carCtrl.wingControlMode == static_cast<int>(PyLong_AsLong(wingControlModeVal));

    PyObject* singleWheelBrakeModeAttr = PyUnicode_FromString("singleWheelBrakeMode");
    PyObject* singleWheelBrakeModeVal = PyObject_GetAttr(p_carCtrlObject, singleWheelBrakeModeAttr);
    bool singleWheelBrakeModeCorrect = p_carCtrl.singleWheelBrakeMode == static_cast<int>(PyLong_AsLong(singleWheelBrakeModeVal));

    PyObject* switch3Attr = PyUnicode_FromString("switch3");
    PyObject* switch3Val = PyObject_GetAttr(p_carCtrlObject, switch3Attr);
    bool switch3Correct = p_carCtrl.switch3 == static_cast<int>(PyLong_AsLong(switch3Val));

    PyObject* telemetryModeAttr = PyUnicode_FromString("telemetryMode");
    PyObject* telemetryModeVal = PyObject_GetAttr(p_carCtrlObject, telemetryModeAttr);
    bool telemetryModeCorrect = p_carCtrl.telemetryMode == static_cast<int>(PyLong_AsLong(telemetryModeVal));

    PyObject* msgAttr = PyUnicode_FromString("msg");
    PyObject* msgVal = PyList_AsTuple(PyObject_GetAttr(p_carCtrlObject, msgAttr));
    bool msgCorrect = strcmp(p_carCtrl.msg[0], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 0))) == 0 &&
                      strcmp(p_carCtrl.msg[1], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 1))) == 0 &&
                      strcmp(p_carCtrl.msg[2], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 2))) == 0 &&
                      strcmp(p_carCtrl.msg[3], PyUnicode_AsUTF8(PyTuple_GetItem(msgVal, 3))) == 0;

    PyObject* msgColorAttr = PyUnicode_FromString("msgColor");
    PyObject* msgColorVal = PyList_AsTuple(PyObject_GetAttr(p_carCtrlObject, msgColorAttr));
    PyObject* test1 = PyTuple_GetItem(msgColorVal, 0);
    double test = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 0)));
    bool msgColorCorrect = p_carCtrl.msgColor[0] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 0))) &&
                           p_carCtrl.msgColor[1] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 1))) &&
                           p_carCtrl.msgColor[2] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 2))) &&
                           p_carCtrl.msgColor[3] == static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorVal, 3)));

    return steerCorrect && accelCmdCorrect && brakeCmdCorrect && clutchCmdCorrect && brakeFrontLeftCmdCorrect &&
           brakeFrontRightCmdCorrect && brakeRearLeftCmdCorrect && brakeRearRightCmdCorrect && wingFrontCmdCorrect &&
           wingRearCmdCorrect && reserved1Correct && reserved2Correct && gearCorrect && raceCmdCorrect && lightCmdCorrect &&
           eBrakeCmdCorrect && wingControlModeCorrect && singleWheelBrakeModeCorrect && switch3Correct &&
           telemetryModeCorrect && msgCorrect && msgColorCorrect;
}

bool CheckCarSetupItemData(tCarSetupItem& p_carSetupItem, PyObject* p_carSetupItemObject)
{
    PyObject* valueAttr = PyUnicode_FromString("value");
    PyObject* valueVal = PyObject_GetAttr(p_carSetupItemObject, valueAttr);
    bool valueCorrect = p_carSetupItem.value == static_cast<float>(PyFloat_AsDouble(valueVal));

    PyObject* minAttr = PyUnicode_FromString("min");
    PyObject* minVal = PyObject_GetAttr(p_carSetupItemObject, minAttr);
    bool minCorrect = p_carSetupItem.min == static_cast<float>(PyFloat_AsDouble(minVal));

    PyObject* maxAttr = PyUnicode_FromString("max");
    PyObject* maxVal = PyObject_GetAttr(p_carSetupItemObject, maxAttr);
    bool maxCorrect = p_carSetupItem.max == static_cast<float>(PyFloat_AsDouble(maxVal));

    PyObject* desiredValueAttr = PyUnicode_FromString("desiredValue");
    PyObject* desiredValueVal = PyObject_GetAttr(p_carSetupItemObject, desiredValueAttr);
    bool desiredValueCorrect = p_carSetupItem.desired_value == static_cast<float>(PyFloat_AsDouble(desiredValueVal));

    PyObject* stepSizeAttr = PyUnicode_FromString("stepSize");
    PyObject* stepSizeVal = PyObject_GetAttr(p_carSetupItemObject, stepSizeAttr);
    bool stepSizeCorrect = p_carSetupItem.stepsize == static_cast<float>(PyFloat_AsDouble(stepSizeVal));

    PyObject* changedAttr = PyUnicode_FromString("changed");
    PyObject* changedVal = PyObject_GetAttr(p_carSetupItemObject, changedAttr);
    bool changedCorrect = p_carSetupItem.changed == static_cast<bool>(PyLong_AsLong(changedVal));

    return valueCorrect && minCorrect && maxCorrect && desiredValueCorrect && stepSizeCorrect && changedCorrect;
}

bool CheckCarPitCmdData(tCarPitCmd& p_carPitCmd, PyObject* p_carPitCmdObject)
{
    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_carPitCmdObject, fuelAttr);
    bool fuelCorrect = p_carPitCmd.fuel == static_cast<float>(PyFloat_AsDouble(fuelVal));

    PyObject* repairAttr = PyUnicode_FromString("repair");
    PyObject* repairVal = PyObject_GetAttr(p_carPitCmdObject, repairAttr);
    bool repairCorrect = p_carPitCmd.repair == static_cast<int>(PyFloat_AsDouble(repairVal));

    PyObject* stopTypeAttr = PyUnicode_FromString("stopType");
    PyObject* stopTypeVal = PyObject_GetAttr(p_carPitCmdObject, stopTypeAttr);
    bool stopTypeCorrect = p_carPitCmd.stopType == static_cast<int>(PyFloat_AsDouble(stopTypeVal));

    PyObject* setupChangedAttr = PyUnicode_FromString("setupChanged");
    PyObject* setupChangedVal = PyObject_GetAttr(p_carPitCmdObject, setupChangedAttr);
    bool setupChangedCorrect = p_carPitCmd.setupChanged == static_cast<bool>(PyLong_AsLong(setupChangedVal));

    PyObject* tireChangeAttr = PyUnicode_FromString("tireChange");
    PyObject* tireChangeVal = PyObject_GetAttr(p_carPitCmdObject, tireChangeAttr);
    bool tireChangeCorrect = p_carPitCmd.tireChange == static_cast<bool>(PyLong_AsLong(tireChangeVal));

    return fuelCorrect && repairCorrect && stopTypeCorrect && setupChangedCorrect && tireChangeCorrect;
}

bool CheckCarSetupData(tCarSetup& p_carSetup, PyObject* p_carSetupObject)
{
    PyObject* fRWeightRepAttr = PyUnicode_FromString("fRWeightRep");
    PyObject* fRWeightRepVal = PyObject_GetAttr(p_carSetupObject, fRWeightRepAttr);
    bool fRWeightRepCorrect = CheckCarSetupItemData(p_carSetup.FRWeightRep, fRWeightRepVal);

    PyObject* fRLWeightRepAttr = PyUnicode_FromString("fRLWeightRep");
    PyObject* fRLWeightRepVal = PyObject_GetAttr(p_carSetupObject, fRLWeightRepAttr);
    bool fRLWeightRepCorrect = CheckCarSetupItemData(p_carSetup.FRLWeightRep, fRLWeightRepVal);

    PyObject* rRLWeightRepAttr = PyUnicode_FromString("rRLWeightRep");
    PyObject* rRLWeightRepVal = PyObject_GetAttr(p_carSetupObject, rRLWeightRepAttr);
    bool rRLWeightRepCorrect = CheckCarSetupItemData(p_carSetup.RRLWeightRep, rRLWeightRepVal);

    PyObject* fuelAttr = PyUnicode_FromString("fuel");
    PyObject* fuelVal = PyObject_GetAttr(p_carSetupObject, fuelAttr);
    bool fuelCorrect = CheckCarSetupItemData(p_carSetup.fuel, fuelVal);

    PyObject* wingAngleAttr = PyUnicode_FromString("wingAngle");
    PyObject* wingAngleVal = PyObject_GetAttr(p_carSetupObject, wingAngleAttr);
    bool wingAngleCorrect = CheckCarSetupItemData(p_carSetup.wingAngle[0], PyTuple_GetItem(wingAngleVal, 0)) &&
                            CheckCarSetupItemData(p_carSetup.wingAngle[1], PyTuple_GetItem(wingAngleVal, 1));

    PyObject* revsLimiterAttr = PyUnicode_FromString("revsLimiter");
    PyObject* revsLimiterVal = PyObject_GetAttr(p_carSetupObject, revsLimiterAttr);
    bool revsLimiterCorrect = CheckCarSetupItemData(p_carSetup.revsLimiter, revsLimiterVal);

    PyObject* gearRatioAttr = PyUnicode_FromString("gearRatio");
    PyObject* gearRatioVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, gearRatioAttr));
    bool gearRatioCorrect = CheckCarSetupItemData(p_carSetup.gearRatio[0], PyTuple_GetItem(gearRatioVal, 0)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[1], PyTuple_GetItem(gearRatioVal, 1)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[2], PyTuple_GetItem(gearRatioVal, 2)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[3], PyTuple_GetItem(gearRatioVal, 3)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[4], PyTuple_GetItem(gearRatioVal, 4)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[5], PyTuple_GetItem(gearRatioVal, 5)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[6], PyTuple_GetItem(gearRatioVal, 6)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[7], PyTuple_GetItem(gearRatioVal, 7)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[8], PyTuple_GetItem(gearRatioVal, 8)) &&
                            CheckCarSetupItemData(p_carSetup.gearRatio[9], PyTuple_GetItem(gearRatioVal, 9));

    PyObject* differentialTypeAttr = PyUnicode_FromString("differentialType");
    PyObject* differentialTypeVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialTypeAttr));
    bool differentialTypeCorrect = p_carSetup.differentialType[0] == static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(differentialTypeVal, 0))) &&
                                   p_carSetup.differentialType[1] == static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(differentialTypeVal, 1))) &&
                                   p_carSetup.differentialType[2] == static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(differentialTypeVal, 2)));

    PyObject* differentialRatioAttr = PyUnicode_FromString("differentialRatio");
    PyObject* differentialRatioVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialRatioAttr));
    bool differentialRatioCorrect = CheckCarSetupItemData(p_carSetup.differentialRatio[0], PyTuple_GetItem(differentialRatioVal, 0)) &&
                                    CheckCarSetupItemData(p_carSetup.differentialRatio[1], PyTuple_GetItem(differentialRatioVal, 1)) &&
                                    CheckCarSetupItemData(p_carSetup.differentialRatio[2], PyTuple_GetItem(differentialRatioVal, 2));

    PyObject* differentialMinTqBiasAttr = PyUnicode_FromString("differentialMinTqBias");
    PyObject* differentialMinTqBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialMinTqBiasAttr));
    bool differentialMinTqBiasCorrect = CheckCarSetupItemData(p_carSetup.differentialMinTqBias[0], PyTuple_GetItem(differentialMinTqBiasVal, 0)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialMinTqBias[1], PyTuple_GetItem(differentialMinTqBiasVal, 1)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialMinTqBias[2], PyTuple_GetItem(differentialMinTqBiasVal, 2));

    PyObject* differentialMaxTqBiasAttr = PyUnicode_FromString("differentialMaxTqBias");
    PyObject* differentialMaxTqBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialMaxTqBiasAttr));
    bool differentialMaxTqBiasCorrect = CheckCarSetupItemData(p_carSetup.differentialMaxTqBias[0], PyTuple_GetItem(differentialMaxTqBiasVal, 0)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialMaxTqBias[1], PyTuple_GetItem(differentialMaxTqBiasVal, 1)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialMaxTqBias[2], PyTuple_GetItem(differentialMaxTqBiasVal, 2));

    PyObject* differentialViscosityAttr = PyUnicode_FromString("differentialViscosity");
    PyObject* differentialViscosityVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialViscosityAttr));
    bool differentialViscosityCorrect = CheckCarSetupItemData(p_carSetup.differentialViscosity[0], PyTuple_GetItem(differentialViscosityVal, 0)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialViscosity[1], PyTuple_GetItem(differentialViscosityVal, 1)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialViscosity[2], PyTuple_GetItem(differentialViscosityVal, 2));

    PyObject* differentialLockingTqAttr = PyUnicode_FromString("differentialLockingTq");
    PyObject* differentialLockingTqVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialLockingTqAttr));
    bool differentialLockingTqCorrect = CheckCarSetupItemData(p_carSetup.differentialLockingTq[0], PyTuple_GetItem(differentialLockingTqVal, 0)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialLockingTq[1], PyTuple_GetItem(differentialLockingTqVal, 1)) &&
                                        CheckCarSetupItemData(p_carSetup.differentialLockingTq[2], PyTuple_GetItem(differentialLockingTqVal, 2));

    PyObject* differentialMaxSlipBiasAttr = PyUnicode_FromString("differentialMaxSlipBias");
    PyObject* differentialMaxSlipBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialMaxSlipBiasAttr));
    bool differentialMaxSlipBiasCorrect = CheckCarSetupItemData(p_carSetup.differentialMaxSlipBias[0], PyTuple_GetItem(differentialMaxSlipBiasVal, 0)) &&
                                          CheckCarSetupItemData(p_carSetup.differentialMaxSlipBias[1], PyTuple_GetItem(differentialMaxSlipBiasVal, 1)) &&
                                          CheckCarSetupItemData(p_carSetup.differentialMaxSlipBias[2], PyTuple_GetItem(differentialMaxSlipBiasVal, 2));

    PyObject* differentialCoastMaxSlipBiasAttr = PyUnicode_FromString("differentialCoastMaxSlipBias");
    PyObject* differentialCoastMaxSlipBiasVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, differentialCoastMaxSlipBiasAttr));
    bool differentialCoastMaxSlipBiasCorrect = CheckCarSetupItemData(p_carSetup.differentialCoastMaxSlipBias[0], PyTuple_GetItem(differentialCoastMaxSlipBiasVal, 0)) &&
                                               CheckCarSetupItemData(p_carSetup.differentialCoastMaxSlipBias[1], PyTuple_GetItem(differentialCoastMaxSlipBiasVal, 1)) &&
                                               CheckCarSetupItemData(p_carSetup.differentialCoastMaxSlipBias[2], PyTuple_GetItem(differentialCoastMaxSlipBiasVal, 2));

    PyObject* steerLockAttr = PyUnicode_FromString("steerLock");
    PyObject* steerLockVal = PyObject_GetAttr(p_carSetupObject, steerLockAttr);
    bool steerLockCorrect = CheckCarSetupItemData(p_carSetup.steerLock, steerLockVal);

    PyObject* brakeRepartitionAttr = PyUnicode_FromString("brakeRepartition");
    PyObject* brakeRepartitionVal = PyObject_GetAttr(p_carSetupObject, brakeRepartitionAttr);
    bool brakeRepartitionCorrect = CheckCarSetupItemData(p_carSetup.brakeRepartition, brakeRepartitionVal);

    PyObject* brakePressureAttr = PyUnicode_FromString("brakePressure");
    PyObject* brakePressureVal = PyObject_GetAttr(p_carSetupObject, brakePressureAttr);
    bool brakePressureCorrect = CheckCarSetupItemData(p_carSetup.brakePressure, brakePressureVal);

    PyObject* rideHeightAttr = PyUnicode_FromString("rideHeight");
    PyObject* rideHeightVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, rideHeightAttr));
    bool rideHeightCorrect = CheckCarSetupItemData(p_carSetup.rideHeight[0], PyTuple_GetItem(rideHeightVal, 0)) &&
                             CheckCarSetupItemData(p_carSetup.rideHeight[1], PyTuple_GetItem(rideHeightVal, 1)) &&
                             CheckCarSetupItemData(p_carSetup.rideHeight[2], PyTuple_GetItem(rideHeightVal, 2)) &&
                             CheckCarSetupItemData(p_carSetup.rideHeight[3], PyTuple_GetItem(rideHeightVal, 3));

    PyObject* toeAttr = PyUnicode_FromString("toe");
    PyObject* toeVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, toeAttr));
    bool toeCorrect = CheckCarSetupItemData(p_carSetup.toe[0], PyTuple_GetItem(toeVal, 0)) &&
                      CheckCarSetupItemData(p_carSetup.toe[1], PyTuple_GetItem(toeVal, 1)) &&
                      CheckCarSetupItemData(p_carSetup.toe[2], PyTuple_GetItem(toeVal, 2)) &&
                      CheckCarSetupItemData(p_carSetup.toe[3], PyTuple_GetItem(toeVal, 3));

    PyObject* camberAttr = PyUnicode_FromString("camber");
    PyObject* camberVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, camberAttr));
    bool camberCorrect = CheckCarSetupItemData(p_carSetup.camber[0], PyTuple_GetItem(camberVal, 0)) &&
                         CheckCarSetupItemData(p_carSetup.camber[1], PyTuple_GetItem(camberVal, 1)) &&
                         CheckCarSetupItemData(p_carSetup.camber[2], PyTuple_GetItem(camberVal, 2)) &&
                         CheckCarSetupItemData(p_carSetup.camber[3], PyTuple_GetItem(camberVal, 3));

    PyObject* tirePressureAttr = PyUnicode_FromString("tirePressure");
    PyObject* tirePressureVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, tirePressureAttr));
    bool tirePressureCorrect = CheckCarSetupItemData(p_carSetup.tirePressure[0], PyTuple_GetItem(tirePressureVal, 0)) &&
                               CheckCarSetupItemData(p_carSetup.tirePressure[1], PyTuple_GetItem(tirePressureVal, 1)) &&
                               CheckCarSetupItemData(p_carSetup.tirePressure[2], PyTuple_GetItem(tirePressureVal, 2)) &&
                               CheckCarSetupItemData(p_carSetup.tirePressure[3], PyTuple_GetItem(tirePressureVal, 3));

    PyObject* tireOpLoadAttr = PyUnicode_FromString("tireOpLoad");
    PyObject* tireOpLoadVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, tireOpLoadAttr));
    bool tireOpLoadCorrect = CheckCarSetupItemData(p_carSetup.tireOpLoad[0], PyTuple_GetItem(tireOpLoadVal, 0)) &&
                             CheckCarSetupItemData(p_carSetup.tireOpLoad[1], PyTuple_GetItem(tireOpLoadVal, 1)) &&
                             CheckCarSetupItemData(p_carSetup.tireOpLoad[2], PyTuple_GetItem(tireOpLoadVal, 2)) &&
                             CheckCarSetupItemData(p_carSetup.tireOpLoad[3], PyTuple_GetItem(tireOpLoadVal, 3));

    PyObject* arbSpringAttr = PyUnicode_FromString("arbSpring");
    PyObject* arbSpringVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, arbSpringAttr));
    bool arbSpringCorrect = CheckCarSetupItemData(p_carSetup.arbSpring[0], PyTuple_GetItem(arbSpringVal, 0)) &&
                            CheckCarSetupItemData(p_carSetup.arbSpring[1], PyTuple_GetItem(arbSpringVal, 1));

    PyObject* arbBellCrankAttr = PyUnicode_FromString("arbBellCrank");
    PyObject* arbBellCrankVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, arbBellCrankAttr));
    bool arbBellCrankCorrect = CheckCarSetupItemData(p_carSetup.arbBellcrank[0], PyTuple_GetItem(arbBellCrankVal, 0)) &&
                               CheckCarSetupItemData(p_carSetup.arbBellcrank[1], PyTuple_GetItem(arbBellCrankVal, 1));

    PyObject* heaveSpringAttr = PyUnicode_FromString("heaveSpring");
    PyObject* heaveSpringVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveSpringAttr));
    bool heaveSpringCorrect = CheckCarSetupItemData(p_carSetup.heaveSpring[0], PyTuple_GetItem(heaveSpringVal, 0)) &&
                              CheckCarSetupItemData(p_carSetup.heaveSpring[1], PyTuple_GetItem(heaveSpringVal, 1));

    PyObject* heaveBellCrankAttr = PyUnicode_FromString("heaveBellCrank");
    PyObject* heaveBellCrankVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveBellCrankAttr));
    bool heaveBellCrankCorrect = CheckCarSetupItemData(p_carSetup.heaveBellcrank[0], PyTuple_GetItem(heaveBellCrankVal, 0)) &&
                                 CheckCarSetupItemData(p_carSetup.heaveBellcrank[1], PyTuple_GetItem(heaveBellCrankVal, 1));

    PyObject* heaveInertanceAttr = PyUnicode_FromString("heaveInertance");
    PyObject* heaveInertanceVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveInertanceAttr));
    bool heaveInertanceCorrect = CheckCarSetupItemData(p_carSetup.heaveInertance[0], PyTuple_GetItem(heaveInertanceVal, 0)) &&
                                 CheckCarSetupItemData(p_carSetup.heaveInertance[1], PyTuple_GetItem(heaveInertanceVal, 1));

    PyObject* heaveFastBumpAttr = PyUnicode_FromString("heaveFastBump");
    PyObject* heaveFastBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveFastBumpAttr));
    bool heaveFastBumpCorrect = CheckCarSetupItemData(p_carSetup.heaveFastBump[0], PyTuple_GetItem(heaveFastBumpVal, 0)) &&
                                CheckCarSetupItemData(p_carSetup.heaveFastBump[1], PyTuple_GetItem(heaveFastBumpVal, 1));

    PyObject* heaveSlowBumpAttr = PyUnicode_FromString("heaveSlowBump");
    PyObject* heaveSlowBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveSlowBumpAttr));
    bool heaveSlowBumpCorrect = CheckCarSetupItemData(p_carSetup.heaveSlowBump[0], PyTuple_GetItem(heaveSlowBumpVal, 0)) &&
                                CheckCarSetupItemData(p_carSetup.heaveSlowBump[1], PyTuple_GetItem(heaveSlowBumpVal, 1));

    PyObject* heaveBumpLevelAttr = PyUnicode_FromString("heaveBumpLevel");
    PyObject* heaveBumpLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveBumpLevelAttr));
    bool heaveBumpLevelCorrect = CheckCarSetupItemData(p_carSetup.heaveBumpLvel[0], PyTuple_GetItem(heaveBumpLevelVal, 0)) &&
                                 CheckCarSetupItemData(p_carSetup.heaveBumpLvel[1], PyTuple_GetItem(heaveBumpLevelVal, 1));

    PyObject* heaveFastReboundAttr = PyUnicode_FromString("heaveFastRebound");
    PyObject* heaveFastReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveFastReboundAttr));
    bool heaveFastReboundCorrect = CheckCarSetupItemData(p_carSetup.heaveFastRebound[0], PyTuple_GetItem(heaveFastReboundVal, 0)) &&
                                   CheckCarSetupItemData(p_carSetup.heaveFastRebound[1], PyTuple_GetItem(heaveFastReboundVal, 1));

    PyObject* heaveSlowReboundAttr = PyUnicode_FromString("heaveSlowRebound");
    PyObject* heaveSlowReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveSlowReboundAttr));
    bool heaveSlowReboundCorrect = CheckCarSetupItemData(p_carSetup.heaveSlowRebound[0], PyTuple_GetItem(heaveSlowReboundVal, 0)) &&
                                   CheckCarSetupItemData(p_carSetup.heaveSlowRebound[1], PyTuple_GetItem(heaveSlowReboundVal, 1));

    PyObject* heaveReboundLevelAttr = PyUnicode_FromString("heaveReboundLevel");
    PyObject* heaveReboundLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, heaveReboundLevelAttr));
    bool heaveReboundLevelCorrect = CheckCarSetupItemData(p_carSetup.heaveReboundLvel[0], PyTuple_GetItem(heaveReboundLevelVal, 0)) &&
                                    CheckCarSetupItemData(p_carSetup.heaveReboundLvel[1], PyTuple_GetItem(heaveReboundLevelVal, 1));

    PyObject* suspSpringAttr = PyUnicode_FromString("suspSpring");
    PyObject* suspSpringVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspSpringAttr));
    bool suspSpringCorrect = CheckCarSetupItemData(p_carSetup.suspSpring[0], PyTuple_GetItem(suspSpringVal, 0)) &&
                             CheckCarSetupItemData(p_carSetup.suspSpring[1], PyTuple_GetItem(suspSpringVal, 1)) &&
                             CheckCarSetupItemData(p_carSetup.suspSpring[2], PyTuple_GetItem(suspSpringVal, 2)) &&
                             CheckCarSetupItemData(p_carSetup.suspSpring[3], PyTuple_GetItem(suspSpringVal, 3));

    PyObject* suspBellCrankAttr = PyUnicode_FromString("suspBellCrank");
    PyObject* suspBellCrankVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspBellCrankAttr));
    bool suspBellCrankCorrect = CheckCarSetupItemData(p_carSetup.suspBellcrank[0], PyTuple_GetItem(suspBellCrankVal, 0)) &&
                                CheckCarSetupItemData(p_carSetup.suspBellcrank[1], PyTuple_GetItem(suspBellCrankVal, 1)) &&
                                CheckCarSetupItemData(p_carSetup.suspBellcrank[2], PyTuple_GetItem(suspBellCrankVal, 2)) &&
                                CheckCarSetupItemData(p_carSetup.suspBellcrank[3], PyTuple_GetItem(suspBellCrankVal, 3));

    PyObject* suspInertanceAttr = PyUnicode_FromString("suspInertance");
    PyObject* suspInertanceVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspInertanceAttr));
    bool suspInertanceCorrect = CheckCarSetupItemData(p_carSetup.suspInertance[0], PyTuple_GetItem(suspInertanceVal, 0)) &&
                                CheckCarSetupItemData(p_carSetup.suspInertance[1], PyTuple_GetItem(suspInertanceVal, 1)) &&
                                CheckCarSetupItemData(p_carSetup.suspInertance[2], PyTuple_GetItem(suspInertanceVal, 2)) &&
                                CheckCarSetupItemData(p_carSetup.suspInertance[3], PyTuple_GetItem(suspInertanceVal, 3));

    PyObject* suspCourseAttr = PyUnicode_FromString("suspCourse");
    PyObject* suspCourseVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspCourseAttr));
    bool suspCourseCorrect = CheckCarSetupItemData(p_carSetup.suspCourse[0], PyTuple_GetItem(suspCourseVal, 0)) &&
                             CheckCarSetupItemData(p_carSetup.suspCourse[1], PyTuple_GetItem(suspCourseVal, 1)) &&
                             CheckCarSetupItemData(p_carSetup.suspCourse[2], PyTuple_GetItem(suspCourseVal, 2)) &&
                             CheckCarSetupItemData(p_carSetup.suspCourse[3], PyTuple_GetItem(suspCourseVal, 3));

    PyObject* suspPackerAttr = PyUnicode_FromString("suspPacker");
    PyObject* suspPackerVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspPackerAttr));
    bool suspPackerCorrect = CheckCarSetupItemData(p_carSetup.suspPacker[0], PyTuple_GetItem(suspPackerVal, 0)) &&
                             CheckCarSetupItemData(p_carSetup.suspPacker[1], PyTuple_GetItem(suspPackerVal, 1)) &&
                             CheckCarSetupItemData(p_carSetup.suspPacker[2], PyTuple_GetItem(suspPackerVal, 2)) &&
                             CheckCarSetupItemData(p_carSetup.suspPacker[3], PyTuple_GetItem(suspPackerVal, 3));

    PyObject* suspFastBumpAttr = PyUnicode_FromString("suspFastBump");
    PyObject* suspFastBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspFastBumpAttr));
    bool suspFastBumpCorrect = CheckCarSetupItemData(p_carSetup.suspFastBump[0], PyTuple_GetItem(suspFastBumpVal, 0)) &&
                               CheckCarSetupItemData(p_carSetup.suspFastBump[1], PyTuple_GetItem(suspFastBumpVal, 1)) &&
                               CheckCarSetupItemData(p_carSetup.suspFastBump[2], PyTuple_GetItem(suspFastBumpVal, 2)) &&
                               CheckCarSetupItemData(p_carSetup.suspFastBump[3], PyTuple_GetItem(suspFastBumpVal, 3));

    PyObject* suspSlowBumpAttr = PyUnicode_FromString("suspSlowBump");
    PyObject* suspSlowBumpVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspSlowBumpAttr));
    bool suspSlowBumpCorrect = CheckCarSetupItemData(p_carSetup.suspSlowBump[0], PyTuple_GetItem(suspSlowBumpVal, 0)) &&
                               CheckCarSetupItemData(p_carSetup.suspSlowBump[1], PyTuple_GetItem(suspSlowBumpVal, 1)) &&
                               CheckCarSetupItemData(p_carSetup.suspSlowBump[2], PyTuple_GetItem(suspSlowBumpVal, 2)) &&
                               CheckCarSetupItemData(p_carSetup.suspSlowBump[3], PyTuple_GetItem(suspSlowBumpVal, 3));

    PyObject* suspBumpLevelAttr = PyUnicode_FromString("suspBumpLevel");
    PyObject* suspBumpLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspBumpLevelAttr));
    bool suspBumpLevelCorrect = CheckCarSetupItemData(p_carSetup.suspBumpLvel[0], PyTuple_GetItem(suspBumpLevelVal, 0)) &&
                                CheckCarSetupItemData(p_carSetup.suspBumpLvel[1], PyTuple_GetItem(suspBumpLevelVal, 1)) &&
                                CheckCarSetupItemData(p_carSetup.suspBumpLvel[2], PyTuple_GetItem(suspBumpLevelVal, 2)) &&
                                CheckCarSetupItemData(p_carSetup.suspBumpLvel[3], PyTuple_GetItem(suspBumpLevelVal, 3));

    PyObject* suspFastReboundAttr = PyUnicode_FromString("suspFastRebound");
    PyObject* suspFastReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspFastReboundAttr));
    bool suspFastReboundCorrect = CheckCarSetupItemData(p_carSetup.suspFastRebound[0], PyTuple_GetItem(suspFastReboundVal, 0)) &&
                                  CheckCarSetupItemData(p_carSetup.suspFastRebound[1], PyTuple_GetItem(suspFastReboundVal, 1)) &&
                                  CheckCarSetupItemData(p_carSetup.suspFastRebound[2], PyTuple_GetItem(suspFastReboundVal, 2)) &&
                                  CheckCarSetupItemData(p_carSetup.suspFastRebound[3], PyTuple_GetItem(suspFastReboundVal, 3));

    PyObject* suspSlowReboundAttr = PyUnicode_FromString("suspSlowRebound");
    PyObject* suspSlowReboundVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspSlowReboundAttr));
    bool suspSlowReboundCorrect = CheckCarSetupItemData(p_carSetup.suspSlowRebound[0], PyTuple_GetItem(suspSlowReboundVal, 0)) &&
                                  CheckCarSetupItemData(p_carSetup.suspSlowRebound[1], PyTuple_GetItem(suspSlowReboundVal, 1)) &&
                                  CheckCarSetupItemData(p_carSetup.suspSlowRebound[2], PyTuple_GetItem(suspSlowReboundVal, 2)) &&
                                  CheckCarSetupItemData(p_carSetup.suspSlowRebound[3], PyTuple_GetItem(suspSlowReboundVal, 3));

    PyObject* suspReboundLevelAttr = PyUnicode_FromString("suspReboundLevel");
    PyObject* suspReboundLevelVal = PyList_AsTuple(PyObject_GetAttr(p_carSetupObject, suspReboundLevelAttr));
    bool suspReboundLevelCorrect = CheckCarSetupItemData(p_carSetup.suspReboundLvel[0], PyTuple_GetItem(suspReboundLevelVal, 0)) &&
                                   CheckCarSetupItemData(p_carSetup.suspReboundLvel[1], PyTuple_GetItem(suspReboundLevelVal, 1)) &&
                                   CheckCarSetupItemData(p_carSetup.suspReboundLvel[2], PyTuple_GetItem(suspReboundLevelVal, 2)) &&
                                   CheckCarSetupItemData(p_carSetup.suspReboundLvel[3], PyTuple_GetItem(suspReboundLevelVal, 3));

    PyObject* reqRepairAttr = PyUnicode_FromString("reqRepair");
    PyObject* reqRepairVal = PyObject_GetAttr(p_carSetupObject, reqRepairAttr);
    bool reqRepairCorrect = CheckCarSetupItemData(p_carSetup.reqRepair, reqRepairVal);

    PyObject* reqTiresetAttr = PyUnicode_FromString("reqTireset");
    PyObject* reqTiresetVal = PyObject_GetAttr(p_carSetupObject, reqTiresetAttr);
    bool reqTiresetCorrect = CheckCarSetupItemData(p_carSetup.reqTireset, reqTiresetVal);

    PyObject* reqPenaltyAttr = PyUnicode_FromString("reqPenalty");
    PyObject* reqPenaltyVal = PyObject_GetAttr(p_carSetupObject, reqPenaltyAttr);
    bool reqPenaltyCorrect = CheckCarSetupItemData(p_carSetup.reqPenalty, reqPenaltyVal);

    return fRWeightRepCorrect && fRLWeightRepCorrect && rRLWeightRepCorrect && fuelCorrect && wingAngleCorrect && revsLimiterCorrect && gearRatioCorrect &&
           differentialTypeCorrect && differentialRatioCorrect && differentialMinTqBiasCorrect && differentialMaxTqBiasCorrect && differentialViscosityCorrect &&
           differentialLockingTqCorrect && differentialMaxSlipBiasCorrect && differentialCoastMaxSlipBiasCorrect && steerLockCorrect && brakeRepartitionCorrect &&
           brakePressureCorrect && rideHeightCorrect && toeCorrect && camberCorrect && tirePressureCorrect && tireOpLoadCorrect && arbSpringCorrect && arbBellCrankCorrect &&
           heaveSpringCorrect && heaveBellCrankCorrect && heaveInertanceCorrect && heaveFastBumpCorrect && heaveSlowBumpCorrect && heaveBumpLevelCorrect &&
           heaveFastReboundCorrect && heaveSlowReboundCorrect && heaveReboundLevelCorrect && suspSpringCorrect && suspBellCrankCorrect && suspInertanceCorrect &&
           suspCourseCorrect && suspPackerCorrect && suspFastBumpCorrect && suspSlowBumpCorrect && suspBumpLevelCorrect && suspFastReboundCorrect && suspSlowReboundCorrect &&
           suspReboundLevelCorrect && reqRepairCorrect && reqTiresetCorrect && reqPenaltyCorrect;
}

bool CheckRaceInfoData(tRaceAdmInfo& p_raceInfo, PyObject* p_raceInfoObject)
{
    PyObject* ncarsAttr = PyUnicode_FromString("ncars");
    PyObject* ncarsVal = PyObject_GetAttr(p_raceInfoObject, ncarsAttr);
    bool ncarsCorrect = p_raceInfo.ncars == static_cast<int>(PyLong_AsLong(ncarsVal));

    PyObject* totLapsAttr = PyUnicode_FromString("totLaps");
    PyObject* totLapsVal = PyObject_GetAttr(p_raceInfoObject, totLapsAttr);
    bool totLapsCorrect = p_raceInfo.totLaps == static_cast<int>(PyLong_AsLong(totLapsVal));

    PyObject* extraLapsAttr = PyUnicode_FromString("extraLaps");
    PyObject* extraLapsVal = PyObject_GetAttr(p_raceInfoObject, extraLapsAttr);
    bool extraLapsCorrect = p_raceInfo.extraLaps == static_cast<int>(PyLong_AsLong(extraLapsVal));

    PyObject* totTimeAttr = PyUnicode_FromString("totTime");
    PyObject* totTimeVal = PyObject_GetAttr(p_raceInfoObject, totTimeAttr);
    bool totTimeCorrect = p_raceInfo.totTime == static_cast<float>(PyFloat_AsDouble(totTimeVal));

    PyObject* stateAttr = PyUnicode_FromString("state");
    PyObject* stateVal = PyObject_GetAttr(p_raceInfoObject, stateAttr);
    bool stateCorrect = p_raceInfo.state == static_cast<int>(PyLong_AsLong(stateVal));

    PyObject* typeAttr = PyUnicode_FromString("type");
    PyObject* typeVal = PyObject_GetAttr(p_raceInfoObject, typeAttr);
    bool typeCorrect = p_raceInfo.type == static_cast<int>(PyLong_AsLong(typeVal));

    PyObject* maxDamageAttr = PyUnicode_FromString("maxDamage");
    PyObject* maxDamageVal = PyObject_GetAttr(p_raceInfoObject, maxDamageAttr);
    bool maxDamageCorrect = p_raceInfo.maxDammage == static_cast<int>(PyLong_AsLong(maxDamageVal));

    PyObject* fpsAttr = PyUnicode_FromString("fps");
    PyObject* fpsVal = PyObject_GetAttr(p_raceInfoObject, fpsAttr);
    bool fpsCorrect = p_raceInfo.fps == static_cast<int>(PyLong_AsLong(fpsVal));

    PyObject* featuresAttr = PyUnicode_FromString("features");
    PyObject* featuresVal = PyObject_GetAttr(p_raceInfoObject, featuresAttr);
    bool featuresCorrect = p_raceInfo.features == static_cast<int>(PyLong_AsLong(featuresVal));

    return ncarsCorrect && totLapsCorrect && extraLapsCorrect && totTimeCorrect && stateCorrect && typeCorrect &&
           maxDamageCorrect && fpsCorrect && featuresCorrect;
}

bool CheckSituationData(tSituation& p_situation, PyObject* p_situationObject)
{
    PyObject* raceInfoAttr = PyUnicode_FromString("raceInfo");
    PyObject* raceInfoVal = PyObject_GetAttr(p_situationObject, raceInfoAttr);
    bool raceInfoCorrect = CheckRaceInfoData(p_situation.raceInfo, raceInfoVal);

    PyObject* deltaTimeAttr = PyUnicode_FromString("deltaTime");
    PyObject* deltaTimeVal = PyObject_GetAttr(p_situationObject, deltaTimeAttr);
    bool deltaTimeCorrect = p_situation.deltaTime == static_cast<float>(PyFloat_AsDouble(deltaTimeVal));

    PyObject* currentTimeAttr = PyUnicode_FromString("currentTime");
    PyObject* currentTimeVal = PyObject_GetAttr(p_situationObject, currentTimeAttr);
    bool currentTimeCorrect = p_situation.currentTime == static_cast<float>(PyFloat_AsDouble(currentTimeVal));

    PyObject* accelTimeAttr = PyUnicode_FromString("accelTime");
    PyObject* accelTimeVal = PyObject_GetAttr(p_situationObject, accelTimeAttr);
    bool accelTimeCorrect = p_situation.accelTime == static_cast<float>(PyFloat_AsDouble(accelTimeVal));

    PyObject* nbPlayersAttr = PyUnicode_FromString("nbPlayers");
    PyObject* nbPlayersVal = PyObject_GetAttr(p_situationObject, nbPlayersAttr);
    bool nbPlayersCorrect = p_situation.nbPlayers == static_cast<int>(PyLong_AsLong(nbPlayersVal));

    return raceInfoCorrect && deltaTimeCorrect && currentTimeCorrect && accelTimeCorrect && nbPlayersCorrect;
}

bool CheckCarData(tCarElt& p_car, PyObject* p_carObject)
{
    PyObject* indexAttr = PyUnicode_FromString("index");
    PyObject* indexVal = PyObject_GetAttr(p_carObject, indexAttr);
    bool indexCorrect = p_car.index == static_cast<int>(PyLong_AsLong(indexVal));

    PyObject* infoAttr = PyUnicode_FromString("info");
    PyObject* infoVal = PyObject_GetAttr(p_carObject, infoAttr);
    bool infoCorrect = CheckInitCarData(p_car.info, infoVal);

    PyObject* pubAttr = PyUnicode_FromString("pub");
    PyObject* pubVal = PyObject_GetAttr(p_carObject, pubAttr);
    bool pubCorrect = CheckPublicCarData(p_car.pub, pubVal);

    PyObject* raceAttr = PyUnicode_FromString("race");
    PyObject* raceVal = PyObject_GetAttr(p_carObject, raceAttr);
    bool raceCorrect = CheckCarRaceInfoData(p_car.race, raceVal);

    PyObject* privAttr = PyUnicode_FromString("priv");
    PyObject* privVal = PyObject_GetAttr(p_carObject, privAttr);
    bool privCorrect = CheckPrivCarData(p_car.priv, privVal);

    PyObject* ctrlAttr = PyUnicode_FromString("ctrl");
    PyObject* ctrlVal = PyObject_GetAttr(p_carObject, ctrlAttr);
    bool ctrlCorrect = CheckCarCtrlData(p_car.ctrl, ctrlVal);

    PyObject* setupAttr = PyUnicode_FromString("setup");
    PyObject* setupVal = PyObject_GetAttr(p_carObject, setupAttr);
    bool setupCorrect = CheckCarSetupData(p_car.setup, setupVal);

    PyObject* pitcmdAttr = PyUnicode_FromString("pitcmd");
    PyObject* pitcmdVal = PyObject_GetAttr(p_carObject, pitcmdAttr);
    bool pitcmdCorrect = CheckCarPitCmdData(p_car.pitcmd, pitcmdVal);

    return indexCorrect && infoCorrect && pubCorrect && raceCorrect && privCorrect && ctrlCorrect && setupCorrect && pitcmdCorrect;
}

bool CheckSDAData(SDAData p_data, PyObject* p_dataObject)
{
    PyObject* tickAttr = PyUnicode_FromString("tickCount");
    PyObject* tickVal = PyObject_GetAttr(p_dataObject, tickAttr);
    bool tickCorrect = p_data.TickCount == static_cast<unsigned long>(PyLong_AsLong(tickVal));

    PyObject* carAttr = PyUnicode_FromString("car");
    PyObject* carVal = PyObject_GetAttr(p_dataObject, carAttr);
    bool carCorrect = CheckCarData(p_data.Car, PyObject_GetAttr(p_dataObject, carVal));

    PyObject* situationAttr = PyUnicode_FromString("situation");
    PyObject* situationVal = PyObject_GetAttr(p_dataObject, situationAttr);
    bool situationCorrect = CheckSituationData(p_data.Situation, PyObject_GetAttr(p_dataObject, situationVal));

    return tickCorrect && carCorrect && situationCorrect;
}

TEST(PythonDriverTests, PythonDriverGetSDATypeObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        SDAData sdaData = CreateSDAData();
        PyObject* sdaDataObject = pythonDriver.GetSDATypeObject(sdaData);

        ASSERT_TRUE(CheckSDAData(sdaData, sdaDataObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carObject = pythonDriver.GetCarObject(carData);

        ASSERT_TRUE(CheckCarData(carData, carObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarInitObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carInitObject = pythonDriver.GetCarInitObject(carData.info);

        ASSERT_TRUE(CheckInitCarData(carData.info, carInitObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarPublicObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carPublicObject = pythonDriver.GetCarPublicObject(carData.pub);

        ASSERT_TRUE(CheckPublicCarData(carData.pub, carPublicObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarRaceInfoObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carRaceInfoObject = pythonDriver.GetCarRaceInfoObject(carData.race);

        ASSERT_TRUE(CheckCarRaceInfoData(carData.race, carRaceInfoObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarPrivObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carPrivObject = pythonDriver.GetCarPrivObject(carData.priv);

        ASSERT_TRUE(CheckPrivCarData(carData.priv, carPrivObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarCtrlObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carCtrlObject = pythonDriver.GetCarCtrlObject(carData.ctrl);

        ASSERT_TRUE(CheckCarCtrlData(carData.ctrl, carCtrlObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarSetupObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carSetupObject = pythonDriver.GetCarSetupObject(carData.setup);

        ASSERT_TRUE(CheckCarSetupData(carData.setup, carSetupObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetCarPitCmdObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        TestSegments testSegments = GenerateSegments();
        tCarElt carData = GenerateCar(testSegments);

        PyObject* carPitCmdObject = pythonDriver.GetCarPitCmdObject(carData.pitcmd);

        ASSERT_TRUE(CheckCarPitCmdData(carData.pitcmd, carPitCmdObject));
    }

    Py_Finalize();
}

TEST(PythonDriverTests, PythonDriverGetSituationObjectTest)
{
    Py_Initialize();
    PythonDriver pythonDriver = PythonDriver();

    for (int i = 0; i < TEST_COUNT; i++)
    {
        tSituation situationData = GenerateSituation();

        PyObject* situationObject = pythonDriver.GetSituationObject(situationData);

        ASSERT_TRUE(CheckSituationData(situationData, situationObject));
    }

    Py_Finalize();
}