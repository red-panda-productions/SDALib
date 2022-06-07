#include "SDASpeedDreams.hpp"
#include "sim.h"

/// @brief sets tCarElt to tCar
/// @param p_other the tCar car
/// @param p_carElt the tCarElt car
/// @return a new tCar
tCar CarConstructor(const tCar& p_other, tCarElt* p_carElt)
{
    tCar car(p_other);
    car.carElt = p_carElt;
    car.ctrl = &p_carElt->ctrl;
    car.params = p_carElt->priv.carHandle;
    for (int j = 0; j < 2; j++)
    {
        car.transmission.differential[TRANS_FRONT_DIFF].inAxis[j] = &(car.wheel[j].feedBack);
        car.transmission.differential[TRANS_FRONT_DIFF].outAxis[j] = &(car.wheel[j].in);
    }

    for (int j = 0; j < 2; j++)
    {
        car.transmission.differential[TRANS_REAR_DIFF].inAxis[j] = &(car.wheel[2 + j].feedBack);
        car.transmission.differential[TRANS_REAR_DIFF].outAxis[j] = &(car.wheel[2 + j].in);
    }

    car.transmission.differential[TRANS_CENTRAL_DIFF].inAxis[0] = &(car.transmission.differential[TRANS_FRONT_DIFF].feedBack);
    car.transmission.differential[TRANS_CENTRAL_DIFF].outAxis[0] = &(car.transmission.differential[TRANS_FRONT_DIFF].in);

    car.transmission.differential[TRANS_CENTRAL_DIFF].inAxis[1] = &(car.transmission.differential[TRANS_REAR_DIFF].feedBack);
    car.transmission.differential[TRANS_CENTRAL_DIFF].outAxis[1] = &(car.transmission.differential[TRANS_REAR_DIFF].in);
    return car;
}

/// @brief function to call the internal functions of the simulator
/// @param p_data the current situation
/// @param p_action the current action
/// @return new SDAData with the new situation
SDAData SDASpeedDreams(const SDAData& p_data, SDAAction& p_action)
{
    SDAData data(p_data);

    // update the car
    data.Situation.cars[0]->ctrl.accelCmd = p_action.Accel;
    data.Situation.cars[0]->ctrl.brakeCmd = p_action.Brake;
    data.Situation.cars[0]->ctrl.steer = p_action.Steer;
    data.Situation.cars[0]->ctrl.gear = p_action.Gear;

    tCar car;

    SimCarTable[0] = CarConstructor(car, data.Situation.cars[0]);

    // update the simulator
    double elapsed = 0;
    while (elapsed <= RCM_MAX_DT_ROBOTS + RCM_MAX_DT_SIMU)
    {
        SimUpdate(&data.Situation, RCM_MAX_DT_SIMU);
        data.Situation.currentTime += RCM_MAX_DT_SIMU;
        elapsed += RCM_MAX_DT_SIMU;
    }

    return data;
}
