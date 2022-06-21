#include "SDASpeedDreams.h"

#define FLOAT_RELAXATION2(target, prev, rate) 			\
do {								\
    tdble __tmp__;						\
    __tmp__ = target;						\
    target = (prev) + (rate) * ((target) - (prev)) * 0.01f;	\
    prev = __tmp__;						\
} while (0)

/// @brief sets tCarElt to tCar
/// @param p_other the tCar car
/// @param p_carElt the tCarElt car
/// @return a new tCar
tCar SDASpeedDreams::CarConstructor(const tCar& p_other, tCarElt* p_carElt)
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
SDAData SDASpeedDreams::UpdateSimulator(const SDAData& p_data, SDAAction& p_action)
{
    SDAData data(p_data);

    // update the car
    data.Situation.cars = new tCarElt*[1] ;
    data.Situation.cars[0] = new tCarElt(p_data.Car);
    data.Situation.cars[0]->ctrl.accelCmd = p_action.Accel;
    data.Situation.cars[0]->ctrl.brakeCmd = p_action.Brake;
    data.Situation.cars[0]->ctrl.steer = p_action.Steer;
    data.Situation.cars[0]->ctrl.gear = p_action.Gear;

    SimCarTable[0] = CarConstructor(p_data.SimCar, data.Situation.cars[0]);

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

void SDASpeedDreams::SimUpdate(tSituation *s, double deltaTime)
{
    int i;
    int ncar;
    tCarElt *carElt;
    tCar *car;

    SimDeltaTime = (tdble) deltaTime;

    SimAtmosphereUpdate(s);

    for (ncar = 0; ncar < s->_ncars; ncar++)
    {
        SimCarTable[ncar].collision = 0;
        SimCarTable[ncar].blocked = 0;
    }

    for (ncar = 0; ncar < s->_ncars; ncar++)
    {
        car = &(SimCarTable[ncar]);
        carElt = car->carElt;

        if (carElt->_state & RM_CAR_STATE_NO_SIMU)
        {
            RemoveCar(car, s);
            continue;
        }
        else if (((s->_maxDammage) && (car->dammage > s->_maxDammage)) ||
                 (car->fuel == 0) ||
                 (car->carElt->_state & RM_CAR_STATE_ELIMINATED))
        {
            RemoveCar(car, s);

            if (carElt->_state & RM_CAR_STATE_NO_SIMU)
            {
                continue;
            }
        }

        if (s->_raceState & RM_RACE_PRESTART &&
            (car->carElt->_skillLevel < 3 || !(s->_features & RM_FEATURE_PENALTIES)))
        {
            car->ctrl->brakeCmd = 1.0;
            car->ctrl->clutchCmd = 1.0;
        }

        ctrlCheck(car);

        SimInstantReConfig(car);

        SimSteerUpdate(car);

        SimGearboxUpdate(car);

        SimEngineUpdateTq(car);

        if (!(s->_raceState & RM_RACE_PRESTART) || car->carElt->_skillLevel == 3)
        {
            SimCarUpdateWheelPos(car);

            SimBrakeSystemUpdate(car);

            SimAeroUpdate(car, s);


            for (i = 0; i < 2; i++)
            {
                SimWingUpdate(car, i, s);
            }


            for (i = 0; i < 4; i++)
            {
                SimWheelUpdateRide(car, i);
            }


            for (i = 0; i < 2; i++)
            {
                SimAxleUpdate(car, i);
            }


            for (i = 0; i < 4; i++)
            {
                SimWheelUpdateForce(car, i);
            }


            SimTransmissionUpdate(car);


            SimWheelUpdateRotation(car);


            SimCarUpdate(car, s);

        }
        else
        {
            SimTransmissionUpdate(car);
            SimEngineUpdateRpm(car, 0.0);
        }
    }

    SimCarCollideCars(s);

    /* printf ("%f - ", s->currentTime); */

    for (ncar = 0; ncar < s->_ncars; ncar++)
    {
        car = &(SimCarTable[ncar]);

        carElt = car->carElt;

        if (carElt->_state & RM_CAR_STATE_NO_SIMU)
        {
            continue;
        }


        SimCarUpdate2(car, s); /* telemetry */

        /* copy back the data to carElt */

        carElt->pub.DynGC = car->DynGC;
        carElt->pub.DynGCg = car->DynGCg;
        sgMakeCoordMat4(carElt->pub.posMat, carElt->_pos_X, carElt->_pos_Y, carElt->_pos_Z - carElt->_statGC_z,
                        (float) RAD2DEG(carElt->_yaw), (float) RAD2DEG(carElt->_roll), (float) RAD2DEG(carElt->_pitch));
        carElt->_trkPos = car->trkPos;

        for (i = 0; i < 4; i++)
        {
            carElt->priv.wheel[i].relPos = car->wheel[i].relPos;
            carElt->_wheelSeg(i) = car->wheel[i].trkPos.seg;
            carElt->_brakeTemp(i) = car->wheel[i].brake.temp;
            carElt->pub.corner[i] = car->corner[i].pos;
        }

        carElt->_gear = car->transmission.gearbox.gear;
        carElt->_gearNext = car->transmission.gearbox.gearNext;
        carElt->_enginerpm = car->engine.rads;
        carElt->_fuel = car->fuel;
        carElt->priv.collision |= car->collision;
        carElt->_dammage = car->dammage;

        carElt->_steerTqCenter = -car->ctrl->steer;
        carElt->_steerTqAlign = car->wheel[FRNT_RGT].torqueAlign + car->wheel[FRNT_LFT].torqueAlign;

    }
}

void SDASpeedDreams::SimAtmosphereUpdate(tSituation *s)
{
    double timeofday = SimTimeOfDay + s->currentTime;

    if ((timeofday > startDay) && (timeofday < endDay))
    {
        Tair = Tair + simDegree;
        GfLogDebug("Tair update = %.7f\n", Tair - 273.15);
    }
    else
    {
        Tair = Tair - simDegree;
        GfLogDebug("Tair update = %.7f\n", Tair - 273.15);
    }
}

void SDASpeedDreams::RemoveCar(tCar *car, tSituation *s)
{
    int i;
    tCarElt *carElt;
    tTrkLocPos trkPos;
    int trkFlag;
    tdble travelTime;
    tdble dang;

    static tdble PULL_Z_OFFSET = 3.0;
    static tdble PULL_SPD = 0.5;

    carElt = car->carElt;

    if (carElt->_state & RM_CAR_STATE_PULLUP)
    {
        carElt->_pos_Z += car->restPos.vel.z * SimDeltaTime;
        carElt->_yaw += car->restPos.vel.az * SimDeltaTime;
        carElt->_roll += car->restPos.vel.ax * SimDeltaTime;
        carElt->_pitch += car->restPos.vel.ay * SimDeltaTime;
        sgMakeCoordMat4(carElt->pub.posMat, carElt->_pos_X, carElt->_pos_Y, carElt->_pos_Z - carElt->_statGC_z,
                        (float) RAD2DEG(carElt->_yaw), (float) RAD2DEG(carElt->_roll), (float) RAD2DEG(carElt->_pitch));

        if (carElt->_pos_Z > (car->restPos.pos.z + PULL_Z_OFFSET))
        {
            carElt->_state &= ~RM_CAR_STATE_PULLUP;
            carElt->_state |= RM_CAR_STATE_PULLSIDE;

            // Moved pullside velocity computation down due to floating point error accumulation.
        }

        return;
    }

    if (carElt->_state & RM_CAR_STATE_PULLSIDE)
    {
        // Recompute speed to avoid missing the parking point due to error accumulation (the pos might be
        // in the 0-10000 range, depending on the track and vel*dt is around 0-0.001, so basically all
        // but the most significant digits are lost under bad conditions, happens e.g on e-track-4).
        // Should not lead to a division by zero because the pullside process stops if the car is within
        // [0.5, 0.5]. Do not move it back.
        travelTime = DIST(car->restPos.pos.x, car->restPos.pos.y, carElt->_pos_X, carElt->_pos_Y) / PULL_SPD;
        car->restPos.vel.x = (car->restPos.pos.x - carElt->_pos_X) / travelTime;
        car->restPos.vel.y = (car->restPos.pos.y - carElt->_pos_Y) / travelTime;

        carElt->_pos_X += car->restPos.vel.x * SimDeltaTime;
        carElt->_pos_Y += car->restPos.vel.y * SimDeltaTime;
        sgMakeCoordMat4(carElt->pub.posMat, carElt->_pos_X, carElt->_pos_Y, carElt->_pos_Z - carElt->_statGC_z,
                        (float) RAD2DEG(carElt->_yaw), (float) RAD2DEG(carElt->_roll), (float) RAD2DEG(carElt->_pitch));

        if ((fabs(car->restPos.pos.x - carElt->_pos_X) < 0.5) && (fabs(car->restPos.pos.y - carElt->_pos_Y) < 0.5))
        {
            carElt->_state &= ~RM_CAR_STATE_PULLSIDE;
            carElt->_state |= RM_CAR_STATE_PULLDN;
        }

        return;
    }

    if (carElt->_state & RM_CAR_STATE_PULLDN)
    {
        carElt->_pos_Z -= car->restPos.vel.z * SimDeltaTime;
        sgMakeCoordMat4(carElt->pub.posMat, carElt->_pos_X, carElt->_pos_Y, carElt->_pos_Z - carElt->_statGC_z,
                        (float) RAD2DEG(carElt->_yaw), (float) RAD2DEG(carElt->_roll), (float) RAD2DEG(carElt->_pitch));

        if (carElt->_pos_Z < car->restPos.pos.z)
        {
            carElt->_state &= ~RM_CAR_STATE_PULLDN;
            carElt->_state |= RM_CAR_STATE_OUT;
        }

        return;
    }

    if (carElt->_state & (RM_CAR_STATE_NO_SIMU & ~RM_CAR_STATE_PIT))
    {
        return;
    }

    if (carElt->_state & RM_CAR_STATE_PIT)
    {
        if ((s->_maxDammage) && (car->dammage > s->_maxDammage))
        {
            // Broken during pit stop.
            carElt->_state &= ~RM_CAR_STATE_PIT;
            carElt->_pit->pitCarIndex = TR_PIT_STATE_FREE;
        }
        else
        {
            return;
        }
    }

    if ((s->_maxDammage) && (car->dammage > s->_maxDammage))
    {
        carElt->_state |= RM_CAR_STATE_BROKEN;
    }
    else
    {
        carElt->_state |= RM_CAR_STATE_OUTOFGAS;
    }

    carElt->_gear = car->transmission.gearbox.gear = 0;
    carElt->_enginerpm = car->engine.rads = 0;

    if (!(carElt->_state & RM_CAR_STATE_DNF))
    {
        if (fabs(carElt->_speed_x) > 1.0)
        {
            return;
        }
    }

    carElt->_state |= RM_CAR_STATE_PULLUP;
    // RM_CAR_STATE_NO_SIMU evaluates to > 0 from here, so we remove the car from the
    // collision detection.
    SimCollideRemoveCar(car, s->_ncars);

    carElt->priv.collision = car->collision = 0;

    for(i = 0; i < 4; i++)
    {
        carElt->_skid[i] = 0;
        carElt->_wheelSpinVel(i) = 0;
        carElt->_brakeTemp(i) = 0;
    }

    carElt->pub.DynGC = car->DynGC;
    carElt->_speed_x = 0;

    // Compute the target zone for the wrecked car.
    trkPos = car->trkPos;
    if (trkPos.toRight >  trkPos.seg->width / 2.0)
    {
        while (trkPos.seg->lside != 0)
        {
            trkPos.seg = trkPos.seg->lside;
        }

        trkPos.toLeft = -3.0;
        trkFlag = TR_TOLEFT;
    }
    else
    {
        while (trkPos.seg->rside != 0)
        {
            trkPos.seg = trkPos.seg->rside;
        }

        trkPos.toRight = -3.0;
        trkFlag = TR_TORIGHT;
    }

    trkPos.type = TR_LPOS_SEGMENT;
    RtTrackLocal2Global(&trkPos, &(car->restPos.pos.x), &(car->restPos.pos.y), trkFlag);
    car->restPos.pos.z = RtTrackHeightL(&trkPos) + carElt->_statGC_z;
    car->restPos.pos.az = RtTrackSideTgAngleL(&trkPos);
    car->restPos.pos.ax = 0;
    car->restPos.pos.ay = 0;

    car->restPos.vel.z = PULL_SPD;
    travelTime = (car->restPos.pos.z + PULL_Z_OFFSET - carElt->_pos_Z) / car->restPos.vel.z;
    dang = car->restPos.pos.az - carElt->_yaw;
    FLOAT_NORM_PI_PI(dang);
    car->restPos.vel.az = dang / travelTime;
    dang = car->restPos.pos.ax - carElt->_roll;
    FLOAT_NORM_PI_PI(dang);
    car->restPos.vel.ax = dang / travelTime;
    dang = car->restPos.pos.ay - carElt->_pitch;
    FLOAT_NORM_PI_PI(dang);
    car->restPos.vel.ay = dang / travelTime;
}

// Remove wrecked cars to avoid glitches in collision detection and to improve performance.
// Called by RemoveCar.
void SDASpeedDreams::SimCollideRemoveCar(tCar *car, int nbcars)
{
    // Find the car to remove.
    int i;
    for (i = 0; i < nbcars; i++) {
        if (&SimCarTable[i] == car) {
            break;
        }
    }

    // Remove it.
    if (SimCarTable[i].shape != NULL) {
        SimCarTable[i].shape = NULL;
    }
}

void SDASpeedDreams::RtTrackLocal2Global(tTrkLocPos *p, tdble *X, tdble *Y, int flag)
{
    tdble r, a;
    tdble tr;

    tTrackSeg *seg = p->seg;
    switch (flag) {
        case TR_TOMIDDLE:
            switch(seg->type){
                case TR_STR:
                    /* Jussi Pajala: must be divided by two to get middle of the track ! */
                    tr = p->toMiddle + seg->startWidth / 2.0f;
                    *X = seg->vertex[TR_SR].x + p->toStart * seg->cos - tr * seg->sin;
                    *Y = seg->vertex[TR_SR].y + p->toStart * seg->sin + tr * seg->cos;
                    break;

                case TR_LFT:
                    a = seg->angle[TR_ZS] + p->toStart;
                    r = seg->radius - p->toMiddle;
                    *X = seg->center.x + r * sin(a);
                    *Y = seg->center.y - r * cos(a);
                    break;

                case TR_RGT:
                    a = seg->angle[TR_ZS] - p->toStart;
                    r = seg->radius + p->toMiddle;
                    *X = seg->center.x - r * sin(a);
                    *Y = seg->center.y + r * cos(a);
                    break;
            }
            break;

        case TR_TORIGHT:
            switch(seg->type){
                case TR_STR:
                    switch (seg->type2) {
                        case TR_MAIN:
                        case TR_LSIDE:
                        case TR_LBORDER:
                            tr = p->toRight;
                            break;
                        case TR_RSIDE:
                        case TR_RBORDER:
                            tr = p->toRight - seg->Kyl * p->toStart;
                            break;
                        default:
                            tr = 0;
                            break;
                    }
                    *X = seg->vertex[TR_SR].x + p->toStart * seg->cos - tr * seg->sin;
                    *Y = seg->vertex[TR_SR].y + p->toStart * seg->sin + tr * seg->cos;
                    break;

                case TR_LFT:
                    a = seg->angle[TR_ZS] + p->toStart;
                    switch (seg->type2) {
                        case TR_MAIN:
                        case TR_LSIDE:
                        case TR_LBORDER:
                            r = seg->radiusr - p->toRight ;
                            break;
                        case TR_RSIDE:
                        case TR_RBORDER:
                            r = seg->radiusl + seg->startWidth + seg->Kyl * p->toStart - p->toRight ;
                            break;
                        default:
                            r = 0;
                            break;
                    }
                    *X = seg->center.x + r * sin(a);
                    *Y = seg->center.y - r * cos(a);
                    break;

                case TR_RGT:
                    a = seg->angle[TR_ZS] - p->toStart;
                    switch (seg->type2) {
                        case TR_MAIN:
                        case TR_LSIDE:
                        case TR_LBORDER:
                            r = seg->radiusr + p->toRight ;
                            break;
                        case TR_RSIDE:
                        case TR_RBORDER:
                            r = seg->radiusl - seg->startWidth - seg->Kyl * p->toStart + p->toRight ;
                            break;
                        default:
                            r = 0;
                            break;
                    }
                    *X = seg->center.x - r * sin(a);
                    *Y = seg->center.y + r * cos(a);
                    break;
            }
            break;

        case TR_TOLEFT:
            switch(seg->type){
                case TR_STR:
                    tr = seg->startWidth + seg->Kyl * p->toStart - p->toLeft;
                    *X = seg->vertex[TR_SR].x + p->toStart * seg->cos - tr * seg->sin;
                    *Y = seg->vertex[TR_SR].y + p->toStart * seg->sin + tr * seg->cos;
                    break;

                case TR_LFT:
                    a = seg->angle[TR_ZS] + p->toStart;
                    r = seg->radiusl + p->toLeft;
                    *X = seg->center.x + r * sin(a);
                    *Y = seg->center.y - r * cos(a);
                    break;

                case TR_RGT:
                    a = seg->angle[TR_ZS] - p->toStart;
                    r = seg->radiusr + seg->startWidth + seg->Kyl * p->toStart - p->toLeft;
                    *X = seg->center.x - r * sin(a);
                    *Y = seg->center.y + r * cos(a);
                    break;
            }
            break;
    }
}

tdble SDASpeedDreams::RtTrackHeightL(tTrkLocPos *p)
{
    tdble	lg;
    tdble	tr = p->toRight;
    tTrackSeg	*seg = p->seg;
    //bool left_side = true; // Never used.
    if ((tr < 0) && (seg->rside != NULL)) {
        //    left_side = false; // Never used.

        seg = seg->rside;
        tr += seg->width;

        if ((tr < 0) && (seg->rside != NULL)) {
            seg = seg->rside;
            tr += RtTrackGetWidth(seg, p->toStart);
        }
    } else if ((tr > seg->width) && (seg->lside != NULL)) {
        tr -= seg->width;
        seg = seg->lside;
        if ((tr > seg->width) && (seg->lside != NULL)) {
            tr -= RtTrackGetWidth(seg, p->toStart);
            seg = seg->lside;
        }
    }

    switch (seg->type) {
        case TR_STR:
            lg = p->toStart;
            break;
        default:
            lg = p->toStart * seg->radius;
            break;
    }
    if (seg->style == TR_CURB) {
        // The final height = starting height + height difference due
        // to track angle + height difference due to curb (this seems
        // to be the way it is implemented in the graphics too: the
        // curb does not adding an angle to the main track, but a
        // height in global coords).
        if (seg->type2 == TR_RBORDER) {
            // alpha shows how far we've moved into this segment.
            tdble alpha = seg->width - tr;
            tdble angle = seg->angle[TR_XS] + p->toStart * seg->Kzw;
            tdble noise = 0 * sin(6.28318548 * lg) * alpha / seg->width;
            tdble start_height = seg->vertex[TR_SR].z + p->toStart * seg->Kzl;
            return start_height + tr * tan(angle) + alpha * atan2(seg->height, seg->width) + noise;

        }

        return seg->vertex[TR_SR].z + p->toStart * seg->Kzl +
               tr * (tan(seg->angle[TR_XS] + p->toStart * seg->Kzw)
                     + atan2(seg->height, seg->width)) +
               0 * sin(6.28318548 * lg) * tr / seg->width;
    }

    return seg->vertex[TR_SR].z + p->toStart * seg->Kzl + tr * tan(seg->angle[TR_XS] + p->toStart * seg->Kzw) +
           0 * sin(6.28318548 * tr) * sin(6.28318548 * lg);
}

tdble SDASpeedDreams::RtTrackSideTgAngleL(tTrkLocPos *p)
{
    switch (p->seg->type) {
        case TR_STR:
            return p->seg->angle[TR_ZS];
            break;
        case TR_RGT:
            return p->seg->angle[TR_ZS] - p->toStart;
            break;
        case TR_LFT:
            return p->seg->angle[TR_ZS] + p->toStart;
            break;
    }
    return 0;
}

tdble SDASpeedDreams::RtTrackGetWidth(tTrackSeg *seg, tdble toStart)
{
    return fabs(seg->startWidth + toStart * seg->Kyl);
}

void SDASpeedDreams::ctrlCheck(tCar *car)
{
    tTransmission	*trans = &(car->transmission);
    tClutch		*clutch = &(trans->clutch);

    /* sanity check */
#ifndef WIN32
    if (isnan(car->ctrl->accelCmd) || isinf(car->ctrl->accelCmd)) car->ctrl->accelCmd = 0;
    if (isnan(car->ctrl->brakeCmd) || isinf(car->ctrl->brakeCmd)) car->ctrl->brakeCmd = 0;
    if (isnan(car->ctrl->clutchCmd) || isinf(car->ctrl->clutchCmd)) car->ctrl->clutchCmd = 0;
    if (isnan(car->ctrl->steer) || isinf(car->ctrl->steer)) car->ctrl->steer = 0;
    if (isnan(car->ctrl->wingFrontCmd) || isinf(car->ctrl->wingFrontCmd)) car->ctrl->wingFrontCmd = 0;
    if (isnan(car->ctrl->wingRearCmd) || isinf(car->ctrl->wingRearCmd)) car->ctrl->wingRearCmd = 0;
    if (isnan(car->ctrl->brakeFrontLeftCmd) || isinf(car->ctrl->brakeFrontLeftCmd)) car->ctrl->brakeFrontLeftCmd = 0;
    if (isnan(car->ctrl->brakeFrontRightCmd) || isinf(car->ctrl->brakeFrontRightCmd)) car->ctrl->brakeFrontRightCmd = 0;
    if (isnan(car->ctrl->brakeRearLeftCmd) || isinf(car->ctrl->brakeRearLeftCmd)) car->ctrl->brakeRearLeftCmd = 0;
    if (isnan(car->ctrl->brakeRearRightCmd) || isinf(car->ctrl->brakeRearRightCmd)) car->ctrl->brakeRearRightCmd = 0;
#else
    if (isnan(car->ctrl->accelCmd)) car->ctrl->accelCmd = 0;
    if (isnan(car->ctrl->brakeCmd)) car->ctrl->brakeCmd = 0;
    if (isnan(car->ctrl->clutchCmd)) car->ctrl->clutchCmd = 0;
    if (isnan(car->ctrl->steer)) car->ctrl->steer = 0;
    if (isnan(car->ctrl->wingFrontCmd)) car->ctrl->wingFrontCmd = 0;
    if (isnan(car->ctrl->wingRearCmd)) car->ctrl->wingRearCmd = 0;
    if (isnan(car->ctrl->brakeFrontLeftCmd)) car->ctrl->brakeFrontLeftCmd = 0;
    if (isnan(car->ctrl->brakeFrontRightCmd)) car->ctrl->brakeFrontRightCmd = 0;
    if (isnan(car->ctrl->brakeRearLeftCmd)) car->ctrl->brakeRearLeftCmd = 0;
    if (isnan(car->ctrl->brakeRearRightCmd)) car->ctrl->brakeRearRightCmd = 0;
#endif

    /* When the car is broken try to send it on the track side */
    if (car->carElt->_state & RM_CAR_STATE_BROKEN)
    {
        car->ctrl->accelCmd = 0.0f;
        car->ctrl->brakeCmd = 0.1f;
        car->ctrl->gear = 0;

        if (car->trkPos.toRight >  car->trkPos.seg->width / 2.0)
        {
            car->ctrl->steer = 0.1f;
        }
        else
        {
            car->ctrl->steer = -0.1f;
        }
    }
    else if (car->carElt->_state & RM_CAR_STATE_ELIMINATED)
    {
        car->ctrl->accelCmd = 0.0f;
        car->ctrl->brakeCmd = 0.1f;
        car->ctrl->gear = 0;
        if (car->trkPos.toRight >  car->trkPos.seg->width / 2.0)
        {
            car->ctrl->steer = 0.1f;
        }
        else
        {
            car->ctrl->steer = -0.1f;
        }
    }
    else if (car->carElt->_state & RM_CAR_STATE_FINISH)
    {
        /* when the finish line is passed, continue at "slow" pace */
        car->ctrl->accelCmd = (tdble) MIN(car->ctrl->accelCmd, 0.20);
        if (car->DynGC.vel.x > 30.0)
        {
            car->ctrl->brakeCmd = (tdble) MAX(car->ctrl->brakeCmd, 0.05);
        }
    }

    /* check boundaries */
    if (car->ctrl->accelCmd > 1.0)
    {
        car->ctrl->accelCmd = 1.0;
    }
    else if (car->ctrl->accelCmd < 0.0)
    {
        car->ctrl->accelCmd = 0.0;
    }
    if (car->ctrl->brakeCmd > 1.0)
    {
        car->ctrl->brakeCmd = 1.0;
    }
    else if (car->ctrl->brakeCmd < 0.0)
    {
        car->ctrl->brakeCmd = 0.0;
    }

    if (car->ctrl->clutchCmd > 1.0)
    {
        car->ctrl->clutchCmd = 1.0;
    }
    else if (car->ctrl->clutchCmd < 0.0)
    {
        car->ctrl->clutchCmd = 0.0;
    }

    if (car->ctrl->steer > 1.0)
    {
        car->ctrl->steer = 1.0;
    }
    else if (car->ctrl->steer < -1.0)
    {
        car->ctrl->steer = -1.0;
    }

    clutch->transferValue = 1.0f - car->ctrl->clutchCmd;

    if (car->ctrl->wingFrontCmd > (float) (PI_2))
    {
        car->ctrl->wingFrontCmd = (float) (PI_2);
    }
    else if (car->ctrl->wingFrontCmd < 0.0)
    {
        car->ctrl->wingFrontCmd = 0.0;
    }

    if (car->ctrl->wingRearCmd > (float) (PI_2))
    {
        car->ctrl->wingRearCmd = (float) (PI_2);
    }
    else if (car->ctrl->wingRearCmd < 0.0)
    {
        car->ctrl->wingRearCmd = 0.0;
    }

    if (car->ctrl->brakeFrontLeftCmd < 0)
    {
        car->ctrl->brakeFrontLeftCmd = 0;
    }
    else if (car->ctrl->brakeFrontLeftCmd > 1.0f)
    {
        car->ctrl->brakeFrontLeftCmd = 1.0f;
    }

    if (car->ctrl->brakeFrontRightCmd < 0)
    {
        car->ctrl->brakeFrontRightCmd = 0;
    }
    else if (car->ctrl->brakeFrontRightCmd > 1.0f)
    {
        car->ctrl->brakeFrontRightCmd = 1.0f;
    }

    if (car->ctrl->brakeRearLeftCmd < 0) {
        car->ctrl->brakeRearLeftCmd = 0;
    }
    else if (car->ctrl->brakeRearLeftCmd > 1.0f)
    {
        car->ctrl->brakeRearLeftCmd = 1.0f;
    }

    if (car->ctrl->brakeRearRightCmd < 0) {
        car->ctrl->brakeRearRightCmd = 0;
    }
    else if (car->ctrl->brakeRearRightCmd > 1.0f)
    {
        car->ctrl->brakeRearRightCmd = 1.0f;
    }
}

void SDASpeedDreams::SimInstantReConfig(tCar *car)
{
    tCarSetupItem *setup;

    if (car->ctrl->setupChangeCmd)
    {
        setup = (car->ctrl->setupChangeCmd->setup);
    }
    else return;

    switch (car->ctrl->setupChangeCmd->type) {
        case DI_BRAKE_REPARTITION:
            SimBrakeSystemReConfig(car);
            break;
        case DI_FRONT_ANTIROLLBAR:
            SimArbReConfig(car, 0);
            break;
        case DI_REAR_ANTIROLLBAR:
            SimArbReConfig(car, 1);
            break;
        case DI_FRONT_DIFF_MAX_SLIP_BIAS:
            car->transmission.differential[TRANS_FRONT_DIFF].dSlipMax = setup->desired_value;
            setup->value = car->transmission.differential[TRANS_FRONT_DIFF].dSlipMax;
            setup->changed = FALSE;
            break;
        case DI_FRONT_DIFF_COAST_MAX_SLIP_BIAS:
            car->transmission.differential[TRANS_FRONT_DIFF].dCoastSlipMax = setup->desired_value;
            setup->value = car->transmission.differential[TRANS_FRONT_DIFF].dCoastSlipMax;
            setup->changed = FALSE;
            break;
        case DI_REAR_DIFF_MAX_SLIP_BIAS:
            car->transmission.differential[TRANS_REAR_DIFF].dSlipMax = setup->desired_value;
            setup->value = car->transmission.differential[TRANS_REAR_DIFF].dSlipMax;
            setup->changed = FALSE;
            break;
        case DI_REAR_DIFF_COAST_MAX_SLIP_BIAS:
            car->transmission.differential[TRANS_REAR_DIFF].dCoastSlipMax = setup->desired_value;
            setup->value = car->transmission.differential[TRANS_REAR_DIFF].dCoastSlipMax;
            setup->changed = FALSE;
            break;
        case DI_CENTRAL_DIFF_MAX_SLIP_BIAS:
            car->transmission.differential[TRANS_CENTRAL_DIFF].dSlipMax = setup->desired_value;
            setup->value = car->transmission.differential[TRANS_CENTRAL_DIFF].dSlipMax;
            setup->changed = FALSE;
            break;
        case DI_CENTRAL_DIFF_COAST_MAX_SLIP_BIAS:
            car->transmission.differential[TRANS_CENTRAL_DIFF].dCoastSlipMax = setup->desired_value;
            setup->value = car->transmission.differential[TRANS_CENTRAL_DIFF].dCoastSlipMax;
            setup->changed = FALSE;
            break;
    }

    car->ctrl->setupChangeCmd = NULL;
}

void SDASpeedDreams::SimBrakeSystemReConfig(tCar *car)
{/* called by SimCarReConfig() in car.cpp */
    tCarSetupItem *setupBrkRep = &(car->carElt->setup.brakeRepartition);
    tCarSetupItem *setupBrkPress = &(car->carElt->setup.brakePressure);

    if (setupBrkRep->changed) {
        car->brkSyst.rep = MIN(setupBrkRep->max, MAX(setupBrkRep->min, setupBrkRep->desired_value));
        setupBrkRep->value = car->brkSyst.rep;
        setupBrkRep->changed = FALSE;
    }

    if (setupBrkPress->changed) {
        car->brkSyst.coeff = MIN(setupBrkPress->max, MAX(setupBrkPress->min, setupBrkPress->desired_value));
        setupBrkPress->value = car->brkSyst.coeff;
        setupBrkPress->changed = FALSE;
    }
}

void SDASpeedDreams::SimArbReConfig(tCar *car, int index)
{
    tCarSetupItem *setupArbK = &(car->carElt->setup.arbSpring[index]);
    tSuspension *arb = &(car->axle[index].arbSusp);
    if (setupArbK->changed) {
        arb->spring.K = MIN(setupArbK->max, MAX(setupArbK->min, setupArbK->desired_value));
        setupArbK->value = arb->spring.K;
        setupArbK->changed = FALSE;
    }
}

void SDASpeedDreams::SimSteerUpdate(tCar *car)
{
    tdble steer, steer2;
    tdble stdelta;
    tdble tanSteer;

    /* input control */
    steer = car->ctrl->steer;
    steer *= car->steer.steerLock;
    stdelta = steer - car->steer.steer;

    if ((fabs(stdelta) / SimDeltaTime) > car->steer.maxSpeed) {
        steer = (float)(SIGN(stdelta) * car->steer.maxSpeed * SimDeltaTime + car->steer.steer);
    }

    car->steer.steer = steer;
    tanSteer = fabs(tan(steer));
    steer2 = atan2((car->wheelbase * tanSteer) , (car->wheelbase - tanSteer * car->wheeltrack));

    if (steer > 0) {
        car->wheel[FRNT_RGT].torques.x =
            car->wheel->cosax * (steer2 - car->wheel[FRNT_RGT].steer) * car->wheel[FRNT_RGT].prespinVel * car->wheel[FRNT_RGT].I / SimDeltaTime;
        car->wheel[FRNT_RGT].steer = steer2;
        car->wheel[FRNT_LFT].torques.x =
            car->wheel->cosax * (steer - car->wheel[FRNT_LFT].steer) * car->wheel[FRNT_LFT].prespinVel * car->wheel[FRNT_LFT].I / SimDeltaTime;
        car->wheel[FRNT_LFT].steer = steer;
    } else {
        car->wheel[FRNT_RGT].torques.x =
            car->wheel->cosax * (steer - car->wheel[FRNT_RGT].steer) * car->wheel[FRNT_RGT].prespinVel * car->wheel[FRNT_RGT].I / SimDeltaTime;
        car->wheel[FRNT_RGT].steer = steer;
        car->wheel[FRNT_LFT].torques.x =
            car->wheel->cosax * (-steer2 - car->wheel[FRNT_LFT].steer) * car->wheel[FRNT_LFT].prespinVel * car->wheel[FRNT_LFT].I / SimDeltaTime;
        car->wheel[FRNT_LFT].steer = -steer2;
    }
}

void SDASpeedDreams::SimGearboxUpdate(tCar *car)
{
    /* manages gear change */
    tTransmission	*trans = &(car->transmission);
    tClutch		*clutch = &(trans->clutch);
    tGearbox		*gearbox = &(trans->gearbox);
    tDifferential	*differential = NULL;

    switch(trans->type) {
        case TRANS_RWD:
            differential = &(trans->differential[TRANS_REAR_DIFF]);
            break;
        case TRANS_FWD:
            differential = &(trans->differential[TRANS_FRONT_DIFF]);
            break;
        case TRANS_4WD:
            differential = &(trans->differential[TRANS_CENTRAL_DIFF]);
            break;
    }

    trans->curI = trans->driveI[gearbox->gear + 1] * clutch->transferValue + trans->freeI[gearbox->gear +  1] * (1.0f - clutch->transferValue);

    if (car->features & FEAT_REALGEARCHANGE) {/* simuv4 new gear change code */
        if ( (car->ctrl->gear != gearbox->gear) && (car->ctrl->gear <= gearbox->gearMax) && (car->ctrl->gear >= gearbox->gearMin) ) {
            /* initiate a shift, go to neutral */
            gearbox->gearNext = car->ctrl->gear;
            if (gearbox->timeToEngage <= 0.0f) {
                if (gearbox->gearNext == 0) {gearbox->timeToEngage = 0.0f;} /* disengaging gears happens immediately */
                else {gearbox->timeToEngage = gearbox->shiftTime * 0.67f;}
                gearbox->gear = 0;
                trans->curOverallRatio = trans->overallRatio[1];
                trans->curI = trans->driveI[1] * clutch->transferValue + trans->freeI[1] * (1.0f - clutch->transferValue);
            }
        }

        if (gearbox->timeToEngage > 0.0f) {
            gearbox->timeToEngage -= SimDeltaTime;
            if (gearbox->timeToEngage <= 0.0f) {
                /* engage new gear */
                gearbox->gear = gearbox->gearNext;
                gearbox->gearNext = 0;
                trans->curOverallRatio = trans->overallRatio[gearbox->gear+1];
                trans->curI = trans->driveI[gearbox->gear + 1] * clutch->transferValue + trans->freeI[gearbox->gear +  1] * (1.0f - clutch->transferValue);
            }
        }
    } else {/* old gear change code */
        if (clutch->state == CLUTCH_RELEASING && gearbox->gear != car->ctrl->gear) {
            /* Fast change during clutch release, re-releasing it */
            clutch->state = CLUTCH_RELEASED;
        }
        if (clutch->state == CLUTCH_RELEASING) {
            clutch->timeToRelease -= SimDeltaTime;
            if (clutch->timeToRelease <= 0.0f) {
                clutch->state = CLUTCH_RELEASED;
            } else  {
                // If user does not engage clutch, we do it automatically.
                if (clutch->transferValue > 0.99f) {
                    clutch->transferValue = 0.0f;
                    trans->curI = trans->freeI[gearbox->gear + 1];

                    // NOTE: Shouldn't usage of accelerator when shifting be let
                    // to the user to decide? Especially when shifting down
                    // in order to accelerate more, this could be annoying.
                    if (car->ctrl->accelCmd > 0.1f) {
                        car->ctrl->accelCmd = 0.1f;
                    }
                }
            }
        } else if ((car->ctrl->gear > gearbox->gear)) {
            if (car->ctrl->gear <= gearbox->gearMax) {
                gearbox->gear = car->ctrl->gear;
                clutch->state = CLUTCH_RELEASING;
                if (gearbox->gear != 0) {
                    clutch->timeToRelease = clutch->releaseTime;
                } else {
                    clutch->timeToRelease = 0;
                }
                trans->curOverallRatio = trans->overallRatio[gearbox->gear+1];
                trans->curI = trans->freeI[gearbox->gear+1];
            }
        } else if ((car->ctrl->gear < gearbox->gear)) {
            if (car->ctrl->gear >= gearbox->gearMin) {
                gearbox->gear = car->ctrl->gear;
                clutch->state = CLUTCH_RELEASING;
                if (gearbox->gear != 0) {
                    clutch->timeToRelease = clutch->releaseTime;
                } else {
                    clutch->timeToRelease = 0;
                }
                trans->curOverallRatio = trans->overallRatio[gearbox->gear+1];
                trans->curI = trans->freeI[gearbox->gear+1];
            }
        }
    }


    differential->in.I = trans->curI + differential->feedBack.I;
    differential->outAxis[0]->I = trans->curI / 2.0f + differential->inAxis[0]->I;
    differential->outAxis[1]->I = trans->curI / 2.0f + differential->inAxis[1]->I;
    if (trans->type == TRANS_4WD) {
        differential = &(trans->differential[TRANS_FRONT_DIFF]);
        differential->outAxis[0]->I = trans->curI / 4.0f + differential->inAxis[0]->I;
        differential->outAxis[1]->I = trans->curI / 4.0f + differential->inAxis[1]->I;
        differential = &(trans->differential[TRANS_REAR_DIFF]);
        differential->outAxis[0]->I = trans->curI / 4.0f + differential->inAxis[0]->I;
        differential->outAxis[1]->I = trans->curI / 4.0f + differential->inAxis[1]->I;
    }
}

/* Update torque output with engine rpm and accelerator command */
void SDASpeedDreams::SimEngineUpdateTq(tCar *car)
{
    int i;
    tEngine	*engine = &(car->engine);
    tEngineCurve *curve = &(engine->curve);
    tTransmission	*trans = &(car->transmission);
    tClutch		*clutch = &(trans->clutch);

    if ((car->fuel <= 0.0f) || (car->carElt->_state & (RM_CAR_STATE_BROKEN | RM_CAR_STATE_ELIMINATED))) {
        engine->rads = 0;
        engine->Tq = 0;
        return;
    }

    // set clutch on when engine revs too low
    if (engine->rads < engine->tickover) {
        clutch->state = CLUTCH_APPLIED;
        clutch->transferValue = 0.0f;
        //		engine->rads = engine->tickover;
    }

    engine->rads = MIN(engine->rads, engine->revsMax);
    tdble EngBrkK = engine->brakeLinCoeff * engine->rads;

    if ( (engine->rads < engine->tickover) ||
        ( (engine->rads == engine->tickover) && (car->ctrl->accelCmd <= 1e-6) ) ) {
        engine->Tq = 0.0f;
        engine->rads = engine->tickover;
    } else {
        tdble Tq_max = 0.0;
        for (i = 0; i < car->engine.curve.nbPts; i++) {
            if (engine->rads < curve->data[i].rads) {
                Tq_max = engine->rads * curve->data[i].a + curve->data[i].b;
                break;
            }
        }
        tdble alpha = car->ctrl->accelCmd;
        if (engine->rads > engine->revsLimiter) {
            alpha = 0.0;
            if (car->features & FEAT_REVLIMIT) {
                engine->timeInLimiter = 0.1f;
            }
        }

        // Option TCL ...
        if (car->features & FEAT_TCLINSIMU)
        {
            if (engine->EnableTCL)
                Tq_max *= (tdble) MAX(0.0,MIN(1.0,engine->TCL));
            /*
                            if (engine->EnableTCL)
                                    fprintf(stderr,"TCL: %.1f %%\n", engine->TCL * 100);
            */
        }
        // ... Option TCL

        if ( (car->features & FEAT_REVLIMIT) && (engine->timeInLimiter > 0.0f) ) {
            alpha = 0.0;
            engine->timeInLimiter -= SimDeltaTime;
        }
        tdble Tq_cur = (Tq_max + EngBrkK) * alpha;
        engine->Tq = Tq_cur;
        engine->Tq -= EngBrkK;
        if (alpha <= 1e-6) {
            engine->Tq -= engine->brakeCoeff;
        }

        tdble cons = Tq_cur * 0.75f;
        if (cons > 0) {
            car->fuel -= (tdble) (cons * engine->rads * engine->fuelcons * 0.0000001 * SimDeltaTime);
        }
        car->fuel = (tdble) MAX(car->fuel, 0.0);
    }
}

void SDASpeedDreams::SimCarUpdateWheelPos(tCar *car)
{
    int i;
    tdble vx;
    tdble vy;
    tdble Cosz, Sinz;

    Cosz = car->Cosz;
    Sinz = car->Sinz;
    vx = car->DynGC.vel.x;
    vy = car->DynGC.vel.y;

    /* Wheels data */
    for (i = 0; i < 4; i++) {
        tdble x = car->wheel[i].staticPos.x;
        tdble y = car->wheel[i].staticPos.y;
        tdble dx = x * Cosz - y * Sinz;
        tdble dy = x * Sinz + y * Cosz;

        car->wheel[i].pos.x = car->DynGCg.pos.x + dx;
        car->wheel[i].pos.y = car->DynGCg.pos.y + dy;
        car->wheel[i].pos.z = car->DynGCg.pos.z - car->statGC.z - x * sin(car->DynGCg.pos.ay) + y * sin(car->DynGCg.pos.ax);

        car->wheel[i].bodyVel.x = vx - car->DynGC.vel.az * y;
        car->wheel[i].bodyVel.y = vy + car->DynGC.vel.az * x;
    }
}

void SDASpeedDreams::SimBrakeSystemUpdate(tCar *car)
{
    tBrakeSyst	*brkSyst = &(car->brkSyst);

    // Option ESP ...
    if (car->features & FEAT_ESPINSIMU)
    {
        tCarElt	*carElt = car->carElt;
        tdble driftAngle = atan2(carElt->_speed_Y, carElt->_speed_X) - carElt->_yaw;
        FLOAT_NORM_PI_PI(driftAngle);
        tdble absDriftAngle = fabs(driftAngle);
        //fprintf(stderr,"driftAngle: %.2f deg\n",driftAngle * 180/PI);

        // Make it be parameters later
        tdble driftAngleLimit = (tdble) (7.5 * PI / 180);       // 7.5 deg activation level
        tdble brakeSide = 0.0025f * driftAngle/driftAngleLimit; // Car side brake command
        tdble brakeBalance = 0.005f;                            // Front/Rear brake command

        if (absDriftAngle > driftAngleLimit)
        {
            car->ctrl->brakeFrontRightCmd -= brakeSide;
            car->ctrl->brakeFrontLeftCmd += brakeSide;
            car->ctrl->brakeRearRightCmd -= brakeBalance + brakeSide;
            car->ctrl->brakeRearLeftCmd -= brakeBalance - brakeSide;
        }

        if (car->ctrl->singleWheelBrakeMode == 1)
        {
            // Sanity check needed
            car->ctrl->brakeFrontRightCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeFrontRightCmd));
            car->ctrl->brakeFrontLeftCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeFrontLeftCmd));
            car->ctrl->brakeRearRightCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeRearRightCmd));
            car->ctrl->brakeRearLeftCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeRearRightCmd));

            car->wheel[FRNT_RGT].brake.pressure = brkSyst->coeff * car->ctrl->brakeFrontRightCmd;
            car->wheel[FRNT_LFT].brake.pressure = brkSyst->coeff * car->ctrl->brakeFrontLeftCmd;
            car->wheel[REAR_RGT].brake.pressure = brkSyst->coeff * car->ctrl->brakeRearRightCmd;
            car->wheel[REAR_LFT].brake.pressure = brkSyst->coeff * car->ctrl->brakeRearLeftCmd;
            //fprintf(stderr,"FR: %.2f FL: %.2f / RR: %.2f RL: %.2f\n",car->ctrl->brakeFrontRightCmd,car->ctrl->brakeFrontLeftCmd,car->ctrl->brakeRearRightCmd,car->ctrl->brakeRearLeftCmd);
        }
        else
        {
            tdble	ctrl = car->ctrl->brakeCmd;

            if (absDriftAngle > driftAngleLimit)
            {
                // Sanity check needed
                car->wheel[FRNT_RGT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl - brakeSide));
                car->wheel[FRNT_LFT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl + brakeSide));
                car->wheel[REAR_RGT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl - brakeSide - brakeBalance));
                car->wheel[REAR_LFT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl + brakeSide - brakeBalance));

                car->wheel[FRNT_RGT].brake.pressure *= brkSyst->coeff * brkSyst->rep;
                car->wheel[FRNT_LFT].brake.pressure *= brkSyst->coeff * brkSyst->rep;
                car->wheel[REAR_RGT].brake.pressure *= brkSyst->coeff * (1 - brkSyst->rep);
                car->wheel[REAR_LFT].brake.pressure *= brkSyst->coeff * (1 - brkSyst->rep);
            }
            else
            {
                ctrl *= brkSyst->coeff;
                car->wheel[FRNT_RGT].brake.pressure = car->wheel[FRNT_LFT].brake.pressure = ctrl * brkSyst->rep;
                car->wheel[REAR_RGT].brake.pressure = car->wheel[REAR_LFT].brake.pressure = ctrl * (1 - brkSyst->rep);
            }
        }
    } // ... Option ESP
    else if (car->ctrl->singleWheelBrakeMode == 1)
    {
        /*
                        car->wheel[FRNT_RGT].brake.pressure = brkSyst->coeff * MIN(car->ctrl->brakeFrontRightCmd, brkSyst->rep);
                        car->wheel[FRNT_LFT].brake.pressure = brkSyst->coeff * MIN(car->ctrl->brakeFrontLeftCmd, brkSyst->rep);
                        car->wheel[REAR_RGT].brake.pressure = brkSyst->coeff * MIN(car->ctrl->brakeRearRightCmd, (1-brkSyst->rep));
                        car->wheel[REAR_LFT].brake.pressure = brkSyst->coeff * MIN(car->ctrl->brakeRearLeftCmd, (1-brkSyst->rep));
        */
        car->wheel[FRNT_RGT].brake.pressure = brkSyst->coeff * car->ctrl->brakeFrontRightCmd;
        car->wheel[FRNT_LFT].brake.pressure = brkSyst->coeff * car->ctrl->brakeFrontLeftCmd;
        car->wheel[REAR_RGT].brake.pressure = brkSyst->coeff * car->ctrl->brakeRearRightCmd;
        car->wheel[REAR_LFT].brake.pressure = brkSyst->coeff * car->ctrl->brakeRearLeftCmd;
    }
    else
    {
        tdble	ctrl = car->ctrl->brakeCmd;
        ctrl *= brkSyst->coeff;
        car->wheel[FRNT_RGT].brake.pressure = car->wheel[FRNT_LFT].brake.pressure = ctrl * brkSyst->rep;
        car->wheel[REAR_RGT].brake.pressure = car->wheel[REAR_LFT].brake.pressure = ctrl * (1 - brkSyst->rep);
    }

    if ( (car->ctrl->ebrakeCmd > 0) && (car->wheel[REAR_RGT].brake.pressure < brkSyst->ebrake_pressure) ) {
        car->wheel[REAR_RGT].brake.pressure = car->wheel[REAR_LFT].brake.pressure = brkSyst->ebrake_pressure;
    }
}

