#pragma once
#define SDA_STEER_ACTION
#define SDA_BRAKE_ACTION
#define SDA_GEAR_ACTION
#define SDA_ACCEL_ACTION
#include "SDAAction.hpp"
#include "SDADriver.hpp"
#include "SDAData.hpp"
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <stdlib.h>
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

///@brief class for connecting SDALib to a Python decision maker
template <typename PointerManager>
class PythonDriver : public AIInterface<PointerManager>
{
public:
    PythonDriver();

    void SetPythonDriverFileName(std::string p_fileName);
    std::string GetPythonDriverFileName();
    PyObject* GetPythonDriverObject();

    SDAAction GetCppSDAAction(PyObject* p_action);

    PyObject* GetPythonSDATypeObject(SDAData& p_data);
    SDAData GetCppSDAData(PyObject* p_data);

#ifdef TEST
public:
#else
private:
#endif
    PyObject* GetPythonCarObject(tCarElt& p_car);
    tCarElt GetCppCarObject(PyObject* p_car);

    PyObject* GetPythonCarInitObject(tInitCar& p_initCar);
    tInitCar GetCppCarInitObject(PyObject* p_initCar);

    PyObject* GetPythonWheelSpecificationObject(tWheelSpec& p_wheelSpec);
    tWheelSpec GetCppWheelSpecificationObject(PyObject* p_wheelSpec);

    PyObject* GetPythonVisualAttributesObject(tVisualAttributes& p_visualAttributes);
    tVisualAttributes GetCppVisualAttributesObject(PyObject* p_visualAttributes);

    PyObject* GetPythonCarPublicObject(tPublicCar& p_publicCar);
    tPublicCar GetCppCarPublicObject(PyObject* p_publicCar);

    PyObject* GetPythonDynamicPointObject(tDynPt& p_dynPt);
    tDynPt GetCppDynamicPointObject(PyObject* p_dynPt);

    PyObject* GetPythonTrackLocationObject(tTrkLocPos& p_trackLoc);
    tTrkLocPos GetCppTrackLocationObject(PyObject* p_trackLoc);

    PyObject* GetPythonTrackSegmentObject(tTrackSeg& p_trackSeg);
    tTrackSeg GetCppTrackSegmentObject(PyObject* p_trackSeg);

    PyObject* GetPythonCarRaceInfoObject(tCarRaceInfo& p_carRaceInfo);
    tCarRaceInfo GetCppCarRaceInfoObject(PyObject* p_carRaceInfo);

    PyObject* GetPythonCarPrivObject(tPrivCar& p_privCar);
    tPrivCar GetCppCarPrivObject(PyObject* p_privCar);

    PyObject* GetPythonPosDObject(tPosd& p_posD);
    tPosd GetCppPosDObject(PyObject* p_posD);

    PyObject* GetPythonCollisionStateObject(tCollisionState& p_collisionState);
    tCollisionState GetCppCollisionStateObject(PyObject* p_collisionState);

    PyObject* GetPythonVectorObject(double p_x, double p_y, double p_z);
    sgVec3* GetCppSgVectorObject(PyObject* p_vec);
    t3Dd GetCppTVectorObject(PyObject* p_vec);

    PyObject* GetPythonCarCtrlObject(tCarCtrl& p_carCtrl);
    tCarCtrl GetCppCarCtrlObject(PyObject* p_carCtrl);

    PyObject* GetPythonCarSetupObject(tCarSetup& p_carSetup);
    tCarSetup GetCppCarSetupObject(PyObject* p_carSetup);

    PyObject* GetPythonCarSetupItemObject(tCarSetupItem& p_carSetupItem);
    tCarSetupItem GetCppCarSetupItemObject(PyObject* p_carSetupItem);

    PyObject* GetPythonCarPitCmdObject(tCarPitCmd& p_carPitCmd);
    tCarPitCmd GetCppCarPitCmdObject(PyObject* p_carPitCmd);

    PyObject* GetPythonSituationObject(tSituation& p_situation);
    tSituation GetCppSituationObject(PyObject* p_situation);

    PyObject* GetPythonRaceInfoObject(tRaceAdmInfo& p_raceInfo);
    tRaceAdmInfo GetCppRaceInfoObject(PyObject* p_raceInfo);

    PyObject* GetObjectFromArgs(PyObject* p_classInit, PyObject* p_initArgs[], int p_length);

    // all classes from SDATypes.py
    PyObject *m_sdaTypesClass, *m_carClass, *m_initCarClass, *m_wheelSpecificationsClass, *m_visualAttributesClass,
        *m_publicCarClass, *m_dynamicPointClass, *m_trackLocationClass, *m_trackSegmentClass, *m_carRaceInfoClass,
        *m_privCarClass, *m_posDClass, *m_collisionStateClass, *m_carCtrlClass, *m_carSetupClass, *m_carSetupItemClass,
        *m_carPitCmdClass, *m_situationClass, *m_raceInfoClass, *m_vectorClass;

    PyObject* m_pythonDriver;

    std::string m_pythonDriverFileName = "Driver";

    void FillCarSetupArray(int p_start, int p_end, PyObject* p_carSetupArray[], tCarSetupItem* p_carSetupItems);
#ifdef TEST
public:
#else
protected:
#endif
    void InitAI() override;

    SDAAction UpdateAI(SDAData& p_data) override;
};
