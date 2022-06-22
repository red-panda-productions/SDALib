#include "SDATypesConverter.h"
#include "portability.h"

#define COPY_PYTHON_OBJECT(target, ref, str) \
    PyObject_SetAttrString(target, str, PyObject_GetAttrString(ref, str));

#define COPY_PYTHON_OBJECT_LIST(target, ref, strs, length) \
    for (int i = 0; i < length; i++) COPY_PYTHON_OBJECT(target, ref, strs[i]);

#define COPY_PYTHON_ARRAY(target, ref, str, length) \
    for (int i = 0; i < length; i++) \
        PyList_SET_ITEM(PyObject_GetAttrString(target, str), i, PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(ref, str)), i));

#define COPY_PYTHON_CARSETUP_ARRAY(target, ref, str, length) \
    for (int i = 0; i < length; i++)                         \
         SetPythonCarSetupItemObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(target, str)), i), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(ref, str)), i));

#define SET_PYTHON_OBJECT_FLOAT(objPy, str, obj) \
    objPy.str = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(obj, str)));

SDATypesConverter::SDATypesConverter()
{
    // set cwd in python
    char buffer[256];
    getcwd(buffer, 256);

    std::cout << buffer << std::endl;
    PyRun_SimpleString("import sys\n");
    std::string s = "sys.path.append(\"" + std::string(buffer) + "\")";
    PyRun_SimpleString(s.c_str());  // the folder where the pythonTest.py is located

    // initialize the SDAType data type
    PyObject *sdaTypesModuleName = PyUnicode_FromString("SDATypes");
    PyObject *sdaTypesModule = PyImport_Import(sdaTypesModuleName);

    if (!PyModule_Check(sdaTypesModule)) return;
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

    py_class_name = "Aero";
    m_aeroClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Wing";
    m_wingClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "DemperDef";
    m_damperDefClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Demper";
    m_damperClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Spring";
    m_springClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Suspension";
    m_suspensionClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Brake";
    m_brakeClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "BrakeSyst";
    m_brakeSystClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "DynAxis";
    m_dynAxisClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Differential";
    m_differentialClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Axle";
    m_axleClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Steer";
    m_steerClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());

    py_class_name = "Wheel";
    m_wheelClass = PyDict_GetItemString(sdaTypesDict, py_class_name.c_str());
}

/// @brief          Translates a python object to SDAAction
/// @param  p_action  The action object in Python
/// @return         The action object in C++
SDAAction SDATypesConverter::GetCppSDAAction(PyObject *p_action)
{
    SDAAction action;

    if (p_action == NULL) return action;

    action.Steer = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_action, "steer")));
    action.Accel = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_action, "accel")));
    action.Brake = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_action, "brake")));
    action.Gear = static_cast<int>(PyFloat_AsDouble(PyObject_GetAttrString(p_action, "clutch")));

    return action;
}

/// @brief gets the python SDATypes object of the current input
/// @param  p_data  The data
/// @return         The python object
PyObject *SDATypesConverter::GetPythonSDATypeObject(SDAData &p_data)
{
    const int size = 4;
    PyObject *sdaTypesArgs[size];

    sdaTypesArgs[0] = GetPythonCarObject(p_data.Car);
    sdaTypesArgs[1] = GetPythonSituationObject(p_data.Situation);
    sdaTypesArgs[2] = GetPythonSimCarObject(p_data.SimCar);
    sdaTypesArgs[3] = PyLong_FromUnsignedLong(p_data.TickCount);

    // initialize SDAType
    PyObject *SDATypes = GetObjectFromArgs(m_sdaTypesClass, sdaTypesArgs, size);

    return SDATypes;
}

/// @brief          Translates a python object to SDAData
/// @param  p_action  The data object in Python
/// @return         The data object in C++
SDAData SDATypesConverter::GetCppSDAData(PyObject *p_data)
{
    SDAData data;

    data.Car = GetCppCarObject((PyObject_GetAttrString(p_data, "car")));
    data.Situation = GetCppSituationObject(PyObject_GetAttrString(p_data, "situation"));
    data.SimCar = GetCppSimCarObject(PyObject_GetAttrString(p_data, "simCar"));
    data.TickCount = static_cast<unsigned long>(PyLong_AsLong(PyObject_GetAttrString(p_data, "tickCount")));

    return data;
}

void SDATypesConverter::SetPythonSDATypeObject(PyObject *p_target, PyObject *p_data)
{
    SetPythonCarObject(PyObject_GetAttrString(p_target, "car"), PyObject_GetAttrString(p_data, "car"));
    SetPythonSituationObject(PyObject_GetAttrString(p_target, "situation"), PyObject_GetAttrString(p_data, "situation"));
    SetPythonSimCarObject(PyObject_GetAttrString(p_target, "simCar"), PyObject_GetAttrString(p_data, "simCar"));
    COPY_PYTHON_OBJECT(p_target, p_data, "tickCount");
}

/// @brief gets the python car object of the current input
/// @param  p_car   The car data
/// @return         The python object
PyObject *SDATypesConverter::GetPythonCarObject(tCarElt &p_car)
{
    const int size = 8;
    PyObject *carArgs[size];

    carArgs[0] = PyLong_FromLong(static_cast<long>(p_car.index));
    carArgs[1] = GetPythonCarInitObject(p_car.info);
    carArgs[2] = GetPythonCarPublicObject(p_car.pub);
    carArgs[3] = GetPythonCarRaceInfoObject(p_car.race);
    carArgs[4] = GetPythonCarPrivObject(p_car.priv);
    carArgs[5] = GetPythonCarCtrlObject(p_car.ctrl);
    carArgs[6] = GetPythonCarSetupObject(p_car.setup);
    carArgs[7] = GetPythonCarPitCmdObject(p_car.pitcmd);

    PyObject *car = GetObjectFromArgs(m_carClass, carArgs, size);

    return car;
}

/// @brief          Translates a python object to tCarElt
/// @param  p_car  The car object in Python
/// @return         The car object in C++
tCarElt SDATypesConverter::GetCppCarObject(PyObject *p_car)
{
    tCarElt car;

    car.index = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_car, "index")));
    car.info = GetCppCarInitObject(PyObject_GetAttrString(p_car, "info"));
    car.pub = GetCppCarPublicObject(PyObject_GetAttrString(p_car, "pub"));
    car.race = GetCppCarRaceInfoObject(PyObject_GetAttrString(p_car, "race"));
    car.priv = GetCppCarPrivObject(PyObject_GetAttrString(p_car, "priv"));
    car.ctrl = GetCppCarCtrlObject(PyObject_GetAttrString(p_car, "ctrl"));
    car.setup = GetCppCarSetupObject(PyObject_GetAttrString(p_car, "setup"));
    car.pitcmd = GetCppCarPitCmdObject(PyObject_GetAttrString(p_car, "pitcmd"));

    return car;
}

void SDATypesConverter::SetPythonCarObject(PyObject *p_target, PyObject *p_data)
{
    COPY_PYTHON_OBJECT(p_target, p_data, "index");
    SetPythonCarInitObject(PyObject_GetAttrString(p_target, "info"), PyObject_GetAttrString(p_data, "info"));
    SetPythonCarPublicObject(PyObject_GetAttrString(p_target, "pub"), PyObject_GetAttrString(p_data, "pub"));
    SetPythonCarRaceInfoObject(PyObject_GetAttrString(p_target, "race"), PyObject_GetAttrString(p_data, "race"));
    SetPythonCarPrivObject(PyObject_GetAttrString(p_target, "priv"), PyObject_GetAttrString(p_data, "priv"));
    SetPythonCarCtrlObject(PyObject_GetAttrString(p_target, "ctrl"), PyObject_GetAttrString(p_data, "ctrl"));
    SetPythonCarSetupObject(PyObject_GetAttrString(p_target, "setup"), PyObject_GetAttrString(p_data, "setup"));
    SetPythonCarPitCmdObject(PyObject_GetAttrString(p_target, "pitcmd"), PyObject_GetAttrString(p_data, "pitcmd"));
}

/// @brief gets the Python init car object of the current input
/// @param  p_initCar The race init data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonCarInitObject(tInitCar &p_initCar)
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

    carInitArgs[14] = GetPythonVectorObject(p_initCar.dimension.x, p_initCar.dimension.y, p_initCar.dimension.z);
    carInitArgs[15] = GetPythonVectorObject(p_initCar.drvPos.x, p_initCar.drvPos.y, p_initCar.drvPos.z);
    carInitArgs[16] = GetPythonVectorObject(p_initCar.bonnetPos.x, p_initCar.bonnetPos.y, p_initCar.bonnetPos.z);
    carInitArgs[17] = PyFloat_FromDouble(p_initCar.tank);
    carInitArgs[18] = PyFloat_FromDouble(p_initCar.steerLock);
    carInitArgs[19] = GetPythonVectorObject(p_initCar.statGC.x, p_initCar.statGC.y, p_initCar.statGC.z);

    carInitArgs[20] = GetPythonWheelSpecificationObject(p_initCar.wheel[0]);
    carInitArgs[21] = GetPythonWheelSpecificationObject(p_initCar.wheel[1]);
    carInitArgs[22] = GetPythonWheelSpecificationObject(p_initCar.wheel[2]);
    carInitArgs[23] = GetPythonWheelSpecificationObject(p_initCar.wheel[3]);

    carInitArgs[24] = GetPythonVisualAttributesObject(p_initCar.visualAttr);
    carInitArgs[25] = PyUnicode_DecodeFSDefault(p_initCar.masterModel);
    carInitArgs[26] = PyUnicode_DecodeFSDefault(p_initCar.skinName);
    carInitArgs[27] = PyLong_FromLong(static_cast<long>(p_initCar.skinTargets));

    // initialize SDAType
    PyObject *carInit = GetObjectFromArgs(m_initCarClass, carInitArgs, size);

    return carInit;
}

/// @brief gets the Cpp init car object of the current input
/// @param  p_initCar The race init Python data
/// @return           The Cpp object
tInitCar SDATypesConverter::GetCppCarInitObject(PyObject *p_initCar)
{
    tInitCar initCar;

    ReplaceString(initCar.name, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "name")), MAX_NAME_LEN);
    ReplaceString(initCar.sname, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "sName")), MAX_NAME_LEN);
    ReplaceString(initCar.codename, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "codename")), 4);
    ReplaceString(initCar.teamname, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "teamName")), MAX_NAME_LEN);
    ReplaceString(initCar.carName, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "carName")), MAX_NAME_LEN);
    ReplaceString(initCar.category, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "category")), MAX_NAME_LEN);
    initCar.raceNumber = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_initCar, "raceNumber")));
    initCar.startRank = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_initCar, "startRank")));
    initCar.driverType = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_initCar, "driverType")));
    initCar.networkplayer = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_initCar, "networkPlayer")));
    initCar.skillLevel = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_initCar, "skillLevel")));

    PyObject *iconColorVal = PyList_AsTuple(PyObject_GetAttrString(p_initCar, "iconColor"));
    initCar.iconColor[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 0)));
    initCar.iconColor[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 1)));
    initCar.iconColor[2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(iconColorVal, 2)));
    initCar.dimension = GetCppTVectorObject(PyObject_GetAttrString(p_initCar, "dimension"));
    initCar.drvPos = GetCppTVectorObject(PyObject_GetAttrString(p_initCar, "drvPos"));
    initCar.bonnetPos = GetCppTVectorObject(PyObject_GetAttrString(p_initCar, "bonnetPos"));
    initCar.tank = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_initCar, "tank")));
    initCar.steerLock = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_initCar, "steerLock")));
    initCar.statGC = GetCppTVectorObject(PyObject_GetAttrString(p_initCar, "statGC"));

    PyObject *wheelVal = PyList_AsTuple(PyObject_GetAttrString(p_initCar, "wheel"));
    initCar.wheel[0] = GetCppWheelSpecificationObject(PyTuple_GetItem(wheelVal, 0));
    initCar.wheel[1] = GetCppWheelSpecificationObject(PyTuple_GetItem(wheelVal, 1));
    initCar.wheel[2] = GetCppWheelSpecificationObject(PyTuple_GetItem(wheelVal, 2));
    initCar.wheel[3] = GetCppWheelSpecificationObject(PyTuple_GetItem(wheelVal, 3));
    initCar.visualAttr = GetCppVisualAttributesObject(PyObject_GetAttrString(p_initCar, "visualAttr"));
    ReplaceString(initCar.masterModel, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "masterModel")), MAX_NAME_LEN);
    ReplaceString(initCar.skinName, PyUnicode_AsUTF8(PyObject_GetAttrString(p_initCar, "skinName")), MAX_NAME_LEN);
    initCar.skinTargets = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_initCar, "skinTargets")));

    return initCar;
}

void SDATypesConverter::SetPythonCarInitObject(PyObject *p_target, PyObject *p_data)
{
    const char* attributeList[16]{"name", "sName","codename","teamName","carName","category","raceNumber",
        "startRank", "driverType", "networkPlayer", "skillLevel", "tank", "steerLock", "masterModel", "skinName", "skinTargets"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 16);

    SetPythonVectorObject(PyObject_GetAttrString(p_target, "dimension"), PyObject_GetAttrString(p_data, "dimension"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "drvPos"), PyObject_GetAttrString(p_data, "drvPos"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "bonnetPos"), PyObject_GetAttrString(p_data, "bonnetPos"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "statGC"), PyObject_GetAttrString(p_data, "statGC"));

    SetPythonVisualAttributesObject(PyObject_GetAttrString(p_target, "visualAttr"), PyObject_GetAttrString(p_data, "visualAttr"));

    COPY_PYTHON_ARRAY(p_target, p_data, "iconColor", 4)

    SetPythonWheelSpecificationObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "wheel")), 0), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "wheel")), 0));
    SetPythonWheelSpecificationObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "wheel")), 1), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "wheel")), 1));
    SetPythonWheelSpecificationObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "wheel")), 2), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "wheel")), 2));
    SetPythonWheelSpecificationObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "wheel")), 3), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "wheel")), 3));
}