void SDASpeedDreams::SimAeroUpdate(tCar *car, tSituation *s)
{
    tdble	hm;
    int		i;
    tCar	*otherCar;
    tdble	x, y;
    tdble	yaw, otherYaw, airSpeed, tmpas, spdang, tmpsdpang, dyaw;
    tdble	dragK = 1.0;

    x = car->DynGCg.pos.x;
    y = car->DynGCg.pos.y;
    yaw = car->DynGCg.pos.az;
    airSpeed = car->DynGC.vel.x;
    spdang = atan2(car->DynGCg.vel.y, car->DynGCg.vel.x);

    if (airSpeed > 10.0) {
        for (i = 0; i < s->_ncars; i++) {
            if (i == car->carElt->index) {
                continue;
            }
            otherCar = &(SimCarTable[i]);
            otherYaw = otherCar->DynGCg.pos.az;
            tmpsdpang = spdang - atan2(y - otherCar->DynGCg.pos.y, x - otherCar->DynGCg.pos.x);
            FLOAT_NORM_PI_PI(tmpsdpang);
            dyaw = yaw - otherYaw;
            FLOAT_NORM_PI_PI(dyaw);
            if ((otherCar->DynGC.vel.x > 10.0) &&
                (fabs(dyaw) < 0.1396)) {
                if (fabs(tmpsdpang) > 2.9671) {	    /* 10 degrees */
                    /* behind another car */
                    tmpas = (tdble) (1.0 - exp(- 2.0 * DIST(x, y, otherCar->DynGCg.pos.x, otherCar->DynGCg.pos.y) /
                                              (otherCar->aero.Cd * otherCar->DynGC.vel.x)));
                    if (tmpas < dragK) {
                        dragK = tmpas;
                    }
                } else if (fabs(tmpsdpang) < 0.1396) {	    /* 8 degrees */
                    /* before another car [not sure how much the drag should be reduced in this case. In no case it should be lowered more than 50% I think. - Christos] */
                    tmpas = (tdble) (1.0 - 0.5f * exp(- 8.0 * DIST(x, y, otherCar->DynGCg.pos.x, otherCar->DynGCg.pos.y) / (car->aero.Cd * car->DynGC.vel.x)));
                    if (tmpas < dragK) {
                        dragK = tmpas;
                    }
                }
            }
        }
    }
    car->airSpeed2 = airSpeed * airSpeed;
    tdble v2 = car->airSpeed2;

    // simulate ground effect drop off caused by non-frontal airflow (diffusor stops working etc.)

    // Never used : remove ?
    //tdble speed = sqrt(car->DynGC.vel.x*car->DynGC.vel.x + car->DynGC.vel.y*car->DynGC.vel.y);
    //tdble cosa = 1.0f;

    car->aero.drag = (tdble) (-SIGN(car->DynGC.vel.x) * car->aero.CdBody * v2 * (1.0f + (tdble)car->dammage / 10000.0f) * dragK * dragK);

    hm = 1.5f * (car->wheel[0].rideHeight + car->wheel[1].rideHeight + car->wheel[2].rideHeight + car->wheel[3].rideHeight);
    hm = hm*hm;
    hm = hm*hm;
    hm = 2 * exp(-3.0f*hm);
    car->aero.lift[0] = - car->aero.Clift[0] * v2 * hm;
    car->aero.lift[1] = - car->aero.Clift[1] * v2 * hm;
}

