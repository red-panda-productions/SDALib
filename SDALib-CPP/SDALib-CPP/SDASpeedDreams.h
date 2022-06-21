#pragma once

#include "SDAData.hpp"
#include "SDAAction.hpp"

class SDASpeedDreams
{
public:
    SDAData UpdateSimulator(const SDAData& p_data, SDAAction& p_action);

private:
    tCar CarConstructor(const tCar& p_other, tCarElt* p_carElt);
    tCar *SimCarTable = 0;
    tdble SimDeltaTime;
    tdble simSkidFactor[6]{0.4f, 0.3f, 0.2f, 0.1f, 0.0f, 0.0f};
    tdble simDammageFactor[6]{0.0f, 0.1f, 0.3f, 0.5f, 0.8f, 1.0f};
    void SimUpdate(tSituation *s, double deltaTime);
    tdble SimTimeOfDay = 0;
    double Tair = 273.15;;
    double simDegree = 0.000000023;
    double startDay = 0.0;
    double endDay = 0.0;
    void SimAtmosphereUpdate(tSituation *s);
    void RemoveCar(tCar *car, tSituation *s);
    void SimCollideRemoveCar(tCar *car, int nbcars);
    void RtTrackLocal2Global(tTrkLocPos *p, tdble *X, tdble *Y, int flag);
    tdble RtTrackHeightL(tTrkLocPos *p);
    tdble RtTrackSideTgAngleL(tTrkLocPos *p);
    tdble RtTrackGetWidth(tTrackSeg *seg, tdble toStart);
    void ctrlCheck(tCar *car);
    void SimInstantReConfig(tCar *car);
    void SimBrakeSystemReConfig(tCar *car);
    void SimArbReConfig(tCar *car, int index);
    void SimSteerUpdate(tCar *car);
    void SimGearboxUpdate(tCar *car);
    void SimEngineUpdateTq(tCar *car);
    void SimCarUpdateWheelPos(tCar *car);
    void SimBrakeSystemUpdate(tCar *car);
    void SimAeroUpdate(tCar *car, tSituation *s);
    void SimWingUpdate(tCar *car, int index, tSituation* s);
    void SimWheelUpdateRide(tCar *car, int index);
    tdble CliftFromAoA(tWing* wing);
    tdble F(tWing* wing);
    void RtTrackGlobal2Local(tTrackSeg *segment, tdble X, tdble Y, tTrkLocPos *p, int type);
    void SimSuspCheckIn(tSuspension *susp);
    void SimBrakeUpdate(tCar *car, tWheel *wheel, tBrake *brake);
    void SimAxleUpdate(tCar *car, int index);
    void SimSuspUpdate(tSuspension *susp);
    tdble springForce(tSuspension *susp);
    tdble damperForce(tSuspension *susp);
    void SimWheelUpdateForce(tCar *car, int index);
    void SimTransmissionUpdate(tCar *car);
    void SimDifferentialUpdate(tCar *car, tDifferential *differential, int first);
    void updateSpool(tCar *car, tDifferential *differential, int first);
    tdble SimEngineUpdateRpm(tCar *car, tdble axleRpm);
    float urandom();
    void SimUpdateFreeWheels(tCar *car, int axlenb);
    void SimWheelUpdateRotation(tCar *car);
    void SimCarUpdate(tCar *car, tSituation * /* s */);
    void SimCarUpdateForces(tCar *car);
    void SimCarUpdateSpeed(tCar *car);
    void SimCarUpdatePos(tCar *car);
    const tdble aMax = 1.0f;
    void SimCarUpdateCornerPos(tCar *car);
    void SimCarCollideZ(tCar *car);
    void RtTrackSurfaceNormalL(tTrkLocPos *p, t3Dd *norm);
    void SimCarCollideXYScene(tCar *car);
    void SimCarCollideCars(tSituation *s);
    void SimCarUpdate2(tCar *car, tSituation * /* s */);
    int SimTelemetry = -1;
    void SimTelemetryOut(tCar *car);
};