/// @brief gets the python wheel specification object of the current input
/// @param  p_wheelSpec The wheel specification data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonWheelSpecificationObject(tWheelSpec &p_wheelSpec)
{
    const int size = 5;
    PyObject *wheelSpecArgs[size];

    wheelSpecArgs[0] = PyFloat_FromDouble(p_wheelSpec.rimRadius);
    wheelSpecArgs[1] = PyFloat_FromDouble(p_wheelSpec.tireHeight);
    wheelSpecArgs[2] = PyFloat_FromDouble(p_wheelSpec.tireWidth);
    wheelSpecArgs[3] = PyFloat_FromDouble(p_wheelSpec.brakeDiskRadius);
    wheelSpecArgs[4] = PyFloat_FromDouble(p_wheelSpec.wheelRadius);

    PyObject *wheelSpec = GetObjectFromArgs(m_wheelSpecificationsClass, wheelSpecArgs, size);

    return wheelSpec;
}

/// @brief gets the Cpp wheel specification object from Python
/// @param  p_wheelSpec The Python object
/// @return           The Cpp object
tWheelSpec SDATypesConverter::GetCppWheelSpecificationObject(PyObject *p_wheelSpec)
{
    tWheelSpec wheelSpec;

    wheelSpec.rimRadius = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheelSpec, "rimRadius")));
    wheelSpec.tireHeight = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheelSpec, "tireHeight")));
    wheelSpec.tireWidth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheelSpec, "tireWidth")));
    wheelSpec.brakeDiskRadius = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheelSpec, "brakeDiskRadius")));
    wheelSpec.wheelRadius = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheelSpec, "wheelRadius")));

    return wheelSpec;
}

void SDATypesConverter::SetPythonWheelSpecificationObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[5]{"rimRadius", "tireHeight", "tireWidth", "brakeDiskRadius", "wheelRadius"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 5);
}

/// @brief gets the python visual attributes object of the current input
/// @param  p_visualAttributes The visual attribute data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonVisualAttributesObject(tVisualAttributes &p_visualAttributes)
{
    const int size = 4;
    PyObject *visualAttributesArgs[size];

    visualAttributesArgs[0] = PyLong_FromLong(static_cast<long>(p_visualAttributes.exhaustNb));
    visualAttributesArgs[1] = GetPythonVectorObject(p_visualAttributes.exhaustPos[0].x,
                                                    p_visualAttributes.exhaustPos[0].y,
                                                    p_visualAttributes.exhaustPos[0].z);
    visualAttributesArgs[2] = GetPythonVectorObject(p_visualAttributes.exhaustPos[1].x,
                                                    p_visualAttributes.exhaustPos[1].y,
                                                    p_visualAttributes.exhaustPos[1].z);
    visualAttributesArgs[3] = PyFloat_FromDouble(p_visualAttributes.exhaustPower);

    PyObject *visualAttributes = GetObjectFromArgs(m_visualAttributesClass, visualAttributesArgs, size);

    return visualAttributes;
}

/// @brief gets the Cpp visual attributes object of a Python object
/// @param  p_visualAttributes The Python object
/// @return           The Cpp object
tVisualAttributes SDATypesConverter::GetCppVisualAttributesObject(PyObject *p_visualAttributes)
{
    tVisualAttributes visualAttributes;

    visualAttributes.exhaustNb = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_visualAttributes, "exhaustNb")));

    PyObject *exhaustPos = PyList_AsTuple(PyObject_GetAttrString(p_visualAttributes, "exhaustPos"));
    visualAttributes.exhaustPos[0] = GetCppTVectorObject(PyTuple_GetItem(exhaustPos, 0));
    visualAttributes.exhaustPos[1] = GetCppTVectorObject(PyTuple_GetItem(exhaustPos, 1));
    visualAttributes.exhaustPower = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_visualAttributes, "exhaustPower")));

    return visualAttributes;
}

void SDATypesConverter::SetPythonVisualAttributesObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[2]{"exhaustNb", "exhaustPower"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 2);

    SetPythonVectorObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "exhaustPos")), 0), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "exhaustPos")), 0));
    SetPythonVectorObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "exhaustPos")), 1), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "exhaustPos")), 1));
}


/// @brief gets the python public car object of the current input
/// @param  p_publicCar The public car data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonCarPublicObject(tPublicCar &p_publicCar)
{
    const int size = 21;
    PyObject *publicCarArgs[size];

    publicCarArgs[0] = GetPythonDynamicPointObject(p_publicCar.DynGC);
    publicCarArgs[1] = GetPythonDynamicPointObject(p_publicCar.DynGCg);
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

    publicCarArgs[19] = GetPythonTrackLocationObject(p_publicCar.trkPos);
    publicCarArgs[20] = PyLong_FromLong(static_cast<long>(p_publicCar.state));

    PyObject *publicCar = GetObjectFromArgs(m_publicCarClass, publicCarArgs, size);

    return publicCar;
}

/// @brief gets the Cpp public car object of the Python object
/// @param  p_publicCar The Python object
/// @return           The Cpp object
tPublicCar SDATypesConverter::GetCppCarPublicObject(PyObject *p_publicCar)
{
    tPublicCar publicCar;

    publicCar.DynGC = GetCppDynamicPointObject(PyObject_GetAttrString(p_publicCar, "dynGC"));
    publicCar.DynGCg = GetCppDynamicPointObject(PyObject_GetAttrString(p_publicCar, "dynGCg"));
    publicCar.speed = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_publicCar, "speed")));

    PyObject *p_posMatTuple = PyList_AsTuple(PyObject_GetAttrString(p_publicCar, "posMat"));
    PyObject *row1Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 0));
    PyObject *row2Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 1));
    PyObject *row3Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 2));
    PyObject *row4Val = PyList_AsTuple(PyTuple_GetItem(p_posMatTuple, 3));

    publicCar.posMat[0][0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 0)));
    publicCar.posMat[0][1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 1)));
    publicCar.posMat[0][2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 2)));
    publicCar.posMat[0][3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row1Val, 3)));

    publicCar.posMat[1][0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 0)));
    publicCar.posMat[1][1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 1)));
    publicCar.posMat[1][2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 2)));
    publicCar.posMat[1][3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row2Val, 3)));

    publicCar.posMat[2][0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 0)));
    publicCar.posMat[2][1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 1)));
    publicCar.posMat[2][2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 2)));
    publicCar.posMat[2][3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row3Val, 3)));

    publicCar.posMat[3][0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 0)));
    publicCar.posMat[3][1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 1)));
    publicCar.posMat[3][2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 2)));
    publicCar.posMat[3][3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(row4Val, 3)));

    publicCar.trkPos = GetCppTrackLocationObject(PyObject_GetAttrString(p_publicCar, "trkPos"));
    publicCar.state = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_publicCar, "state")));

    return publicCar;
}

void SDATypesConverter::SetPythonCarPublicObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[2]{"speed", "state"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 2);

    SetPythonDynamicPointObject(PyObject_GetAttrString(p_target, "dynGC"), PyObject_GetAttrString(p_data, "dynGC"));
    SetPythonDynamicPointObject(PyObject_GetAttrString(p_target, "dynGCg"), PyObject_GetAttrString(p_data, "dynGCg"));
    SetPythonTrackLocationObject(PyObject_GetAttrString(p_target, "trkPos"), PyObject_GetAttrString(p_data, "trkPos"));

    //posmat
    PyObject *p_posMatTuple = PyList_AsTuple(PyObject_GetAttrString(p_data, "posMat"));
    PyObject *row1Val = PyTuple_GetItem(p_posMatTuple, 0);
    PyObject *row2Val = PyTuple_GetItem(p_posMatTuple, 1);
    PyObject *row3Val = PyTuple_GetItem(p_posMatTuple, 2);
    PyObject *row4Val = PyTuple_GetItem(p_posMatTuple, 3);

    PyObject* matrix = PyObject_GetAttrString(p_target, "posMat");
    PyList_Insert(matrix, 0, row1Val);
    PyList_Insert(matrix, 1, row2Val);
    PyList_Insert(matrix, 2, row3Val);
    PyList_Insert(matrix, 3, row4Val);
}


/// @brief gets the python dynamic point object of the current input
/// @param  p_dynPt The dynamic point data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonDynamicPointObject(tDynPt &p_dynPt)
{
    const int size = 3;
    PyObject *dynamicPointArgs[size];

    dynamicPointArgs[0] = GetPythonVectorObject(p_dynPt.pos.x, p_dynPt.pos.y, p_dynPt.pos.z);
    dynamicPointArgs[1] = GetPythonVectorObject(p_dynPt.vel.x, p_dynPt.vel.y, p_dynPt.vel.z);
    dynamicPointArgs[2] = GetPythonVectorObject(p_dynPt.acc.x, p_dynPt.acc.y, p_dynPt.acc.z);

    PyObject *dynamicPoint = GetObjectFromArgs(m_dynamicPointClass, dynamicPointArgs, size);

    return dynamicPoint;
}

/// @brief gets the Cpp dynamic point object of the Python object
/// @param  p_dynPt The Python object
/// @return           The Cpp object
tDynPt SDATypesConverter::GetCppDynamicPointObject(PyObject *p_dynPt)
{
    tDynPt dynPt;

    dynPt.pos = GetCppPosDObject(PyObject_GetAttrString(p_dynPt, "pos"));
    dynPt.vel = GetCppPosDObject(PyObject_GetAttrString(p_dynPt, "vel"));
    dynPt.acc = GetCppPosDObject(PyObject_GetAttrString(p_dynPt, "acc"));

    return dynPt;
}

void SDATypesConverter::SetPythonDynamicPointObject(PyObject* p_target, PyObject* p_data)
{
    SetPythonPosDObject(PyObject_GetAttrString(p_target, "pos"), PyObject_GetAttrString(p_data, "pos"));
    SetPythonPosDObject(PyObject_GetAttrString(p_target, "vel"), PyObject_GetAttrString(p_data, "vel"));
    SetPythonPosDObject(PyObject_GetAttrString(p_target, "acc"), PyObject_GetAttrString(p_data, "acc"));
}

/// @brief gets the python track location object of the current input
/// @param  p_trackLoc The track location data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonTrackLocationObject(tTrkLocPos &p_trackLoc)
{
    const int size = 6;
    PyObject *trackLocationArgs[size];

    trackLocationArgs[0] = GetPythonTrackSegmentObject(*p_trackLoc.seg);
    trackLocationArgs[1] = PyLong_FromLong(static_cast<long>(p_trackLoc.type));
    trackLocationArgs[2] = PyFloat_FromDouble(p_trackLoc.toStart);
    trackLocationArgs[3] = PyFloat_FromDouble(p_trackLoc.toRight);
    trackLocationArgs[4] = PyFloat_FromDouble(p_trackLoc.toMiddle);
    trackLocationArgs[5] = PyFloat_FromDouble(p_trackLoc.toLeft);

    PyObject *trackLocation = GetObjectFromArgs(m_trackLocationClass, trackLocationArgs, size);

    return trackLocation;
}

/// @brief gets the Cpp track location object of the Python object
/// @param  p_trackLoc The Python object
/// @return           The Cpp object
tTrkLocPos SDATypesConverter::GetCppTrackLocationObject(PyObject *p_trackLoc)
{
    tTrkLocPos trackLocPos;

    trackLocPos.seg = new tTrackSeg[1];
    trackLocPos.seg[0] = GetCppTrackSegmentObject(PyObject_GetAttrString(p_trackLoc, "seg"));
    trackLocPos.type = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackLoc, "type")));
    trackLocPos.toStart = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackLoc, "toStart")));
    trackLocPos.toRight = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackLoc, "toRight")));
    trackLocPos.toMiddle = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackLoc, "toMiddle")));
    trackLocPos.toLeft = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackLoc, "toLeft")));

    return trackLocPos;
}

void SDATypesConverter::SetPythonTrackLocationObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[5]{"type", "toStart", "toRight", "toMiddle", "toLeft"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 5);

    SetPythonTrackSegmentObject(PyObject_GetAttrString(p_target, "seg"), PyObject_GetAttrString(p_data, "seg"));
}

/// @brief gets the python track segment object of the current input
/// @param  p_trackSeg The track segment data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonTrackSegmentObject(tTrackSeg &p_trackSeg)
{
    const int size = 37;
    PyObject *trackSegArgs[size];

    trackSegArgs[0] = PyLong_FromLong(static_cast<long>(p_trackSeg.id));
    trackSegArgs[1] = PyLong_FromLong(static_cast<long>(p_trackSeg.type));
    trackSegArgs[2] = PyLong_FromLong(static_cast<long>(p_trackSeg.type2));
    trackSegArgs[3] = PyLong_FromLong(static_cast<long>(p_trackSeg.style));
    trackSegArgs[4] = PyFloat_FromDouble(p_trackSeg.length);
    trackSegArgs[5] = PyFloat_FromDouble(p_trackSeg.Time);
    trackSegArgs[6] = PyFloat_FromDouble(p_trackSeg.width);
    trackSegArgs[7] = PyFloat_FromDouble(p_trackSeg.startWidth);
    trackSegArgs[8] = PyFloat_FromDouble(p_trackSeg.endWidth);
    trackSegArgs[9] = PyFloat_FromDouble(p_trackSeg.lgfromstart);
    trackSegArgs[10] = PyFloat_FromDouble(p_trackSeg.radius);
    trackSegArgs[11] = PyFloat_FromDouble(p_trackSeg.radiusr);
    trackSegArgs[12] = PyFloat_FromDouble(p_trackSeg.radiusl);
    trackSegArgs[13] = PyFloat_FromDouble(p_trackSeg.arc);
    trackSegArgs[14] = GetPythonVectorObject(p_trackSeg.center.x, p_trackSeg.center.y, p_trackSeg.center.z);

    trackSegArgs[15] = GetPythonVectorObject(p_trackSeg.vertex[0].x, p_trackSeg.vertex[0].y, p_trackSeg.vertex[0].z);
    trackSegArgs[16] = GetPythonVectorObject(p_trackSeg.vertex[1].x, p_trackSeg.vertex[1].y, p_trackSeg.vertex[1].z);
    trackSegArgs[17] = GetPythonVectorObject(p_trackSeg.vertex[2].x, p_trackSeg.vertex[2].y, p_trackSeg.vertex[2].z);
    trackSegArgs[18] = GetPythonVectorObject(p_trackSeg.vertex[3].x, p_trackSeg.vertex[3].y, p_trackSeg.vertex[3].z);

    trackSegArgs[19] = PyFloat_FromDouble(p_trackSeg.angle[0]);
    trackSegArgs[20] = PyFloat_FromDouble(p_trackSeg.angle[1]);
    trackSegArgs[21] = PyFloat_FromDouble(p_trackSeg.angle[2]);
    trackSegArgs[22] = PyFloat_FromDouble(p_trackSeg.angle[3]);
    trackSegArgs[23] = PyFloat_FromDouble(p_trackSeg.angle[4]);
    trackSegArgs[24] = PyFloat_FromDouble(p_trackSeg.angle[5]);
    trackSegArgs[25] = PyFloat_FromDouble(p_trackSeg.angle[6]);

    trackSegArgs[26] = PyFloat_FromDouble(p_trackSeg.sin);
    trackSegArgs[27] = PyFloat_FromDouble(p_trackSeg.cos);
    trackSegArgs[28] = PyFloat_FromDouble(p_trackSeg.Kzl);
    trackSegArgs[29] = PyFloat_FromDouble(p_trackSeg.Kzw);
    trackSegArgs[30] = PyFloat_FromDouble(p_trackSeg.Kyl);
    trackSegArgs[31] = GetPythonVectorObject(p_trackSeg.rgtSideNormal.x, p_trackSeg.rgtSideNormal.y,
                                             p_trackSeg.rgtSideNormal.z);
    trackSegArgs[32] = PyLong_FromLong(static_cast<long>(p_trackSeg.envIndex));
    trackSegArgs[33] = PyFloat_FromDouble(p_trackSeg.height);
    trackSegArgs[34] = PyLong_FromLong(static_cast<long>(p_trackSeg.raceInfo));
    trackSegArgs[35] = PyFloat_FromDouble(p_trackSeg.DoVfactor);
    trackSegArgs[36] = PyFloat_FromDouble(p_trackSeg.SpeedLimit);

    PyObject *trackSegment = GetObjectFromArgs(m_trackSegmentClass, trackSegArgs, size);

    return trackSegment;
}