void SDASpeedDreams::SimWingUpdate(tCar *car, int index, tSituation* s)
{
    tWing  *wing = &(car->wing[index]);

    /* return with 0 if no wing present */
    if (wing->WingType == -1) {
        wing->forces.x = wing->forces.z = 0.0f;
        return;
    }

    if (index == 1) {
        // Check wing angle controller
        if (car->ctrl->wingControlMode == 2)
            // Update wing angle
            wing->angle = car->ctrl->wingRearCmd;
        car->aero.Cd = car->aero.CdBody - wing->Kx*sin(wing->angle);
    }
    else
        // Check wing angle controller
        if (car->ctrl->wingControlMode == 2)
            // Update wing angle
            wing->angle = car->ctrl->wingFrontCmd;

    tdble vt2 = car->airSpeed2;
    // compute angle of attack
    tdble aoa = atan2(car->DynGC.vel.z, car->DynGC.vel.x) + car->DynGCg.pos.ay;

    aoa += wing->angle;

    if (wing->WingType == 2) //thin wing works for every direction
    {
        tdble x;
        while (aoa > PI) aoa -= (tdble) (2 * PI);
        while (aoa < -PI) aoa += (tdble) (2 * PI);
        /* first calculate coefficients */
        if (aoa > PI_2)
        {
            if (aoa > PI - wing->AoStall) wing->forces.x = (tdble) (wing->Kx1 * (PI - aoa) * (PI - aoa) + wing->Kx2);
            else wing->forces.x = (tdble) (wing->Kx3 - wing->Kx4 * cos(2*aoa));
            if (aoa > PI - wing->AoStall + wing->Stallw)
            {x = (tdble)0.0;}
            else
            {
                x = (tdble) (aoa - PI + wing->AoStall - wing->Stallw);
                x = (tdble) (x * x / (x * x + wing->Stallw * wing->Stallw));
            }
            wing->forces.z = (tdble) (-(1-x) * wing->Kz1 * (aoa - PI + wing->AoAatZero) - x * (wing->Kz2 * sin(2*aoa) + wing->Kz3));
        }
        else if (aoa > 0)
        {
            if (aoa < wing->AoStall) wing->forces.x = wing->Kx1 * aoa * aoa + wing->Kx2;
            else wing->forces.x = wing->Kx3 - wing->Kx4 * cos(2*aoa);
            if (aoa < wing->AoStall - wing->Stallw)
            {x = (tdble)0.0;}
            else
            {
                x = aoa - wing->AoStall + wing->Stallw;
                x = x * x / (x * x + wing->Stallw * wing->Stallw);
            }
            wing->forces.z = -(1-x) * wing->Kz1 * (aoa - wing->AoAatZero) - x * (wing->Kz2 * sin(2*aoa) + wing->Kz3);
        }
        else if (aoa > -PI_2)
        {
            if (aoa > -wing->AoStall) wing->forces.x = wing->Kx1 * aoa * aoa + wing->Kx2;
            else wing->forces.x = wing->Kx3 - wing->Kx4 * cos(2*aoa);
            if (aoa > -wing->AoStall + wing->Stallw)
            {x = (tdble)0.0;}
            else
            {
                x = aoa + wing->AoStall - wing->Stallw;
                x = x * x / (x * x + wing->Stallw * wing->Stallw);
            }
            wing->forces.z = -(1-x) * wing->Kz1 * (aoa - wing->AoAatZero) - x * (wing->Kz2 * sin(2*aoa) - wing->Kz3);
        }
        else
        {
            if (aoa < wing->AoStall - PI) wing->forces.x = (tdble) (wing->Kx1 * (PI + aoa) * (PI + aoa) + wing->Kx2);
            else wing->forces.x = wing->Kx3 - wing->Kx4 * cos(2*aoa);
            if (aoa < wing->AoStall - wing->Stallw - PI)
            {x = (tdble)0.0;}
            else
            {
                x = (tdble) (aoa - wing->AoStall + wing->Stallw + PI);
                x = x * x / (x * x + wing->Stallw * wing->Stallw);
            }
            wing->forces.z = (tdble) (-(1-x) * wing->Kz1 * (aoa + wing->AoAatZero + PI) - x * (wing->Kz2 * sin(2*aoa) - wing->Kz3));
        }

        /* add induced drag */
        if (wing->AR > 0.001)
        {
            if (wing->forces.x > 0.0)
                wing->forces.x += (tdble) (wing->forces.z * wing->forces.z / (wing->AR * 2.8274)); //0.9*PI
            else wing->forces.x -= (tdble) (wing->forces.z * wing->forces.z / (wing->AR * 2.8274));
        }

        /* then multiply with 0.5*rho*area and the square of velocity */
        wing->forces.x *= (tdble)(- car->DynGC.vel.x * fabs(car->DynGC.vel.x) * wing->Kx * (1.0f + (tdble)car->dammage / 10000.0));
        wing->forces.z *= wing->Kx * vt2;
    }
    else if (car->DynGC.vel.x > 0.0f)
    {
        if (wing->WingType == 0)
        {
            // the sinus of the angle of attack
            tdble sinaoa = sin(aoa);

            // make drag always negative and have a minimal angle of attack
            wing->forces.x = (tdble) (wing->Kx * vt2 * (1.0f + (tdble)car->dammage / 10000.0) * MAX(fabs(sinaoa), 0.02));

            // If angle of attack is too large, no downforce, only drag
            if (fabs(aoa) > PI_2)
            {
                wing->forces.z = 0.0;
            }
            else
            {
                // 0 deg -> 30 deg as it was in simuV2.1
                if (fabs(aoa) < PI_6)
                {
                    sinaoa = sin(aoa);
                }
                else // 30 deg -> 90 deg smoothly reduced downforce
                {
                    sinaoa = (tdble) (0.25f * (1.0f - ((aoa-PI_3)/PI_6)*((aoa-PI_3)/PI_6)*((aoa-PI_3)/PI_6)));
                }
                wing->forces.z = (tdble) MIN(0.0,wing->Kz * vt2 * sinaoa);
            }
        }
        else if (wing->WingType == 1)
        {
            wing->forces.x = (tdble) (wing->Kx * vt2 * (1.0f + (tdble)car->dammage / 10000.0) * MAX(fabs(sin(aoa - wing->AoAatZRad)), 0.02));
            wing->forces.z = (tdble) MIN(0.0,wing->Kx* vt2 * CliftFromAoA(wing));
            // fprintf(stderr,"%d fz: %g (%g)\n",index,wing->forces.z,CliftFromAoA(wing));
        }
    }
    else
        wing->forces.x = wing->forces.z = 0.0f;
}

