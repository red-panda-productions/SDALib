#  This program has been developed by students from the bachelor Computer Science at
#  Utrecht University within the Software Project course.
#  © Copyright Utrecht University (Department of Information and Computing Sciences)

import site
import sys

sys.path.append(site.getsitepackages())

import SDATypes
import simulator

class SDADriver:
    speedLimit = 80

    def __init__(self):
        return

    def UpdateAI(self, sdaData):
        # tests whether this can be called
        sdaAction = SDATypes.SDAAction(1, 1, 1, 1)

        newSDAData = sdaData #should be a valid SDAData struct
        simulator.update(sdaData, sdaAction, newSDAData)

        sdaAction.steer = sdaData.car.pub.dynGC.vel.x
        sdaAction.accel = sdaData.situation.deltaTime
        sdaAction.brake = sdaData.situation.raceInfo.totTime
        sdaAction.clutch = newSDAData.tickCount

        return sdaAction
