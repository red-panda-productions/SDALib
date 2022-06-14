import SDATypes
import SpeedDreamsFunction


class SDADriver:
    speedLimit = 80

    def __init__(self):
        return

    def UpdateAI(self, sdaData):
        # tests whether this can be called
        sdaAction = SDATypes.SDAAction(0, 0, 0, 0)

        newSDAData = SpeedDreamsFunction.call(sdaData, sdaAction)

        sdaAction.steer = sdaData.car.pub.dynGC.vel.x
        sdaAction.accel = sdaData.situation.deltaTime
        sdaAction.brake = sdaData.situation.raceInfo.totTime
        sdaAction.clutch = newSDAData.situation.raceInfo.totTime

        return sdaAction