void SDASpeedDreams::SimWheelUpdateRide(tCar *car, int index)
{
    tWheel *wheel = &(car->wheel[index]);
    tdble Zroad;

    // compute suspension travel
    RtTrackGlobal2Local(car->trkPos.seg, wheel->pos.x, wheel->pos.y, &(wheel->trkPos), TR_LPOS_SEGMENT);
    wheel->zRoad = Zroad = RtTrackHeightL(&(wheel->trkPos));

    // Wheel susp.x is not the wheel movement, look at SimSuspCheckIn, it becomes there scaled with
    // susp->spring.bellcrank, so we invert this here.

    tdble new_susp_x = (wheel->susp.x - wheel->susp.v * SimDeltaTime) / wheel->susp.spring.bellcrank;
    tdble max_extend =  wheel->pos.z - Zroad;
    wheel->rideHeight = max_extend;

    if (max_extend > new_susp_x + 0.01)
    {
        wheel->susp.state = SIM_WH_INAIR;
    }
    else
    {
        wheel->susp.state = 0;
    }

    if (max_extend < new_susp_x)
    {
        new_susp_x = max_extend;
    }

    tdble prex = wheel->susp.x;
    tdble prev = wheel->susp.v;
    wheel->susp.x = new_susp_x;

    // verify the suspension travel, beware, wheel->susp.x will be scaled by SimSuspCheckIn
    SimSuspCheckIn(&(wheel->susp));
    wheel->susp.v = (prex - wheel->susp.x) / SimDeltaTime;
    wheel->susp.a = (prev - wheel->susp.v) / SimDeltaTime;

    // update wheel brake
    SimBrakeUpdate(car, wheel, &(wheel->brake));

    // Option TCL ...
    if (car->features & FEAT_TCLINSIMU)
    {
        if (index == 3)
        {	// After using the values for the last wheel
            tEngine	*engine = &(car->engine);
            engine->TCL = 1.0;			// Reset the TCL accel command
        }
    }
    // ... Option TCL
}

tdble SDASpeedDreams::CliftFromAoA(tWing* wing)
{
    tdble angle = (tdble) (wing->angle * 180/PI);
    //fprintf(stderr,"wing->angle: %g rad = angle: %g deg\n",wing->angle,angle);

    wing->Kz_org = 4.0f * wing->Kx;

    if (angle <= wing->AoAatMax)
    {
        wing->a = wing->f * (angle + wing->AoAOffset);
        //fprintf(stderr,"a: %g\n",wing->a);
        double s = sin(wing->a/180.0*PI);
        //fprintf(stderr,"s: %g\n",s);
        return (tdble)(s * s * (wing->CliftMax + wing->d) - wing->d);
    }
    else
    {
        wing->a = (angle - wing->AoAatMax - 90.0f);
        //fprintf(stderr,"a: %g F(a): %g\n",wing->a,F(wing));
        return (tdble)((wing->CliftMax - F(wing) * (wing->CliftMax - wing->CliftAsymp)) * wing->Kx);
    }
}

tdble SDASpeedDreams::F(tWing* wing)
{
    return 1 - exp( pow(-(wing->a / wing->b),wing->c));
}

void SDASpeedDreams::RtTrackGlobal2Local(tTrackSeg *segment, tdble X, tdble Y, tTrkLocPos *p, int type)
{
    int 	segnotfound = 1;
    tdble 	x, y;
    tTrackSeg 	*seg = segment;
    tTrackSeg 	*sseg;
    tdble 	theta, a2;
    int 	depl = 0;
    tdble	curWidth;

    p->type = type;

    while (segnotfound) {

        switch(seg->type) {
            case TR_STR:
                /* rotation */
                tdble ts;

                x = X - seg->vertex[TR_SR].x;
                y = Y - seg->vertex[TR_SR].y;
                ts = x * seg->cos + y * seg->sin;
                p->seg = seg;
                p->toStart = ts;
                p->toRight = y * seg->cos - x * seg->sin;
                if ((ts < 0) && (depl < 1)) {
                    /* get back */
                    seg = seg->prev;
                    depl = -1;
                } else if ((ts > seg->length) && (depl > -1)) {
                    seg = seg->next;
                    depl = 1;
                } else {
                    segnotfound = 0;
                }
                break;

            case TR_LFT:
                /* rectangular to polar */
                x = X - seg->center.x;
                y = Y - seg->center.y;
                a2 = seg->arc / 2.0f;
                theta = atan2(y, x) - (seg->angle[TR_CS] + a2);
                FLOAT_NORM_PI_PI(theta);
                p->seg = seg;
                p->toStart = theta + a2;
                p->toRight = seg->radiusr - sqrt(x*x + y*y);
                if ((theta < -a2) && (depl < 1)) {
                    seg = seg->prev;
                    depl = -1;
                } else if ((theta > a2) && (depl > -1)) {
                    seg = seg->next;
                    depl = 1;
                } else {
                    segnotfound = 0;
                }
                break;

            case TR_RGT:
                /* rectangular to polar */

                x = X - seg->center.x;
                y = Y - seg->center.y;
                a2 = seg->arc / 2.0f;
                theta = seg->angle[TR_CS] - a2 - atan2(y, x);
                FLOAT_NORM_PI_PI(theta);
                p->seg = seg;
                p->toStart = theta + a2;
                p->toRight = sqrt(x*x + y*y) - seg->radiusr;
                if ((theta < -a2) && (depl < 1)) {
                    seg = seg->prev;
                    depl = -1;
                } else if ((theta > a2) && (depl > -1)) {
                    seg = seg->next;
                    depl = 1;
                } else {
                    segnotfound = 0;
                }
                break;
        }
    }

    /* The track is of constant width */
    /* This is subject to change */
    p->toMiddle = p->toRight - seg->width / 2.0f;
    p->toLeft = seg->width - p->toRight;

    /* Consider all the track with the sides */
    /* Stay on main segment */
    if (type == TR_LPOS_TRACK) {
        if (seg->rside != NULL) {
            sseg = seg->rside;
            p->toRight += RtTrackGetWidth(sseg, p->toStart);
            sseg = sseg->rside;
            if (sseg) {
                p->toRight += RtTrackGetWidth(sseg, p->toStart);
            }
        }
        if (seg->lside != NULL) {
            sseg = seg->lside;
            p->toLeft += RtTrackGetWidth(sseg, p->toStart);
            sseg = sseg->lside;
            if (sseg) {
                p->toLeft += RtTrackGetWidth(sseg, p->toStart);
            }
        }
    }

    /* Relative to a segment, change to the side segment if necessary */
    if (type == TR_LPOS_SEGMENT) {
        if ((p->toRight < 0) && (seg->rside != NULL)) {
            sseg = seg->rside;
            p->seg = sseg;
            curWidth = RtTrackGetWidth(sseg, p->toStart);
            p->toRight +=  curWidth;
            p->toLeft -= seg->width;
            p->toMiddle += (seg->width + curWidth) / 2.0f;
            if ((p->toRight < 0) && (sseg->rside != NULL)) {
                p->toLeft -= curWidth;
                p->toMiddle += curWidth / 2.0f;
                seg = sseg;
                sseg = seg->rside;
                curWidth = RtTrackGetWidth(sseg, p->toStart);
                p->seg = sseg;
                p->toRight +=  curWidth;
                p->toMiddle += curWidth / 2.0f;
            }
        } else if ((p->toLeft < 0) && (seg->lside != NULL)) {
            sseg = seg->lside;
            p->seg = sseg;
            curWidth = RtTrackGetWidth(sseg, p->toStart);
            p->toRight += -seg->width;
            p->toMiddle -= (seg->width + curWidth) / 2.0f;
            p->toLeft += curWidth;
            if ((p->toLeft < 0) && (sseg->lside != NULL)) {
                p->toRight -= curWidth;
                p->toMiddle -= curWidth / 2.0f;
                seg = sseg;
                sseg = seg->lside;
                curWidth = RtTrackGetWidth(sseg, p->toStart);
                p->seg = sseg;
                p->toMiddle -= curWidth / 2.0f;
                p->toLeft += curWidth;
            }
        }
    }
}

void SDASpeedDreams::SimSuspCheckIn(tSuspension *susp)
{
    /*susp->state = 0;*/
    /* note: susp->state is reset in SimWheelUpdateRide in wheel.cpp */
    if (susp->x < susp->spring.packers) {
        susp->x = susp->spring.packers;
        susp->state |= SIM_SUSP_COMP;
    }
    if (susp->x >= susp->spring.xMax) {
        susp->x = susp->spring.xMax;
        susp->state |= SIM_SUSP_EXT;
    }
    susp->x *= susp->spring.bellcrank;
}

void SDASpeedDreams::SimBrakeUpdate(tCar *car, tWheel *wheel, tBrake *brake)
{
    brake->Tq = brake->coeff * brake->pressure;

    // Option ABS ...
    if (car->features & FEAT_ABSINSIMU)
    {
        if (brake->EnableABS)
            brake->Tq *= brake->ABS;
    }
    // ... Option ABS

    // Option TCL ...
    if (car->features & FEAT_TCLINSIMU)
    {
        // Brake most spinning wheel
        tdble TCL_BrakeScale = 125.0f;	// Make it be a parameter later
        brake->Tq += (tdble) MAX(0.0,MIN(5000.0,TCL_BrakeScale * brake->TCL)); // Sanity check
        brake->TCL = 0.0; // Reset for next timestep
    }
    // ... Option TCL

    brake->temp -= (tdble) (fabs(car->DynGC.vel.x) * 0.0001 + 0.0002);
    if (brake->temp < 0 ) brake->temp = 0;
    brake->temp += (tdble) (brake->pressure * brake->radius * fabs(wheel->spinVel) * 0.00000000005);
    if (brake->temp > 1.0) brake->temp = 1.0;
}

void SDASpeedDreams::SimAxleUpdate(tCar *car, int index)
{
    tAxle *axle = &(car->axle[index]);
    tdble str, stl, sgn, vtl, vtr;

    str = car->wheel[index*2].susp.x;
    stl = car->wheel[index*2+1].susp.x;
    vtr = car->wheel[index*2].susp.v;
    vtl = car->wheel[index*2+1].susp.v;

    sgn = (tdble) (SIGN(stl - str));
    axle->arbSusp.x = fabs(stl - str);
    tSpring *spring = &(axle->arbSusp.spring);

    // To save CPU power we compute the force here directly. Just the spring
    // is considered.
    tdble f;
    f = spring->K * axle->arbSusp.x;

    // right
    car->wheel[index*2].axleFz =  + sgn * f;
    // left
    car->wheel[index*2+1].axleFz = - sgn * f;

    /* heave/center spring */
    axle->heaveSusp.x = (tdble) (0.5 * (stl + str));
    axle->heaveSusp.v = (tdble) (0.5 * (vtl + vtr));
    SimSuspUpdate(&(axle->heaveSusp));
    f = (tdble) (0.5 * axle->heaveSusp.force);
    car->wheel[index*2].axleFz3rd = f;
    car->wheel[index*2+1].axleFz3rd = f;
}

