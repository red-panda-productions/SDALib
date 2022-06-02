#include "portability.h"
#include "PythonDriver.h"

#define CREATE_PYTHON_DRIVER_IMPLEMENTATION(p_type)                                                                          \
    template PythonDriver<p_type>::PythonDriver(const char *sdaTypes);                                                       \
    template PyObject *PythonDriver<p_type>::GetSDATypeObject(SDAData &p_data);                                              \
    template PyObject *PythonDriver<p_type>::GetCarObject(tCarElt &p_car);                                                   \
    template PyObject *PythonDriver<p_type>::GetCarInitObject(tInitCar &p_initCar);                                          \
    template PyObject *PythonDriver<p_type>::GetWheelSpecificationObject(tWheelSpec &p_wheelSpec);                           \
    template PyObject *PythonDriver<p_type>::GetVisualAttributesObject(tVisualAttributes &p_visualAttributes);               \
    template PyObject *PythonDriver<p_type>::GetCarPublicObject(tPublicCar &p_publicCar);                                    \
    template PyObject *PythonDriver<p_type>::GetDynamicPointObject(tDynPt &p_dynPt);                                         \
    template PyObject *PythonDriver<p_type>::GetTrackLocationObject(tTrkLocPos &p_trackLoc);                                 \
    template PyObject *PythonDriver<p_type>::GetTrackSegmentObject(tTrackSeg &p_trackSeg);                                   \
    template PyObject *PythonDriver<p_type>::GetCarRaceInfoObject(tCarRaceInfo &p_carRaceInfo);                              \
    template PyObject *PythonDriver<p_type>::GetCarPrivObject(tPrivCar &p_privCar);                                          \
    template PyObject *PythonDriver<p_type>::GetPosDObject(tPosd &p_posD);                                                   \
    template PyObject *PythonDriver<p_type>::GetCollisionStateObject(tCollisionState &p_collisionState);                     \
    template PyObject *PythonDriver<p_type>::GetVectorObject(double p_x, double p_y, double p_z);                            \
    template PyObject *PythonDriver<p_type>::GetCarCtrlObject(tCarCtrl &p_carCtrl);                                          \
    template PyObject *PythonDriver<p_type>::GetCarSetupObject(tCarSetup &p_carSetup);                                       \
    template PyObject *PythonDriver<p_type>::GetCarSetupItemObject(tCarSetupItem &p_carSetupItem);                           \
    template PyObject *PythonDriver<p_type>::GetCarPitCmdObject(tCarPitCmd &p_carPitCmd);                                    \
    template PyObject *PythonDriver<p_type>::GetSituationObject(tSituation &p_situation);                                    \
    template PyObject *PythonDriver<p_type>::GetRaceInfoObject(tRaceAdmInfo &p_raceInfo);                                    \
    template PyObject *PythonDriver<p_type>::GetObjectFromArgs(PyObject *p_classInit, PyObject *p_initArgs[], int p_length); \
    template void PythonDriver<p_type>::FillCarSetupArray(int p_start, int p_end, PyObject *p_carSetupArray[], tCarSetupItem *p_carSetupItems);

/// @brief Constructs PythonDriver and sets up python code
template <typename PointerManager>
PythonDriver<PointerManager>::PythonDriver(const char *sdaTypes)
{
    // set cwd in python
    char buffer[256];
    getcwd(buffer, 256);

    std::cout << buffer << std::endl;
    PyRun_SimpleString("import sys\n");
    std::string s = "sys.path.append(\"" + std::string(buffer) + "\")";
    PyRun_SimpleString(s.c_str());  // the folder where the pythonTest.py is located

    // initialize the SDAType data type
    PyObject *sdaTypesModuleName = PyUnicode_FromString(sdaTypes);
    PyObject *sdaTypesModule = PyImport_Import(sdaTypesModuleName);
    PyObject *sdaTypesDict = PyModule_GetDict(sdaTypesModule);

    std::string py_class_name = "SDATypes";
    m_sdaTypesClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Car";
    m_carClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "InitCar";
    m_initCarClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "WheelSpecifications";
    m_wheelSpecificationsClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "VisualAttributes";
    m_visualAttributesClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "PublicCar";
    m_publicCarClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "DynamicPoint";
    m_dynamicPointClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "TrackLocation";
    m_trackLocationClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "TrackSegment";
    m_trackSegmentClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "CarRaceInfo";
    m_carRaceInfoClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "PrivCar";
    m_privCarClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "PosD";
    m_posDClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "CollisionState";
    m_collisionStateClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "CarCtrl";
    m_carCtrlClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "CarSetup";
    m_carSetupClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "CarSetupItem";
    m_carSetupItemClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "CarPitCmd";
    m_carPitCmdClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Situation";
    m_situationClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "RaceInfo";
    m_raceInfoClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Vector";
    m_vectorClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());
}

/// @brief Initializes the python AI
template <typename PointerManager>
void PythonDriver<PointerManager>::InitAI()
{
    try
    {
        PyObject *driverModuleName = PyUnicode_DecodeFSDefault("Driver");
        PyObject *driverModule = PyImport_Import(driverModuleName);
        PyObject *driverDict = PyModule_GetDict(driverModule);

        std::string py_class_name = "SDADriver";
        PyObject *driverClass = PyDict_GetItemString(driverDict, py_class_name.c_str());

        PyObject *py_arg_tuple = PyTuple_New(0);
        m_pythonDriver = PyObject_CallObject(driverClass, py_arg_tuple);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }
}

