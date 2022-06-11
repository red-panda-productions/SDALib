# object containing all data send to the decision maker
class SDATypes:
    def __init__(self, car, situation, tickCount):
        self.car = car  # Car
        self.situation = situation  # Situation
        self.tickCount = tickCount  # long


# the main car structure
class Car:
    def __init__(self, index, info, pub, race, priv, ctrl, setup, pitcmd):
        self.index = index  # int
        self.info = info  # InitCar
        self.pub = pub  # PublicCar
        self.race = race  # CarRaceInfo
        self.priv = priv  # PrivCar
        self.ctrl = ctrl  # CarCtrl
        self.setup = setup  # CarSetup
        self.pitcmd = pitcmd  # CarPitCmd


# static public car info
class InitCar:
    def __init__(self, name, sName, codename, teamName, carName, category, raceNumber, startRank, driverType,
                 networkPlayer, skillLevel, iconColor1, iconColor2, iconColor3, dimension, drvPos, bonnetPos, tank,
                 steerLock, statGC, wheel1, wheel2, wheel3, wheel4, visualAttr, masterModel, skinName, skinTargets):
        self.name = name  # string
        self.sName = sName  # string
        self.codename = codename  # string
        self.teamName = teamName  # string
        self.carName = carName  # string
        self.category = category  # string
        self.raceNumber = raceNumber  # int
        self.startRank = startRank  # int
        self.driverType = driverType  # int
        self.networkPlayer = networkPlayer  # int
        self.skillLevel = skillLevel  # int
        self.iconColor = [iconColor1, iconColor2, iconColor3]  # float[]
        self.dimension = dimension  # Vector
        self.drvPos = drvPos  # Vector
        self.bonnetPos = bonnetPos  # Vector
        self.tank = tank  # float
        self.steerLock = steerLock  # float
        self.statGC = statGC  # Vector
        self.wheel = [wheel1, wheel2, wheel3, wheel4]  # WheelSpecifications[]
        self.visualAttr = visualAttr  # VisualAttributes
        self.masterModel = masterModel  # string
        self.skinName = skinName  # string
        self.skinTargets = skinTargets  # int


# Wheels specification
class WheelSpecifications:
    def __init__(self, rimRadius, tireHeight, tireWidth, brakeDiskRadius, wheelRadius):
        self.rimRadius = rimRadius  # float
        self.tireHeight = tireHeight  # float
        self.tireWidth = tireWidth  # float
        self.brakeDiskRadius = brakeDiskRadius  # float
        self.wheelRadius = wheelRadius  # float


# static visual attributes
class VisualAttributes:
    def __init__(self, exhaustNb, exhaustPos1, exhaustPos2, exhaustPower):
        self.exhaustNb = exhaustNb  # int
        self.exhaustPos = [exhaustPos1, exhaustPos2]  # Vector[]
        self.exhaustPower = exhaustPower  # float


# public info on the cars
class PublicCar:
    def __init__(self, dynGC, dynGCg, speed, posMat1, posMat2, posMat3, posMat4, posMat5, posMat6, posMat7, posMat8,
                 posMat9, posMat10, posMat11, posMat12, posMat13, posMat14, posMat15, posMat16, trkPos, state):
        self.dynGC = dynGC  # DynamicPoint
        self.dynGCg = dynGCg  # DynamicPoint
        self.speed = speed  # float
        self.posMat = [[posMat1, posMat2, posMat3, posMat4], [posMat5, posMat6, posMat7, posMat8],
                       [posMat9, posMat10, posMat11, posMat12], [posMat13, posMat14, posMat15, posMat16]]  # float[][]
        self.trkPos = trkPos  # TrackLocation
        self.state = state  # int


# dynamic point structure
class DynamicPoint:
    def __init__(self, pos, vel, acc):
        self.pos = pos  # Vector
        self.vel = vel  # Vector
        self.acc = acc  # Vector


# location on the track in local coordinates
class TrackLocation:
    def __init__(self, seg, type, toStart, toRight, toMiddle, toLeft):
        self.seg = seg  # TrackSegment
        self.type = type  # int
        self.toStart = toStart  # float
        self.toRight = toRight  # float
        self.toMiddle = toMiddle  # float
        self.toLeft = toLeft  # float