void SDASpeedDreams::SimSuspUpdate(tSuspension *susp)
{
    tdble prevforce = susp->force;
    susp->force = (springForce(susp) + damperForce(susp) + susp->inertance * susp->a) * susp->spring.bellcrank;
    if (susp->force * prevforce < 0.0) {susp->force = 0.0;}
}

tdble SDASpeedDreams::springForce(tSuspension *susp)
{
    tSpring *spring = &(susp->spring);
    tdble f;

    /* K is < 0 */
    f = spring->K * (susp->x - spring->x0) + spring->F0;
    if (f < 0.0f) {
        f = 0.0f;
    }

    return f;
}

tdble SDASpeedDreams::damperForce(tSuspension *susp)
{
    tDamperDef *dampdef;
    tdble     f;
    tdble     av;
    tdble     v;

    v = susp->v;

    if (fabs(v) > 10.0f) {
        v = (float)(SIGN(v) * 10.0);
    }

    if (v < 0.0f) {
        /* rebound */
        dampdef = &(susp->damper.rebound);
    } else {
        /* bump */
        dampdef = &(susp->damper.bump);
    }

    av = fabs(v);
    if (av < dampdef->v1) {
        f = (dampdef->C1 * av + dampdef->b1);
    } else {
        f = (dampdef->C2 * av + dampdef->b2);
    }

    f *= (float)(SIGN(v));

    return f;
}

void SDASpeedDreams::SimWheelUpdateForce(tCar *car, int index)
{
    tWheel *wheel = &(car->wheel[index]);
    tdble axleFz = wheel->axleFz;
    tdble vt, v, v2, wrl;  // wheel related velocity
    tdble Fn, Ft;
    tdble waz;
    tdble CosA, SinA;
    tdble s, sa, sx, sy;  // slip vector
    tdble stmp, F, Bx;
    tdble mu;
    tdble MaxTorqueSlipAngle;
    tdble tireCond = 1.0;
    tdble reaction_force = 0.0f;
    wheel->state = 0;

    // VERTICAL STUFF CONSIDERING SMALL PITCH AND ROLL ANGLES
    // update suspension force
    SimSuspUpdate(&(wheel->susp));
    // check suspension state
    wheel->state |= wheel->susp.state;

    if (((wheel->state & SIM_SUSP_EXT) == 0) && ((wheel->state & SIM_WH_INAIR) == 0))
    {
        wheel->forces.z = axleFz + wheel->susp.force + wheel->axleFz3rd;

        if (car->features & FEAT_FIXEDWHEELFORCE)
        {
            wheel->susp.v -= wheel->susp.spring.bellcrank * SimDeltaTime * wheel->forces.z / wheel->mass;
        }
        else
        {
            wheel->susp.v -= wheel->susp.spring.bellcrank * SimDeltaTime * wheel->susp.force / wheel->mass;
        }

        if (wheel->forces.z < 0.0f)
        {
            wheel->forces.z = 0.0f;
        }
    }
    else
    {
        if (wheel->state & SIM_SUSP_EXT)
        {
            /* calculate the force needed to reach susp->spring.xMax
             * it becomes 0 from the 2. time step being extended
             * works even if both SIM_SUSP_EXT and SIM_WH_INAIR is set */
            wheel->forces.z = -wheel->susp.a * wheel->mass / wheel->susp.spring.bellcrank;
            wheel->susp.v = 0.0f;
        }
        else
        {  // SIM_WH_INAIR is set, but SIM_SUSP_EXT is not
            wheel->forces.z = axleFz + wheel->susp.force + wheel->axleFz3rd;

            if (car->features & FEAT_FIXEDWHEELFORCE)
            {
                wheel->susp.v -= wheel->susp.spring.bellcrank * SimDeltaTime * wheel->forces.z / wheel->mass;
            }
            else
            {
                wheel->susp.v -= wheel->susp.spring.bellcrank * SimDeltaTime * wheel->susp.force / wheel->mass;
            }

            wheel->forces.z = 0.0f; /* zero for zero grip and prevent getting into the air */
        }
    }

    reaction_force = wheel->forces.z;

    // update wheel coord, center relative to GC
    wheel->relPos.z = -wheel->susp.x / wheel->susp.spring.bellcrank + wheel->radius;

    // HORIZONTAL FORCES
    waz = wheel->steer + wheel->staticPos.az;
    CosA = cos(waz);
    SinA = sin(waz);

    // tangent velocity.
    vt = wheel->bodyVel.x * CosA + wheel->bodyVel.y * SinA;
    v2 = wheel->bodyVel.x * wheel->bodyVel.x + wheel->bodyVel.y * wheel->bodyVel.y;
    v = sqrt(v2);

    // slip angle ? from [0 =  means the tire is going straight ahead (no slip)]
    //              to   [3-6 = there is slip]
    if (v < 0.000001f)
    {
        sa = 0.0f;
    }
    else
    {
        sa = atan2(wheel->bodyVel.y, wheel->bodyVel.x) - waz;
    }

    FLOAT_NORM_PI_PI(sa);

    // slip ratio = the spin velocity divided by its actual world velocity. A slip ratio of -1 means full braking lock; a ratio of 0 means the tire is spinning at the exact same rate as the road is disappearing below it. A slip ratio of 1 means it's spinning.
    wrl = wheel->spinVel * wheel->radius;

    if ((wheel->state & SIM_SUSP_EXT) != 0)
    {
        sx = sy = 0.0f;
    }
    else if (v < 0.000001f)
    {
        if (car->features & FEAT_SLOWGRIP)
        {
            sx = -wrl;
        }
        else
        {
            sx = wrl;
        }

        sy = 0.0f;
    }
    else
    {
        if (car->features & FEAT_SLOWGRIP)
        {
            sx = (vt - wrl) / MAX(fabs(vt), 1.0f);  // avoid divergence
            sy = sin(sa);
        }
        else
        {
            sx = (vt - wrl) / fabs(vt);
            sy = sin(sa);
        }
    }

    Ft = 0.0f;
    Fn = 0.0f;
    s = sqrt(sx * sx + sy * sy);

    {
        // calculate _skid and _reaction for sound.
        if (v < 2.0f)
        {
            car->carElt->_skid[index] = 0.0f;
        }
        else
        {
            car->carElt->_skid[index] = MIN(1.0f, (s * reaction_force * 0.0002f));
        }

        car->carElt->_reaction[index] = reaction_force;
    }

    stmp = MIN(s, 150.0f);

    // MAGIC FORMULA
    Bx = wheel->mfB * stmp;
    F = sin(wheel->mfC * atan(Bx * (1.0f - wheel->mfE) + wheel->mfE * atan(Bx))) * (1.0f + stmp * simSkidFactor[car->carElt->_skillLevel]);

    // load sensitivity
    mu = wheel->mu * (wheel->lfMin + (wheel->lfMax - wheel->lfMin) * exp(wheel->lfK * wheel->forces.z / wheel->opLoad));

    // temperature and degradation
    if (car->features & FEAT_TIRETEMPDEG)
    {
        tireCond = 1 - wheel->muTmult * (wheel->Ttire - wheel->Topt) * (wheel->Ttire - wheel->Topt);

        if (wheel->treadDepth > wheel->critTreadDepth)
        {
            tireCond *= wheel->muTDmult[1] * wheel->treadDepth + wheel->muTDoffset[1];
        }
        else
        {
            tireCond *= wheel->muTDmult[0] * wheel->treadDepth + wheel->muTDoffset[0];
        }

        tireCond = (tdble)MAX(tireCond, 0.1);
        mu *= tireCond;
    }

    F *= wheel->forces.z * mu * wheel->trkPos.seg->surface->kFriction; /* coeff */

    /* aligning torque for force feedback */
    if ((s > 0.000001f) && (v > 1.5f))
    {
        // maximal torque at wheel slip angle = TorqueRatio * wheel slip angle at maximal side force
        MaxTorqueSlipAngle = wheel->AlignTqFactor * asin(car->carElt->_wheelSlipOpt(index));
        wheel->torqueAlign = 0.025 * wheel->forces.z * mu * wheel->trkPos.seg->surface->kFriction * sin(2.0f * atan(sa / MaxTorqueSlipAngle));
    }
    else
        wheel->torqueAlign = 0.0f;

// For debugging weather simultation on some tracks
#ifdef SD_DEBUG
        // GfLogDebug("Simu v2.1 kFriction : %f   ", wheel->trkPos.seg->surface->kFriction);
#endif

    wheel->rollRes = wheel->forces.z * wheel->trkPos.seg->surface->kRollRes;
    car->carElt->priv.wheel[index].rollRes = wheel->rollRes;

    if (s > 0.000001f)
    {
        // wheel axis based
        Ft -= F * sx / s;
        Fn -= F * sy / s;
    }
    else
    {
        Ft -= F;
    }

    if (!(car->features & FEAT_SLOWGRIP))
    {
        FLOAT_RELAXATION2(Fn, wheel->preFn, 50.0f);
        FLOAT_RELAXATION2(Ft, wheel->preFt, 50.0f);
    }

    wheel->relPos.az = waz;

    wheel->forces.x = Ft * CosA - Fn * SinA;
    wheel->forces.y = Ft * SinA + Fn * CosA;
    wheel->spinTq = Ft * wheel->radius;
    wheel->sa = sa;
    wheel->sx = sx;

    wheel->feedBack.spinVel = wheel->spinVel;
    wheel->feedBack.Tq = wheel->spinTq;
    wheel->feedBack.brkTq = wheel->brake.Tq;

    car->carElt->_wheelFx(index) = wheel->forces.x;
    car->carElt->_wheelFy(index) = wheel->forces.y;
    car->carElt->_wheelFz(index) = wheel->forces.z;
    car->carElt->_wheelSlipNorm(index) = stmp;
    car->carElt->_wheelSlipSide(index) = sy * v;
    car->carElt->_wheelSlipAccel(index) = sx * v;
    car->carElt->_reaction[index] = reaction_force;
    car->carElt->_tyreEffMu(index) = mu;

    tdble Work = 0.0;

    /* update tire temperature and degradation */
    if (car->features & FEAT_TIRETEMPDEG)
    {
        // heat from the work of friction
        Work = (wheel->forces.x * (wrl * CosA - wheel->bodyVel.x) + wheel->forces.y * (wrl * SinA - wheel->bodyVel.y)) * SimDeltaTime;
        wheel->Ttire += Work * wheel->heatingm;
        // air cooling
        wheel->Ttire -= wheel->aircoolm * (1 + wheel->speedcoolm * v) * (wheel->Ttire - Tair) * SimDeltaTime;

        // tire wear
        if (wheel->treadDepth > 0.0)
        {
            if (/*(car->carElt->_skillLevel < 2) && */ (car->carElt->_skid[index] > 0.1f))  // For test
                wheel->treadDepth -= 20.0 * car->carElt->_skid[index] * wheel->wearrate * Work;
            else
                wheel->treadDepth -= wheel->wearrate * Work;
        }
        else
        {
            wheel->treadDepth = 0.0;
        }  // note: lets it go to slightly negative for one cycle

        // filling carElt
        car->carElt->_tyreT_in(index) = wheel->Ttire;
        car->carElt->_tyreT_mid(index) = wheel->Ttire;
        car->carElt->_tyreT_out(index) = wheel->Ttire;
        car->carElt->_tyreCondition(index) = tireCond;
        car->carElt->_tyreTreadDepth(index) = wheel->treadDepth;
        car->carElt->_tyreCritTreadDepth(index) = wheel->critTreadDepth;
    }

    // Option TCL ...
    if (car->features & FEAT_TCLINSIMU)
    {
        tdble TCL_SlipScale = 1.00f;  // Make it be a parameter later
        tdble TCL_AccelScale = 0.9f;  // Make it be a parameter later

        tEngine *engine = &(car->engine);  // Get engine

        if (sx < -TCL_SlipScale)  // Slip is over our limit
        {                         // Store the TCL_Brake command for this wheel
            wheel->brake.TCL = -sx;
            // Store the minimum TCL_Accel command for the engine
            engine->TCL = (tdble)MIN(TCL_AccelScale * wheel->brake.TCL, engine->TCL);
            // fprintf(stderr,"sx: %.1f TCL: %.3f %%\n",sx,wheel->brake.TCL);
        };
    }
    // ... Option TCL

    // Option ABS ...
    if (car->features & FEAT_ABSINSIMU)
    {
        tdble ABS_SlipScale = 0.1f;   // Make it be a parameter later
        tdble ABS_BrakeScale = 1.0f;  // Make it be a parameter later

        // If slip is over the limit, reduce brake command for this wheel
        if (sx > ABS_SlipScale)
            wheel->brake.ABS = (tdble)MAX(0.0, MIN(1.0, 1 - ABS_BrakeScale * sx));
        else
            wheel->brake.ABS = 1.0f;
    }
    // ... Option ABS
}

void SDASpeedDreams::SimTransmissionUpdate(tCar *car)
{
    tTransmission	*trans = &(car->transmission);
    tClutch		*clutch = &(trans->clutch);
    tGearbox		*gearbox = &(trans->gearbox);
    tDifferential	*differential, *differential0, *differential1;
    tdble		transfer = MIN(clutch->transferValue * 3.0f, 1.0f);

    switch(trans->type) {
        case TRANS_RWD:
            differential = &(trans->differential[TRANS_REAR_DIFF]);
            differential->in.Tq = (car->engine.Tq_response + car->engine.Tq) * trans->curOverallRatio * transfer * trans->gearEff[gearbox->gear+1];
            SimDifferentialUpdate(car, differential, 1);
            SimUpdateFreeWheels(car, 0);
            /* 	printf("s0 %f - s1 %f (%f)	inTq %f -- Tq0 %f - Tq1 %f (%f)\n", */
            /* 	       differential->outAxis[0]->spinVel, differential->outAxis[1]->spinVel, differential->outAxis[0]->spinVel - differential->outAxis[1]->spinVel, */
            /* 	       differential->in.Tq, */
            /* 	       differential->outAxis[0]->Tq, differential->outAxis[1]->Tq, differential->outAxis[0]->Tq - differential->outAxis[1]->Tq); */
            break;
        case TRANS_FWD:
            differential = &(trans->differential[TRANS_FRONT_DIFF]);
            differential->in.Tq = (car->engine.Tq_response + car->engine.Tq) * trans->curOverallRatio * transfer * trans->gearEff[gearbox->gear+1];
            SimDifferentialUpdate(car, differential, 1);
            SimUpdateFreeWheels(car, 1);
            /* 	printf("s0 %f - s1 %f (%f)	inTq %f -- Tq0 %f - Tq1 %f (%f)\n", */
            /* 	       differential->outAxis[0]->spinVel, differential->outAxis[1]->spinVel, differential->outAxis[0]->spinVel - differential->outAxis[1]->spinVel, */
            /* 	       differential->in.Tq, */
            /* 	       differential->outAxis[0]->Tq, differential->outAxis[1]->Tq, differential->outAxis[0]->Tq - differential->outAxis[1]->Tq); */
            break;
        case TRANS_4WD:
            differential = &(trans->differential[TRANS_CENTRAL_DIFF]);
            differential0 = &(trans->differential[TRANS_FRONT_DIFF]);
            differential1 = &(trans->differential[TRANS_REAR_DIFF]);

            differential->in.Tq = (car->engine.Tq_response + car->engine.Tq) * trans->curOverallRatio * transfer * trans->gearEff[gearbox->gear+1];
            differential->inAxis[0]->spinVel = (differential0->inAxis[0]->spinVel + differential0->inAxis[1]->spinVel) / 2.0f;
            differential->inAxis[1]->spinVel = (differential1->inAxis[0]->spinVel + differential1->inAxis[1]->spinVel) / 2.0f;
            //		differential->inAxis[0]->Tq = (differential0->inAxis[0]->Tq + differential0->inAxis[1]->Tq) / differential->ratio;
            //		differential->inAxis[1]->Tq = (differential1->inAxis[0]->Tq + differential1->inAxis[1]->Tq) / differential->ratio;
            differential->inAxis[0]->Tq = 0;
            differential->inAxis[1]->Tq = 0;
            //		differential->inAxis[0]->brkTq = (differential0->inAxis[0]->brkTq + differential0->inAxis[1]->brkTq) / differential->ratio;
            //		differential->inAxis[1]->brkTq = (differential1->inAxis[0]->brkTq + differential1->inAxis[1]->brkTq) / differential->ratio;
            differential->inAxis[0]->brkTq = 0;
            differential->inAxis[1]->brkTq = 0;

            SimDifferentialUpdate(car, differential, 1);
            /* 	printf("\nCentral : s0 %f - s1 %f (%f)	inTq %f -- Tq0 %f - Tq1 %f (%f)\n", */
            /* 	       differential->outAxis[0]->spinVel, differential->outAxis[1]->spinVel, differential->outAxis[0]->spinVel - differential->outAxis[1]->spinVel, */
            /* 	       differential->in.Tq, */
            /* 	       differential->outAxis[0]->Tq, differential->outAxis[1]->Tq, differential->outAxis[0]->Tq - differential->outAxis[1]->Tq); */

            differential = differential0;
            SimDifferentialUpdate(car, differential, 0);
            /* 	printf("Front   : s0 %f - s1 %f (%f)	inTq %f -- Tq0 %f - Tq1 %f (%f)\n", */
            /* 	       differential->outAxis[0]->spinVel, differential->outAxis[1]->spinVel, differential->outAxis[0]->spinVel - differential->outAxis[1]->spinVel, */
            /* 	       differential->in.Tq, */
            /* 	       differential->outAxis[0]->Tq, differential->outAxis[1]->Tq, differential->outAxis[0]->Tq - differential->outAxis[1]->Tq); */

            differential = differential1;
            SimDifferentialUpdate(car, differential, 0);
            /* 	printf("Rear    : s0 %f - s1 %f (%f)	inTq %f -- Tq0 %f - Tq1 %f (%f)\n", */
            /* 	       differential->outAxis[0]->spinVel, differential->outAxis[1]->spinVel, differential->outAxis[0]->spinVel - differential->outAxis[1]->spinVel, */
            /* 	       differential->in.Tq, */
            /* 	       differential->outAxis[0]->Tq, differential->outAxis[1]->Tq, differential->outAxis[0]->Tq - differential->outAxis[1]->Tq); */
            break;
    }
}

