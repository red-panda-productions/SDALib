#pragma once
#include "SDAAction.hpp"
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

///@broef class for converting structs from Python to Cpp and back
class SDATypesConverter
{
public:
    SDATypesConverter();

    SDAAction GetCppSDAAction(PyObject* p_action);

    PyObject* GetPythonSDATypeObject(SDAData& p_data);
    SDAData GetCppSDAData(PyObject* p_data);
    void SetPythonSDATypeObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarObject(tCarElt& p_car);
    tCarElt GetCppCarObject(PyObject* p_car);
    void SetPythonCarObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarInitObject(tInitCar& p_initCar);
    tInitCar GetCppCarInitObject(PyObject* p_initCar);
    void SetPythonCarInitObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonWheelSpecificationObject(tWheelSpec& p_wheelSpec);
    tWheelSpec GetCppWheelSpecificationObject(PyObject* p_wheelSpec);
    void SetPythonWheelSpecificationObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonVisualAttributesObject(tVisualAttributes& p_visualAttributes);
    tVisualAttributes GetCppVisualAttributesObject(PyObject* p_visualAttributes);
    void SetPythonVisualAttributesObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarPublicObject(tPublicCar& p_publicCar);
    tPublicCar GetCppCarPublicObject(PyObject* p_publicCar);
    void SetPythonCarPublicObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonDynamicPointObject(tDynPt& p_dynPt);
    tDynPt GetCppDynamicPointObject(PyObject* p_dynPt);
    void SetPythonDynamicPointObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonTrackLocationObject(tTrkLocPos& p_trackLoc);
    tTrkLocPos GetCppTrackLocationObject(PyObject* p_trackLoc);
    void SetPythonTrackLocationObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonTrackSegmentObject(tTrackSeg& p_trackSeg);
    tTrackSeg GetCppTrackSegmentObject(PyObject* p_trackSeg);
    void SetPythonTrackSegmentObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarRaceInfoObject(tCarRaceInfo& p_carRaceInfo);
    tCarRaceInfo GetCppCarRaceInfoObject(PyObject* p_carRaceInfo);
    void SetPythonCarRaceInfoObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarPrivObject(tPrivCar& p_privCar);
    tPrivCar GetCppCarPrivObject(PyObject* p_privCar);
    void SetPythonCarPrivObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonPosDObject(tPosd& p_posD);
    tPosd GetCppPosDObject(PyObject* p_posD);
    void SetPythonPosDObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCollisionStateObject(tCollisionState& p_collisionState);
    tCollisionState GetCppCollisionStateObject(PyObject* p_collisionState);
    void SetPythonCollisionStateObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonVectorObject(double p_x, double p_y, double p_z);
    void GetCppSgVectorObject(float* p_outputVec, PyObject* p_vec);
    t3Dd GetCppTVectorObject(PyObject* p_vec);
    void SetPythonVectorObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarCtrlObject(tCarCtrl& p_carCtrl);
    tCarCtrl GetCppCarCtrlObject(PyObject* p_carCtrl);
    void SetPythonCarCtrlObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarSetupObject(tCarSetup& p_carSetup);
    tCarSetup GetCppCarSetupObject(PyObject* p_carSetup);
    void SetPythonCarSetupObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarSetupItemObject(tCarSetupItem& p_carSetupItem);
    tCarSetupItem GetCppCarSetupItemObject(PyObject* p_carSetupItem);
    void SetPythonCarSetupItemObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarPitCmdObject(tCarPitCmd& p_carPitCmd);
    tCarPitCmd GetCppCarPitCmdObject(PyObject* p_carPitCmd);
    void SetPythonCarPitCmdObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonSituationObject(tSituation& p_situation);
    tSituation GetCppSituationObject(PyObject* p_situation);
    void SetPythonSituationObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonRaceInfoObject(tRaceAdmInfo& p_raceInfo);
    tRaceAdmInfo GetCppRaceInfoObject(PyObject* p_raceInfo);
    void SetPythonRaceInfoObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonAeroObject(tAero& p_aero);
    tRaceAdmInfo GetCppAeroObject(PyObject* p_aero);
    void SetPythonAeroObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonWingObject(tWing& p_wing);
    tRaceAdmInfo GetCppWingObject(PyObject* p_wing);
    void SetPythonWingObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonDamperDefObject(tDamperDef& p_damperDef);
    tRaceAdmInfo GetCppDamperDefObject(PyObject* p_damperDef);
    void SetPythonDamperDefObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonDamperObject(tDamper& p_damper);
    tRaceAdmInfo GetCppDamperObject(PyObject* p_damper);
    void SetPythonDamperObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonSpringObject(tSpring& p_spring);
    tRaceAdmInfo GetCppSpringObject(PyObject* p_spring);
    void SetPythonSpringObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonSuspensionObject(tSuspension& p_suspension);
    tRaceAdmInfo GetCppSuspensionObject(PyObject* p_suspension);
    void SetPythonSuspensionObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonBrakeObject(tBrake& p_brake);
    tRaceAdmInfo GetCppBrakeObject(PyObject* p_brake);
    void SetPythonBrakeObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonBrakeSystObject(tBrakeSyst& p_brakeSyst);
    tRaceAdmInfo GetCppBrakeSystObject(PyObject* p_brakeSyst);
    void SetPythonBrakeSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonDynAxisSystObject(tDynAxis& p_dynAxis);
    tRaceAdmInfo GetCppDynAxisSystObject(PyObject* p_dynAxis);
    void SetPythonDynAxisSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonDifferentialSystObject(tDifferential& p_differential);
    tRaceAdmInfo GetCppDifferentialSystObject(PyObject* p_differential);
    void SetPythonDifferentialSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonAxleSystObject(tAxle& p_axle);
    tRaceAdmInfo GetCppAxleSystObject(PyObject* p_axle);
    void SetPythonAxleSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonSteerSystObject(tSteer& p_steer);
    tRaceAdmInfo GetCppSteerSystObject(PyObject* p_steer);
    void SetPythonSteerSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonWheelSystObject(tWheel& p_wheel);
    tRaceAdmInfo GetCppWheelSystObject(PyObject* p_wheel);
    void SetPythonWheelSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonTransmissionSystObject(tTransmission& p_transmission);
    tRaceAdmInfo GetCppTransmissionSystObject(PyObject* p_transmission);
    void SetPythonTransmissionSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonEngineSystObject(tEngine& p_engine);
    tRaceAdmInfo GetCppEngineSystObject(PyObject* p_engine);
    void SetPythonEngineSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetPythonCarSystObject(tCar& p_car);
    tRaceAdmInfo GetCppCarSystObject(PyObject* p_car);
    void SetPythonCarSystObject(PyObject* p_target, PyObject* p_data);

    PyObject* GetObjectFromArgs(PyObject* p_classInit, PyObject* p_initArgs[], int p_length);

    // all classes from SDATypes.py
    PyObject *m_sdaTypesClass, *m_carClass, *m_initCarClass, *m_wheelSpecificationsClass, *m_visualAttributesClass,
        *m_publicCarClass, *m_dynamicPointClass, *m_trackLocationClass, *m_trackSegmentClass, *m_carRaceInfoClass,
        *m_privCarClass, *m_posDClass, *m_collisionStateClass, *m_carCtrlClass, *m_carSetupClass, *m_carSetupItemClass,
        *m_carPitCmdClass, *m_situationClass, *m_raceInfoClass, *m_vectorClass;

    void FillCarSetupArray(int p_start, int p_end, PyObject* p_carSetupArray[], tCarSetupItem* p_carSetupItems);

    void ReplaceString(char* p_toFill, const char* p_data, int p_length);
};