# Track Segment
class TrackSegment:
    def __init__(self, id, type, type2, style, length, width, startWidth, endWidth, lgFromStart, radius, radiusR,
                 radiusL, arc, center, vertex1, vertex2, vertex3, vertex4, angle1, angle2, angle3, angle4, angle5,
                 angle6, angle7, sin, cos, kzl, kzw, kyl, rgtSideNormal, envIndex, height,
                 raceInfo, doVFactor, speedLimit):
        self.trackId = id  # int
        self.type = type  # int
        self.type2 = type2  # int
        self.style = style  # int
        self.length = length  # float
        self.width = width  # float
        self.startWidth = startWidth  # float
        self.endWidth = endWidth  # float
        self.lgFromStart = lgFromStart  # float
        self.radius = radius  # float
        self.radiusR = radiusR  # float
        self.radiusL = radiusL  # float
        self.arc = arc  # float
        self.center = center  # Vector
        self.vertex = [vertex1, vertex2, vertex3, vertex4]  # Vector[]
        self.angle = [angle1, angle2, angle3, angle4, angle5, angle6, angle7]  # float[]
        self.sin = sin  # float
        self.cos = cos  # float
        self.kzl = kzl  # float
        self.kzw = kzw  # float
        self.kyl = kyl  # float
        self.rgtSideNormal = rgtSideNormal  # Vector
        self.envIndex = envIndex  # int
        self.height = height  # float
        self.raceInfo = raceInfo  # int
        self.doVFactor = doVFactor  # float
        self.speedLimit = speedLimit  # float


# Race Administrative info
class CarRaceInfo:
    def __init__(self, bestLapTime, commitBestLapTime, deltaBestLapTime, curLapTime,
                 lastLapTime, curTime, topSpeed, currentMinSpeedForLap, laps, bestLap, nbPitStops, remainingLaps, pos,
                 timeBehindLeader, lapsBehindLeader, timeBehindPrev, timeBeforeNext, distRaced, distFromStartLine,
                 currentSector, nbSectors, trackLength, scheduledEventTime, event, penaltyTime,
                 prevFromStartLine, wrongWayTime):
        self.bestLapTime = bestLapTime  # float
        self.commitBestLapTime = commitBestLapTime  # bool
        self.deltaBestLapTime = deltaBestLapTime  # float
        self.curLapTime = curLapTime  # float
        self.lastLapTime = lastLapTime  # float
        self.curTime = curTime  # float
        self.topSpeed = topSpeed  # float
        self.currentMinSpeedForLap = currentMinSpeedForLap  # float
        self.laps = laps  # int
        self.bestLap = bestLap  # int
        self.nbPitStops = nbPitStops  # int
        self.remainingLaps = remainingLaps  # int
        self.pos = pos  # int
        self.timeBehindLeader = timeBehindLeader  # float
        self.lapsBehindLeader = lapsBehindLeader  # int
        self.timeBehindPrev = timeBehindPrev  # float
        self.timeBeforeNext = timeBeforeNext  # float
        self.distRaced = distRaced  # float
        self.distFromStartLine = distFromStartLine  # float
        self.currentSector = currentSector  # int
        self.nbSectors = nbSectors  # int
        self.trackLength = trackLength  # float
        self.scheduledEventTime = scheduledEventTime  # float
        self.event = event  # int
        self.penaltyTime = penaltyTime  # float
        self.prevFromStartLine = prevFromStartLine  # float
        self.wrongWayTime = wrongWayTime  # float