void SDASpeedDreams::SimDifferentialUpdate(tCar *car, tDifferential *differential, int first)
{
    tdble   DrTq, DrTq0, DrTq1;
    tdble   ndot0, ndot1;
    tdble   spinVel0, spinVel1;
    tdble   inTq0, inTq1;
    tdble   spdRatio/*, spdRatioMax*/;
    tdble   /*deltaSpd,*/ deltaTq;
    tdble   BrTq;
    tdble   engineReaction;
    tdble   meanv;

    if (differential->type == DIFF_SPOOL) {
        updateSpool(car, differential, first);
        return;
    }

    DrTq = differential->in.Tq * differential->efficiency;

    spinVel0 = differential->inAxis[0]->spinVel;
    spinVel1 = differential->inAxis[1]->spinVel;

    inTq0 = differential->inAxis[0]->Tq;
    inTq1 = differential->inAxis[1]->Tq;


    spdRatio = fabs(spinVel0 + spinVel1);
    if (spdRatio != 0) {
        spdRatio = fabs(spinVel0 - spinVel1) / spdRatio;

        switch (differential->type) {
            case DIFF_FREE:
                // I would think that the following is what a FREE
                // differential should look like, with both wheels
                // independent and linked through a spider gear.
                //
                // The reaction from each wheel is transmitted back to the
                // spider gear. If both wheels react equally, then the
                // spider gear does not turn. If one of the wheel is
                // immobile, so that DrTq/2=inTq0 for example, then the
                // reaction does not act against the drivetrain, but since
                // the spider gear can turn freely, it acts on the other wheel.
                //
                // This system is equivalent to a rotating gear attached
                // in between two parallel surfaces, with DrTq being
                // equivalent to a force acting in the center of the
                // gear. If one surface is fixed, only the other surface
                // moves and all the force is 'transferred' to the moving
                // surface. Or, the way I like to think of it, the
                // immobile surface reacts with an equal and opposite
                // force[1] that cancels DrTq/2 exactly and which is
                // transmitted directly with the rotating gear to the
                // other, free, surface.
                //
                //
                // A lot of explanation for 3 lines of code..  TODO: Check
                // what bias would mean in such a system. Would it be
                // implemented between the spider and the wheels?  Or
                // between the spider and the drivetrain? If the latter
                // then it meanst the spider would always be turning, even
                // under an even load. I think in this case it is safest
                // to ignore it completely because it is frequently used
                // in cars with just FWD or RWD, and very frequently in
                // just the front part of 4WD cars, while the default
                // differential bias setting is 0.1...
                //
                // [1] For an object to remain at rest, all forces acting
                // on it must sum to 0.

                {
                    float spiderTq = inTq1 - inTq0;
                    DrTq0 = DrTq*0.5f + spiderTq;
                    DrTq1 = DrTq*0.5f - spiderTq;
                }
                break;


            case DIFF_LIMITED_SLIP:
                // Limited slip differential with:
                // - Gradual frictive locking
                // - Open differential behaviour when not locked
                //
                // The spider gear transfers torque between the two axles
                // When DrTq=lockInputTq, then the locking is at 66% (and
                // almost 100% at double the torque).  When the
                // differential is locked, there is a pressure applied due
                // to the different amount of spin of each wheel.  This
                // pressure creates a torque bias at the input, limited by
                // dSlipMax.
                // So the user should use lockInputTq to regulate how fast
                // the differential locks and dSlipMax to regulate how much
                // more torque should go to the slower moving wheel.
                {
                    float spiderTq = inTq1 - inTq0;
                    float propTq = DrTq/differential->lockInputTq;
                    float rate = 0.0f;
                    if (propTq > 0.0f) {
                        rate = 1.0f - exp(-propTq*propTq);
                    }

                    float pressure = tanh(rate*(spinVel1-spinVel0));
                    float bias = differential->dSlipMax * 0.5f* pressure;
                    float open = 1.0f;// - rate;
                    DrTq0 = DrTq*(0.5f+bias) + spiderTq*open;
                    DrTq1 = DrTq*(0.5f-bias) - spiderTq*open;
                }
                break;

            case DIFF_ELECTRONIC_LSD: ;
            case DIFF_15WAY_LSD:
                //Similar to DIFF_LIMITED_SLIP,
                //but has different dSlipMax for power (acceleration)
                //and coast (deceleration), instead working as a free
                //differential in coast direction.
                //Electronic LSD has the same working, but its parameters
                //can be changed during driving.
                {
                    float spiderTq = inTq1 - inTq0;
                    float propTq = DrTq/differential->lockInputTq;
                    float rate = 0.0f;
                    rate = 1.0f - exp(-propTq*propTq);

                    float pressure = tanh(rate*(spinVel1-spinVel0));
                    float bias = (DrTq >= 0 ? differential->dSlipMax : differential->dCoastSlipMax) * 0.5f* pressure;
                    float open = 1.0f;// - rate;
                    DrTq0 = DrTq*(0.5f+bias) + spiderTq*open;
                    DrTq1 = DrTq*(0.5f-bias) - spiderTq*open;
                }
                break;

            case DIFF_VISCOUS_COUPLER:
                if (spinVel0 >= spinVel1) {
                    DrTq0 = DrTq * differential->dTqMin;
                    DrTq1 = DrTq * (1 - differential->dTqMin);
                } else {
                    deltaTq = (tdble) (differential->dTqMin + (1.0 - exp(-fabs(differential->viscosity * spinVel0 - spinVel1))) /
                                                                 differential->viscomax * differential->dTqMax);
                    DrTq0 = DrTq * deltaTq;
                    DrTq1 = DrTq * (1 - deltaTq);
                }

                break;
            default: /* NONE ? */
                DrTq0 = DrTq1 = 0;
                break;
        }
    } else {
        DrTq0 = (tdble) (DrTq / 2.0);
        DrTq1 = (tdble) (DrTq / 2.0);
    }


    ndot0 = SimDeltaTime * (DrTq0 - inTq0) / differential->outAxis[0]->I;
    spinVel0 += ndot0;
    ndot1 = SimDeltaTime * (DrTq1 - inTq1) / differential->outAxis[1]->I;
    spinVel1 += ndot1;

    BrTq = (tdble) (- SIGN(spinVel0) * differential->inAxis[0]->brkTq);
    ndot0 = SimDeltaTime * BrTq / differential->outAxis[0]->I;
    if (((ndot0 * spinVel0) < 0.0) && (fabs(ndot0) > fabs(spinVel0))) {
        ndot0 = -spinVel0;
    }
    if ((spinVel0 == 0.0) && (ndot0 < 0.0)) ndot0 = 0;
    spinVel0 += ndot0;

    BrTq = (tdble) (- SIGN(spinVel1) * differential->inAxis[1]->brkTq);
    ndot1 = SimDeltaTime * BrTq / differential->outAxis[1]->I;
    if (((ndot1 * spinVel1) < 0.0) && (fabs(ndot1) > fabs(spinVel1))) {
        ndot1 = -spinVel1;
    }
    if ((spinVel1 == 0.0) && (ndot1 < 0.0)) ndot1 = 0;
    spinVel1 += ndot1;

    if (first) {
        meanv = (tdble) ((spinVel0 + spinVel1) / 2.0);
        engineReaction = SimEngineUpdateRpm(car, meanv);
        if (meanv != 0.0) {
            engineReaction = engineReaction/meanv;
            if ((spinVel1*spinVel0)>0) {
                if (engineReaction != 0.0) {
                    spinVel1 *= engineReaction;
                    spinVel0 *= engineReaction;
                }
            }
        }
    }

    differential->outAxis[0]->spinVel = spinVel0;
    differential->outAxis[1]->spinVel = spinVel1;

    differential->outAxis[0]->Tq = (differential->outAxis[0]->spinVel - differential->inAxis[0]->spinVel) / SimDeltaTime * differential->outAxis[0]->I;
    differential->outAxis[1]->Tq = (differential->outAxis[1]->spinVel - differential->inAxis[1]->spinVel) / SimDeltaTime * differential->outAxis[1]->I;
}

void SDASpeedDreams::updateSpool(tCar *car, tDifferential *differential, int first)
{
    tdble   DrTq;
    tdble   ndot;
    tdble   spinVel;
    tdble   BrTq;
    tdble   engineReaction;
    tdble   I;
    tdble   inTq, brkTq;

    DrTq = differential->in.Tq * differential->efficiency;

    I = differential->outAxis[0]->I + differential->outAxis[1]->I;
    inTq = differential->inAxis[0]->Tq + differential->inAxis[1]->Tq;
    brkTq = differential->inAxis[0]->brkTq + differential->inAxis[1]->brkTq;

    ndot = SimDeltaTime * (DrTq - inTq) / I;
    spinVel = differential->inAxis[0]->spinVel + ndot;

    BrTq = (tdble) (- SIGN(spinVel) * brkTq);
    ndot = SimDeltaTime * BrTq / I;

    if (((ndot * spinVel) < 0.0) && (fabs(ndot) > fabs(spinVel))) {
        ndot = -spinVel;
    }
    if ((spinVel == 0.0) && (ndot < 0.0)) ndot = 0;

    spinVel += ndot;
    if (first) {
        engineReaction = SimEngineUpdateRpm(car, spinVel);
        if (engineReaction != 0.0) {
            spinVel = engineReaction;
        }
    }
    differential->outAxis[0]->spinVel = differential->outAxis[1]->spinVel = spinVel;

    differential->outAxis[0]->Tq = (differential->outAxis[0]->spinVel - differential->inAxis[0]->spinVel) / SimDeltaTime * differential->outAxis[0]->I;
    differential->outAxis[1]->Tq = (differential->outAxis[1]->spinVel - differential->inAxis[1]->spinVel) / SimDeltaTime * differential->outAxis[1]->I;
}

tdble SDASpeedDreams::SimEngineUpdateRpm(tCar *car, tdble axleRpm)
{
    tTransmission *trans = &(car->transmission);
    tClutch *clutch = &(trans->clutch);
    tEngine *engine = &(car->engine);
    float freerads;
    float transfer;


    if (car->fuel <= 0.0) {
        engine->rads = 0;
        clutch->state = CLUTCH_APPLIED;
        clutch->transferValue = 0.0;
        return 0.0;
    }

    freerads = engine->rads;
    freerads += engine->Tq / engine->I * SimDeltaTime;
    {
        tdble dp = engine->pressure;
        engine->pressure = engine->pressure*0.9f + 0.1f*engine->Tq;
        dp = (0.001f*fabs(engine->pressure - dp));
        dp = fabs(dp);
        tdble rth = urandom();
        if (dp > rth) {
            engine->exhaust_pressure += rth;
        }
        engine->exhaust_pressure *= 0.9f;
        car->carElt->priv.smoke += 5.0f*engine->exhaust_pressure;
        car->carElt->priv.smoke *= 0.99f;
    }


    // This is a method for the joint torque that the engine experiences
    // to be changed smoothly and not instantaneously.
    // The closest alpha is to 1, the faster the transition is.
    transfer = 0.0;
    float ttq = 0.0;
    float I_response = trans->differential[0].feedBack.I + trans->differential[1].feedBack.I;
    engine->Tq_response = 0.0;
    tdble dI = fabs(trans->curI - engine->I_joint);
    tdble sdI = dI;

    // limit the difference to avoid model instability
    if (sdI>1.0) {
        sdI = 1.0;
    }

    float alpha = 0.1f; // transition coefficient
    engine->I_joint = (tdble) (engine->I_joint*(1.0-alpha) +  alpha*trans->curI);

    // only use these values when the clutch is engaged or the gear
    // has changed.
    if ((clutch->transferValue > 0.01) && (trans->gearbox.gear)) {

        transfer = clutch->transferValue * clutch->transferValue * clutch->transferValue * clutch->transferValue;

        ttq = (float) (dI* tanh(0.01*(axleRpm * trans->curOverallRatio * transfer + freerads * (1.0-transfer) -engine->rads))*100.0);
        engine->rads = (tdble) ((1.0-sdI) * (axleRpm * trans->curOverallRatio * transfer + freerads * (1.0-transfer)) + sdI *(engine->rads + ((ttq)*SimDeltaTime)/(engine->I)));
        if (engine->rads < 0.0) {
            engine->rads = 0;
            engine->Tq = 0.0;
        }
    } else {
        engine->rads = freerads;
    }
    if (engine->rads < engine->tickover) {
        engine->rads = engine->tickover;
        engine->Tq = 0.0;
    } else if (engine->rads > engine->revsMax) {
        engine->rads = engine->revsMax;
        if ( (clutch->transferValue > 0.01) &&
            ((trans->curOverallRatio > 0.01) || (trans->curOverallRatio < -0.01)) )
            return engine->revsMax / trans->curOverallRatio;
        else {return 0.0;}
    }

    if ((trans->curOverallRatio!=0.0) && (I_response > 0)) {
        return axleRpm - sdI * ttq * trans->curOverallRatio   * SimDeltaTime / ( I_response);
    } else {
        return 0.0;
    }
}

float SDASpeedDreams::urandom() {
    return ((((float)rand()-1)/((float)RAND_MAX)));
}

void SDASpeedDreams::SimUpdateFreeWheels(tCar *car, int axlenb)
{
    int i;
    tWheel *wheel;
    tdble BrTq;		// brake torque
    tdble ndot;		// rotation acceleration
    tdble I;

    for (i = axlenb * 2; i < axlenb * 2 + 2; i++)
    {
        wheel = &(car->wheel[i]);

        I = wheel->I + car->axle[axlenb].I / 2.0f;

        ndot = SimDeltaTime * wheel->spinTq / I;
        wheel->spinVel -= ndot;

        BrTq = (tdble)(- SIGN(wheel->spinVel) * wheel->brake.Tq);
        ndot = SimDeltaTime * BrTq / I;

        if (fabs(ndot) > fabs(wheel->spinVel))
        {
            ndot = -wheel->spinVel;
        }

        wheel->spinVel += ndot;
        wheel->in.spinVel = wheel->spinVel;
    }
}

void SDASpeedDreams::SimWheelUpdateRotation(tCar *car)
{
    int i;
    tWheel *wheel;
    tdble deltan;
    tdble cosaz, sinaz;

    tdble maxslip = 0.0;

    for (i = 0; i < 4; i++)
    {
        wheel = &(car->wheel[i]);
        /*calculate gyroscopic forces*/
        cosaz = cos(wheel->relPos.az);
        sinaz = sin(wheel->relPos.az);

        if( (i == 0) || (i == 1) )
        {
            wheel->torques.y = wheel->torques.x * sinaz;
            wheel->torques.x = wheel->torques.x * cosaz;
        }
        else
        {
            wheel->torques.x = wheel->torques.y =0.0;
        }

        deltan = -(wheel->in.spinVel - wheel->prespinVel) * wheel->I / SimDeltaTime;
        wheel->torques.x -= deltan * wheel->cosax *sinaz;
        wheel->torques.y += deltan * wheel->cosax *cosaz;
        wheel->torques.z = deltan * wheel->sinax;
        /*update rotation*/
        wheel->spinVel = wheel->in.spinVel;

        if ( (car->features & FEAT_SLOWGRIP) && (wheel->brake.Tq <= 1.0) && (car->ctrl->accelCmd * car->transmission.clutch.transferValue < 0.05) )
        {
            /* prevent wheelspin value oscillating around wheel tangential velocity */
            tdble waz = wheel->steer + wheel->staticPos.az;
            tdble vt = wheel->bodyVel.x * cos(waz) + wheel->bodyVel.y * sin(waz);
            tdble wrl = wheel->spinVel * wheel->radius;
            tdble oldwrl = wheel->prespinVel * wheel->radius;
            if( (vt-wrl)*(vt-oldwrl) < 0.0 ) {
                wheel->spinVel = vt / wheel->radius;
            }

            wheel->prespinVel = wheel->spinVel;
        }
        else
        {
            FLOAT_RELAXATION2(wheel->spinVel, wheel->prespinVel, 50.0f);
        }

        wheel->relPos.ay += wheel->spinVel * SimDeltaTime;
        FLOAT_NORM_PI_PI(wheel->relPos.ay);
        car->carElt->_wheelSpinVel(i) = wheel->spinVel;

        // Option TCL ...
        if (car->features & FEAT_TCLINSIMU)
        {
            if (maxslip < wheel->brake.TCL)
                maxslip = wheel->brake.TCL;
        }
        // ... Option TCL
    }

    // Option TCL ...
    if (maxslip > 0.0)
    {
        for (i = 0; i < 4; i++)
        {
            wheel = &(car->wheel[i]);
            if (wheel->brake.TCL != maxslip)
                wheel->brake.TCL = 0.0;
        }
    }
    // ... Option TCL
}

