#  This program has been developed by students from the bachelor Computer Science at
#  Utrecht University within the Software Project course.
#  © Copyright Utrecht University (Department of Information and Computing Sciences)

import SDATypes

class SDADriver:
    speedLimit = 80

    def __init__(self):
        return

    def UpdateAI(self, SDAData):
        # tests whether this can be called
        steer = SDAData.car.pub.dynGC.vel.x
        accel = SDAData.situation.deltaTime
        brake = SDAData.situation.raceInfo.totTime
        clutch = 0

        return steer, accel, brake, clutch