/// @brief gets the Cpp track segment object of the Python object
/// @param  p_trackSeg The Python object
/// @return           The Cpp object
tTrackSeg SDATypesConverter::GetCppTrackSegmentObject(PyObject *p_trackSeg)
{
    tTrackSeg segment = {};
    segment.name = nullptr;

    segment.id = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackSeg, "trackId")));
    segment.type = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackSeg, "type")));
    segment.type2 = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackSeg, "type2")));
    segment.style = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackSeg, "style")));
    segment.length = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "length")));
    segment.Time = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "time")));
    segment.width = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "width")));
    segment.startWidth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "startWidth")));
    segment.endWidth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "endWidth")));
    segment.lgfromstart = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "lgFromStart")));
    segment.radius = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "radius")));
    segment.radiusr = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "radiusR")));
    segment.radiusl = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "radiusL")));
    segment.arc = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "arc")));
    segment.center = GetCppTVectorObject(PyObject_GetAttrString(p_trackSeg, "center"));

    PyObject *vertexTuple = PyList_AsTuple(PyObject_GetAttrString(p_trackSeg, "vertex"));
    segment.vertex[0] = GetCppTVectorObject(PyTuple_GetItem(vertexTuple, 0));
    segment.vertex[1] = GetCppTVectorObject(PyTuple_GetItem(vertexTuple, 1));
    segment.vertex[2] = GetCppTVectorObject(PyTuple_GetItem(vertexTuple, 2));
    segment.vertex[3] = GetCppTVectorObject(PyTuple_GetItem(vertexTuple, 3));

    PyObject *angleTuple = PyList_AsTuple(PyObject_GetAttrString(p_trackSeg, "angle"));
    segment.angle[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 0)));
    segment.angle[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 1)));
    segment.angle[2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 2)));
    segment.angle[3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 3)));
    segment.angle[4] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 4)));
    segment.angle[5] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 5)));
    segment.angle[6] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(angleTuple, 6)));

    segment.sin = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "sin")));
    segment.cos = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "cos")));
    segment.Kzl = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "kzl")));
    segment.Kzw = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "kzw")));
    segment.Kyl = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "kyl")));
    segment.rgtSideNormal = GetCppTVectorObject(PyObject_GetAttrString(p_trackSeg, "rgtSideNormal"));
    segment.envIndex = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackSeg, "envIndex")));
    segment.height = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "height")));
    segment.raceInfo = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_trackSeg, "raceInfo")));
    segment.DoVfactor = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "doVFactor")));
    segment.SpeedLimit = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_trackSeg, "speedLimit")));

    return segment;
}

void SDATypesConverter::SetPythonTrackSegmentObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[24]{"id", "type", "type2", "style", "length", "time", "width", "startWidth", "endWidth", "lgFromStart", "radius", "radiusR",
                                 "radiusL", "arc",  "sin", "cos", "kzl", "kzw", "kyl", "envIndex", "height",
                                 "raceInfo", "doVFactor", "speedLimit"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 24);

    SetPythonVectorObject(PyObject_GetAttrString(p_target, "center"), PyObject_GetAttrString(p_data, "center"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "rgtSideNormal"), PyObject_GetAttrString(p_data, "rgtSideNormal"));

    COPY_PYTHON_ARRAY(p_target, p_data, "vertex", 4);
    COPY_PYTHON_ARRAY(p_target, p_data, "angle", 7);
}

/// @brief gets the python race car info object of the current input
/// @param  p_carRaceInfo The race car info data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonCarRaceInfoObject(tCarRaceInfo &p_carRaceInfo)
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

    PyObject *carRaceInfo = GetObjectFromArgs(m_carRaceInfoClass, carRaceInfoArgs, size);

    return carRaceInfo;
}

/// @brief gets the Cpp race car info object of the Python object
/// @param  p_carRaceInfo The Python object
/// @return           The Cpp object
tCarRaceInfo SDATypesConverter::GetCppCarRaceInfoObject(PyObject *p_carRaceInfo)
{
    tCarRaceInfo carRaceInfo;

    carRaceInfo.bestLapTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "bestLapTime")));
    carRaceInfo.commitBestLapTime = static_cast<bool>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "commitBestLapTime")));
    carRaceInfo.deltaBestLapTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "deltaBestLapTime")));
    carRaceInfo.curLapTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "curLapTime")));
    carRaceInfo.lastLapTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "lastLapTime")));
    carRaceInfo.curTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "curTime")));
    carRaceInfo.topSpeed = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "topSpeed")));
    carRaceInfo.currentMinSpeedForLap = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "currentMinSpeedForLap")));
    carRaceInfo.laps = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "laps")));
    carRaceInfo.bestLap = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "bestLap")));
    carRaceInfo.nbPitStops = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "nbPitStops")));
    carRaceInfo.remainingLaps = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "remainingLaps")));
    carRaceInfo.pos = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "pos")));
    carRaceInfo.timeBehindLeader = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "timeBehindLeader")));
    carRaceInfo.lapsBehindLeader = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "lapsBehindLeader")));
    carRaceInfo.timeBehindPrev = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "timeBehindPrev")));
    carRaceInfo.timeBeforeNext = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "timeBeforeNext")));
    carRaceInfo.distRaced = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "distRaced")));
    carRaceInfo.distFromStartLine = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "distFromStartLine")));
    carRaceInfo.currentSector = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "currentSector")));
    carRaceInfo.nbSectors = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "nbSectors")));
    carRaceInfo.trackLength = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "trackLength")));
    carRaceInfo.scheduledEventTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "scheduledEventTime")));
    carRaceInfo.event = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carRaceInfo, "event")));
    carRaceInfo.penaltyTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "penaltyTime")));
    carRaceInfo.prevFromStartLine = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "prevFromStartLine")));
    carRaceInfo.wrongWayTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carRaceInfo, "wrongWayTime")));

    return carRaceInfo;
}

void SDATypesConverter::SetPythonCarRaceInfoObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[27]{"bestLapTime", "commitBestLapTime", "deltaBestLapTime", "curLapTime",
                                  "lastLapTime", "curTime", "topSpeed", "currentMinSpeedForLap", "laps", "bestLap", "nbPitStops", "remainingLaps", "pos",
                                  "timeBehindLeader", "lapsBehindLeader", "timeBehindPrev", "timeBeforeNext", "distRaced", "distFromStartLine",
                                  "currentSector", "nbSectors", "trackLength", "scheduledEventTime", "event", "penaltyTime",
                                  "prevFromStartLine", "wrongWayTime"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 27);
}


/// @brief gets the python private car object of the current input
/// @param  p_posD The private car data
/// @return        The python object
PyObject *SDATypesConverter::GetPythonCarPrivObject(tPrivCar &p_privCar)
{
    const int size = 54;
    PyObject *privCarArgs[size];

    privCarArgs[0] = PyLong_FromLong(static_cast<long>(p_privCar.driverIndex));
    privCarArgs[1] = PyLong_FromLong(static_cast<long>(p_privCar.moduleIndex));
    privCarArgs[2] = PyUnicode_DecodeFSDefault(p_privCar.modName);

    privCarArgs[3] = GetPythonPosDObject(p_privCar.corner[0]);
    privCarArgs[4] = GetPythonPosDObject(p_privCar.corner[1]);
    privCarArgs[5] = GetPythonPosDObject(p_privCar.corner[2]);
    privCarArgs[6] = GetPythonPosDObject(p_privCar.corner[3]);

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

    privCarArgs[42] = GetPythonVectorObject(p_privCar.normal.x, p_privCar.normal.y, p_privCar.normal.z);
    privCarArgs[43] = GetPythonVectorObject(p_privCar.collpos.x, p_privCar.collpos.y, p_privCar.collpos.z);

    privCarArgs[44] = PyLong_FromLong(static_cast<long>(p_privCar.dammage));
    privCarArgs[45] = PyLong_FromLong(static_cast<long>(p_privCar.debug));

    privCarArgs[46] = GetPythonCollisionStateObject(p_privCar.collision_state);

    privCarArgs[47] = PyFloat_FromDouble(p_privCar.localPressure);
    privCarArgs[48] = PyFloat_FromDouble(p_privCar.driveSkill);
    privCarArgs[49] = PyFloat_FromDouble(p_privCar.steerTqCenter);
    privCarArgs[50] = PyFloat_FromDouble(p_privCar.steerTqAlign);

    privCarArgs[51] = PyLong_FromLong(static_cast<long>(p_privCar.dashboardInstantNb));
    privCarArgs[52] = PyLong_FromLong(static_cast<long>(p_privCar.dashboardRequestNb));
    privCarArgs[53] = PyLong_FromLong(static_cast<long>(p_privCar.dashboardActiveItem));

    PyObject *privCar = GetObjectFromArgs(m_privCarClass, privCarArgs, size);

    return privCar;
}

/// @brief gets the Cpp private car object of the Python object
/// @param  p_posD The Python object
/// @return        The Cpp object
tPrivCar SDATypesConverter::GetCppCarPrivObject(PyObject *p_privCar)
{
    tPrivCar privCar;

    privCar.driverIndex = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "driverIndex")));
    privCar.moduleIndex = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "moduleIndex")));
    ReplaceString(privCar.modName, PyUnicode_AsUTF8(PyObject_GetAttrString(p_privCar, "modName")), 32);

    PyObject *cornerTuple = PyList_AsTuple(PyObject_GetAttrString(p_privCar, "corner"));
    privCar.corner[0] = GetCppPosDObject(PyTuple_GetItem(cornerTuple, 0));
    privCar.corner[1] = GetCppPosDObject(PyTuple_GetItem(cornerTuple, 1));
    privCar.corner[2] = GetCppPosDObject(PyTuple_GetItem(cornerTuple, 2));
    privCar.corner[3] = GetCppPosDObject(PyTuple_GetItem(cornerTuple, 3));

    privCar.gear = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "gear")));
    privCar.gearNext = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "gearNext")));
    privCar.fuel = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "fuel")));
    privCar.fuel_consumption_total = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "fuelConsumptionTotal")));
    privCar.fuel_consumption_instant = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "fuelConsumptionInstant")));
    privCar.enginerpm = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineRPM")));
    privCar.enginerpmRedLine = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineRPMRedLine")));
    privCar.enginerpmMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineRPMMax")));
    privCar.enginerpmMaxTq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineRPMMaxTq")));
    privCar.enginerpmMaxPw = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineRPMMaxPw")));
    privCar.engineMaxTq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineMaxTq")));
    privCar.engineMaxPw = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "engineMaxPw")));

    PyObject *gearTuple = PyList_AsTuple(PyObject_GetAttrString(p_privCar, "gearRatio"));
    privCar.gearRatio[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 0)));
    privCar.gearRatio[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 1)));
    privCar.gearRatio[2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 2)));
    privCar.gearRatio[3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 3)));
    privCar.gearRatio[4] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 4)));
    privCar.gearRatio[5] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 5)));
    privCar.gearRatio[6] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 6)));
    privCar.gearRatio[7] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 7)));
    privCar.gearRatio[8] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 8)));
    privCar.gearRatio[9] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(gearTuple, 9)));

    privCar.gearNb = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "gearNb")));
    privCar.gearOffset = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "gearOffset")));

    PyObject *skidTuple = PyList_AsTuple(PyObject_GetAttrString(p_privCar, "skid"));
    privCar.skid[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidTuple, 0)));
    privCar.skid[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidTuple, 1)));
    privCar.skid[2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidTuple, 2)));
    privCar.skid[3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(skidTuple, 3)));

    PyObject *reactionTuple = PyList_AsTuple(PyObject_GetAttrString(p_privCar, "reaction"));
    privCar.reaction[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionTuple, 0)));
    privCar.reaction[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionTuple, 1)));
    privCar.reaction[2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionTuple, 2)));
    privCar.reaction[3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(reactionTuple, 3)));

    privCar.collision = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "collision")));
    privCar.simcollision = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "simCollision")));
    privCar.smoke = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "smoke")));
    privCar.normal = GetCppTVectorObject(PyObject_GetAttrString(p_privCar, "normal"));
    privCar.collpos = GetCppTVectorObject(PyObject_GetAttrString(p_privCar, "collPos"));
    privCar.dammage = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "damage")));
    privCar.debug = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "debug")));
    privCar.collision_state = GetCppCollisionStateObject(PyObject_GetAttrString(p_privCar, "collisionState"));
    privCar.localPressure = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "localPressure")));
    privCar.driveSkill = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "driveSkill")));
    privCar.steerTqCenter = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "steerTqCenter")));
    privCar.steerTqAlign = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_privCar, "steerTqAlign")));
    privCar.dashboardInstantNb = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "dashboardInstantNb")));
    privCar.dashboardRequestNb = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "dashboardRequestNb")));
    privCar.dashboardActiveItem = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_privCar, "dashboardActiveItem")));

    return privCar;
}

