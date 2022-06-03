import SDATypes

class SDADriver:
    speedLimit = 80
    def __init__(self):
        return

    def UpdateAI(self, SDAData):
        speed = SDAData.car.pub.dynGC.vel.x * 3.6
        # speedLimit = SDAData.speedLimit

        accel = 0  # float between 0 and 1
        brake = 0  # float between 0 and 1

        if speed > self.speedLimit:
            brake = (speed - self.speedLimit) / 10
        elif speed < self.speedLimit:
            accel = (self.speedLimit - speed) / 10

        # SaveToDatabase(str(brake))
        # SaveToDatabase(str(accel))

        return 0, 5, 3, 1  # steer, accel, brake, clutch

    def clamp(self, value, min_value, max_value):
        if value < min_value:
            return min_value
        if value > max_value:
            return max_value
        return value