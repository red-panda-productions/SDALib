class SDADriver:
    def __init__(self):
        return

    def UpdateAI(self, SDAData):
        speed = SDAData.car.pub.dynGC.vel.X * 3.6
        speedLimit = 80  # SDAData.speedLimit

        accel = 0  # float between 0 and 1
        brake = 0  # float between 0 and 1

        if speed > speedLimit:
            brake = (speed - speedLimit) / 10
        elif speed < speedLimit:
            accel = (speedLimit - speed) / 10

        # SaveToDatabase(str(brake))
        # SaveToDatabase(str(accel))

        return 0, self.clamp(accel, 0, 1), self.clamp(brake, 0, 1), 0  # steer, accel, brake, clutch

    def clamp(self, value, min_value, max_value):
        if value < min_value:
            return min_value
        if value > max_value:
            return max_value
        return value