#
class PrivCar:
    def __init__(self, driverIndex, moduleIndex, modName, corner1, corner2, corner3, corner4, gear, gearNext, fuel,
                 fuelConsumptionTotal, fuelConsumptionInstant, engineRPM, engineRPMRedLine, engineRPMMax,
                 engineRPMMaxTq, engineRPMMaxPw, engineMaxTq, engineMaxPw, gearRatio1, gearRatio2, gearRatio3,
                 gearRatio4, gearRatio5, gearRatio6, gearRatio7, gearRatio8, gearRatio9, gearRatio10, gearNb,
                 gearOffset, skid1, skid2, skid3, skid4, reaction1, reaction2, reaction3, reaction4, collision,
                 simCollision, smoke, normal, collPos, damage, debug, collisionState, localPressure, driveSkill,
                 steerTqCenter, steerTqAlign, dashboardInstantNb, dashboardRequestNb, dashboardActiveItem):
        self.driverIndex = driverIndex  # int
        self.moduleIndex = moduleIndex  # int
        self.modName = modName  # string
        self.corner = [corner1, corner2, corner3, corner4]  # PosD[]
        self.gear = gear  # int
        self.gearNext = gearNext  # int
        self.fuel = fuel  # float
        self.fuelConsumptionTotal = fuelConsumptionTotal  # float
        self.fuelConsumptionInstant = fuelConsumptionInstant  # float
        self.engineRPM = engineRPM  # float
        self.engineRPMRedLine = engineRPMRedLine  # float
        self.engineRPMMax = engineRPMMax  # float
        self.engineRPMMaxTq = engineRPMMaxTq  # float
        self.engineRPMMaxPw = engineRPMMaxPw  # float
        self.engineMaxTq = engineMaxTq  # float
        self.engineMaxPw = engineMaxPw  # float
        self.gearRatio = [gearRatio1, gearRatio2, gearRatio3, gearRatio4, gearRatio5, gearRatio6, gearRatio7,
                          gearRatio8, gearRatio9, gearRatio10]  # float[]
        self.gearNb = gearNb  # int
        self.gearOffset = gearOffset  # int
        self.skid = [skid1, skid2, skid3, skid4]  # float[]
        self.reaction = [reaction1, reaction2, reaction3, reaction4]  # float[]
        self.collision = collision  # int
        self.simCollision = simCollision  # int
        self.smoke = smoke  # float
        self.normal = normal  # Vector
        self.collPos = collPos  # Vector
        self.damage = damage  # int
        self.debug = debug  # int
        self.collisionState = collisionState  # CollisionState
        self.localPressure = localPressure  # float
        self.driveSkill = driveSkill  # float
        self.steerTqCenter = steerTqCenter  # float
        self.steerTqAlign = steerTqAlign  # float
        self.dashboardInstantNb = dashboardInstantNb  # int
        self.dashboardRequestNb = dashboardRequestNb  # int
        self.dashboardActiveItem = dashboardActiveItem  # int


# six degrees of freedom position
class PosD:
    def __init__(self, x, y, z, xy, ax, ay, az):
        self.x = x  # float
        self.y = y  # float
        self.z = z  # float
        self.xy = xy  # float
        self.ax = ax  # float
        self.ay = ay  # float
        self.az = az  # float


# keeps track of the collision
class CollisionState:
    def __init__(self, collisionCount, pos, force):
        self.collisionCount = collisionCount  # int
        self.pos = pos  # Vector
        self.force = force  # Vector


# a vector
class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