void SDATypesConverter::SetPythonCarPrivObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[29]{"driverIndex", "moduleIndex", "modName",  "gear", "gearNext", "fuel",
                                  "fuelConsumptionTotal", "fuelConsumptionInstant", "engineRPM", "engineRPMRedLine", "engineRPMMax",
                                  "engineRPMMaxTq", "engineRPMMaxPw", "engineMaxTq", "engineMaxPw",  "gearNb",
                                  "gearOffset","collision",
                                  "simCollision", "smoke", "damage", "debug", "localPressure", "driveSkill",
                                  "steerTqCenter", "steerTqAlign", "dashboardInstantNb", "dashboardRequestNb", "dashboardActiveItem"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 29);

    COPY_PYTHON_ARRAY(p_target, p_data, "gearRatio", 10);
    COPY_PYTHON_ARRAY(p_target, p_data, "skid", 4);
    COPY_PYTHON_ARRAY(p_target, p_data, "reaction", 4);

    SetPythonCollisionStateObject(PyObject_GetAttrString(p_target, "collisionState"), PyObject_GetAttrString(p_data, "collisionState"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "normal"), PyObject_GetAttrString(p_data, "normal"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "collPos"), PyObject_GetAttrString(p_data, "collPos"));

    SetPythonPosDObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "corner")), 0), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "corner")), 0));
    SetPythonPosDObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "corner")), 1), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "corner")), 1));
    SetPythonPosDObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "corner")), 2), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "corner")), 2));
    SetPythonPosDObject(PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_target, "corner")), 3), PyTuple_GetItem(PyList_AsTuple(PyObject_GetAttrString(p_data, "corner")), 3));
}


/// @brief gets the python posD object of the current input
/// @param  p_posD The posD data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonPosDObject(tPosd &p_posD)
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

    PyObject *posD = GetObjectFromArgs(m_posDClass, posDArgs, size);

    return posD;
}

/// @brief gets the Cpp posD object of the Python input
/// @param  p_posD The Python object
/// @return           The Cpp object
tPosd SDATypesConverter::GetCppPosDObject(PyObject *p_posD)
{
    tPosd posD;

    posD.x = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "x")));
    posD.y = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "y")));
    posD.z = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "z")));
    posD.xy = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "xy")));
    posD.ax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "ax")));
    posD.ay = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "ay")));
    posD.az = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_posD, "az")));

    return posD;
}

void SDATypesConverter::SetPythonPosDObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[7]{"x", "y", "z", "xy", "ax", "ay", "az"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 7);
}


/// @brief gets the collision state object of the current input
/// @param  p_collisionState The collision state data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonCollisionStateObject(tCollisionState &p_collisionState)
{
    const int size = 3;
    PyObject *collisionStateArgs[size];

    collisionStateArgs[0] = PyLong_FromLong(static_cast<long>(p_collisionState.collision_count));
    collisionStateArgs[1] = GetPythonVectorObject(p_collisionState.pos[0], p_collisionState.pos[1],
                                                  p_collisionState.pos[2]);
    collisionStateArgs[2] = GetPythonVectorObject(p_collisionState.force[0], p_collisionState.force[1],
                                                  p_collisionState.force[2]);

    PyObject *collisionState = GetObjectFromArgs(m_collisionStateClass, collisionStateArgs, size);

    return collisionState;
}

/// @brief gets the Cpp collision state object of the Python object
/// @param  p_collisionState The Python object
/// @return           The Cpp object
tCollisionState SDATypesConverter::GetCppCollisionStateObject(PyObject *p_collisionState)
{
    tCollisionState collisionState;

    collisionState.collision_count = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_collisionState, "collisionCount")));
    GetCppSgVectorObject(collisionState.pos, PyObject_GetAttrString(p_collisionState, "pos"));
    GetCppSgVectorObject(collisionState.force, PyObject_GetAttrString(p_collisionState, "force"));

    return collisionState;
}

void SDATypesConverter::SetPythonCollisionStateObject(PyObject* p_target, PyObject* p_data)
{
    COPY_PYTHON_OBJECT(p_target, p_data, "collisionCount");
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "pos"), PyObject_GetAttrString(p_data, "pos"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "force"), PyObject_GetAttrString(p_data, "force"));
}


/// @brief gets the python vector object of the current input
/// @param  p_x The vector x data
/// @param  p_y The vector y data
/// @param  p_z The vector z data
/// @return             The python object
PyObject *SDATypesConverter::GetPythonVectorObject(double p_x, double p_y, double p_z)
{
    const int size = 3;
    PyObject *vectorArgs[size];

    vectorArgs[0] = PyFloat_FromDouble(p_x);
    vectorArgs[1] = PyFloat_FromDouble(p_y);
    vectorArgs[2] = PyFloat_FromDouble(p_z);

    PyObject *vector = GetObjectFromArgs(m_vectorClass, vectorArgs, size);

    return vector;
}

/// @brief gets the Cpp sgVec3 vector object of the Python object
/// @param  p_vec The Python object
/// @return       The Cpp object
void SDATypesConverter::GetCppSgVectorObject(float *p_outputVec, PyObject *p_vec)
{
    p_outputVec[0] = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_vec, "x")));
    p_outputVec[1] = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_vec, "y")));
    p_outputVec[2] = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_vec, "z")));
}

/// @brief gets the Cpp t3Dd vector object of the Python object
/// @param  p_vec The Python object
/// @return       The Cpp object
t3Dd SDATypesConverter::GetCppTVectorObject(PyObject *p_vec)
{
    t3Dd vec;

    vec.x = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_vec, "x")));
    vec.y = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_vec, "y")));
    vec.z = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_vec, "z")));

    return vec;
}

void SDATypesConverter::SetPythonVectorObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[3]{"x", "y", "z"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 3);
}

/// @brief gets the python car control object of the current input
/// @param  p_carCtrl The car control data
/// @return           The python object
PyObject *SDATypesConverter::GetPythonCarCtrlObject(tCarCtrl &p_carCtrl)
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

    PyObject *carCtrlItem = GetObjectFromArgs(m_carCtrlClass, carCtrlArgs, size);

    return carCtrlItem;
}

/// @brief gets the Cpp car control object of the Python object
/// @param  p_carCtrl The Python object
/// @return           The Cpp object
tCarCtrl SDATypesConverter::GetCppCarCtrlObject(PyObject *p_carCtrl)
{
    tCarCtrl carCtrl;

    carCtrl.steer = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "steer")));
    carCtrl.accelCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "accelCmd")));
    carCtrl.brakeCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "brakeCmd")));
    carCtrl.clutchCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "clutchCmd")));
    carCtrl.brakeFrontLeftCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "brakeFrontLeftCmd")));
    carCtrl.brakeFrontRightCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "brakeFrontRightCmd")));
    carCtrl.brakeRearLeftCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "brakeRearLeftCmd")));
    carCtrl.brakeRearRightCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "brakeRearRightCmd")));
    carCtrl.wingFrontCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "wingFrontCmd")));
    carCtrl.wingRearCmd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "wingRearCmd")));
    carCtrl.reserved1 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "reserved1")));
    carCtrl.reserved2 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carCtrl, "reserved2")));
    carCtrl.gear = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "gear")));
    carCtrl.raceCmd = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "raceCmd")));
    carCtrl.lightCmd = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "lightCmd")));
    carCtrl.ebrakeCmd = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "eBrakeCmd")));
    carCtrl.wingControlMode = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "wingControlMode")));
    carCtrl.singleWheelBrakeMode = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "singleWheelBrakeMode")));
    carCtrl.switch3 = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "switch3")));
    carCtrl.telemetryMode = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carCtrl, "telemetryMode")));

    PyObject *msgTuple = PyList_AsTuple(PyObject_GetAttrString(p_carCtrl, "msg"));
    ReplaceString(carCtrl.msg[0], PyUnicode_AsUTF8(PyTuple_GetItem(msgTuple, 0)), 32);
    ReplaceString(carCtrl.msg[1], PyUnicode_AsUTF8(PyTuple_GetItem(msgTuple, 1)), 32);
    ReplaceString(carCtrl.msg[2], PyUnicode_AsUTF8(PyTuple_GetItem(msgTuple, 2)), 32);
    ReplaceString(carCtrl.msg[3], PyUnicode_AsUTF8(PyTuple_GetItem(msgTuple, 3)), 32);

    PyObject *msgColorTuple = PyList_AsTuple(PyObject_GetAttrString(p_carCtrl, "msgColor"));
    carCtrl.msgColor[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorTuple, 0)));
    carCtrl.msgColor[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorTuple, 1)));
    carCtrl.msgColor[2] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorTuple, 2)));
    carCtrl.msgColor[3] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(msgColorTuple, 3)));

    return carCtrl;
}

void SDATypesConverter::SetPythonCarCtrlObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[20]{"steer", "accelCmd", "brakeCmd", "clutchCmd", "brakeFrontLeftCmd", "brakeFrontRightCmd", "brakeRearLeftCmd",
                                 "brakeRearRightCmd", "wingFrontCmd", "wingRearCmd", "reserved1", "reserved2", "gear", "raceCmd", "lightCmd", "eBrakeCmd",
                                 "wingControlMode", "singleWheelBrakeMode", "switch3", "telemetryMode"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 20);

    COPY_PYTHON_ARRAY(p_target, p_data, "msg", 4);
    COPY_PYTHON_ARRAY(p_target, p_data, "msgColor", 4);
}


/// @brief gets the python car setup object of the current input
/// @param  p_carSetup The car setup data
/// @return             The python object
PyObject *SDATypesConverter::GetPythonCarSetupObject(tCarSetup &p_carSetup)
{
    const int size = 133;
    PyObject *carSetupArgs[size];

    carSetupArgs[0] = GetPythonCarSetupItemObject(p_carSetup.FRWeightRep);
    carSetupArgs[1] = GetPythonCarSetupItemObject(p_carSetup.FRLWeightRep);
    carSetupArgs[2] = GetPythonCarSetupItemObject(p_carSetup.RRLWeightRep);
    carSetupArgs[3] = GetPythonCarSetupItemObject(p_carSetup.fuel);
    FillCarSetupArray(4, 6, carSetupArgs, p_carSetup.wingAngle);
    carSetupArgs[6] = GetPythonCarSetupItemObject(p_carSetup.revsLimiter);
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

    carSetupArgs[41] = GetPythonCarSetupItemObject(p_carSetup.steerLock);
    carSetupArgs[42] = GetPythonCarSetupItemObject(p_carSetup.brakeRepartition);
    carSetupArgs[43] = GetPythonCarSetupItemObject(p_carSetup.brakePressure);

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

    carSetupArgs[130] = GetPythonCarSetupItemObject(p_carSetup.reqRepair);
    carSetupArgs[131] = GetPythonCarSetupItemObject(p_carSetup.reqTireset);
    carSetupArgs[132] = GetPythonCarSetupItemObject(p_carSetup.reqPenalty);

    PyObject *carSetup = GetObjectFromArgs(m_carSetupClass, carSetupArgs, size);

    return carSetup;
}

/// @brief fills the input array with items in the p_carSetupItems
/// @param  p_start The start point in the array
/// @param  p_end The end point in the array
/// @param  p_carSetupArray The car setup item python object array
/// @param  p_carSetupItem The car setup item array
void SDATypesConverter::FillCarSetupArray(int p_start, int p_end, PyObject *p_carSetupArray[],
                                          tCarSetupItem *p_carSetupItems)
{
    for (int i = 0; i < p_end - p_start; i++)
    {
        p_carSetupArray[p_start + i] = GetPythonCarSetupItemObject(p_carSetupItems[i]);
    }
}

