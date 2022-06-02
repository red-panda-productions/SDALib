# object containing all data send to the decision maker
class SDATypes:
    def __init__(self, car, situation, tickCount):
        self.car = a


# the main car structure
class Car:
    def __init__(self, index, info, pub, race, priv, ctrl, setup, pitcmd):
        self.index = a


# static public car info
class InitCar:
    def __init__(self, name, sName, codename, teamName, carName, category, raceNumber, startRank, driverType,
                 networkPlayer, skillLevel, iconColor1, iconColor2, iconColor3, dimension, drvPos, bonnetPos, tank,
                 steerLock, statGC, wheel1, wheel2, wheel3, wheel4, visualAttr, masterModel, skinName, skinTargets):
        self.name = a


# Wheels specification
class WheelSpecifications:
    def __init__(self, rimRadius, tireHeight, tireWidth, brakeDiskRadius, wheelRadius):
        self.rimRadius = a


# static visual attributes
class VisualAttributes:
    def __init__(self, exhaustNb, exhaustPos1, exhaustPos2, exhaustPower):
        self.exhaustNb = a


# public info on the cars
class PublicCar:
    def __init__(self, dynGC, dynGCg, speed, posMat1, posMat2, posMat3, posMat4, posMat5, posMat6, posMat7, posMat8,
                 posMat9, posMat10, posMat11, posMat12, posMat13, posMat14, posMat15, posMat16, trkPos, state):
        self.dynGC = a


# dynamic point structure
class DynamicPoint:
    def __init__(self, pos, vel, acc):
        self.pos = a


# location on the track in local coordinates
class TrackLocation:
    def __init__(self, seg, type, toStart, toRight, toMiddle, toLeft):
        self.seg = s


# Track Segment
class TrackSegment:
    def __init__(self, id, type, type2, style, length, width, startWidth, endWidth, lgFromStart, radius, radiusR,
                 radiusL, arc, center, vertex1, vertex2, vertex3, vertex4, angle1, angle2, angle3, angle4, angle5,
                 angle6, angle7, sin, cos, kzl, kzw, kyl, rgtSideNormal, envIndex, height,
                 raceInfo, doVFactor):  # TODO: , speedLimit):
        self.id = a


# Race Administrative info
class CarRaceInfo:
    def __init__(self, bestLapTime, commitBestLapTime, deltaBestLapTime, curLapTime,
                 lastLapTime, curTime, topSpeed, currentMinSpeedForLap, laps, bestLap, nbPitStops, remainingLaps, pos,
                 timeBehindLeader, lapsBehindLeader, timeBehindPrev, timeBeforeNext, distRaced, distFromStartLine,
                 currentSector, nbSectors, trackLength, scheduledEventTime, event, penaltyTime,
                 prevFromStartLine, wrongWayTime):
        self.bestLapTime = a


#
class PrivCar:
    def __init__(self, driverIndex, moduleIndex, modName, corner1, corner2, corner3, corner4, gear, gearNext, fuel,
                 fuelConsumptionTotal, fuelConsumptionInstant, engineRPM, engineRPMRedLine, engineRPMMax,
                 engineRPMMaxTq, engineRPMMaxPw, engineMaxTq, engineMaxPw, gearRatio1, gearRatio2, gearRatio3,
                 gearRatio4, gearRatio5, gearRatio6, gearRatio7, gearRatio8, gearRatio9, gearRatio10, gearNb,
                 gearOffset, skid1, skid2, skid3, skid4, reaction1, reaction2, reaction3, reaction4, collision,
                 simCollision, smoke, normal, collPos, damage, debug, collisionState, localPressure, driveSkill,
                 steerTqCenter, steerTqAlign, dashboardInstantNb, dashboardRequestNb, dashboardActiveItem):
        self.driverIndex = a


# six degrees of freedom position
class PosD:
    def __init__(self, x, y, z, xy, ax, ay, az):
        self.x = a


# keeps track of the collision
class CollisionState:
    def __init__(self, collisionCount, pos, force):
        self.collisionCount = z


# a vector
class Vector:
    def __init__(self, x, y, z):
        self.x = a


