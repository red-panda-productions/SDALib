import SDATypes

class SDADriver:
    speedLimit = 80
    def __init__(self):
        return

    def UpdateAI(self, SDAData):

        #tests whether this can be called
        steer = SDAData.car.pub.dynGC.vel.x
        accel = SDAData.situation.deltaTime
        brake = SDAData.situation.raceInfo.totTime
        clutch = 0

        return steer, accel, brake, clutch

    def clamp(self, value, min_value, max_value):
        if value < min_value:
            return min_value
        if value > max_value:
            return max_value
        return value