/// @brief gets the Cpp car setup object of the Python object
/// @param  p_carSetup The Python object
/// @return             The Cpp object
tCarSetup SDATypesConverter::GetCppCarSetupObject(PyObject *p_carSetup)
{
    tCarSetup carSetup;

    carSetup.FRWeightRep = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "fRWeightRep"));
    carSetup.FRLWeightRep = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "fRLWeightRep"));
    carSetup.RRLWeightRep = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "rRLWeightRep"));
    carSetup.fuel = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "fuel"));

    PyObject *wingAngleTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "wingAngle"));
    carSetup.wingAngle[0] = GetCppCarSetupItemObject(PyTuple_GetItem(wingAngleTuple, 0));
    carSetup.wingAngle[1] = GetCppCarSetupItemObject(PyTuple_GetItem(wingAngleTuple, 1));

    carSetup.revsLimiter = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "revsLimiter"));

    PyObject *gearRatioTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "gearRatio"));
    carSetup.gearRatio[0] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 0));
    carSetup.gearRatio[1] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 1));
    carSetup.gearRatio[2] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 2));
    carSetup.gearRatio[3] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 3));
    carSetup.gearRatio[4] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 4));
    carSetup.gearRatio[5] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 5));
    carSetup.gearRatio[6] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 6));
    carSetup.gearRatio[7] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 7));
    carSetup.gearRatio[8] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 8));
    carSetup.gearRatio[9] = GetCppCarSetupItemObject(PyTuple_GetItem(gearRatioTuple, 9));

    PyObject *differentialTypeTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialType"));
    carSetup.differentialType[0] = static_cast<int>(PyLong_AsLong(PyTuple_GetItem(differentialTypeTuple, 0)));
    carSetup.differentialType[1] = static_cast<int>(PyLong_AsLong(PyTuple_GetItem(differentialTypeTuple, 1)));
    carSetup.differentialType[2] = static_cast<int>(PyLong_AsLong(PyTuple_GetItem(differentialTypeTuple, 2)));

    PyObject *differentialRatioTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialRatio"));
    carSetup.differentialRatio[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialRatioTuple, 0));
    carSetup.differentialRatio[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialRatioTuple, 1));
    carSetup.differentialRatio[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialRatioTuple, 2));

    PyObject *differentialMinTqBiasTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialMinTqBias"));
    carSetup.differentialMinTqBias[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMinTqBiasTuple, 0));
    carSetup.differentialMinTqBias[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMinTqBiasTuple, 1));
    carSetup.differentialMinTqBias[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMinTqBiasTuple, 2));

    PyObject *differentialMaxTqBiasTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialMaxTqBias"));
    carSetup.differentialMaxTqBias[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMaxTqBiasTuple, 0));
    carSetup.differentialMaxTqBias[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMaxTqBiasTuple, 1));
    carSetup.differentialMaxTqBias[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMaxTqBiasTuple, 2));

    PyObject *differentialViscosityTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialViscosity"));
    carSetup.differentialViscosity[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialViscosityTuple, 0));
    carSetup.differentialViscosity[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialViscosityTuple, 1));
    carSetup.differentialViscosity[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialViscosityTuple, 2));

    PyObject *differentialLockingTqTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialLockingTq"));
    carSetup.differentialLockingTq[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialLockingTqTuple, 0));
    carSetup.differentialLockingTq[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialLockingTqTuple, 1));
    carSetup.differentialLockingTq[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialLockingTqTuple, 2));

    PyObject *differentialMaxSlipBiasTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialMaxSlipBias"));
    carSetup.differentialMaxSlipBias[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMaxSlipBiasTuple, 0));
    carSetup.differentialMaxSlipBias[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMaxSlipBiasTuple, 1));
    carSetup.differentialMaxSlipBias[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialMaxSlipBiasTuple, 2));

    PyObject *differentialCoastMaxSlipBiasTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "differentialCoastMaxSlipBias"));
    carSetup.differentialCoastMaxSlipBias[0] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialCoastMaxSlipBiasTuple, 0));
    carSetup.differentialCoastMaxSlipBias[1] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialCoastMaxSlipBiasTuple, 1));
    carSetup.differentialCoastMaxSlipBias[2] = GetCppCarSetupItemObject(PyTuple_GetItem(differentialCoastMaxSlipBiasTuple, 2));

    carSetup.steerLock = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "steerLock"));
    carSetup.brakeRepartition = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "brakeRepartition"));
    carSetup.brakePressure = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "brakePressure"));

    PyObject *rideHeightTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "rideHeight"));
    carSetup.rideHeight[0] = GetCppCarSetupItemObject(PyTuple_GetItem(rideHeightTuple, 0));
    carSetup.rideHeight[1] = GetCppCarSetupItemObject(PyTuple_GetItem(rideHeightTuple, 1));
    carSetup.rideHeight[2] = GetCppCarSetupItemObject(PyTuple_GetItem(rideHeightTuple, 2));
    carSetup.rideHeight[3] = GetCppCarSetupItemObject(PyTuple_GetItem(rideHeightTuple, 3));

    PyObject *toeTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "toe"));
    carSetup.toe[0] = GetCppCarSetupItemObject(PyTuple_GetItem(toeTuple, 0));
    carSetup.toe[1] = GetCppCarSetupItemObject(PyTuple_GetItem(toeTuple, 1));
    carSetup.toe[2] = GetCppCarSetupItemObject(PyTuple_GetItem(toeTuple, 2));
    carSetup.toe[3] = GetCppCarSetupItemObject(PyTuple_GetItem(toeTuple, 3));

    PyObject *camberTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "camber"));
    carSetup.camber[0] = GetCppCarSetupItemObject(PyTuple_GetItem(camberTuple, 0));
    carSetup.camber[1] = GetCppCarSetupItemObject(PyTuple_GetItem(camberTuple, 1));
    carSetup.camber[2] = GetCppCarSetupItemObject(PyTuple_GetItem(camberTuple, 2));
    carSetup.camber[3] = GetCppCarSetupItemObject(PyTuple_GetItem(camberTuple, 3));

    PyObject *tirePressureTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "tirePressure"));
    carSetup.tirePressure[0] = GetCppCarSetupItemObject(PyTuple_GetItem(tirePressureTuple, 0));
    carSetup.tirePressure[1] = GetCppCarSetupItemObject(PyTuple_GetItem(tirePressureTuple, 1));
    carSetup.tirePressure[2] = GetCppCarSetupItemObject(PyTuple_GetItem(tirePressureTuple, 2));
    carSetup.tirePressure[3] = GetCppCarSetupItemObject(PyTuple_GetItem(tirePressureTuple, 3));

    PyObject *tireOpLoadTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "tireOpLoad"));
    carSetup.tireOpLoad[0] = GetCppCarSetupItemObject(PyTuple_GetItem(tireOpLoadTuple, 0));
    carSetup.tireOpLoad[1] = GetCppCarSetupItemObject(PyTuple_GetItem(tireOpLoadTuple, 1));
    carSetup.tireOpLoad[2] = GetCppCarSetupItemObject(PyTuple_GetItem(tireOpLoadTuple, 2));
    carSetup.tireOpLoad[3] = GetCppCarSetupItemObject(PyTuple_GetItem(tireOpLoadTuple, 3));

    PyObject *arbSpringTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "arbSpring"));
    carSetup.arbSpring[0] = GetCppCarSetupItemObject(PyTuple_GetItem(arbSpringTuple, 0));
    carSetup.arbSpring[1] = GetCppCarSetupItemObject(PyTuple_GetItem(arbSpringTuple, 1));

    PyObject *arbBellCrankTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "arbBellCrank"));
    carSetup.arbBellcrank[0] = GetCppCarSetupItemObject(PyTuple_GetItem(arbBellCrankTuple, 0));
    carSetup.arbBellcrank[1] = GetCppCarSetupItemObject(PyTuple_GetItem(arbBellCrankTuple, 1));

    PyObject *heaveSpringTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveSpring"));
    carSetup.heaveSpring[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveSpringTuple, 0));
    carSetup.heaveSpring[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveSpringTuple, 1));

    PyObject *heaveBellCrankTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveBellCrank"));
    carSetup.heaveBellcrank[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveBellCrankTuple, 0));
    carSetup.heaveBellcrank[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveBellCrankTuple, 1));

    PyObject *heaveInertanceTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveInertance"));
    carSetup.heaveInertance[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveInertanceTuple, 0));
    carSetup.heaveInertance[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveInertanceTuple, 1));

    PyObject *heaveFastBumpTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveFastBump"));
    carSetup.heaveFastBump[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveFastBumpTuple, 0));
    carSetup.heaveFastBump[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveFastBumpTuple, 1));

    PyObject *heaveSlowBumpTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveSlowBump"));
    carSetup.heaveSlowBump[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveSlowBumpTuple, 0));
    carSetup.heaveSlowBump[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveSlowBumpTuple, 1));

    PyObject *heaveBumpLevelTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveBumpLevel"));
    carSetup.heaveBumpLvel[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveBumpLevelTuple, 0));
    carSetup.heaveBumpLvel[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveBumpLevelTuple, 1));

    PyObject *heaveFastReboundTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveFastRebound"));
    carSetup.heaveFastRebound[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveFastReboundTuple, 0));
    carSetup.heaveFastRebound[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveFastReboundTuple, 1));

    PyObject *heaveSlowReboundTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveSlowRebound"));
    carSetup.heaveSlowRebound[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveSlowReboundTuple, 0));
    carSetup.heaveSlowRebound[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveSlowReboundTuple, 1));

    PyObject *heaveReboundLevelTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "heaveReboundLevel"));
    carSetup.heaveReboundLvel[0] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveReboundLevelTuple, 0));
    carSetup.heaveReboundLvel[1] = GetCppCarSetupItemObject(PyTuple_GetItem(heaveReboundLevelTuple, 1));

    PyObject *suspSpringTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspSpring"));
    carSetup.suspSpring[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSpringTuple, 0));
    carSetup.suspSpring[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSpringTuple, 1));
    carSetup.suspSpring[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSpringTuple, 2));
    carSetup.suspSpring[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSpringTuple, 3));

    PyObject *suspBellCrankTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspBellCrank"));
    carSetup.suspBellcrank[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBellCrankTuple, 0));
    carSetup.suspBellcrank[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBellCrankTuple, 1));
    carSetup.suspBellcrank[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBellCrankTuple, 2));
    carSetup.suspBellcrank[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBellCrankTuple, 3));

    PyObject *suspInertanceTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspInertance"));
    carSetup.suspInertance[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspInertanceTuple, 0));
    carSetup.suspInertance[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspInertanceTuple, 1));
    carSetup.suspInertance[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspInertanceTuple, 2));
    carSetup.suspInertance[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspInertanceTuple, 3));

    PyObject *suspCourseTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspCourse"));
    carSetup.suspCourse[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspCourseTuple, 0));
    carSetup.suspCourse[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspCourseTuple, 1));
    carSetup.suspCourse[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspCourseTuple, 2));
    carSetup.suspCourse[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspCourseTuple, 3));

    PyObject *suspPackerTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspPacker"));
    carSetup.suspPacker[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspPackerTuple, 0));
    carSetup.suspPacker[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspPackerTuple, 1));
    carSetup.suspPacker[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspPackerTuple, 2));
    carSetup.suspPacker[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspPackerTuple, 3));

    PyObject *suspFastBumpTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspFastBump"));
    carSetup.suspFastBump[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastBumpTuple, 0));
    carSetup.suspFastBump[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastBumpTuple, 1));
    carSetup.suspFastBump[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastBumpTuple, 2));
    carSetup.suspFastBump[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastBumpTuple, 3));

    PyObject *suspSlowBumpTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspSlowBump"));
    carSetup.suspSlowBump[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowBumpTuple, 0));
    carSetup.suspSlowBump[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowBumpTuple, 1));
    carSetup.suspSlowBump[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowBumpTuple, 2));
    carSetup.suspSlowBump[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowBumpTuple, 3));

    PyObject *suspBumpLevelTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspBumpLevel"));
    carSetup.suspBumpLvel[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBumpLevelTuple, 0));
    carSetup.suspBumpLvel[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBumpLevelTuple, 1));
    carSetup.suspBumpLvel[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBumpLevelTuple, 2));
    carSetup.suspBumpLvel[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspBumpLevelTuple, 3));

    PyObject *suspFastReboundTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspFastRebound"));
    carSetup.suspFastRebound[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastReboundTuple, 0));
    carSetup.suspFastRebound[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastReboundTuple, 1));
    carSetup.suspFastRebound[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastReboundTuple, 2));
    carSetup.suspFastRebound[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspFastReboundTuple, 3));

    PyObject *suspSlowReboundTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspSlowRebound"));
    carSetup.suspSlowRebound[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowReboundTuple, 0));
    carSetup.suspSlowRebound[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowReboundTuple, 1));
    carSetup.suspSlowRebound[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowReboundTuple, 2));
    carSetup.suspSlowRebound[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspSlowReboundTuple, 3));

    PyObject *suspReboundLevelTuple = PyList_AsTuple(PyObject_GetAttrString(p_carSetup, "suspReboundLevel"));
    carSetup.suspReboundLvel[0] = GetCppCarSetupItemObject(PyTuple_GetItem(suspReboundLevelTuple, 0));
    carSetup.suspReboundLvel[1] = GetCppCarSetupItemObject(PyTuple_GetItem(suspReboundLevelTuple, 1));
    carSetup.suspReboundLvel[2] = GetCppCarSetupItemObject(PyTuple_GetItem(suspReboundLevelTuple, 2));
    carSetup.suspReboundLvel[3] = GetCppCarSetupItemObject(PyTuple_GetItem(suspReboundLevelTuple, 3));

    carSetup.reqRepair = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "reqRepair"));
    carSetup.reqTireset = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "reqTireset"));
    carSetup.reqPenalty = GetCppCarSetupItemObject(PyObject_GetAttrString(p_carSetup, "reqPenalty"));

    return carSetup;
}


void SDATypesConverter::SetPythonCarSetupObject(PyObject* p_target, PyObject* p_data)
{
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "wingAngle", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "gearRatio", 10);
    COPY_PYTHON_ARRAY(p_target, p_data, "differentialType", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialRatio", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialMinTqBias", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialMaxTqBias", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialViscosity", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialLockingTq", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialMaxSlipBias", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "differentialCoastMaxSlipBias", 3);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "rideHeight", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "toe", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "camber", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "tirePressure", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "tireOpLoad", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "arbSpring", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "arbBellCrank", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveSpring", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveBellCrank", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveInertance", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveFastBump", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveSlowBump", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveBumpLevel", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveFastRebound", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveSlowRebound", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "heaveReboundLevel", 2);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspSpring", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspBellCrank", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspInertance", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspCourse", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspPacker", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspFastBump", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspSlowBump", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspBumpLevel", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspFastRebound", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspSlowRebound", 4);
    COPY_PYTHON_CARSETUP_ARRAY(p_target, p_data, "suspReboundLevel", 4);

    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "fRWeightRep"), PyObject_GetAttrString(p_data, "fRWeightRep"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "fRLWeightRep"), PyObject_GetAttrString(p_data, "fRLWeightRep"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "rRLWeightRep"), PyObject_GetAttrString(p_data, "rRLWeightRep"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "fuel"), PyObject_GetAttrString(p_data, "fuel"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "revsLimiter"), PyObject_GetAttrString(p_data, "revsLimiter"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "steerLock"), PyObject_GetAttrString(p_data, "steerLock"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "brakeRepartition"), PyObject_GetAttrString(p_data, "brakeRepartition"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "brakePressure"), PyObject_GetAttrString(p_data, "brakePressure"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "reqRepair"), PyObject_GetAttrString(p_data, "reqRepair"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "reqTireset"), PyObject_GetAttrString(p_data, "reqTireset"));
    SetPythonCarSetupItemObject(PyObject_GetAttrString(p_target, "reqPenalty"), PyObject_GetAttrString(p_data, "reqPenalty"));
}

/// @brief gets the python car setup item object of the current input
/// @param  p_carSetupItem The car setup item data
/// @return             The python object
PyObject *SDATypesConverter::GetPythonCarSetupItemObject(tCarSetupItem &p_carSetupItem)
{
    const int size = 6;
    PyObject *carSetupItemArgs[size];

    carSetupItemArgs[0] = PyFloat_FromDouble(p_carSetupItem.value);
    carSetupItemArgs[1] = PyFloat_FromDouble(p_carSetupItem.min);
    carSetupItemArgs[2] = PyFloat_FromDouble(p_carSetupItem.max);
    carSetupItemArgs[3] = PyFloat_FromDouble(p_carSetupItem.desired_value);
    carSetupItemArgs[4] = PyFloat_FromDouble(p_carSetupItem.stepsize);
    carSetupItemArgs[5] = PyBool_FromLong(static_cast<long>(p_carSetupItem.changed));

    PyObject *carSetupItem = GetObjectFromArgs(m_carSetupItemClass, carSetupItemArgs, size);

    return carSetupItem;
}

