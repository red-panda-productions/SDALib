# object containing all data send to the decision maker
class SDATypes:
    def __init__(self, car, situation, tickCount):
        self.tickCount = tickCount  # int
        self.car = car  # Car
        self.situation = situation  # Situation


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
                 networkPlayer, skillLevel, iconColor, dimension, drvPos, bonnetPos, tank, steerLock, statGC, wheel,
                 visualAttr, masterModel, skinName, skinTargets):
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
        self.iconColor = iconColor  # float[]
        self.dimension = dimension  # Vector
        self.drvPos = drvPos  # Vector
        self.bonnetPos = bonnetPos  # Vector
        self.tank = tank  # float
        self.steerLock = steerLock  # float
        self.statGC = statGC  # Vector
        self.wheel = wheel  # WheelSpecifications[]
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
    def __init__(self, exhaustNb, exhaustPos, exhaustPower):
        self.exhaustNb = exhaustNb  # int
        self.exhaustPos = exhaustPos  # Vector[]
        self.exhaustPower = exhaustPower  # float


# public info on the cars
class PublicCar:
    def __init__(self, dynGC, dynGCg, speed, posMat, trkPos, state):
        self.dynGC = dynGC  # DynamicPoint
        self.dynGCg = dynGCg  # DynamicPoint
        self.speed = speed  # float
        self.posMat = posMat  # float[,]
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
    def __init__(self, name, id, type, type2, style, length, width, startWidth, endWidth, lgFromStart, radius, radiusR,
                 radiusL, arc, center, vertex, angle, sin, cos, kzl, kzw, kyl, rgtSideNormal, envIndex, height,
                 raceInfo, doVFactor):
        self.name = name  # string
        self.id = id  # int
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
        self.vertex = vertex  # Vector[]
        self.angle = angle  # float[]
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
    def __init__(self, driverIndex, moduleIndex, modName, corner, gear, gearNext, fuel, fuelConsumptionTotal,
                 fuelConsumptionInstant, engineRPM, engineRPMRedLine, engineRPMMax, engineRPMMaxTq, engineRPMMaxPw,
                 engineMaxTq, engineMaxPw, gearRatio, gearNb, gearOffset, skid, reaction, collision, simCollision,
                 smoke, normal, collPos, damage, debug, collisionState, localPressure, driveSkill,
                 steerTqCenter, steerTqAlign, dashboardInstantNb, dashboardRequestNb, dashabordActiveItem):
        self.driverIndex = driverIndex  # int
        self.moduleIndex = moduleIndex  # int
        self.modName = modName  # string
        self.corner = corner  # PosD[]
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
        self.gearRatio = gearRatio  # float[]
        self.gearNb = gearNb  # int
        self.gearOffset = gearOffset  # int
        self.skid = skid  # float[]
        self.reaction = reaction  # float[]
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
        self.dashboardActiveItem = dashabordActiveItem  # int


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


#
class CarCtrl:
    def __init__(self, steer, accelCmd, brakeCmd, clutchCmd, brakeFrontLeftCmd, brakeFrontRightCmd, brakeRearLeftCmd,
                 brakeRearRightCmd, wingFrontCmd, wingRearCmd, reserved1, reserved2, gear, raceCmd, lightCmd, eBrakeCmd,
                 wingControlMode, singleWheelBrakeMode, switch3, telemetryMode, msg, msgColor):
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
        self.msg = msg  # string[]
        self.msgColor = msgColor  # float[]


# car setup parameters
class CarSetup:
    def __init__(self, fRWeightRep, fRLWeightRep, rRLWeightRep, fuel, wingAngle, revsLimiter, gearRatio,
                 differentialType, differentialRatio, differentialMinTqBias, differentialMaxTqBias,
                 differentialViscosity, differentialLockingTq, differentialMaxSlipBias, differentialCoastMaxSlipBias,
                 steerLock, brakeRepartition, brakePressure, rideHeight, toe, camber, tirePressure, tireOpLoad,
                 arbSprint, heaveBellCrank, heaveIntertance, heaveFastBump, heaveSlowBump, heaveBumpLevel,
                 heaveFastRebound, heaveSlowRebound, heaveReboundLevel, suspSprint, suspBellCrank, suspInertance,
                 suspCourse, suspPacker, suspFastBump, suspSlowBump, suspBumpLevel, suspFastRebound, suspSlowRebound,
                 suspReboundLevel, reqRepair, reqTireset, reqPenalty):
        self.fRWeightRep = fRWeightRep  # CarSetupItem
        self.fRLWeightRep = fRLWeightRep  # CarSetupItem
        self.rRLWeightRep = rRLWeightRep  # CarSetupItem
        self.fuel = fuel  # CarSetupItem
        self.wingAngle = wingAngle  # CarSetupItem[]
        self.revsLimiter = revsLimiter  # CarSetupItem
        self.gearRatio = gearRatio  # CarSetupItem[]
        self.differentialType = differentialType  # int[]
        self.differentialRatio = differentialRatio  # CarSetupItem[]
        self.differentialMinTqBias = differentialMinTqBias  # CarSetupItem[]
        self.differentialMaxTqBias = differentialMaxTqBias  # CarSetupItem[]
        self.differentialViscosity = differentialViscosity  # CarSetupItem[]
        self.differentialLockingTq = differentialLockingTq  # CarSetupItem[]
        self.differentialMaxSlipBias = differentialMaxSlipBias  # CarSetupItem[]
        self.differentialCoastMaxSlipBias = differentialCoastMaxSlipBias  # CarSetupItem[]
        self.steerLock = steerLock  # CarSetupItem
        self.brakeRepartition = brakeRepartition  # CarSetupItem
        self.brakePressure = brakePressure  # CarSetupItem
        self.rideHeight = rideHeight  # CarSetupItem[]
        self.toe = toe  # CarSetupItem[]
        self.camber = camber  # CarSetupItem[]
        self.tirePressure = tirePressure  # CarSetupItem[]
        self.tireOpLoad = tireOpLoad  # CarSetupItem[]
        self.arbSprint = arbSprint  # CarSetupItem[]
        self.heaveBellCrank = heaveBellCrank  # CarSetupItem[]
        self.heaveIntertance = heaveIntertance  # CarSetupItem[]
        self.heaveFastBump = heaveFastBump  # CarSetupItem[]
        self.heaveSlowBump = heaveSlowBump  # CarSetupItem[]
        self.heaveBumpLevel = heaveBumpLevel  # CarSetupItem[]
        self.heaveFastRebound = heaveFastRebound  # CarSetupItem[]
        self.heaveSlowRebound = heaveSlowRebound  # CarSetupItem[]
        self.heaveReboundLevel = heaveReboundLevel  # CarSetupItem[]
        self.suspSprint = suspSprint  # CarSetupItem[]
        self.suspBellCrank = suspBellCrank  # CarSetupItem[]
        self.suspInertance = suspInertance  # CarSetupItem[]
        self.suspCourse = suspCourse  # CarSetupItem[]
        self.suspPacker = suspPacker  # CarSetupItem[]
        self.suspFastBump = suspFastBump  # CarSetupItem[]
        self.suspSlowBump = suspSlowBump  # CarSetupItem[]
        self.suspBumpLevel = suspBumpLevel  # CarSetupItem[]
        self.suspFastRebound = suspFastRebound  # CarSetupItem[]
        self.suspSlowRebound = suspSlowRebound  # CarSetupItem[]
        self.suspReboundLevel = suspReboundLevel  # CarSetupItem[]
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
        self.setupChanges = setupChanged  # bool
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