void SDASpeedDreams::SimCarUpdate(tCar *car, tSituation * /* s */)
{
    SimCarUpdateForces(car);

    SimCarUpdateSpeed(car);

    SimCarUpdateCornerPos(car);

    SimCarUpdatePos(car);

    SimCarCollideZ(car);

    SimCarCollideXYScene(car);


    /* update car->carElt->setup.reqRepair with damage */
    tCarSetupItem *repair = &(car->carElt->setup.reqRepair);
    if ((repair->desired_value > 0.0) && (repair->max == repair->desired_value)) {
        repair->max = repair->desired_value = (tdble) car->dammage;
    } else {
        repair->max = (tdble) car->dammage;
    }

}

void SDASpeedDreams::SimCarUpdateForces(tCar *car)
{
    tForces	F;
    int		i;
    tdble	m, w, minv;
    tdble	SinTheta;
    tdble	Cosz, Sinz;
    tdble	v, R, Rv, Rm, Rx, Ry;
    tdble	desiredF, desiredTq;

    Cosz = car->Cosz = cos(car->DynGCg.pos.az);
    Sinz = car->Sinz = sin(car->DynGCg.pos.az);

    car->preDynGC = car->DynGCg;

    /* total mass */
    m = car->mass + car->fuel;
    minv = (tdble) (1.0 / m);
    w = -m * G;

    /* Weight */
    SinTheta = (tdble) ((-car->wheel[FRNT_RGT].zRoad - car->wheel[FRNT_LFT].zRoad
                        + car->wheel[REAR_RGT].zRoad + car->wheel[REAR_LFT].zRoad) / (2.0 * car->wheelbase));
    F.F.x = -w * SinTheta;
    SinTheta = (tdble) ((-car->wheel[FRNT_RGT].zRoad - car->wheel[REAR_RGT].zRoad
                        + car->wheel[FRNT_LFT].zRoad + car->wheel[REAR_LFT].zRoad) / (2.0 * car->wheeltrack));
    F.F.y = -w * SinTheta;
    F.F.z = (tdble) (w - (F.F.x*F.F.x + F.F.y*F.F.y)/(2.0*w));/*Taylor-polinom of sqrt(w^2-F.F.x^2-F.F.y^2)*/
    F.M.x = F.M.y = F.M.z = 0;

    /* Wheels */
    for (i = 0; i < 4; i++) {
        /* forces */
        F.F.x += car->wheel[i].forces.x;
        F.F.y += car->wheel[i].forces.y;
        F.F.z += car->wheel[i].forces.z;
        /* moments */
        F.M.x += car->wheel[i].forces.z * car->wheel[i].staticPos.y +
                 car->wheel[i].forces.y * (car->statGC.z + car->wheel[i].rideHeight);
        F.M.x += car->wheel[i].torques.x;
        F.M.y -= car->wheel[i].forces.z * car->wheel[i].staticPos.x +
                 car->wheel[i].forces.x * (car->statGC.z + car->wheel[i].rideHeight);
        F.M.y += car->wheel[i].torques.y;
        F.M.z += -car->wheel[i].forces.x * car->wheel[i].staticPos.y +
                 car->wheel[i].forces.y * car->wheel[i].staticPos.x;
        F.M.z += car->wheel[i].torques.z;
    }

    /* Aero Drag */
    F.F.x += car->aero.drag;

    /* Wings & Aero Downforce */
    for (i = 0; i < 2; i++) {
        /* forces */
        F.F.z += car->wing[i].forces.z + car->aero.lift[i];
        F.F.x += car->wing[i].forces.x;
        /* moments */
        F.M.y -= (car->wing[i].forces.z + car->aero.lift[i]) * car->wing[i].staticPos.x +
                 car->wing[i].forces.x * car->wing[i].staticPos.z;
    }

    /* Rolling Resistance */
    v = car->DynGC.vel.xy;
    R = 0;
    for (i = 0; i < 4; i++) {
        R += car->wheel[i].rollRes;
    }
    if (v > 0.00001) {
        if (v > 10.0) {
            Rv = R / v;
        } else {
            Rv = (tdble) (R / 10.0);
        }
        if ((Rv * minv * SimDeltaTime) > v) {
            Rv = v * m / SimDeltaTime;
        }
    } else {
        Rv = 0;
    }
    Rx = Rv * car->DynGC.vel.x;
    Ry = Rv * car->DynGC.vel.y;

    F.F.x -= Rx;
    F.F.y -= Ry;

    if ((R * car->wheelbase / 2.0 * car->Iinv.z) > fabs(car->DynGCg.vel.az)) {
        Rm = car->DynGCg.vel.az / car->Iinv.z;
    } else {
        Rm = (tdble) (SIGN(car->DynGCg.vel.az) * R * car->wheelbase / 2.0);
    }
    F.M.z -= Rm;

    /* simulate sticking when car almost stationary */
    if ((car->features & FEAT_SLOWGRIP) && ( v < 0.1 ) ) {
        w = -w; //make it positive
        /* desired force to stop sideway slide */
        desiredF = - m * car->DynGC.vel.y / SimDeltaTime;
        if ( (fabs(desiredF - F.F.y)) < w ) {F.F.y = desiredF;}
        else if ( (desiredF - F.F.y) > 0.0 ) {F.F.y += w;}
        else {F.F.y -= w;}
        /* desired torque to stop yaw */
        desiredTq = - car->DynGC.vel.az / ( SimDeltaTime * car->Iinv.z );
        if ( (fabs(desiredTq - F.M.z)) < 0.5 * w * car->wheelbase) {F.M.z = desiredTq;}
        else if ( (desiredTq - F.M.z) > 0.0 ) {F.M.z += 0.5f * w * car->wheelbase;}
        else {F.M.z -= 0.5f * w * car->wheelbase;}
        /* desired force to really stop the car when braking to 0 */
        if ( ((car->ctrl->brakeCmd > 0.05) || (car->ctrl->ebrakeCmd > 0.1) || (car->ctrl->brakeFrontLeftCmd > 0.02)
             || (car->ctrl->brakeFrontRightCmd > 0.02) || (car->ctrl->brakeRearLeftCmd > 0.02) || (car->ctrl->brakeRearRightCmd > 0.02) )
            && (car->ctrl->accelCmd * car->transmission.clutch.transferValue < 0.05) && (fabs(car->DynGC.vel.x) < 0.02) ) {
            desiredF = - m * car->DynGC.vel.x / SimDeltaTime;
            w *= 0.5;
            if ( (fabs(desiredF - F.F.x)) < w ) {F.F.x = desiredF;}
            else if ( (desiredF - F.F.x) > 0.0 ) {F.F.x += w;}
            else {F.F.x -= w;}
        }
    }

    /* compute accelerations */
    car->DynGC.acc.x = F.F.x * minv;
    car->DynGC.acc.y = F.F.y * minv;
    car->DynGC.acc.z = F.F.z * minv;

    car->DynGCg.acc.x = (F.F.x * Cosz - F.F.y * Sinz) * minv;
    car->DynGCg.acc.y = (F.F.x * Sinz + F.F.y * Cosz) * minv;
    car->DynGCg.acc.z = car->DynGC.acc.z;

    car->DynGCg.acc.ax = car->DynGC.acc.ax = F.M.x * car->Iinv.x;
    car->DynGCg.acc.ay = car->DynGC.acc.ay = F.M.y * car->Iinv.y;
    car->DynGCg.acc.az = car->DynGC.acc.az = F.M.z * car->Iinv.z;
}

void SDASpeedDreams::SimCarUpdateSpeed(tCar *car)
{
    // fuel consumption
    tdble delta_fuel = car->fuel_prev - car->fuel;
    car->fuel_prev = car->fuel;
    if (delta_fuel > 0) {
        car->carElt->_fuelTotal += delta_fuel;
    }
    tdble fi;
    tdble as = sqrt(car->airSpeed2);
    if (as<0.1) {
        fi = 99.9f;
    } else {
        fi = 100000 * delta_fuel / (as*SimDeltaTime);
    }
    tdble alpha = 0.1f;
    car->carElt->_fuelInstant = (tdble)((1.0-alpha)*car->carElt->_fuelInstant + alpha*fi);

    tdble	Cosz, Sinz;
    //tdble	mass;

    //mass = car->mass + car->fuel;

    Cosz = car->Cosz;
    Sinz = car->Sinz;

    car->DynGCg.vel.x += car->DynGCg.acc.x * SimDeltaTime;
    car->DynGCg.vel.y += car->DynGCg.acc.y * SimDeltaTime;
    car->DynGCg.vel.z += car->DynGCg.acc.z * SimDeltaTime;

    car->DynGCg.vel.ax += car->DynGCg.acc.ax * SimDeltaTime;
    car->DynGCg.vel.ay += car->DynGCg.acc.ay * SimDeltaTime;
    car->DynGCg.vel.az += car->DynGCg.acc.az * SimDeltaTime;

    /* spin limitation */
    if (fabs(car->DynGCg.vel.az) > 9.0) {
        car->DynGCg.vel.az = (tdble) (SIGN(car->DynGCg.vel.az) * 9.0);
    }

    car->DynGC.vel.ax = car->DynGCg.vel.ax;
    car->DynGC.vel.ay = car->DynGCg.vel.ay;
    car->DynGC.vel.az = car->DynGCg.vel.az;

    car->DynGC.vel.x = car->DynGCg.vel.x * Cosz + car->DynGCg.vel.y * Sinz;
    car->DynGC.vel.y = -car->DynGCg.vel.x * Sinz + car->DynGCg.vel.y * Cosz;
    car->DynGC.vel.z = car->DynGCg.vel.z;

    /* 2D speed */
    car->DynGC.vel.xy = sqrt(car->DynGCg.vel.x * car->DynGCg.vel.x  +
                             car->DynGCg.vel.y * car->DynGCg.vel.y);
}


void SDASpeedDreams::SimCarUpdatePos(tCar *car)
{
    tdble vx, vy;
    //tdble accx, accy;

    vx = car->DynGCg.vel.x;
    vy = car->DynGCg.vel.y;

    //accx = car->DynGCg.acc.x;
    //accy = car->DynGCg.acc.y;

    car->DynGCg.pos.x += vx * SimDeltaTime;
    car->DynGCg.pos.y += vy * SimDeltaTime;
    car->DynGCg.pos.z += car->DynGCg.vel.z * SimDeltaTime;

    car->DynGCg.pos.ax += car->DynGCg.vel.ax * SimDeltaTime;
    car->DynGCg.pos.ay += car->DynGCg.vel.ay * SimDeltaTime;
    car->DynGCg.pos.az += car->DynGCg.vel.az * SimDeltaTime;

    FLOAT_NORM_PI_PI(car->DynGCg.pos.az);

    if (car->DynGCg.pos.ax > aMax) {
        car->DynGCg.pos.ax = aMax;
        car->DynGCg.vel.ax = 0;
    }
    if (car->DynGCg.pos.ax < -aMax) {
        car->DynGCg.pos.ax = -aMax;
        car->DynGCg.vel.ax = 0;
    }
    if (car->DynGCg.pos.ay > aMax) {
        car->DynGCg.pos.ay = aMax;
        car->DynGCg.vel.ay = 0;
    }
    if (car->DynGCg.pos.ay < -aMax) {
        car->DynGCg.pos.ay = -aMax;
        car->DynGCg.vel.ay = 0;
    }

    car->DynGC.pos.x = car->DynGCg.pos.x;
    car->DynGC.pos.y = car->DynGCg.pos.y;
    car->DynGC.pos.z = car->DynGCg.pos.z;

    car->DynGC.pos.ax = car->DynGCg.pos.ax;
    car->DynGC.pos.ay = car->DynGCg.pos.ay;
    car->DynGC.pos.az = car->DynGCg.pos.az;

    RtTrackGlobal2Local(car->trkPos.seg, car->DynGCg.pos.x, car->DynGCg.pos.y, &(car->trkPos), TR_LPOS_MAIN);
}

void SDASpeedDreams::SimCarUpdateCornerPos(tCar *car)
{
    tdble Cosz = car->Cosz;
    tdble Sinz = car->Sinz;
    tdble vx = car->DynGCg.vel.x;
    tdble vy = car->DynGCg.vel.y;
    int i;

    for (i = 0; i < 4; i++) {
        tdble x = car->corner[i].pos.x + car->statGC.x;
        tdble y = car->corner[i].pos.y + car->statGC.y;
        tdble dx = x * Cosz - y * Sinz;
        tdble dy = x * Sinz + y * Cosz;

        car->corner[i].pos.ax = car->DynGCg.pos.x + dx;
        car->corner[i].pos.ay = car->DynGCg.pos.y + dy;
        /*car->corner[i].pos.az = car->DynGC.pos.z - car->statGC.z + x * sin(car->DynGC.pos.ay) + y * sin(car->DynGC.pos.ax);*/

        /* add the body rotation to the wheel        */
        /* the speed is vel.az * r                   */
        /* where r = sqrt(x*x + y*y)                 */
        /* the tangent vector is -y / r and x / r    */
        // compute corner velocity at local frame
        car->corner[i].vel.ax = - car->DynGC.vel.az * y;
        car->corner[i].vel.ay = car->DynGC.vel.az * x;

        // rotate to global and add global center of mass velocity
        // note: global to local.
        car->corner[i].vel.x = vx
                               + car->corner[i].vel.ax * Cosz - car->corner[i].vel.ay * Sinz;
        car->corner[i].vel.y = vy
                               + car->corner[i].vel.ax * Sinz + car->corner[i].vel.ay * Cosz;

        // add local center of mass velocity
        car->corner[i].vel.ax += car->DynGC.vel.x;
        car->corner[i].vel.ay += car->DynGC.vel.y;
    }
}

void SDASpeedDreams::SimCarCollideZ(tCar *car)
{
    int i;
    t3Dd normal;
    tdble dotProd;
    tWheel *wheel;
    const float CRASH_THRESHOLD = -5.0f;
    tdble dz = 0.0f;

    if (car->carElt->_state & RM_CAR_STATE_NO_SIMU) {
        return;
    }

    for (i = 0; i < 4; i++) {
        wheel = &(car->wheel[i]);
        if ( (wheel->state & SIM_SUSP_COMP)&&(!(wheel->state & SIM_WH_INAIR)) ) {
            dz = MAX(dz, wheel->susp.spring.packers - wheel->rideHeight);
            wheel->rideHeight = wheel->susp.spring.packers;
            RtTrackSurfaceNormalL(&(wheel->trkPos), &normal);
            dotProd = (car->DynGCg.vel.x * normal.x + car->DynGCg.vel.y * normal.y + car->DynGCg.vel.z * normal.z) * wheel->trkPos.seg->surface->kRebound;
            if (dotProd < 0.0f) {
                if (dotProd < CRASH_THRESHOLD) {
                    car->collision |= SEM_COLLISION_Z_CRASH;
                }

                if ((car->carElt->_state & RM_CAR_STATE_FINISH) == 0) {
                    int deltaDamage = (int)(wheel->trkPos.seg->surface->kDammage * fabs(dotProd) * simDammageFactor[car->carElt->_skillLevel]);
                    if (deltaDamage > 1)
                    {
                        car->collision |= SEM_COLLISION_Z;
                        car->dammage += deltaDamage;
                    }
                }
                car->DynGCg.vel.x -= normal.x * dotProd;
                car->DynGCg.vel.y -= normal.y * dotProd;
                car->DynGCg.vel.z -= normal.z * dotProd;
            }
        }
    }
    car->DynGCg.pos.z += dz; //elevate car when it is slightly sinken into ground
}

