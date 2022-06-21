import sys
sys.path.append("C:\Q3_2022\Software_Project\Project\SDALib\SDALib-CPP\cmake-build-debug\SDALib-Python")

import SDATypes
import simulator

class SDADriver:
    speedLimit = 80

    def __init__(self):
        return

    def UpdateAI(self, sdaData):
        # tests whether this can be called
        sdaAction = SDATypes.SDAAction(0, 0, 0, 0)

        newSDAData = SDATypes.SDAData(0, 0, 0)
        simulator.update(sdaData, sdaAction, newSDAData)

        sdaAction.steer = sdaData.car.pub.dynGC.vel.x
        sdaAction.accel = sdaData.situation.deltaTime
        sdaAction.brake = sdaData.situation.raceInfo.totTime
        sdaAction.clutch = newSDAData.situation.raceInfo.totTime

        return sdaAction