/// @brief          Updates the python AI by passing the data to python
/// @param  p_data  The data
/// @return         The result of the AI
template <typename PointerManager>
SDAAction PythonDriver<PointerManager>::UpdateAI(SDAData &p_data)
{
    SDAAction action;
    try
    {
        PyObject *args = GetSDATypeObject(p_data);
        PyObject *result = PyList_AsTuple(
            PyObject_CallMethod(m_pythonDriver, "UpdateAI", NULL, args));  // TODO: maybe change format
        action.Steer = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(result, 0)));
        action.Accel = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(result, 1)));
        action.Brake = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(result, 2)));
        action.Gear = static_cast<int>(PyFloat_AsDouble(PyTuple_GetItem(result, 3)));
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return action;
}

/// @brief gets the python SDATypes object of the current input
/// @param  p_data  The data
/// @return         The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetSDATypeObject(SDAData &p_data)
{
    PyObject *SDATypes;

    try
    {
        const int size = 3;
        PyObject *sdaTypesArgs[size];

        sdaTypesArgs[0] = GetCarObject(p_data.Car);
        sdaTypesArgs[1] = GetSituationObject(p_data.Situation);
        sdaTypesArgs[2] = PyLong_FromUnsignedLong(p_data.TickCount);

        // initialize SDAType
        SDATypes = GetObjectFromArgs(m_sdaTypesClass, sdaTypesArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return SDATypes;
}

/// @brief gets the python car object of the current input
/// @param  p_car   The car data
/// @return         The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarObject(tCarElt &p_car)
{
    PyObject *car;

    try
    {
        const int size = 8;
        PyObject *carArgs[size];

        carArgs[0] = PyLong_FromLong(static_cast<long>(p_car.index));
        carArgs[1] = GetCarInitObject(p_car.info);
        carArgs[2] = GetCarPublicObject(p_car.pub);
        carArgs[3] = GetCarRaceInfoObject(p_car.race);
        carArgs[4] = GetCarPrivObject(p_car.priv);
        carArgs[5] = GetCarCtrlObject(p_car.ctrl);
        carArgs[6] = GetCarSetupObject(p_car.setup);
        carArgs[7] = GetCarPitCmdObject(p_car.pitcmd);

        car = GetObjectFromArgs(m_carClass, carArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return car;
}

/// @brief gets the python init car object of the current input
/// @param  p_initCar The race init data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarInitObject(tInitCar &p_initCar)
{
    PyObject *carInit;

    try
    {
        const int size = 28;
        PyObject *carInitArgs[size];

        carInitArgs[0] = PyUnicode_DecodeFSDefault(p_initCar.name);
        carInitArgs[1] = PyUnicode_DecodeFSDefault(p_initCar.sname);
        carInitArgs[2] = PyUnicode_DecodeFSDefault(p_initCar.codename);
        carInitArgs[3] = PyUnicode_DecodeFSDefault(p_initCar.teamname);
        carInitArgs[4] = PyUnicode_DecodeFSDefault(p_initCar.carName);
        carInitArgs[5] = PyUnicode_DecodeFSDefault(p_initCar.category);
        carInitArgs[6] = PyLong_FromLong(static_cast<long>(p_initCar.raceNumber));
        carInitArgs[7] = PyLong_FromLong(static_cast<long>(p_initCar.startRank));
        carInitArgs[8] = PyLong_FromLong(static_cast<long>(p_initCar.driverType));
        carInitArgs[9] = PyLong_FromLong(static_cast<long>(p_initCar.networkplayer));
        carInitArgs[10] = PyLong_FromLong(static_cast<long>(p_initCar.skillLevel));

        carInitArgs[11] = PyFloat_FromDouble(p_initCar.iconColor[0]);
        carInitArgs[12] = PyFloat_FromDouble(p_initCar.iconColor[1]);
        carInitArgs[13] = PyFloat_FromDouble(p_initCar.iconColor[2]);

        carInitArgs[14] = GetVectorObject(p_initCar.dimension.x, p_initCar.dimension.y, p_initCar.dimension.z);
        carInitArgs[15] = GetVectorObject(p_initCar.drvPos.x, p_initCar.drvPos.y, p_initCar.drvPos.z);
        carInitArgs[16] = GetVectorObject(p_initCar.bonnetPos.x, p_initCar.bonnetPos.y, p_initCar.bonnetPos.z);
        carInitArgs[17] = PyFloat_FromDouble(p_initCar.tank);
        carInitArgs[18] = PyFloat_FromDouble(p_initCar.steerLock);
        carInitArgs[19] = GetVectorObject(p_initCar.statGC.x, p_initCar.statGC.y, p_initCar.statGC.z);

        carInitArgs[20] = GetWheelSpecificationObject(p_initCar.wheel[0]);
        carInitArgs[21] = GetWheelSpecificationObject(p_initCar.wheel[1]);
        carInitArgs[22] = GetWheelSpecificationObject(p_initCar.wheel[2]);
        carInitArgs[23] = GetWheelSpecificationObject(p_initCar.wheel[3]);

        carInitArgs[24] = GetVisualAttributesObject(p_initCar.visualAttr);
        carInitArgs[25] = PyUnicode_DecodeFSDefault(p_initCar.masterModel);
        carInitArgs[26] = PyUnicode_DecodeFSDefault(p_initCar.skinName);
        carInitArgs[27] = PyLong_FromLong(static_cast<long>(p_initCar.skinTargets));

        // initialize SDAType
        carInit = GetObjectFromArgs(m_initCarClass, carInitArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return carInit;
}

/// @brief gets the python wheel specification object of the current input
/// @param  p_wheelSpec The wheel specification data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetWheelSpecificationObject(tWheelSpec &p_wheelSpec)
{
    PyObject *wheelSpec;

    try
    {
        const int size = 5;
        PyObject *wheelSpecArgs[size];

        wheelSpecArgs[0] = PyFloat_FromDouble(p_wheelSpec.rimRadius);
        wheelSpecArgs[1] = PyFloat_FromDouble(p_wheelSpec.tireHeight);
        wheelSpecArgs[2] = PyFloat_FromDouble(p_wheelSpec.tireWidth);
        wheelSpecArgs[3] = PyFloat_FromDouble(p_wheelSpec.brakeDiskRadius);
        wheelSpecArgs[4] = PyFloat_FromDouble(p_wheelSpec.wheelRadius);
        // initialize SDAType
        wheelSpec = GetObjectFromArgs(m_wheelSpecificationsClass, wheelSpecArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return wheelSpec;
}

/// @brief gets the python visual attributes object of the current input
/// @param  p_visualAttributes The visual attribute data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetVisualAttributesObject(tVisualAttributes &p_visualAttributes)
{
    PyObject *visualAttributes;

    try
    {
        const int size = 4;
        PyObject *visualAttributesArgs[size];

        visualAttributesArgs[0] = PyLong_FromLong(static_cast<long>(p_visualAttributes.exhaustNb));
        visualAttributesArgs[1] = GetVectorObject(p_visualAttributes.exhaustPos[0].x,
                                                  p_visualAttributes.exhaustPos[0].y,
                                                  p_visualAttributes.exhaustPos[0].z);
        visualAttributesArgs[2] = GetVectorObject(p_visualAttributes.exhaustPos[1].x,
                                                  p_visualAttributes.exhaustPos[1].y,
                                                  p_visualAttributes.exhaustPos[1].z);
        visualAttributesArgs[3] = PyFloat_FromDouble(p_visualAttributes.exhaustPower);

        visualAttributes = GetObjectFromArgs(m_visualAttributesClass, visualAttributesArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return visualAttributes;
}

/// @brief gets the python public car object of the current input
/// @param  p_publicCar The public car data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarPublicObject(tPublicCar &p_publicCar)
{
    PyObject *publicCar;

    try
    {
        const int size = 21;
        PyObject *publicCarArgs[size];

        publicCarArgs[0] = GetDynamicPointObject(p_publicCar.DynGC);
        publicCarArgs[1] = GetDynamicPointObject(p_publicCar.DynGCg);
        publicCarArgs[2] = PyFloat_FromDouble(p_publicCar.speed);

        publicCarArgs[3] = PyFloat_FromDouble(p_publicCar.posMat[0][0]);
        publicCarArgs[4] = PyFloat_FromDouble(p_publicCar.posMat[0][1]);
        publicCarArgs[5] = PyFloat_FromDouble(p_publicCar.posMat[0][2]);
        publicCarArgs[6] = PyFloat_FromDouble(p_publicCar.posMat[0][3]);
        publicCarArgs[7] = PyFloat_FromDouble(p_publicCar.posMat[1][0]);
        publicCarArgs[8] = PyFloat_FromDouble(p_publicCar.posMat[1][1]);
        publicCarArgs[9] = PyFloat_FromDouble(p_publicCar.posMat[1][2]);
        publicCarArgs[10] = PyFloat_FromDouble(p_publicCar.posMat[1][3]);
        publicCarArgs[11] = PyFloat_FromDouble(p_publicCar.posMat[2][0]);
        publicCarArgs[12] = PyFloat_FromDouble(p_publicCar.posMat[2][1]);
        publicCarArgs[13] = PyFloat_FromDouble(p_publicCar.posMat[2][2]);
        publicCarArgs[14] = PyFloat_FromDouble(p_publicCar.posMat[2][3]);
        publicCarArgs[15] = PyFloat_FromDouble(p_publicCar.posMat[3][0]);
        publicCarArgs[16] = PyFloat_FromDouble(p_publicCar.posMat[3][1]);
        publicCarArgs[17] = PyFloat_FromDouble(p_publicCar.posMat[3][2]);
        publicCarArgs[18] = PyFloat_FromDouble(p_publicCar.posMat[3][3]);

        publicCarArgs[19] = GetTrackLocationObject(p_publicCar.trkPos);
        publicCarArgs[20] = PyLong_FromLong(static_cast<long>(p_publicCar.state));

        publicCar = GetObjectFromArgs(m_publicCarClass, publicCarArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return publicCar;
}

/// @brief gets the python dynamic point object of the current input
/// @param  p_dynPt The dynamic point data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetDynamicPointObject(tDynPt &p_dynPt)
{
    PyObject *dynamicPoint;

    try
    {
        const int size = 3;
        PyObject *dynamicPointArgs[size];

        dynamicPointArgs[0] = GetVectorObject(p_dynPt.pos.x, p_dynPt.pos.y, p_dynPt.pos.z);
        dynamicPointArgs[1] = GetVectorObject(p_dynPt.vel.x, p_dynPt.vel.y, p_dynPt.vel.z);
        dynamicPointArgs[2] = GetVectorObject(p_dynPt.acc.x, p_dynPt.acc.y, p_dynPt.acc.z);

        dynamicPoint = GetObjectFromArgs(m_dynamicPointClass, dynamicPointArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return dynamicPoint;
}

/// @brief gets the python track location object of the current input
/// @param  p_trackLoc The track location data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetTrackLocationObject(tTrkLocPos &p_trackLoc)
{
    PyObject *trackLocation;

    try
    {
        const int size = 6;
        PyObject *trackLocationArgs[size];

        trackLocationArgs[0] = GetTrackSegmentObject(*p_trackLoc.seg);
        trackLocationArgs[1] = PyLong_FromLong(static_cast<long>(p_trackLoc.type));
        trackLocationArgs[2] = PyFloat_FromDouble(p_trackLoc.toStart);
        trackLocationArgs[3] = PyFloat_FromDouble(p_trackLoc.toRight);
        trackLocationArgs[4] = PyFloat_FromDouble(p_trackLoc.toMiddle);
        trackLocationArgs[5] = PyFloat_FromDouble(p_trackLoc.toLeft);

        trackLocation = GetObjectFromArgs(m_trackLocationClass, trackLocationArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return trackLocation;
}

/// @brief gets the python track segment object of the current input
/// @param  p_trackSeg The track segment data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetTrackSegmentObject(tTrackSeg &p_trackSeg)
{
    PyObject *trackSegment;

    try
    {
        const int size = 35;
        // TODO: const int size = 36;
        PyObject *trackSegArgs[size];

        trackSegArgs[0] = PyLong_FromLong(static_cast<long>(p_trackSeg.id));
        trackSegArgs[1] = PyLong_FromLong(static_cast<long>(p_trackSeg.type));
        trackSegArgs[2] = PyLong_FromLong(static_cast<long>(p_trackSeg.type2));
        trackSegArgs[3] = PyLong_FromLong(static_cast<long>(p_trackSeg.style));
        trackSegArgs[4] = PyFloat_FromDouble(p_trackSeg.length);
        trackSegArgs[5] = PyFloat_FromDouble(p_trackSeg.width);
        trackSegArgs[6] = PyFloat_FromDouble(p_trackSeg.startWidth);
        trackSegArgs[7] = PyFloat_FromDouble(p_trackSeg.endWidth);
        trackSegArgs[8] = PyFloat_FromDouble(p_trackSeg.lgfromstart);
        trackSegArgs[9] = PyFloat_FromDouble(p_trackSeg.radius);
        trackSegArgs[10] = PyFloat_FromDouble(p_trackSeg.radiusr);
        trackSegArgs[11] = PyFloat_FromDouble(p_trackSeg.radiusl);
        trackSegArgs[12] = PyFloat_FromDouble(p_trackSeg.arc);
        trackSegArgs[13] = GetVectorObject(p_trackSeg.center.x, p_trackSeg.center.y, p_trackSeg.center.z);

        trackSegArgs[14] = GetVectorObject(p_trackSeg.vertex[0].x, p_trackSeg.vertex[0].y, p_trackSeg.vertex[0].z);
        trackSegArgs[15] = GetVectorObject(p_trackSeg.vertex[1].x, p_trackSeg.vertex[1].y, p_trackSeg.vertex[1].z);
        trackSegArgs[16] = GetVectorObject(p_trackSeg.vertex[2].x, p_trackSeg.vertex[2].y, p_trackSeg.vertex[2].z);
        trackSegArgs[17] = GetVectorObject(p_trackSeg.vertex[3].x, p_trackSeg.vertex[3].y, p_trackSeg.vertex[3].z);

        trackSegArgs[18] = PyFloat_FromDouble(p_trackSeg.angle[0]);
        trackSegArgs[19] = PyFloat_FromDouble(p_trackSeg.angle[1]);
        trackSegArgs[20] = PyFloat_FromDouble(p_trackSeg.angle[2]);
        trackSegArgs[21] = PyFloat_FromDouble(p_trackSeg.angle[3]);
        trackSegArgs[22] = PyFloat_FromDouble(p_trackSeg.angle[4]);
        trackSegArgs[23] = PyFloat_FromDouble(p_trackSeg.angle[5]);
        trackSegArgs[24] = PyFloat_FromDouble(p_trackSeg.angle[6]);

        trackSegArgs[25] = PyFloat_FromDouble(p_trackSeg.sin);
        trackSegArgs[26] = PyFloat_FromDouble(p_trackSeg.cos);
        trackSegArgs[27] = PyFloat_FromDouble(p_trackSeg.Kzl);
        trackSegArgs[28] = PyFloat_FromDouble(p_trackSeg.Kzw);
        trackSegArgs[29] = PyFloat_FromDouble(p_trackSeg.Kyl);
        trackSegArgs[30] = GetVectorObject(p_trackSeg.rgtSideNormal.x, p_trackSeg.rgtSideNormal.y,
                                           p_trackSeg.rgtSideNormal.z);
        trackSegArgs[31] = PyLong_FromLong(static_cast<long>(p_trackSeg.envIndex));
        trackSegArgs[32] = PyFloat_FromDouble(p_trackSeg.height);
        trackSegArgs[33] = PyLong_FromLong(static_cast<long>(p_trackSeg.raceInfo));
        trackSegArgs[34] = PyFloat_FromDouble(p_trackSeg.DoVfactor);
        // TODO: trackSegArgs[35] = PyFloat_FromDouble(p_trackSeg.SpeedLimit);

        trackSegment = GetObjectFromArgs(m_trackSegmentClass, trackSegArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return trackSegment;
}

/// @brief gets the python race car info object of the current input
/// @param  p_carRaceInfo The race car info data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarRaceInfoObject(tCarRaceInfo &p_carRaceInfo)
{
    PyObject *carRaceInfo;

    try
    {
        const int size = 27;
        PyObject *carRaceInfoArgs[size];

        carRaceInfoArgs[0] = PyFloat_FromDouble(p_carRaceInfo.bestLapTime);
        carRaceInfoArgs[1] = PyBool_FromLong(static_cast<long>(p_carRaceInfo.commitBestLapTime));
        carRaceInfoArgs[2] = PyFloat_FromDouble(p_carRaceInfo.deltaBestLapTime);
        carRaceInfoArgs[3] = PyFloat_FromDouble(p_carRaceInfo.curLapTime);
        carRaceInfoArgs[4] = PyFloat_FromDouble(p_carRaceInfo.lastLapTime);
        carRaceInfoArgs[5] = PyFloat_FromDouble(p_carRaceInfo.curTime);
        carRaceInfoArgs[6] = PyFloat_FromDouble(p_carRaceInfo.topSpeed);
        carRaceInfoArgs[7] = PyFloat_FromDouble(p_carRaceInfo.currentMinSpeedForLap);
        carRaceInfoArgs[8] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.laps));
        carRaceInfoArgs[9] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.bestLap));
        carRaceInfoArgs[10] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.nbPitStops));
        carRaceInfoArgs[11] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.remainingLaps));
        carRaceInfoArgs[12] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.pos));
        carRaceInfoArgs[13] = PyFloat_FromDouble(p_carRaceInfo.timeBehindLeader);
        carRaceInfoArgs[14] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.lapsBehindLeader));
        carRaceInfoArgs[15] = PyFloat_FromDouble(p_carRaceInfo.timeBehindPrev);
        carRaceInfoArgs[16] = PyFloat_FromDouble(p_carRaceInfo.timeBeforeNext);
        carRaceInfoArgs[17] = PyFloat_FromDouble(p_carRaceInfo.distRaced);
        carRaceInfoArgs[18] = PyFloat_FromDouble(p_carRaceInfo.distFromStartLine);
        carRaceInfoArgs[19] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.currentSector));
        carRaceInfoArgs[20] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.nbSectors));
        carRaceInfoArgs[21] = PyFloat_FromDouble(p_carRaceInfo.trackLength);
        carRaceInfoArgs[22] = PyFloat_FromDouble(p_carRaceInfo.scheduledEventTime);
        carRaceInfoArgs[23] = PyLong_FromLong(static_cast<long>(p_carRaceInfo.event));
        carRaceInfoArgs[24] = PyFloat_FromDouble(p_carRaceInfo.penaltyTime);
        carRaceInfoArgs[25] = PyFloat_FromDouble(p_carRaceInfo.prevFromStartLine);
        carRaceInfoArgs[26] = PyFloat_FromDouble(p_carRaceInfo.wrongWayTime);

        carRaceInfo = GetObjectFromArgs(m_carRaceInfoClass, carRaceInfoArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return carRaceInfo;
}

/// @brief gets the python posD object of the current input
/// @param  p_posD The posD data
/// @return        The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarPrivObject(tPrivCar &p_privCar)
{
    PyObject *privCar;

    try
    {
        const int size = 54;
        PyObject *privCarArgs[size];

        privCarArgs[0] = PyLong_FromLong(static_cast<long>(p_privCar.driverIndex));
        privCarArgs[1] = PyLong_FromLong(static_cast<long>(p_privCar.moduleIndex));
        privCarArgs[2] = PyUnicode_DecodeFSDefault(p_privCar.modName);

        privCarArgs[3] = GetPosDObject(p_privCar.corner[0]);
        privCarArgs[4] = GetPosDObject(p_privCar.corner[1]);
        privCarArgs[5] = GetPosDObject(p_privCar.corner[2]);
        privCarArgs[6] = GetPosDObject(p_privCar.corner[3]);

        privCarArgs[7] = PyLong_FromLong(static_cast<long>(p_privCar.gear));
        privCarArgs[8] = PyLong_FromLong(static_cast<long>(p_privCar.gearNext));

        privCarArgs[9] = PyFloat_FromDouble(p_privCar.fuel);
        privCarArgs[10] = PyFloat_FromDouble(p_privCar.fuel_consumption_total);
        privCarArgs[11] = PyFloat_FromDouble(p_privCar.fuel_consumption_instant);
        privCarArgs[12] = PyFloat_FromDouble(p_privCar.enginerpm);
        privCarArgs[13] = PyFloat_FromDouble(p_privCar.enginerpmRedLine);
        privCarArgs[14] = PyFloat_FromDouble(p_privCar.enginerpmMax);
        privCarArgs[15] = PyFloat_FromDouble(p_privCar.enginerpmMaxTq);
        privCarArgs[16] = PyFloat_FromDouble(p_privCar.enginerpmMaxPw);
        privCarArgs[17] = PyFloat_FromDouble(p_privCar.engineMaxTq);
        privCarArgs[18] = PyFloat_FromDouble(p_privCar.engineMaxPw);

        privCarArgs[19] = PyFloat_FromDouble(p_privCar.gearRatio[0]);
        privCarArgs[20] = PyFloat_FromDouble(p_privCar.gearRatio[1]);
        privCarArgs[21] = PyFloat_FromDouble(p_privCar.gearRatio[2]);
        privCarArgs[22] = PyFloat_FromDouble(p_privCar.gearRatio[3]);
        privCarArgs[23] = PyFloat_FromDouble(p_privCar.gearRatio[4]);
        privCarArgs[24] = PyFloat_FromDouble(p_privCar.gearRatio[5]);
        privCarArgs[25] = PyFloat_FromDouble(p_privCar.gearRatio[6]);
        privCarArgs[26] = PyFloat_FromDouble(p_privCar.gearRatio[7]);
        privCarArgs[27] = PyFloat_FromDouble(p_privCar.gearRatio[8]);
        privCarArgs[28] = PyFloat_FromDouble(p_privCar.gearRatio[9]);

        privCarArgs[29] = PyLong_FromLong(static_cast<long>(p_privCar.gearNb));
        privCarArgs[30] = PyLong_FromLong(static_cast<long>(p_privCar.gearOffset));

        privCarArgs[31] = PyFloat_FromDouble(p_privCar.skid[0]);
        privCarArgs[32] = PyFloat_FromDouble(p_privCar.skid[1]);
        privCarArgs[33] = PyFloat_FromDouble(p_privCar.skid[2]);
        privCarArgs[34] = PyFloat_FromDouble(p_privCar.skid[3]);

        privCarArgs[35] = PyFloat_FromDouble(p_privCar.reaction[0]);
        privCarArgs[36] = PyFloat_FromDouble(p_privCar.reaction[1]);
        privCarArgs[37] = PyFloat_FromDouble(p_privCar.reaction[2]);
        privCarArgs[38] = PyFloat_FromDouble(p_privCar.reaction[3]);

        privCarArgs[39] = PyLong_FromLong(static_cast<long>(p_privCar.collision));
        privCarArgs[40] = PyLong_FromLong(static_cast<long>(p_privCar.simcollision));
        privCarArgs[41] = PyFloat_FromDouble(p_privCar.smoke);

        privCarArgs[42] = GetVectorObject(p_privCar.normal.x, p_privCar.normal.y, p_privCar.normal.z);
        privCarArgs[43] = GetVectorObject(p_privCar.collpos.x, p_privCar.collpos.y, p_privCar.collpos.z);

        privCarArgs[44] = PyLong_FromLong(static_cast<long>(p_privCar.dammage));
        privCarArgs[45] = PyLong_FromLong(static_cast<long>(p_privCar.debug));

        privCarArgs[46] = GetCollisionStateObject(p_privCar.collision_state);

        privCarArgs[47] = PyFloat_FromDouble(p_privCar.localPressure);
        privCarArgs[48] = PyFloat_FromDouble(p_privCar.driveSkill);
        privCarArgs[49] = PyFloat_FromDouble(p_privCar.steerTqCenter);
        privCarArgs[50] = PyFloat_FromDouble(p_privCar.steerTqAlign);

        privCarArgs[51] = PyLong_FromLong(static_cast<long>(p_privCar.dashboardInstantNb));
        privCarArgs[52] = PyLong_FromLong(static_cast<long>(p_privCar.dashboardRequestNb));
        privCarArgs[53] = PyLong_FromLong(static_cast<long>(p_privCar.dashboardActiveItem));

        privCar = GetObjectFromArgs(m_privCarClass, privCarArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return privCar;
}

/// @brief gets the python posD object of the current input
/// @param  p_posD The posD data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetPosDObject(tPosd &p_posD)
{
    PyObject *posD;

    try
    {
        const int size = 7;
        PyObject *posDArgs[size];

        posDArgs[0] = PyFloat_FromDouble(p_posD.x);
        posDArgs[1] = PyFloat_FromDouble(p_posD.y);
        posDArgs[2] = PyFloat_FromDouble(p_posD.z);
        posDArgs[3] = PyFloat_FromDouble(p_posD.xy);
        posDArgs[4] = PyFloat_FromDouble(p_posD.ax);
        posDArgs[5] = PyFloat_FromDouble(p_posD.ay);
        posDArgs[6] = PyFloat_FromDouble(p_posD.az);

        posD = GetObjectFromArgs(m_posDClass, posDArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return posD;
}

/// @brief gets the collision state object of the current input
/// @param  p_collisionState The collision state data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCollisionStateObject(tCollisionState &p_collisionState)
{
    PyObject *collisionState;

    try
    {
        const int size = 3;
        PyObject *collisionStateArgs[size];

        collisionStateArgs[0] = PyLong_FromLong(static_cast<long>(p_collisionState.collision_count));
        collisionStateArgs[1] = GetVectorObject(p_collisionState.pos[0], p_collisionState.pos[1],
                                                p_collisionState.pos[2]);
        collisionStateArgs[2] = GetVectorObject(p_collisionState.force[0], p_collisionState.force[1],
                                                p_collisionState.force[2]);

        collisionState = GetObjectFromArgs(m_collisionStateClass, collisionStateArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return collisionState;
}

/// @brief gets the python vector object of the current input
/// @param  p_vec The vector data
/// @return             The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetVectorObject(double p_x, double p_y, double p_z)
{
    PyObject *vector;

    try
    {
        const int size = 3;
        PyObject *vectorArgs[size];

        vectorArgs[0] = PyFloat_FromDouble(p_x);
        vectorArgs[1] = PyFloat_FromDouble(p_y);
        vectorArgs[2] = PyFloat_FromDouble(p_z);

        vector = GetObjectFromArgs(m_vectorClass, vectorArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return vector;
}

/// @brief gets the python car control object of the current input
/// @param  p_carCtrl The car control data
/// @return           The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarCtrlObject(tCarCtrl &p_carCtrl)
{
    PyObject *carCtrlItem;

    try
    {
        const int size = 28;
        PyObject *carCtrlArgs[size];

        carCtrlArgs[0] = PyFloat_FromDouble(p_carCtrl.steer);
        carCtrlArgs[1] = PyFloat_FromDouble(p_carCtrl.accelCmd);
        carCtrlArgs[2] = PyFloat_FromDouble(p_carCtrl.brakeCmd);
        carCtrlArgs[3] = PyFloat_FromDouble(p_carCtrl.clutchCmd);
        carCtrlArgs[4] = PyFloat_FromDouble(p_carCtrl.brakeFrontLeftCmd);
        carCtrlArgs[5] = PyFloat_FromDouble(p_carCtrl.brakeFrontRightCmd);
        carCtrlArgs[6] = PyFloat_FromDouble(p_carCtrl.brakeRearLeftCmd);
        carCtrlArgs[7] = PyFloat_FromDouble(p_carCtrl.brakeRearRightCmd);
        carCtrlArgs[8] = PyFloat_FromDouble(p_carCtrl.wingFrontCmd);
        carCtrlArgs[9] = PyFloat_FromDouble(p_carCtrl.wingRearCmd);
        carCtrlArgs[10] = PyFloat_FromDouble(p_carCtrl.reserved1);
        carCtrlArgs[11] = PyFloat_FromDouble(p_carCtrl.reserved2);

        carCtrlArgs[12] = PyLong_FromLong(static_cast<long>(p_carCtrl.gear));
        carCtrlArgs[13] = PyLong_FromLong(static_cast<long>(p_carCtrl.raceCmd));
        carCtrlArgs[14] = PyLong_FromLong(static_cast<long>(p_carCtrl.lightCmd));
        carCtrlArgs[15] = PyLong_FromLong(static_cast<long>(p_carCtrl.ebrakeCmd));
        carCtrlArgs[16] = PyLong_FromLong(static_cast<long>(p_carCtrl.wingControlMode));
        carCtrlArgs[17] = PyLong_FromLong(static_cast<long>(p_carCtrl.singleWheelBrakeMode));
        carCtrlArgs[18] = PyLong_FromLong(static_cast<long>(p_carCtrl.switch3));
        carCtrlArgs[19] = PyLong_FromLong(static_cast<long>(p_carCtrl.telemetryMode));

        carCtrlArgs[20] = PyUnicode_DecodeFSDefault(p_carCtrl.msg[0]);
        carCtrlArgs[21] = PyUnicode_DecodeFSDefault(p_carCtrl.msg[1]);
        carCtrlArgs[22] = PyUnicode_DecodeFSDefault(p_carCtrl.msg[2]);
        carCtrlArgs[23] = PyUnicode_DecodeFSDefault(p_carCtrl.msg[3]);

        carCtrlArgs[24] = PyFloat_FromDouble(static_cast<double>(p_carCtrl.msgColor[0]));
        carCtrlArgs[25] = PyFloat_FromDouble(static_cast<double>(p_carCtrl.msgColor[1]));
        carCtrlArgs[26] = PyFloat_FromDouble(static_cast<double>(p_carCtrl.msgColor[2]));
        carCtrlArgs[27] = PyFloat_FromDouble(static_cast<double>(p_carCtrl.msgColor[3]));

        carCtrlItem = GetObjectFromArgs(m_carCtrlClass, carCtrlArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return carCtrlItem;
}

/// @brief gets the python car setup object of the current input
/// @param  p_carSetup The car setup data
/// @return             The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarSetupObject(tCarSetup &p_carSetup)
{
    PyObject *carSetup;

    try
    {
        const int size = 133;
        PyObject *carSetupArgs[size];

        carSetupArgs[0] = GetCarSetupItemObject(p_carSetup.FRWeightRep);
        carSetupArgs[1] = GetCarSetupItemObject(p_carSetup.FRLWeightRep);
        carSetupArgs[2] = GetCarSetupItemObject(p_carSetup.RRLWeightRep);
        carSetupArgs[3] = GetCarSetupItemObject(p_carSetup.fuel);
        FillCarSetupArray(4, 6, carSetupArgs, p_carSetup.wingAngle);
        carSetupArgs[6] = GetCarSetupItemObject(p_carSetup.revsLimiter);
        FillCarSetupArray(7, 17, carSetupArgs, p_carSetup.gearRatio);

        carSetupArgs[17] = PyLong_FromLong(static_cast<long>(p_carSetup.differentialType[0]));
        carSetupArgs[18] = PyLong_FromLong(static_cast<long>(p_carSetup.differentialType[1]));
        carSetupArgs[19] = PyLong_FromLong(static_cast<long>(p_carSetup.differentialType[2]));

        FillCarSetupArray(20, 23, carSetupArgs, p_carSetup.differentialRatio);
        FillCarSetupArray(23, 26, carSetupArgs, p_carSetup.differentialMinTqBias);
        FillCarSetupArray(26, 29, carSetupArgs, p_carSetup.differentialMaxTqBias);
        FillCarSetupArray(29, 32, carSetupArgs, p_carSetup.differentialViscosity);
        FillCarSetupArray(32, 35, carSetupArgs, p_carSetup.differentialLockingTq);
        FillCarSetupArray(35, 38, carSetupArgs, p_carSetup.differentialMaxSlipBias);
        FillCarSetupArray(38, 41, carSetupArgs, p_carSetup.differentialCoastMaxSlipBias);

        carSetupArgs[41] = GetCarSetupItemObject(p_carSetup.steerLock);
        carSetupArgs[42] = GetCarSetupItemObject(p_carSetup.brakeRepartition);
        carSetupArgs[43] = GetCarSetupItemObject(p_carSetup.brakePressure);

        FillCarSetupArray(44, 48, carSetupArgs, p_carSetup.rideHeight);
        FillCarSetupArray(48, 52, carSetupArgs, p_carSetup.toe);
        FillCarSetupArray(52, 56, carSetupArgs, p_carSetup.camber);
        FillCarSetupArray(56, 60, carSetupArgs, p_carSetup.tirePressure);
        FillCarSetupArray(60, 64, carSetupArgs, p_carSetup.tireOpLoad);

        FillCarSetupArray(64, 66, carSetupArgs, p_carSetup.arbSpring);
        FillCarSetupArray(66, 68, carSetupArgs, p_carSetup.arbBellcrank);
        FillCarSetupArray(68, 70, carSetupArgs, p_carSetup.heaveSpring);
        FillCarSetupArray(70, 72, carSetupArgs, p_carSetup.heaveBellcrank);
        FillCarSetupArray(72, 74, carSetupArgs, p_carSetup.heaveInertance);
        FillCarSetupArray(74, 76, carSetupArgs, p_carSetup.heaveFastBump);
        FillCarSetupArray(76, 78, carSetupArgs, p_carSetup.heaveSlowBump);
        FillCarSetupArray(78, 80, carSetupArgs, p_carSetup.heaveBumpLvel);
        FillCarSetupArray(80, 82, carSetupArgs, p_carSetup.heaveFastRebound);
        FillCarSetupArray(82, 84, carSetupArgs, p_carSetup.heaveSlowRebound);
        FillCarSetupArray(84, 86, carSetupArgs, p_carSetup.heaveReboundLvel);

        FillCarSetupArray(86, 90, carSetupArgs, p_carSetup.suspSpring);
        FillCarSetupArray(90, 94, carSetupArgs, p_carSetup.suspBellcrank);
        FillCarSetupArray(94, 98, carSetupArgs, p_carSetup.suspInertance);
        FillCarSetupArray(98, 102, carSetupArgs, p_carSetup.suspCourse);
        FillCarSetupArray(102, 106, carSetupArgs, p_carSetup.suspPacker);
        FillCarSetupArray(106, 110, carSetupArgs, p_carSetup.suspFastBump);
        FillCarSetupArray(110, 114, carSetupArgs, p_carSetup.suspSlowBump);
        FillCarSetupArray(114, 118, carSetupArgs, p_carSetup.suspBumpLvel);
        FillCarSetupArray(118, 122, carSetupArgs, p_carSetup.suspFastRebound);
        FillCarSetupArray(122, 126, carSetupArgs, p_carSetup.suspSlowRebound);
        FillCarSetupArray(126, 130, carSetupArgs, p_carSetup.suspReboundLvel);

        carSetupArgs[130] = GetCarSetupItemObject(p_carSetup.reqRepair);
        carSetupArgs[131] = GetCarSetupItemObject(p_carSetup.reqTireset);
        carSetupArgs[132] = GetCarSetupItemObject(p_carSetup.reqPenalty);

        carSetup = GetObjectFromArgs(m_carSetupClass, carSetupArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return carSetup;
}

/// @brief fills the input array with items in the p_carSetupItems
/// @param  p_start The start point in the array
/// @param  p_end The end point in the array
/// @param  p_carSetupArray The car setup item python object array
/// @param  p_carSetupItem The car setup item array
template <typename PointerManager>
void PythonDriver<PointerManager>::FillCarSetupArray(int p_start, int p_end, PyObject *p_carSetupArray[],
                                                     tCarSetupItem *p_carSetupItems)
{
    for (int i = 0; i < p_end - p_start; i++)
    {
        p_carSetupArray[p_start + i] = GetCarSetupItemObject(p_carSetupItems[i]);
    }
}

/// @brief gets the python car setup item object of the current input
/// @param  p_carSetupItem The car setup item data
/// @return             The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarSetupItemObject(tCarSetupItem &p_carSetupItem)
{
    PyObject *carSetupItem;

    try
    {
        const int size = 6;
        PyObject *carSetupItemArgs[size];

        carSetupItemArgs[0] = PyFloat_FromDouble(p_carSetupItem.value);
        carSetupItemArgs[1] = PyFloat_FromDouble(p_carSetupItem.min);
        carSetupItemArgs[2] = PyFloat_FromDouble(p_carSetupItem.max);
        carSetupItemArgs[3] = PyFloat_FromDouble(p_carSetupItem.desired_value);
        carSetupItemArgs[4] = PyFloat_FromDouble(p_carSetupItem.stepsize);
        carSetupItemArgs[5] = PyBool_FromLong(static_cast<long>(p_carSetupItem.changed));

        carSetupItem = GetObjectFromArgs(m_carSetupItemClass, carSetupItemArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return carSetupItem;
}

/// @brief gets the python car pit cmd object of the current input
/// @param  p_carPitCmd The car pit cmd data
/// @return             The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetCarPitCmdObject(tCarPitCmd &p_carPitCmd)
{
    PyObject *carPitCmd;

    try
    {
        const int size = 5;
        PyObject *carPitCmdArgs[size];

        carPitCmdArgs[0] = PyFloat_FromDouble(p_carPitCmd.fuel);
        carPitCmdArgs[1] = PyLong_FromLong(static_cast<long>(p_carPitCmd.repair));
        carPitCmdArgs[2] = PyLong_FromLong(static_cast<long>(p_carPitCmd.stopType));
        carPitCmdArgs[3] = PyBool_FromLong(static_cast<long>(p_carPitCmd.setupChanged));
        carPitCmdArgs[4] = PyBool_FromLong(static_cast<long>(p_carPitCmd.tireChange));

        carPitCmd = GetObjectFromArgs(m_carPitCmdClass, carPitCmdArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return carPitCmd;
}

/// @brief gets the python situation object of the current input
/// @param  p_situation The situation data
/// @return             The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetSituationObject(tSituation &p_situation)
{
    PyObject *situation;

    try
    {
        const int size = 5;
        PyObject *situationArgs[size];

        situationArgs[0] = GetRaceInfoObject(p_situation.raceInfo);
        situationArgs[1] = PyFloat_FromDouble(p_situation.deltaTime);
        situationArgs[2] = PyFloat_FromDouble(p_situation.currentTime);
        situationArgs[3] = PyFloat_FromDouble(p_situation.accelTime);
        situationArgs[4] = PyLong_FromLong(static_cast<long>(p_situation.nbPlayers));

        situation = GetObjectFromArgs(m_situationClass, situationArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return situation;
}

/// @brief gets the python race info object of the current input
/// @param  p_raceInfo The race info data
/// @return             The python object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetRaceInfoObject(tRaceAdmInfo &p_raceInfo)
{
    PyObject *raceInfo;

    try
    {
        const int size = 9;
        PyObject *raceInfoArgs[size];

        raceInfoArgs[0] = PyLong_FromLong(static_cast<long>(p_raceInfo.ncars));
        raceInfoArgs[1] = PyLong_FromLong(static_cast<long>(p_raceInfo.totLaps));
        raceInfoArgs[2] = PyLong_FromLong(static_cast<long>(p_raceInfo.extraLaps));
        raceInfoArgs[3] = PyFloat_FromDouble(p_raceInfo.totTime);
        raceInfoArgs[4] = PyLong_FromLong(static_cast<long>(p_raceInfo.state));
        raceInfoArgs[5] = PyLong_FromLong(static_cast<long>(p_raceInfo.type));
        raceInfoArgs[6] = PyLong_FromLong(static_cast<long>(p_raceInfo.maxDammage));
        raceInfoArgs[7] = PyLong_FromLong(static_cast<long>(p_raceInfo.fps));
        raceInfoArgs[8] = PyLong_FromLong(static_cast<long>(p_raceInfo.features));

        raceInfo = GetObjectFromArgs(m_raceInfoClass, raceInfoArgs, size);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return raceInfo;
}

template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetObjectFromArgs(PyObject *p_classInit, PyObject *p_initArgs[], int p_length)
{
    PyObject *objectInit;

    try
    {
        // set arguments
        PyObject *initArgsPy = PyTuple_New(p_length);

        for (Py_ssize_t i = 0; i < p_length; i++)
        {
            PyTuple_SetItem(initArgsPy, i, p_initArgs[i]);
        }

        // initialize SDAType
        objectInit = PyObject_CallObject(p_classInit, initArgsPy);
    }
    catch (std::exception &)
    {
        PyErr_Print();
    }

    return objectInit;
}