# info returned by the driver during a race
class CarCtrl:
    def __init__(self, steer, accelCmd, brakeCmd, clutchCmd, brakeFrontLeftCmd, brakeFrontRightCmd, brakeRearLeftCmd,
                 brakeRearRightCmd, wingFrontCmd, wingRearCmd, reserved1, reserved2, gear, raceCmd, lightCmd, eBrakeCmd,
                 wingControlMode, singleWheelBrakeMode, switch3, telemetryMode, msg1, msg2, msg3, msg4, msgColor1,
                 msgColor2, msgColor3, msgColor4):
        self.steer = x


# car setup parameters
class CarSetup:
    def __init__(self, fRWeightRep, fRLWeightRep, rRLWeightRep, fuel, wingAngle1, wingAngle2, revsLimiter, gearRatio1,
                 gearRatio2, gearRatio3, gearRatio4, gearRatio5, gearRatio6, gearRatio7, gearRatio8, gearRatio9,
                 gearRatio10, differentialType1, differentialType2, differentialType3, differentialRatio1,
                 differentialRatio2, differentialRatio3, differentialMinTqBias1, differentialMinTqBias2,
                 differentialMinTqBias3, differentialMaxTqBias1, differentialMaxTqBias2, differentialMaxTqBias3,
                 differentialViscosity1, differentialViscosity2, differentialViscosity3, differentialLockingTq1,
                 differentialLockingTq2, differentialLockingTq3, differentialMaxSlipBias1, differentialMaxSlipBias2,
                 differentialMaxSlipBias3, differentialCoastMaxSlipBias1, differentialCoastMaxSlipBias2,
                 differentialCoastMaxSlipBias3, steerLock, brakeRepartition, brakePressure, rideHeight1, rideHeight2,
                 rideHeight3, rideHeight4, toe1, toe2, toe3, toe4, camber1, camber2, camber3, camber4, tirePressure1,
                 tirePressure2, tirePressure3, tirePressure4, tireOpLoad1, tireOpLoad2, tireOpLoad3, tireOpLoad4,
                 arbSpring1, arbSpring2, arbBellCrank1, arbBellCrank2, heaveSpring1, heaveSpring2, heaveBellCrank1,
                 heaveBellCrank2, heaveInertance1, heaveInertance2, heaveFastBump1, heaveFastBump2, heaveSlowBump1,
                 heaveSlowBump2, heaveBumpLevel1, heaveBumpLevel2, heaveFastRebound1, heaveFastRebound2,
                 heaveSlowRebound1, heaveSlowRebound2, heaveReboundLevel1, heaveReboundLevel2, suspSpring1, suspSpring2,
                 suspSpring3, suspSpring4, suspBellCrank1, suspBellCrank2, suspBellCrank3, suspBellCrank4,
                 suspInertance1, suspInertance2, suspInertance3, suspInertance4, suspCourse1, suspCourse2, suspCourse3,
                 suspCourse4, suspPacker1, suspPacker2, suspPacker3, suspPacker4, suspFastBump1, suspFastBump2,
                 suspFastBump3, suspFastBump4, suspSlowBump1, suspSlowBump2, suspSlowBump3, suspSlowBump4,
                 suspBumpLevel1, suspBumpLevel2, suspBumpLevel3, suspBumpLevel4, suspFastRebound1, suspFastRebound2,
                 suspFastRebound3, suspFastRebound4, suspSlowRebound1, suspSlowRebound2, suspSlowRebound3,
                 suspSlowRebound4, suspReboundLevel1, suspReboundLevel2, suspReboundLevel3, suspReboundLevel4,
                 reqRepair, reqTireset, reqPenalty):
        self.fRWeightRep = x


# storing one parameter of the car setup
class CarSetupItem:
    def __init__(self, value, min, max, desiredValue, stepSize, changed):
        self.value = x


# car during pit stop
class CarPitCmd:
    def __init__(self, fuel, repair, stopType, setupChanged, tireChange):
        self.fuel = x


# the cars situation
class Situation:
    def __init__(self, raceInfo, deltaTime, currentTime, accelTime, nbPlayers):
        self.raceInfo = x


# general info on the current race
class RaceInfo:
    def __init__(self, ncars, totLaps, extraLaps, totTime, state, type, maxDamage, fps, features):
        self.ncars = x