/// @brief gets the Cpp car setup item object of the Python object
/// @param  p_carSetupItem The Python object
/// @return             The Cpp object
tCarSetupItem SDATypesConverter::GetCppCarSetupItemObject(PyObject *p_carSetupItem)
{
    tCarSetupItem carSetupItem;

    carSetupItem.value = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carSetupItem, "value")));
    carSetupItem.min = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carSetupItem, "min")));
    carSetupItem.max = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carSetupItem, "max")));
    carSetupItem.desired_value = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carSetupItem, "desiredValue")));
    carSetupItem.stepsize = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carSetupItem, "stepSize")));
    carSetupItem.changed = static_cast<bool>(PyLong_AsLong(PyObject_GetAttrString(p_carSetupItem, "changed")));

    return carSetupItem;
}

void SDATypesConverter::SetPythonCarSetupItemObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[6]{"value", "min", "max", "desiredValue", "stepSize", "changed"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 6);
}

/// @brief gets the python car pit cmd object of the current input
/// @param  p_carPitCmd The car pit cmd data
/// @return             The python object
PyObject *SDATypesConverter::GetPythonCarPitCmdObject(tCarPitCmd &p_carPitCmd)
{
    const int size = 5;
    PyObject *carPitCmdArgs[size];

    carPitCmdArgs[0] = PyFloat_FromDouble(p_carPitCmd.fuel);
    carPitCmdArgs[1] = PyLong_FromLong(static_cast<long>(p_carPitCmd.repair));
    carPitCmdArgs[2] = PyLong_FromLong(static_cast<long>(p_carPitCmd.stopType));
    carPitCmdArgs[3] = PyBool_FromLong(static_cast<long>(p_carPitCmd.setupChanged));
    carPitCmdArgs[4] = PyBool_FromLong(static_cast<long>(p_carPitCmd.tireChange));

    PyObject *carPitCmd = GetObjectFromArgs(m_carPitCmdClass, carPitCmdArgs, size);

    return carPitCmd;
}

/// @brief gets the Cpp car pit cmd object of Python object
/// @param  p_carPitCmd The Python object
/// @return             The Cpp object
tCarPitCmd SDATypesConverter::GetCppCarPitCmdObject(PyObject *p_carPitCmd)
{
    tCarPitCmd carPitCmd;

    carPitCmd.fuel = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_carPitCmd, "fuel")));
    carPitCmd.repair = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carPitCmd, "repair")));
    carPitCmd.stopType = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_carPitCmd, "stopType")));
    carPitCmd.setupChanged = static_cast<bool>(PyLong_AsLong(PyObject_GetAttrString(p_carPitCmd, "setupChanged")));
    carPitCmd.tireChange = PyLong_AsLong(PyObject_GetAttrString(p_carPitCmd, "tireChange")) == 1 ? CarPitCmd::ALL : CarPitCmd::NONE;

    return carPitCmd;
}

void SDATypesConverter::SetPythonCarPitCmdObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[5]{"fuel", "repair", "stopType", "setupChanged", "tireChange"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 5);
}


/// @brief gets the python situation object of the current input
/// @param  p_situation The situation data
/// @return             The python object
PyObject *SDATypesConverter::GetPythonSituationObject(tSituation &p_situation)
{
    const int size = 5;
    PyObject *situationArgs[size];

    situationArgs[1] = PyFloat_FromDouble(p_situation.deltaTime);
    situationArgs[0] = GetPythonRaceInfoObject(p_situation.raceInfo);
    situationArgs[2] = PyFloat_FromDouble(p_situation.currentTime);
    situationArgs[3] = PyFloat_FromDouble(p_situation.accelTime);
    situationArgs[4] = PyLong_FromLong(static_cast<long>(p_situation.nbPlayers));

    PyObject *situation = GetObjectFromArgs(m_situationClass, situationArgs, size);

    return situation;
}

/// @brief gets the Cpp situation object of the Python object
/// @param  p_situation The Python object
/// @return             The Cpp object
tSituation SDATypesConverter::GetCppSituationObject(PyObject *p_situation)
{
    tSituation situation;

    situation.raceInfo = GetCppRaceInfoObject(PyObject_GetAttrString(p_situation, "raceInfo"));
    situation.deltaTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_situation, "deltaTime")));
    situation.currentTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_situation, "currentTime")));
    situation.accelTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_situation, "accelTime")));
    situation.nbPlayers = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_situation, "nbPlayers")));

    return situation;
}

void SDATypesConverter::SetPythonSituationObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[4]{"deltaTime", "currentTime", "accelTime", "nbPlayers"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 4);

    SetPythonRaceInfoObject(PyObject_GetAttrString(p_target, "raceInfo"), PyObject_GetAttrString(p_data, "raceInfo"));
}


/// @brief gets the python race info object of the current input
/// @param  p_raceInfo The race info data
/// @return             The python object
PyObject *SDATypesConverter::GetPythonRaceInfoObject(tRaceAdmInfo &p_raceInfo)
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

    PyObject *raceInfo = GetObjectFromArgs(m_raceInfoClass, raceInfoArgs, size);

    return raceInfo;
}

/// @brief gets the Cpp race info object of the Python object
/// @param  p_raceInfo The Python object
/// @return             The Cpp object
tRaceAdmInfo SDATypesConverter::GetCppRaceInfoObject(PyObject *p_raceInfo)
{
    tRaceAdmInfo raceAdmInfo;

    raceAdmInfo.ncars = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "ncars")));
    raceAdmInfo.totLaps = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "totLaps")));
    raceAdmInfo.extraLaps = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "extraLaps")));
    raceAdmInfo.totTime = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_raceInfo, "totTime")));
    raceAdmInfo.state = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "state")));
    raceAdmInfo.type = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "type")));
    raceAdmInfo.maxDammage = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "maxDamage")));
    raceAdmInfo.fps = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "fps")));
    raceAdmInfo.features = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_raceInfo, "features")));

    return raceAdmInfo;
}

void SDATypesConverter::SetPythonRaceInfoObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[9]{"ncars", "totLaps", "extraLaps", "totTime", "state", "type", "maxDamage", "fps", "features"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 9);
}


/// @brief gets the python object from p_classInit using p_initArgs arguments of p_length
/// @param  p_classInit The class init
/// @param  p_initArgs The class init arguments
/// @param  p_length The length of class init arguments
/// @return             The python object
PyObject *SDATypesConverter::GetObjectFromArgs(PyObject *p_classInit, PyObject *p_initArgs[], int p_length)
{
    if (!PyCallable_Check(p_classInit)) return NULL;

    PyObject *initArgsPy = PyTuple_New(p_length);

    for (Py_ssize_t i = 0; i < p_length; i++)
    {
        if (p_initArgs[i] == NULL) return NULL;
        PyTuple_SetItem(initArgsPy, i, p_initArgs[i]);
    }

    // initialize SDAType
    PyObject *objectInit = PyObject_CallObject(p_classInit, initArgsPy);

    return objectInit;
}

void SDATypesConverter::ReplaceString(char *p_toFill, const char *p_data, int p_length)
{
    for (int i = 0; i < p_length; i++)
    {
        p_toFill[i] = p_data[i];
    }
}

PyObject* SDATypesConverter::GetPythonAeroObject(tAero& p_aero)
{
    const int size = 7;
    PyObject *aeroArgs[size];

    aeroArgs[0] = PyFloat_FromDouble(p_aero.drag);
    aeroArgs[1] = PyFloat_FromDouble(p_aero.lift[0]);
    aeroArgs[2] = PyFloat_FromDouble(p_aero.lift[1]);
    aeroArgs[3] = PyFloat_FromDouble(p_aero.Clift[0]);
    aeroArgs[4] = PyFloat_FromDouble(p_aero.Clift[1]);
    aeroArgs[5] = PyFloat_FromDouble(p_aero.Cd);
    aeroArgs[6] = PyFloat_FromDouble(p_aero.CdBody);

    PyObject *aero = GetObjectFromArgs(m_aeroClass, aeroArgs, size);

    return aero;
}

tAero SDATypesConverter::GetCppAeroObject(PyObject* p_aero)
{
    tAero aero;

    aero.drag = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_aero, "drag")));

    PyObject *liftTuple = PyList_AsTuple(PyObject_GetAttrString(p_aero, "lift"));
    aero.lift[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(liftTuple, 0)));
    aero.lift[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(liftTuple, 1)));

    PyObject *cliftTuple = PyList_AsTuple(PyObject_GetAttrString(p_aero, "Clift"));
    aero.Clift[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(cliftTuple, 0)));
    aero.Clift[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(cliftTuple, 1)));

    aero.Cd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_aero, "Cd")));
    aero.CdBody = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_aero, "CdBody")));

    return aero;
}

void SDATypesConverter::SetPythonAeroObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[3]{"drag", "Cd", "CdBody"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 3);
    COPY_PYTHON_ARRAY(p_target, p_data, "lift", 2);
    COPY_PYTHON_ARRAY(p_target, p_data, "Clift", 2);
}

PyObject* SDATypesConverter::GetPythonWingObject(tWing& p_wing)
{
    const int size = 29;
    PyObject *wingArgs[size];

    wingArgs[0] = GetPythonVectorObject(p_wing.forces.x, p_wing.forces.y, p_wing.forces.z);
    wingArgs[1] = PyFloat_FromDouble(p_wing.Kx);
    wingArgs[2] = PyFloat_FromDouble(p_wing.Kz);
    wingArgs[3] = PyFloat_FromDouble(p_wing.Kz_org);
    wingArgs[4] = PyFloat_FromDouble(p_wing.angle);
    wingArgs[5] = GetPythonVectorObject(p_wing.staticPos.x, p_wing.staticPos.y, p_wing.staticPos.z);
    wingArgs[6] = PyFloat_FromDouble(p_wing.AoAatMax);
    wingArgs[7] = PyFloat_FromDouble(p_wing.AoAatZero);
    wingArgs[8] = PyFloat_FromDouble(p_wing.AoAatZRad);
    wingArgs[9] = PyFloat_FromDouble(p_wing.AoAOffset);
    wingArgs[10] = PyFloat_FromDouble(p_wing.CliftMax);
    wingArgs[11] = PyFloat_FromDouble(p_wing.CliftZero);
    wingArgs[12] = PyFloat_FromDouble(p_wing.CliftAsymp);
    wingArgs[13] = PyFloat_FromDouble(p_wing.a);
    wingArgs[14] = PyFloat_FromDouble(p_wing.b);
    wingArgs[15] = PyFloat_FromDouble(p_wing.c);
    wingArgs[16] = PyFloat_FromDouble(p_wing.d);
    wingArgs[17] = PyFloat_FromDouble(p_wing.f);
    wingArgs[18] = PyFloat_FromDouble(p_wing.AoStall);
    wingArgs[19] = PyFloat_FromDouble(p_wing.Stallw);
    wingArgs[20] = PyFloat_FromDouble(p_wing.AR);
    wingArgs[21] = PyFloat_FromDouble(p_wing.Kx1);
    wingArgs[22] = PyFloat_FromDouble(p_wing.Kx2);
    wingArgs[23] = PyFloat_FromDouble(p_wing.Kx3);
    wingArgs[24] = PyFloat_FromDouble(p_wing.Kx4);
    wingArgs[25] = PyFloat_FromDouble(p_wing.Kz1);
    wingArgs[26] = PyFloat_FromDouble(p_wing.Kz2);
    wingArgs[27] = PyFloat_FromDouble(p_wing.Kz3);
    wingArgs[28] = PyLong_FromLong(static_cast<long>(p_wing.WingType));

    PyObject *aero = GetObjectFromArgs(m_wingClass, wingArgs, size);

    return aero;
}

tWing SDATypesConverter::GetCppWingObject(PyObject* p_wing)
{
    tWing wing;

    wing.forces = GetCppTVectorObject(PyObject_GetAttrString(p_wing, "forces"));
    wing.staticPos = GetCppTVectorObject(PyObject_GetAttrString(p_wing, "staticPos"));
    wing.WingType = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_wing, "WingType")));
    wing.Kx = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kx")));
    wing.Kz = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kz")));
    wing.Kz_org = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kz_org")));
    wing.angle = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "angle")));
    wing.AoAatMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "AoAatMax")));
    wing.AoAatZero = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "AoAatZero")));
    wing.AoAatZRad = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "AoAatZRad")));
    wing.AoAOffset = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "AoAOffset")));
    wing.CliftMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "CliftMax")));
    wing.CliftZero = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "CliftZero")));
    wing.CliftAsymp = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "CliftAsymp")));
    wing.a = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "a")));
    wing.b = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "b")));
    wing.c = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "c")));
    wing.d = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "d")));
    wing.f = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "f")));
    wing.AoStall = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "AoStall")));
    wing.Stallw = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Stallw")));
    wing.AR = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "AR")));
    wing.Kx1 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kx1")));
    wing.Kx2 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kx2")));
    wing.Kx3 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kx3")));
    wing.Kx4 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kx4")));
    wing.Kz1 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kz1")));
    wing.Kz2 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kz2")));
    wing.Kz3 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wing, "Kz3")));

    return wing;
}

void SDATypesConverter::SetPythonWingObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[28]{  "Kx", "Kz", "Kz_org", "angle", "AoAatMax", "AoAatZero", "AoAatZRad",
                                 "AoAOffset", "CliftMax", "CliftZero", "CliftAsymp",
                                 "a", "b", "c", "d", "f", "AoStall", "Stallw", "AR", "Kx1", "Kx2", "Kx3", "Kx4", "Kz1", "Kz2",
                                 "Kz3", "WingType"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 28);

    SetPythonVectorObject(PyObject_GetAttrString(p_target, "forces"), PyObject_GetAttrString(p_data, "forces"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "staticPos"), PyObject_GetAttrString(p_data, "staticPos"));

}

