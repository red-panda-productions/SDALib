#  This program has been developed by students from the bachelor Computer Science at
#  Utrecht University within the Software Project course.
#  Copyright Utrecht University (Department of Information and Computing Sciences)

import site
import sys

sys.path.append(site.getsitepackages())

import SDATypes

class SDADriver:
    speedLimit = 80

    def __init__(self):
        return

    # returns the decision maker action from the current SDAData
    def UpdateAI(self, sdaData):
        # find the velocity of the car
        speed = sdaData.car.pub.dynGC.vel.x * 3.6
        self.speedLimit = sdaData.car.pub.trkPos.seg.speedLimit

        accel = 0  # float between 0 and 1
        brake = 0  # float between 0 and 1

        # brake if the user goes over the speed limit
        if speed > self.speedLimit:
            brake = (speed - self.speedLimit) / 10
        # accelerate if the user goes under the speed limit
        elif speed < self.speedLimit:
            accel = (self.speedLimit - speed) / 10

        return SDATypes.SDAAction(0, self.clamp(accel, 0, 1), self.clamp(brake, 0, 1), 0)  # steer, accel, brake, clutch

    # clamps the values value between min_value and max_value
    def clamp(self, value, min_value, max_value):
        if value < min_value:
            return min_value
        if value > max_value:
            return max_value
        return value