# info returned by the driver during a race
class CarCtrl:
    def __init__(self, steer, accelCmd, brakeCmd, clutchCmd, brakeFrontLeftCmd, brakeFrontRightCmd, brakeRearLeftCmd,
                 brakeRearRightCmd, wingFrontCmd, wingRearCmd, reserved1, reserved2, gear, raceCmd, lightCmd, eBrakeCmd,
                 wingControlMode, singleWheelBrakeMode, switch3, telemetryMode, msg1, msg2, msg3, msg4, msgColor1,
                 msgColor2, msgColor3, msgColor4):
        self.steer = steer  # float
        self.accelCmd = accelCmd  # float
        self.brakeCmd = brakeCmd  # float
        self.clutchCmd = clutchCmd  # float
        self.brakeFrontLeftCmd = brakeFrontLeftCmd  # float
        self.brakeFrontRightCmd = brakeFrontRightCmd  # float
        self.brakeRearLeftCmd = brakeRearLeftCmd  # float
        self.brakeRearRightCmd = brakeRearRightCmd  # float
        self.wingFrontCmd = wingFrontCmd  # float
        self.wingRearCmd = wingRearCmd  # float
        self.reserved1 = reserved1  # float
        self.reserved2 = reserved2  # float
        self.gear = gear  # int
        self.raceCmd = raceCmd  # int
        self.lightCmd = lightCmd  # int
        self.eBrakeCmd = eBrakeCmd  # int
        self.wingControlMode = wingControlMode  # int
        self.singleWheelBrakeMode = singleWheelBrakeMode  # int
        self.switch3 = switch3  # int
        self.telemetryMode = telemetryMode  # int
        self.msg = [msg1, msg2, msg3, msg4]  # string[]
        self.msgColor = [msgColor1, msgColor2, msgColor3, msgColor4]  # float[]


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
        self.fRWeightRep = fRWeightRep  # CarSetupItem
        self.fRLWeightRep = fRLWeightRep  # CarSetupItem
        self.rRLWeightRep = rRLWeightRep  # CarSetupItem
        self.fuel = fuel  # CarSetupItem
        self.wingAngle = [wingAngle1, wingAngle2]  # CarSetupItem[]
        self.revsLimiter = revsLimiter  # CarSetupItem
        self.gearRatio = [gearRatio1, gearRatio2, gearRatio3, gearRatio4, gearRatio5, gearRatio6, gearRatio7,
                          gearRatio8, gearRatio9, gearRatio10]  # CarSetupItem[]
        self.differentialType = [differentialType1, differentialType2, differentialType3]  # int[]
        self.differentialRatio = [differentialRatio1, differentialRatio2, differentialRatio3]  # CarSetupItem[]
        self.differentialMinTqBias = [differentialMinTqBias1, differentialMinTqBias2,
                                      differentialMinTqBias3]  # CarSetupItem[]
        self.differentialMaxTqBias = [differentialMaxTqBias1, differentialMaxTqBias2,
                                      differentialMaxTqBias3]  # CarSetupItem[]
        self.differentialViscosity = [differentialViscosity1, differentialViscosity2,
                                      differentialViscosity3]  # CarSetupItem[]
        self.differentialLockingTq = [differentialLockingTq1, differentialLockingTq2,
                                      differentialLockingTq3]  # CarSetupItem[]
        self.differentialMaxSlipBias = [differentialMaxSlipBias1, differentialMaxSlipBias2,
                                        differentialMaxSlipBias3]  # CarSetupItem[]
        self.differentialCoastMaxSlipBias = [differentialCoastMaxSlipBias1, differentialCoastMaxSlipBias2,
                                             differentialCoastMaxSlipBias3]  # CarSetupItem[]
        self.steerLock = steerLock  # CarSetupItem
        self.brakeRepartition = brakeRepartition  # CarSetupItem
        self.brakePressure = brakePressure  # CarSetupItem
        self.rideHeight = [rideHeight1, rideHeight2, rideHeight3, rideHeight4]  # CarSetupItem[]
        self.toe = [toe1, toe2, toe3, toe4]  # CarSetupItem[]
        self.camber = [camber1, camber2, camber3, camber4]  # CarSetupItem[]
        self.tirePressure = [tirePressure1, tirePressure2, tirePressure3, tirePressure4]  # CarSetupItem[]
        self.tireOpLoad = [tireOpLoad1, tireOpLoad2, tireOpLoad3, tireOpLoad4]  # CarSetupItem[]
        self.arbSpring = [arbSpring1, arbSpring2]  # CarSetupItem[]
        self.arbBellCrank = [arbBellCrank1, arbBellCrank2]  # CarSetupItem[]
        self.heaveSpring = [heaveSpring1, heaveSpring2]  # CarSetupItem[]
        self.heaveBellCrank = [heaveBellCrank1, heaveBellCrank2]  # CarSetupItem[]
        self.heaveInertance = [heaveInertance1, heaveInertance2]  # CarSetupItem[]
        self.heaveFastBump = [heaveFastBump1, heaveFastBump2]  # CarSetupItem[]
        self.heaveSlowBump = [heaveSlowBump1, heaveSlowBump2]  # CarSetupItem[]
        self.heaveBumpLevel = [heaveBumpLevel1, heaveBumpLevel2]  # CarSetupItem[]
        self.heaveFastRebound = [heaveFastRebound1, heaveFastRebound2]  # CarSetupItem[]
        self.heaveSlowRebound = [heaveSlowRebound1, heaveSlowRebound2]  # CarSetupItem[]
        self.heaveReboundLevel = [heaveReboundLevel1, heaveReboundLevel2]  # CarSetupItem[]
        self.suspSpring = [suspSpring1, suspSpring2, suspSpring3, suspSpring4]  # CarSetupItem[]
        self.suspBellCrank = [suspBellCrank1, suspBellCrank2, suspBellCrank3, suspBellCrank4]  # CarSetupItem[]
        self.suspInertance = [suspInertance1, suspInertance2, suspInertance3, suspInertance4]  # CarSetupItem[]
        self.suspCourse = [suspCourse1, suspCourse2, suspCourse3, suspCourse4]  # CarSetupItem[]
        self.suspPacker = [suspPacker1, suspPacker2, suspPacker3, suspPacker4]  # CarSetupItem[]
        self.suspFastBump = [suspFastBump1, suspFastBump2, suspFastBump3, suspFastBump4]  # CarSetupItem[]
        self.suspSlowBump = [suspSlowBump1, suspSlowBump2, suspSlowBump3, suspSlowBump4]  # CarSetupItem[]
        self.suspBumpLevel = [suspBumpLevel1, suspBumpLevel2, suspBumpLevel3, suspBumpLevel4]  # CarSetupItem[]
        self.suspFastRebound = [suspFastRebound1, suspFastRebound2, suspFastRebound3,
                                suspFastRebound4]  # CarSetupItem[]
        self.suspSlowRebound = [suspSlowRebound1, suspSlowRebound2, suspSlowRebound3,
                                suspSlowRebound4]  # CarSetupItem[]
        self.suspReboundLevel = [suspReboundLevel1, suspReboundLevel2, suspReboundLevel3,
                                 suspReboundLevel4]  # CarSetupItem[]
        self.reqRepair = reqRepair  # CarSetupItem
        self.reqTireset = reqTireset  # CarSetupItem
        self.reqPenalty = reqPenalty  # CarSetupItem