PyObject* SDATypesConverter::GetPythonDamperDefObject(tDamperDef& p_damperDef)
{
    const int size = 5;
    PyObject *damperDefArgs[size];

    damperDefArgs[0] = PyFloat_FromDouble(p_damperDef.C1);
    damperDefArgs[1] = PyFloat_FromDouble(p_damperDef.b1);
    damperDefArgs[2] = PyFloat_FromDouble(p_damperDef.v1);
    damperDefArgs[3] = PyFloat_FromDouble(p_damperDef.C2);
    damperDefArgs[4] = PyFloat_FromDouble(p_damperDef.b2);

    PyObject *damperDef = GetObjectFromArgs(m_damperDefClass, damperDefArgs, size);

    return damperDef;
}

tDamperDef SDATypesConverter::GetCppDamperDefObject(PyObject* p_damperDef)
{
    tDamperDef damperDef;

    damperDef.C1 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_damperDef, "C1")));
    damperDef.b1 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_damperDef, "b1")));
    damperDef.v1 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_damperDef, "v1")));
    damperDef.C2 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_damperDef, "C2")));
    damperDef.b2 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_damperDef, "b2")));

    return damperDef;
}

void SDATypesConverter::SetPythonDamperDefObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[5]{ "C1", "b1", "v1", "C2", "b2"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 5);
}

PyObject* SDATypesConverter::GetPythonDamperObject(tDamper& p_damper)
{
    const int size = 2;
    PyObject *damperArgs[size];

    damperArgs[0] = GetPythonDamperDefObject(p_damper.bump);
    damperArgs[1] = GetPythonDamperDefObject(p_damper.rebound);

    PyObject *damper = GetObjectFromArgs(m_damperClass, damperArgs, size);

    return damper;
}

tDamper SDATypesConverter::GetCppDamperObject(PyObject* p_damper)
{
    tDamper damper;

    damper.bump = GetCppDamperDefObject(PyObject_GetAttrString(p_damper, "bump"));
    damper.rebound = GetCppDamperDefObject(PyObject_GetAttrString(p_damper, "rebound"));

    return damper;
}

void SDATypesConverter::SetPythonDamperObject(PyObject* p_target, PyObject* p_data)
{
    SetPythonDamperDefObject(PyObject_GetAttrString(p_target, "bump"), PyObject_GetAttrString(p_data, "bump"));
    SetPythonDamperDefObject(PyObject_GetAttrString(p_target, "rebound"), PyObject_GetAttrString(p_data, "rebound"));
}

PyObject* SDATypesConverter::GetPythonSpringObject(tSpring& p_spring)
{
    const int size = 6;
    PyObject *springArgs[size];

    springArgs[0] = PyFloat_FromDouble(p_spring.K);
    springArgs[1] = PyFloat_FromDouble(p_spring.F0);
    springArgs[2] = PyFloat_FromDouble(p_spring.x0);
    springArgs[3] = PyFloat_FromDouble(p_spring.xMax);
    springArgs[4] = PyFloat_FromDouble(p_spring.bellcrank);
    springArgs[5] = PyFloat_FromDouble(p_spring.packers);

    PyObject *spring = GetObjectFromArgs(m_springClass, springArgs, size);

    return spring;
}
tSpring SDATypesConverter::GetCppSpringObject(PyObject* p_spring)
{
    tSpring spring;

    spring.K = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_spring, "K")));
    spring.F0 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_spring, "F0")));
    spring.x0 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_spring, "x0")));
    spring.xMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_spring, "xMax")));
    spring.bellcrank = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_spring, "bellcrank")));
    spring.packers = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_spring, "packers")));

    return spring;
}

void SDATypesConverter::SetPythonSpringObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[6]{ "K", "F0", "x0", "xMax", "bellcrank", "packers"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 6);
}

PyObject* SDATypesConverter::GetPythonSuspensionObject(tSuspension& p_suspension)
{
    const int size = 8;
    PyObject *suspensionArgs[size];

    suspensionArgs[0] = GetPythonSpringObject(p_suspension.spring);
    suspensionArgs[1] = GetPythonDamperObject(p_suspension.damper);
    suspensionArgs[2] = PyFloat_FromDouble(p_suspension.inertance);
    suspensionArgs[3] = PyFloat_FromDouble(p_suspension.x);
    suspensionArgs[4] = PyFloat_FromDouble(p_suspension.v);
    suspensionArgs[5] = PyFloat_FromDouble(p_suspension.a);
    suspensionArgs[6] = PyFloat_FromDouble(p_suspension.force);
    suspensionArgs[7] = PyLong_FromLong(static_cast<long>(p_suspension.state));

    PyObject *suspension = GetObjectFromArgs(m_suspensionClass, suspensionArgs, size);

    return suspension;
}
tSuspension SDATypesConverter::GetCppSuspensionObject(PyObject* p_suspension)
{
    tSuspension suspension;

    suspension.spring = GetCppSpringObject(PyObject_GetAttrString(p_suspension, "spring"));
    suspension.damper = GetCppDamperObject(PyObject_GetAttrString(p_suspension, "damper"));
    suspension.inertance = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_suspension, "inertance")));
    suspension.x = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_suspension, "x")));
    suspension.v = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_suspension, "v")));
    suspension.a = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_suspension, "a")));
    suspension.force = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_suspension, "force")));
    suspension.state = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_suspension, "state")));

    return suspension;
}

void SDATypesConverter::SetPythonSuspensionObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[6]{ "inertance", "x", "v", "a", "force", "state"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 6);

    SetPythonSpringObject(PyObject_GetAttrString(p_target, "spring"), PyObject_GetAttrString(p_data, "spring"));
    SetPythonDamperObject(PyObject_GetAttrString(p_target, "damper"), PyObject_GetAttrString(p_data, "damper"));
}

PyObject* SDATypesConverter::GetPythonBrakeObject(tBrake& p_brake)
{
    const int size = 9;
    PyObject *brakeArgs[size];

    brakeArgs[0] = PyFloat_FromDouble(p_brake.pressure);
    brakeArgs[1] = PyFloat_FromDouble(p_brake.Tq);
    brakeArgs[2] = PyFloat_FromDouble(p_brake.coeff);
    brakeArgs[3] = PyFloat_FromDouble(p_brake.I);
    brakeArgs[4] = PyFloat_FromDouble(p_brake.radius);
    brakeArgs[5] = PyFloat_FromDouble(p_brake.temp);
    brakeArgs[6] = PyFloat_FromDouble(p_brake.TCL);
    brakeArgs[7] = PyFloat_FromDouble(p_brake.ABS);
    brakeArgs[8] = PyBool_FromLong(static_cast<long>(p_brake.EnableABS));

    PyObject *brake = GetObjectFromArgs(m_brakeClass, brakeArgs, size);

    return brake;
}

tBrake SDATypesConverter::GetCppBrakeObject(PyObject* p_brake)
{
    tBrake brake;

    brake.pressure = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "pressure")));
    brake.Tq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "Tq")));
    brake.coeff = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "coeff")));
    brake.I = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "I")));
    brake.radius = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "radius")));
    brake.temp = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "temp")));
    brake.TCL = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "TCL")));
    brake.ABS = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brake, "ABS")));
    brake.EnableABS = static_cast<bool>(PyLong_AsLong(PyObject_GetAttrString(p_brake, "EnableABS")));

    return brake;
}

void SDATypesConverter::SetPythonBrakeObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[9]{ "pressure", "Tq", "coeff", "I", "radius", "temp", "TCL", "ABS", "EnableABS"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 9);
}

PyObject* SDATypesConverter::GetPythonBrakeSystObject(tBrakeSyst& p_brakeSyst)
{
    const int size = 3;
    PyObject *brakeSystArgs[size];

    brakeSystArgs[0] = PyFloat_FromDouble(p_brakeSyst.rep);
    brakeSystArgs[1] = PyFloat_FromDouble(p_brakeSyst.coeff);
    brakeSystArgs[2] = PyFloat_FromDouble(p_brakeSyst.ebrake_pressure);

    PyObject *brakeSyst = GetObjectFromArgs(m_brakeSystClass, brakeSystArgs, size);

    return brakeSyst;
}
tBrakeSyst SDATypesConverter::GetCppBrakeSystObject(PyObject* p_brakeSyst)
{
    tBrakeSyst brakeSyst;

    brakeSyst.rep = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brakeSyst, "rep")));
    brakeSyst.coeff = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brakeSyst, "coeff")));
    brakeSyst.ebrake_pressure = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_brakeSyst, "ebrake_pressure")));

    return brakeSyst;
}
void SDATypesConverter::SetPythonBrakeSystObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[3]{ "rep", "coeff", "ebrake_pressure"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 3);
}

PyObject* SDATypesConverter::GetPythonDynAxisSystObject(tDynAxis& p_dynAxis)
{
    const int size = 4;
    PyObject *dynAxisArgs[size];

    dynAxisArgs[0] = PyFloat_FromDouble(p_dynAxis.spinVel);
    dynAxisArgs[1] = PyFloat_FromDouble(p_dynAxis.Tq);
    dynAxisArgs[2] = PyFloat_FromDouble(p_dynAxis.brkTq);
    dynAxisArgs[3] = PyFloat_FromDouble(p_dynAxis.I);

    PyObject *dynAxisSyst = GetObjectFromArgs(m_dynAxisClass, dynAxisArgs, size);

    return dynAxisSyst;
}

tDynAxis SDATypesConverter::GetCppDynAxisSystObject(PyObject* p_dynAxis)
{
    tDynAxis dynAxis;

    dynAxis.spinVel = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_dynAxis, "spinVel")));
    dynAxis.Tq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_dynAxis, "Tq")));
    dynAxis.brkTq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_dynAxis, "brkTq")));
    dynAxis.I = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_dynAxis, "I")));

    return dynAxis;
}

void SDATypesConverter::SetPythonDynAxisSystObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[4]{ "spinVel", "Tq", "brkTq", "I"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 4);
}

PyObject* SDATypesConverter::GetPythonDifferentialSystObject(tDifferential& p_differential)
{
    const int size = 14;
    PyObject *differentialArgs[size];

    differentialArgs[0] =PyLong_FromLong(static_cast<long>(p_differential.type));
    differentialArgs[1] = PyFloat_FromDouble(p_differential.ratio);
    differentialArgs[2] = PyFloat_FromDouble(p_differential.I);
    differentialArgs[3] = PyFloat_FromDouble(p_differential.efficiency);
    differentialArgs[4] = PyFloat_FromDouble(p_differential.bias);
    differentialArgs[5] = PyFloat_FromDouble(p_differential.dTqMin);
    differentialArgs[6] = PyFloat_FromDouble(p_differential.dTqMax);
    differentialArgs[7] = PyFloat_FromDouble(p_differential.dSlipMax);
    differentialArgs[8] = PyFloat_FromDouble(p_differential.dCoastSlipMax);
    differentialArgs[9] = PyFloat_FromDouble(p_differential.lockInputTq);
    differentialArgs[10] = PyFloat_FromDouble(p_differential.viscosity);
    differentialArgs[11] = PyFloat_FromDouble(p_differential.viscomax);
    differentialArgs[12] = GetPythonDynAxisSystObject(p_differential.in);
    differentialArgs[13] = GetPythonDynAxisSystObject(p_differential.feedBack);

    PyObject *differential = GetObjectFromArgs(m_differentialClass, differentialArgs, size);

    return differential;
}

tDifferential SDATypesConverter::GetCppDifferentialSystObject(PyObject* p_differential)
{
    tDifferential differential;

    differential.type = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_differential, "type")));
    differential.ratio = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "ratio")));
    differential.I = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "I")));
    differential.efficiency = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "efficiency")));
    differential.bias = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "bias")));
    differential.dTqMin = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "dTqMin")));
    differential.dTqMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "dTqMax")));
    differential.dSlipMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "dSlipMax")));
    differential.dCoastSlipMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "dCoastSlipMax")));
    differential.lockInputTq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "lockInputTq")));
    differential.viscosity = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "viscosity")));
    differential.viscomax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_differential, "viscomax")));
    differential.in = GetCppDynAxisSystObject(PyObject_GetAttrString(p_differential, "invar"));
    differential.feedBack = GetCppDynAxisSystObject(PyObject_GetAttrString(p_differential, "feedback"));

    return differential;
}

void SDATypesConverter::SetPythonDifferentialSystObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[12]{ "type", "ratio", "I", "efficiency", "bias", "dTqMin", "dTqMax", "dSlipMax", "dCoastSlipMax",
        "lockInputTq", "viscosity", "viscomax" };
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 12);

    SetPythonDynAxisSystObject(PyObject_GetAttrString(p_target, "invar"), PyObject_GetAttrString(p_data, "invar"));
    SetPythonDynAxisSystObject(PyObject_GetAttrString(p_target, "feedback"), PyObject_GetAttrString(p_data, "feedback"));
}

PyObject* SDATypesConverter::GetPythonAxleSystObject(tAxle& p_axle)
{
    const int size = 7;
    PyObject *axleArgs[size];

    axleArgs[0] = PyFloat_FromDouble(p_axle.xpos);
    axleArgs[1] = GetPythonSuspensionObject(p_axle.arbSusp);
    axleArgs[2] = GetPythonSuspensionObject(p_axle.heaveSusp);
    axleArgs[3] = PyFloat_FromDouble(p_axle.wheight0);
    axleArgs[4] = PyFloat_FromDouble(p_axle.force[0]);
    axleArgs[5] = PyFloat_FromDouble(p_axle.force[1]);
    axleArgs[6] = PyFloat_FromDouble(p_axle.I);

    PyObject *axle = GetObjectFromArgs(m_axleClass, axleArgs, size);

    return axle;
}

tAxle SDATypesConverter::GetCppAxleSystObject(PyObject* p_axle)
{
    tAxle axle;

    axle.xpos = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_axle, "xpos")));
    axle.arbSusp = GetCppSuspensionObject(PyObject_GetAttrString(p_axle, "arbSusp"));
    axle.heaveSusp = GetCppSuspensionObject(PyObject_GetAttrString(p_axle, "heaveSusp"));

    PyObject *forceVal = PyList_AsTuple(PyObject_GetAttrString(p_axle, "force"));
    axle.force[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(forceVal, 0)));
    axle.force[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(forceVal, 1)));

    axle.wheight0 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_axle, "wheight0")));
    axle.I = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_axle, "I")));

    return axle;
}
void SDATypesConverter::SetPythonAxleSystObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[3]{ "xpos", "wheight0", "I"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 3);

    SetPythonSuspensionObject(PyObject_GetAttrString(p_target, "arbSusp"), PyObject_GetAttrString(p_data, "arbSusp"));
    SetPythonSuspensionObject(PyObject_GetAttrString(p_target, "heaveSusp"), PyObject_GetAttrString(p_data, "heaveSusp"));
    COPY_PYTHON_ARRAY(p_target, p_data, "force", 2);
}

