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
    SDAData GetSDADataCpp(PyObject* p_data);
    SDAAction GetSDAActionCpp(PyObject* p_action);
    PyObject* GetSDATypeObject(SDAData& p_data);

    PyObject* GetPythonDriverObject();

#ifdef TEST
public:
#else
private:
#endif
    PyObject* GetCarObject(tCarElt& p_car);
    PyObject* GetCarInitObject(tInitCar& p_initCar);
    PyObject* GetWheelSpecificationObject(tWheelSpec& p_wheelSpec);
    PyObject* GetVisualAttributesObject(tVisualAttributes& p_visualAttributes);

    PyObject* GetCarPublicObject(tPublicCar& p_publicCar);
    PyObject* GetDynamicPointObject(tDynPt& p_dynPt);
    PyObject* GetTrackLocationObject(tTrkLocPos& p_trackLoc);
    PyObject* GetTrackSegmentObject(tTrackSeg& p_trackSeg);

    PyObject* GetCarRaceInfoObject(tCarRaceInfo& p_carRaceInfo);
    PyObject* GetCarPrivObject(tPrivCar& p_privCar);
    PyObject* GetPosDObject(tPosd& p_posD);
    PyObject* GetCollisionStateObject(tCollisionState& p_collisionState);
    PyObject* GetVectorObject(double p_x, double p_y, double p_z);

    PyObject* GetCarCtrlObject(tCarCtrl& p_carCtrl);
    PyObject* GetCarSetupObject(tCarSetup& p_carSetup);
    PyObject* GetCarSetupItemObject(tCarSetupItem& p_carSetupItem);

    PyObject* GetCarPitCmdObject(tCarPitCmd& p_carPitCmd);

    PyObject* GetSituationObject(tSituation& p_situation);
    PyObject* GetRaceInfoObject(tRaceAdmInfo& p_raceInfo);

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