# storing one parameter of the car setup
class CarSetupItem:
    def __init__(self, value, min, max, desiredValue, stepSize, changed):
        self.value = value  # float
        self.min = min  # float
        self.max = max  # float
        self.desiredValue = desiredValue  # float
        self.stepSize = stepSize  # float
        self.changed = changed  # bool


# car during pit stop
class CarPitCmd:
    def __init__(self, fuel, repair, stopType, setupChanged, tireChange):
        self.fuel = fuel  # float
        self.repair = repair  # int
        self.stopType = stopType  # int
        self.setupChanged = setupChanged  # bool
        self.tireChange = tireChange  # bool


# the cars situation
class Situation:
    def __init__(self, raceInfo, deltaTime, currentTime, accelTime, nbPlayers):
        self.raceInfo = raceInfo  # RaceInfo
        self.deltaTime = deltaTime  # float
        self.currentTime = currentTime  # float
        self.accelTime = accelTime  # float
        self.nbPlayers = nbPlayers  # int


# general info on the current race
class RaceInfo:
    def __init__(self, ncars, totLaps, extraLaps, totTime, state, type, maxDamage, fps, features):
        self.ncars = ncars  # int
        self.totLaps = totLaps  # int
        self.extraLaps = extraLaps  # int
        self.totTime = totTime  # float
        self.state = state  # int
        self.type = type  # int
        self.maxDamage = maxDamage  # int
        self.fps = fps  # int
        self.features = features  # int

# reaction from the decision maker
class SDAAction:
    def __init__(self, steer, accel, brake, clutch):
        self.steer = steer
        self.accel = accel
        self.brake = brake
        self.clutch = clutch