PyObject* SDATypesConverter::GetPythonSteerSystObject(tSteer& p_steer)
{
    const int size = 3;
    PyObject *steerArgs[size];

    steerArgs[0] = PyFloat_FromDouble(p_steer.steerLock);
    steerArgs[1] = PyFloat_FromDouble(p_steer.maxSpeed);
    steerArgs[2] = PyFloat_FromDouble(p_steer.steer);

    PyObject *steer = GetObjectFromArgs(m_steerClass, steerArgs, size);

    return steer;
}

tSteer SDATypesConverter::GetCppSteerSystObject(PyObject* p_steer)
{
    tSteer steer;

    steer.steerLock = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_steer, "steerLock")));
    steer.maxSpeed = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_steer, "maxSpeed")));
    steer.steer = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_steer, "steer")));

    return steer;
}

void SDATypesConverter::SetPythonSteerSystObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[3]{ "steerLock", "maxSpeed", "steer"};
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 3);
}

PyObject* SDATypesConverter::GetPythonWheelSystObject(tWheel& p_wheel)
{
    const int size = 61;
    PyObject *wheelArgs[size];

    wheelArgs[0] = GetPythonSuspensionObject(p_wheel.susp);
    wheelArgs[1] = GetPythonBrakeObject(p_wheel.brake);
    wheelArgs[2] = GetPythonVectorObject(p_wheel.forces.x, p_wheel.forces.y, p_wheel.forces.z);
    wheelArgs[3] = GetPythonVectorObject(p_wheel.torques.x, p_wheel.torques.y, p_wheel.torques.z);
    wheelArgs[4] = PyFloat_FromDouble(p_wheel.torqueAlign);
    wheelArgs[5] = PyFloat_FromDouble(p_wheel.rollRes);
    wheelArgs[6] = PyFloat_FromDouble(p_wheel.rideHeight);
    wheelArgs[7] = PyFloat_FromDouble(p_wheel.zRoad);
    wheelArgs[8] = GetPythonVectorObject(p_wheel.pos.x, p_wheel.pos.y, p_wheel.pos.z);
    wheelArgs[9] = GetPythonVectorObject(p_wheel.bodyVel.x, p_wheel.bodyVel.y, p_wheel.bodyVel.z);
    wheelArgs[10] = PyFloat_FromDouble(p_wheel.driveTq);
    wheelArgs[11] = PyFloat_FromDouble(p_wheel.vt);
    wheelArgs[12] = PyFloat_FromDouble(p_wheel.spinTq);
    wheelArgs[13] = PyFloat_FromDouble(p_wheel.spinVel);
    wheelArgs[14] = PyFloat_FromDouble(p_wheel.prespinVel);
    wheelArgs[15] = PyLong_FromLong(static_cast<long>(p_wheel.state));
    wheelArgs[16] = PyFloat_FromDouble(p_wheel.axleFz);
    wheelArgs[17] = PyFloat_FromDouble(p_wheel.axleFz3rd);
    wheelArgs[18] = GetPythonTrackLocationObject(p_wheel.trkPos);
    wheelArgs[19] = GetPythonPosDObject(p_wheel.relPos);
    wheelArgs[20] = PyFloat_FromDouble(p_wheel.sa);
    wheelArgs[21] = PyFloat_FromDouble(p_wheel.sx);
    wheelArgs[22] = PyFloat_FromDouble(p_wheel.steer);
    wheelArgs[23] = GetPythonPosDObject(p_wheel.staticPos);
    wheelArgs[24] = PyFloat_FromDouble(p_wheel.cosax);
    wheelArgs[25] = PyFloat_FromDouble(p_wheel.sinax);
    wheelArgs[26] = PyFloat_FromDouble(p_wheel.weight0);
    wheelArgs[27] = PyFloat_FromDouble(p_wheel.tireSpringRate);
    wheelArgs[28] = PyFloat_FromDouble(p_wheel.radius);
    wheelArgs[29] = PyFloat_FromDouble(p_wheel.mu);
    wheelArgs[30] = PyFloat_FromDouble(p_wheel.I);
    wheelArgs[31] = PyFloat_FromDouble(p_wheel.curI);
    wheelArgs[32] = PyFloat_FromDouble(p_wheel.mfC);
    wheelArgs[33] = PyFloat_FromDouble(p_wheel.mfB);
    wheelArgs[34] = PyFloat_FromDouble(p_wheel.mfE);
    wheelArgs[35] = PyFloat_FromDouble(p_wheel.lfMax);
    wheelArgs[36] = PyFloat_FromDouble(p_wheel.lfMin);
    wheelArgs[37] = PyFloat_FromDouble(p_wheel.lfK);
    wheelArgs[38] = PyFloat_FromDouble(p_wheel.opLoad);
    wheelArgs[39] = PyFloat_FromDouble(p_wheel.AlignTqFactor);
    wheelArgs[40] = PyFloat_FromDouble(p_wheel.mass);
    wheelArgs[41] = PyFloat_FromDouble(p_wheel.camber);
    wheelArgs[42] = PyFloat_FromDouble(p_wheel.pressure);
    wheelArgs[43] = PyFloat_FromDouble(p_wheel.Ttire);
    wheelArgs[44] = PyFloat_FromDouble(p_wheel.Topt);
    wheelArgs[45] = PyFloat_FromDouble(p_wheel.Tinit);
    wheelArgs[46] = PyFloat_FromDouble(p_wheel.muTmult);
    wheelArgs[47] = PyFloat_FromDouble(p_wheel.heatingm);
    wheelArgs[48] = PyFloat_FromDouble(p_wheel.aircoolm);
    wheelArgs[49] = PyFloat_FromDouble(p_wheel.speedcoolm);
    wheelArgs[50] = PyFloat_FromDouble(p_wheel.wearrate);
    wheelArgs[51] = PyFloat_FromDouble(p_wheel.treadDepth);
    wheelArgs[52] = PyFloat_FromDouble(p_wheel.critTreadDepth);
    wheelArgs[53] = PyFloat_FromDouble(p_wheel.muTDmult[0]);
    wheelArgs[54] = PyFloat_FromDouble(p_wheel.muTDmult[1]);
    wheelArgs[55] = PyFloat_FromDouble(p_wheel.muTDoffset[0]);
    wheelArgs[56] = PyFloat_FromDouble(p_wheel.muTDoffset[1]);
    wheelArgs[57] = GetPythonDynAxisSystObject(p_wheel.in);
    wheelArgs[58] = GetPythonDynAxisSystObject(p_wheel.feedBack);
    wheelArgs[59] = PyFloat_FromDouble(p_wheel.preFn);
    wheelArgs[60] = PyFloat_FromDouble(p_wheel.preFt);

    PyObject *wheel = GetObjectFromArgs(m_wheelClass, wheelArgs, size);

    return wheel;
}

tWheel SDATypesConverter::GetCppWheelSystObject(PyObject* p_wheel)
{
    tWheel wheel;

    wheel.susp = GetCppSuspensionObject(PyObject_GetAttrString(p_wheel, "susp"));
    wheel.brake = GetCppBrakeObject(PyObject_GetAttrString(p_wheel, "brake"));
    wheel.forces = GetCppTVectorObject(PyObject_GetAttrString(p_wheel, "forces"));
    wheel.torques = GetCppTVectorObject(PyObject_GetAttrString(p_wheel, "torques"));
    wheel.torqueAlign = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "torqueAlign")));
    wheel.rollRes = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "roolRes")));
    wheel.rideHeight = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "rideHeight")));
    wheel.zRoad = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "zRoad")));
    wheel.pos = GetCppTVectorObject(PyObject_GetAttrString(p_wheel, "pos"));
    wheel.bodyVel = GetCppTVectorObject(PyObject_GetAttrString(p_wheel, "bodyVel"));
    wheel.driveTq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "driveTq")));
    wheel.vt = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "vt")));
    wheel.spinTq = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "spinTq")));
    wheel.spinVel = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "spinVel")));
    wheel.prespinVel = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "prespinVel")));
    wheel.state = static_cast<int>(PyLong_AsLong(PyObject_GetAttrString(p_wheel, "state")));
    wheel.axleFz = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "axleFz")));
    wheel.axleFz3rd = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "axleFz3rd")));
    wheel.trkPos = GetCppTrackLocationObject(PyObject_GetAttrString(p_wheel, "trkPos"));
    wheel.relPos = GetCppPosDObject(PyObject_GetAttrString(p_wheel, "relPos"));
    wheel.sa = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "sa")));
    wheel.sx = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "sx")));
    wheel.steer = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "steer")));
    wheel.staticPos = GetCppPosDObject(PyObject_GetAttrString(p_wheel, "staticPos"));
    wheel.cosax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "cosax")));
    wheel.sinax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "sinax")));
    wheel.weight0 = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "weight0")));
    wheel.tireSpringRate = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "tireSpringRate")));
    wheel.radius = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "radius")));
    wheel.mu = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "mu")));
    wheel.I = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "I")));
    wheel.curI = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "curI")));
    wheel.mfC = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "mfC")));
    wheel.mfB = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "mfB")));
    wheel.mfE = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "mfE")));
    wheel.lfMax = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "lfMax")));
    wheel.lfMin = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "lfMin")));
    wheel.lfK = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "lfK")));
    wheel.opLoad = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "opLoad")));
    wheel.AlignTqFactor = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "AlignTqFactor")));
    wheel.mass = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "mass")));
    wheel.camber = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "camber")));
    wheel.pressure = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "pressure")));
    wheel.Ttire = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "Ttire")));
    wheel.Topt = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "Topt")));
    wheel.Tinit = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "Tinit")));
    wheel.muTmult = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "muTmult")));
    wheel.heatingm = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "heatingm")));
    wheel.aircoolm = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "aircoolm")));
    wheel.speedcoolm = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "speedcoolm")));
    wheel.wearrate = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "wearrate")));
    wheel.treadDepth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "treadDepth")));
    wheel.critTreadDepth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "critTreadDepth")));

    PyObject *muTDmultVal = PyList_AsTuple(PyObject_GetAttrString(p_wheel, "muTDmult"));
    wheel.muTDmult[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDmultVal, 0)));
    wheel.muTDmult[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDmultVal, 1)));

    PyObject *muTDoffsetVal = PyList_AsTuple(PyObject_GetAttrString(p_wheel, "muTDoffset"));
    wheel.muTDoffset[0] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDoffsetVal, 0)));
    wheel.muTDoffset[1] = static_cast<float>(PyFloat_AsDouble(PyTuple_GetItem(muTDoffsetVal, 1)));

    wheel.in = GetCppDynAxisSystObject(PyObject_GetAttrString(p_wheel, "invar"));
    wheel.feedBack = GetCppDynAxisSystObject(PyObject_GetAttrString(p_wheel, "feedBack"));

    wheel.treadDepth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "treadDepth")));
    wheel.treadDepth = static_cast<float>(PyFloat_AsDouble(PyObject_GetAttrString(p_wheel, "treadDepth")));

    return wheel;
}
void SDATypesConverter::SetPythonWheelSystObject(PyObject* p_target, PyObject* p_data)
{
    const char* attributeList[46]{ "torqueAlign", "roolRes", "rideHeight", "zRoad", "driveTq", "vt", "spinTq","spinVel", "prespinVel", "state",
        "axleFz", "axleFz3rd","sa", "sx", "steer", "cosax", "sinax", "weight0", "tireSpringRate", "radius", "mu", "I", "curI", "mfC", "mfB", "mfE",
        "lfMax", "lfMin", "lfK", "opLoad", "AlignTqFactor", "mass", "camber", "pressure","Ttire", "Topt", "Tinit", "muTmult", "heatingm", "aircoolm", "speedcoolm",
                                 "wearrate", "treadDepth", "critTreadDepth", "preFn", "preFt" };
    COPY_PYTHON_OBJECT_LIST(p_target, p_data, attributeList, 46);

    SetPythonSuspensionObject(PyObject_GetAttrString(p_target, "susp"), PyObject_GetAttrString(p_data, "susp"));
    SetPythonBrakeObject(PyObject_GetAttrString(p_target, "brake"), PyObject_GetAttrString(p_data, "brake"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "forces"), PyObject_GetAttrString(p_data, "forces"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "torques"), PyObject_GetAttrString(p_data, "torques"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "pos"), PyObject_GetAttrString(p_data, "pos"));
    SetPythonVectorObject(PyObject_GetAttrString(p_target, "bodyVel"), PyObject_GetAttrString(p_data, "bodyVel"));
    SetPythonTrackLocationObject(PyObject_GetAttrString(p_target, "trkPos"), PyObject_GetAttrString(p_data, "trkPos"));
    SetPythonPosDObject(PyObject_GetAttrString(p_target, "relPos"), PyObject_GetAttrString(p_data, "relPos"));
    SetPythonPosDObject(PyObject_GetAttrString(p_target, "staticPos"), PyObject_GetAttrString(p_data, "staticPos"));

    COPY_PYTHON_ARRAY(p_target, p_data, "muTDmult", 2);
    COPY_PYTHON_ARRAY(p_target, p_data, "muTDoffset", 2);

    SetPythonDynAxisSystObject(PyObject_GetAttrString(p_target, "invar"), PyObject_GetAttrString(p_data, "invar"));
    SetPythonDynAxisSystObject(PyObject_GetAttrString(p_target, "feedBack"), PyObject_GetAttrString(p_data, "feedBack"));
}

PyObject* GetPythonTransmissionSystObject(tTransmission& p_transmission);
tTransmission GetCppTransmissionSystObject(PyObject* p_transmission);
void SetPythonTransmissionSystObject(PyObject* p_target, PyObject* p_data);

PyObject* GetPythonEngineSystObject(tEngine& p_engine);
tEngine GetCppEngineSystObject(PyObject* p_engine);
void SetPythonEngineSystObject(PyObject* p_target, PyObject* p_data);

PyObject* GetPythonCarSystObject(tCar& p_car);
tCar GetCppCarSystObject(PyObject* p_car);
void SetPythonCarSystObject(PyObject* p_target, PyObject* p_data);

// 5 missing