void SDASpeedDreams::RtTrackSurfaceNormalL(tTrkLocPos *p, t3Dd *norm)
{
    tTrkLocPos	p1;
    t3Dd	px1, px2, py1, py2;
    t3Dd	v1, v2;
    tdble	lg;

    p1.seg = p->seg;

    p1.toStart = 0;
    p1.toRight = p->toRight;
    RtTrackLocal2Global(&p1, &px1.x, &px1.y, TR_TORIGHT);
    px1.z = RtTrackHeightL(&p1);

    if (p1.seg->type == TR_STR) {
        p1.toStart = p1.seg->length;
    } else {
        p1.toStart = p1.seg->arc;
    }
    RtTrackLocal2Global(&p1, &px2.x, &px2.y, TR_TORIGHT);
    px2.z = RtTrackHeightL(&p1);

    p1.toRight = 0;
    p1.toStart = p->toStart;
    RtTrackLocal2Global(&p1, &py1.x, &py1.y, TR_TORIGHT);
    py1.z = RtTrackHeightL(&p1);

    p1.toRight = p1.seg->width;
    RtTrackLocal2Global(&p1, &py2.x, &py2.y, TR_TORIGHT);
    py2.z = RtTrackHeightL(&p1);


    v1.x = px2.x - px1.x;
    v1.y = px2.y - px1.y;
    v1.z = px2.z - px1.z;
    v2.x = py2.x - py1.x;
    v2.y = py2.y - py1.y;
    v2.z = py2.z - py1.z;

    norm->x = v1.y * v2.z - v2.y * v1.z;
    norm->y = v2.x * v1.z - v1.x * v2.z;
    norm->z = v1.x * v2.y - v2.x * v1.y;
    lg = sqrt(norm->x * norm->x + norm->y * norm->y + norm->z * norm->z);
    if (lg == 0.0) {
        lg = 1.0f;
    } else {
        lg = 1.0f / lg;
    }
    norm->x *= lg;
    norm->y *= lg;
    norm->z *= lg;
}

void SDASpeedDreams::SimCarCollideXYScene(tCar *car)
{
    tTrackSeg *seg = car->trkPos.seg;
    tTrkLocPos trkpos;
    int i;
    tDynPt *corner;
    tdble initDotProd;
    tdble dotProd, cx, cy, dotprod2;
    tTrackBarrier *curBarrier;
    tdble dmg;

    if (car->carElt->_state & RM_CAR_STATE_NO_SIMU) {
        return;
    }

    corner = &(car->corner[0]);
    for (i = 0; i < 4; i++, corner++) {
        seg = car->trkPos.seg;
        RtTrackGlobal2Local(seg, corner->pos.ax, corner->pos.ay, &trkpos, TR_LPOS_TRACK);
        seg = trkpos.seg;
        tdble toSide;

        if (trkpos.toRight < 0.0) {
            // collision with right border.
            curBarrier = seg->barrier[TR_SIDE_RGT];
            toSide = trkpos.toRight;
        } else if (trkpos.toLeft < 0.0) {
            // collision with left border.
            curBarrier = seg->barrier[TR_SIDE_LFT];
            toSide = trkpos.toLeft;
        } else {
            continue;
        }

        const tdble& nx = curBarrier->normal.x;
        const tdble& ny = curBarrier->normal.y;

        car->DynGCg.pos.x -= nx * toSide;
        car->DynGCg.pos.y -= ny * toSide;

        // Corner position relative to center of gravity.
        cx = corner->pos.ax - car->DynGCg.pos.x;
        cy = corner->pos.ay - car->DynGCg.pos.y;

        car->blocked = 1;
        car->collision |= SEM_COLLISION;

        // Impact speed perpendicular to barrier (of corner).
        initDotProd = nx * corner->vel.x + ny * corner->vel.y;

        // Compute dmgDotProd (base value for later damage) with a heuristic.
        tdble absvel = (tdble) (MAX(1.0, sqrt(car->DynGCg.vel.x*car->DynGCg.vel.x + car->DynGCg.vel.y*car->DynGCg.vel.y)));
        tdble GCgnormvel = car->DynGCg.vel.x*nx + car->DynGCg.vel.y*ny;
        tdble cosa = GCgnormvel/absvel;
        tdble dmgDotProd = GCgnormvel*cosa;

        dotProd = initDotProd * curBarrier->surface->kFriction;
        car->DynGCg.vel.x -= nx * dotProd;
        car->DynGCg.vel.y -= ny * dotProd;
        dotprod2 = (nx * cx + ny * cy);

        // Angular velocity change caused by friction of colliding car part with wall.
        static tdble VELSCALE = 10.0f;
        static tdble VELMAX = 6.0f;
        car->DynGCg.vel.az -= dotprod2 * dotProd / VELSCALE;
        if (fabs(car->DynGCg.vel.az) > VELMAX) {
            car->DynGCg.vel.az = (tdble) (SIGN(car->DynGCg.vel.az) * VELMAX);
        }

        // Dammage.
        dotProd = initDotProd;
        if (dotProd < 0.0f && (car->carElt->_state & RM_CAR_STATE_FINISH) == 0) {
            dmg = (tdble) (curBarrier->surface->kDammage * fabs(0.5*dmgDotProd*dmgDotProd) * simDammageFactor[car->carElt->_skillLevel]);
            car->dammage += (int)dmg;
        } else {
            dmg = 0.0f;
        }

        dotProd *= curBarrier->surface->kRebound;

        // If the car moves toward the barrier, rebound.
        if (dotProd < 0.0f) {
            car->collision |= SEM_COLLISION_XYSCENE;
            car->normal.x = nx * dmg;
            car->normal.y = ny * dmg;
            car->collpos.x = corner->pos.ax;
            car->collpos.y = corner->pos.ay;
            car->DynGCg.vel.x -= nx * dotProd;
            car->DynGCg.vel.y -= ny * dotProd;
        }
    }
}

void SDASpeedDreams::SimCarCollideCars(tSituation *s)
{
    tCar *car;
    tCarElt *carElt;
    int i;

    for (i = 0; i < s->_ncars; i++) {
        carElt = s->cars[i];
        if (carElt->_state & RM_CAR_STATE_NO_SIMU) {
            continue;
        }

        car = &(SimCarTable[carElt->index]);
        //dtSelectObject(car);
        // Fit the bounding box around the car, statGC's are the static offsets.
        //dtLoadIdentity();
        //dtTranslate(-carElt->_statGC_x, -carElt->_statGC_y, 0.0f);
        // Place the bounding box such that it fits the car in the world.
        //dtMultMatrixf((const float *)(carElt->_posMat));
        memset(&(car->VelColl), 0, sizeof(tPosd));
    }

    // Running the collision detection. If no collision is detected, call dtProceed.
    // dtProceed just works if all objects are disjoint.
    //if (dtTest() == 0) {
    //    dtProceed();
    //}

    for (i = 0; i < s->_ncars; i++) {
        carElt = s->cars[i];
        if (carElt->_state & RM_CAR_STATE_NO_SIMU) {
            continue;
        }
        car = &(SimCarTable[carElt->index]);
        if (car->collision & SEM_COLLISION_CAR) {
            car->DynGCg.vel.x = car->VelColl.x;
            car->DynGCg.vel.y = car->VelColl.y;
            car->DynGCg.vel.az = car->VelColl.az;
        }
    }
}

void SDASpeedDreams::SimCarUpdate2(tCar *car, tSituation * /* s */)
{
    if ((SimTelemetry == car->carElt->index)
        || (car->ctrl->telemetryMode > 0))
        SimTelemetryOut(car);
}

void SDASpeedDreams::SimTelemetryOut(tCar *car)
{
    int i;
    tdble Fzf, Fzr;

    if (car->ctrl->telemetryMode == 1)  // Full data output
    {
        printf("-----------------------------\nCar: %d %s ---\n", car->carElt->index, car->carElt->_name);
        printf("Seg: %d (%s)  Ts:%f  Tr:%f\n",
               car->trkPos.seg->id, car->trkPos.seg->name, car->trkPos.toStart, car->trkPos.toRight);
        printf("---\nMx: %f  My: %f  Mz: %f (N/m)\n", car->DynGC.acc.ax, car->DynGC.acc.ay, car->DynGC.acc.az);
        printf("Wx: %f  Wy: %f  Wz: %f (rad/s)\n", car->DynGC.vel.ax, car->DynGC.vel.ay, car->DynGC.vel.az);
        printf("Ax: %f  Ay: %f  Az: %f (rad)\n", car->DynGCg.pos.ax, car->DynGCg.pos.ay, car->DynGCg.pos.az);
        printf("---\nAx: %f  Ay: %f  Az: %f (Gs)\n", car->DynGC.acc.x / 9.81, car->DynGC.acc.y / 9.81, car->DynGC.acc.z / 9.81);
        printf("Vx: %f  Vy: %f  Vz: %f (m/s)\n", car->DynGC.vel.x, car->DynGC.vel.y, car->DynGC.vel.z);
        printf("Px: %f  Py: %f  Pz: %f (m)\n---\n", car->DynGCg.pos.x, car->DynGCg.pos.y, car->DynGCg.pos.z);
        printf("As: %f\n---\n", sqrt(car->airSpeed2));
        for (i = 0; i < 4; i++)
        {
            printf("wheel %d - RH:%f susp:%f zr:%.2f ", i, car->wheel[i].rideHeight, car->wheel[i].susp.x, car->wheel[i].zRoad);
            printf("sx:%f sa:%f w:%f ", car->wheel[i].sx, car->wheel[i].sa, car->wheel[i].spinVel);
            printf("fx:%f fy:%f fz:%f\n", car->wheel[i].forces.x, car->wheel[i].forces.y, car->wheel[i].forces.z);
        }
        Fzf = (tdble)((car->aero.lift[0] + car->wing[0].forces.z) / 9.81);
        Fzr = (tdble)((car->aero.lift[1] + car->wing[1].forces.z) / 9.81);
        printf("Aero Fx:%f Fz:%f Fzf=%f Fzr=%f ratio=%f\n", car->aero.drag / 9.81, Fzf + Fzr,
               Fzf, Fzr, (Fzf + Fzr) / (car->aero.drag + 0.1) * 9.81);
    }
    else if (car->ctrl->telemetryMode == 2)
    {
        // Mass from total and mass from wheels
        float MassTotal = car->mass + car->fuel;
        float mass = car->wheel[FRNT_RGT].weight0 + car->wheel[FRNT_LFT].weight0 + car->wheel[REAR_RGT].weight0 + car->wheel[REAR_LFT].weight0;
        float massfactor = (mass + car->fuel * G) / mass;
        float MassTotal2 = massfactor / G * (car->wheel[0].weight0 + car->wheel[1].weight0 + car->wheel[2].weight0 + car->wheel[3].weight0);

        // Measurements of downforce at axles (Forces without part caused by weight)
        float MeasurementFront = car->wheel[0].forces.z + car->wheel[1].forces.z - massfactor * (car->wheel[0].weight0 + car->wheel[1].weight0);
        float MeasurementRear = car->wheel[2].forces.z + car->wheel[3].forces.z - massfactor * (car->wheel[2].weight0 + car->wheel[3].weight0);

        float RideHeightFront = (car->wheel[0].rideHeight + car->wheel[1].rideHeight) / 2;
        float RideHeightRear = (car->wheel[2].rideHeight + car->wheel[3].rideHeight) / 2;
        float hm = 3 * (RideHeightFront + RideHeightRear);
        hm = hm * hm;
        hm = hm * hm;
        hm = 2 * exp(-3 * hm);

        printf("Car spd:%.1f km/h %.2f m/s air spd:%.2f m/s spd2:%.2f m2/s2\n", car->DynGC.vel.x * 3.6f, car->DynGC.vel.x, sqrt(car->airSpeed2), car->airSpeed2);
        printf("Car x:%.3f m z:%.3f m r:%.3f m zr:%.3f m\n", car->statGC.x, car->statGC.z, RideHeightRear, car->statGC.z + RideHeightRear);

        printf("Mass:%.0f kg fuel:%.2f kg total: %.2f kg / %.2f N\n", car->mass, car->fuel, MassTotal, MassTotal * G);
        printf("Mass:%.2f kg Mass:%.2f kg Delta:%.5f kg\n", MassTotal, MassTotal2, MassTotal - MassTotal2);

        printf("Ride height factor:%.3f\n", hm);

        printf("Wheel f x:%.3f m z:%.3f m\n", car->wheel[0].staticPos.x, car->wheel[0].staticPos.z);
        printf("Wheel r x:%.3f m z:%.3f m\n", car->wheel[2].staticPos.x, car->wheel[2].staticPos.z);

        printf("Wheel f - RH:%.3f m ", RideHeightFront);
        printf("Fx:%.3f Fz:%.3f N\n", car->wheel[0].forces.x + car->wheel[1].forces.x, MeasurementFront);
        printf("Wheel r - RH:%.3f m ", RideHeightRear);
        printf("Fx:%.3f Fz:%.3f N\n", car->wheel[2].forces.x + car->wheel[3].forces.x, MeasurementRear);

        printf("Wheel f - Tq:%.3f Nm\n", car->wheel[0].torques.y + car->wheel[1].torques.y);
        printf("Wheel r - Tq:%.3f Nm\n", car->wheel[2].torques.y + car->wheel[3].torques.y);

        printf("Wing f x:%.3f m z:%.3f m\n", car->wing[0].staticPos.x, car->wing[0].staticPos.z);
        printf("Wing r x:%.3f m z:%.3f m\n", car->wing[1].staticPos.x, car->wing[1].staticPos.z);

        tdble WFxf = (tdble)(car->wing[0].forces.x);
        tdble WFzf = (tdble)(car->wing[0].forces.z);
        printf("Wing f Fx:%.3f N Fz:%.3f N Fx:%.3f kg Fz:%.3f kg\n", WFxf, WFzf, WFxf / G, WFzf / G);

        tdble WFxr = (tdble)(car->wing[1].forces.x);
        tdble WFzr = (tdble)(car->wing[1].forces.z);
        printf("Wing r Fx:%.3f N Fz:%.3f N Fx:%.3f kg Fz:%.3f kg\n", WFxr, WFzr, WFxr / G, WFzr / G);

        Fzf = (tdble)((car->aero.lift[0] * G + car->wing[0].forces.z));
        Fzr = (tdble)((car->aero.lift[1] * G + car->wing[1].forces.z));
        tdble AFzf = (tdble)(car->aero.lift[0] * G);
        tdble AFzr = (tdble)(car->aero.lift[1] * G);
        printf("Aero Lift  Fzf=%.3f N Fzr=%.3f N Fz:%.3f N\n", AFzf, AFzr, AFzf + AFzr);
        printf("Aero Wing  Fzf=%.3f N Fzr=%.3f N Fz:%.3f N\n", Fzf - AFzf, Fzr - AFzr, Fzf + Fzr - AFzf - AFzr);
        printf("Aero Force Fzf=%.3f N Fzr=%.3f N Fz:%.3f N\n", Fzf, Fzr, Fzf + Fzr);

        printf("Aero Drag  Fx:%.3f N\n", car->aero.drag);

        printf("Downforce front:%.3f N\n", MeasurementFront);
        printf("Downforce rear:%.3f N\n", MeasurementRear);
        printf("Downforce total:%.3f N\n", MeasurementFront + MeasurementRear);
    }
    else if (car->ctrl->telemetryMode == 3)
    {
        for (i = 0; i < 4; i++)
        {
            /* forces */
            fprintf(stderr, "%d: fx:%.1f N  fy:%.1f N  fz:%.1f N\n", i, car->wheel[i].forces.x, car->wheel[i].forces.y, car->wheel[i].forces.z);
        }
    }
    else if (car->ctrl->telemetryMode == 4)
    {
        /* Balances */
        double ForceFront = car->wheel[0].forces.x + car->wheel[1].forces.x;
        double ForceRear = car->wheel[2].forces.x + car->wheel[3].forces.x;
        double ForceRight = car->wheel[0].forces.x + car->wheel[2].forces.x;
        double ForceLeft = car->wheel[1].forces.x + car->wheel[3].forces.x;
        double ForceTotal1 = MAX(0.1, fabs(ForceFront) + fabs(ForceRear));
        double ForceTotal2 = MAX(0.1, fabs(ForceLeft) + fabs(ForceRight));
        fprintf(stderr, "BxFR%+7.1f%% BxLR%+7.1f%% ", 100.0 * ForceFront / (ForceTotal1), 100.0 * ForceLeft / (ForceTotal2));

        ForceFront = car->wheel[0].forces.y + car->wheel[1].forces.y;
        ForceRear = car->wheel[2].forces.y + car->wheel[3].forces.y;
        ForceRight = car->wheel[0].forces.y + car->wheel[2].forces.y;
        ForceLeft = car->wheel[1].forces.y + car->wheel[3].forces.y;
        ForceTotal1 = MAX(0.1, fabs(ForceFront) + fabs(ForceRear));
        ForceTotal2 = MAX(0.1, fabs(ForceLeft) + fabs(ForceRight));
        fprintf(stderr, "ByFR%+7.1f%% ByLR%+7.1f%% ", 100.0 * ForceFront / (ForceTotal1), 100.0 * ForceLeft / (ForceTotal2));

        ForceFront = car->wheel[0].forces.z + car->wheel[1].forces.z;
        ForceRear = car->wheel[2].forces.z + car->wheel[3].forces.z;
        ForceRight = car->wheel[0].forces.z + car->wheel[2].forces.z;
        ForceLeft = car->wheel[1].forces.z + car->wheel[3].forces.z;
        ForceTotal1 = MAX(0.1, fabs(ForceFront) + fabs(ForceRear));
        ForceTotal2 = MAX(0.1, fabs(ForceLeft) + fabs(ForceRight));
        fprintf(stderr, "BzFR%+7.1f%% BzLR%+7.1f%%\n", 100.0 * ForceFront / (ForceTotal1), 100.0 * ForceLeft / (ForceTotal2));
    }
}