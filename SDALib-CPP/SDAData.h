#pragma once

struct SDAData
{
public:
	SDAData(const char* msg)
	{

#ifdef SDA_SPEED_DATA

#endif

#ifdef SDA_TOP_SPEED_DATA

#endif

#ifdef SDA_STEER_CMD_DATA

#endif

#ifdef SDA_ACCEL_CMD_DATA

#endif

#ifdef SDA_BRAKE_CMD_DATA

#endif

#ifdef SDA_CLUTCH_CMD_DATA

#endif

#ifdef SDA_TIME_LAST_STEER

#endif

#ifdef SDA_OFFROAD_DATA

#endif

#ifdef SDA_TIME_OF_DAY_DATA

#endif

#ifdef SDA_CLOUDS_DATA

#endif


	}


#ifdef SDA_SPEED_DATA
private:
	float m_speed;
public:
	float Speed() const { return m_speed; }
#endif

#ifdef SDA_TOP_SPEED_DATA
private:
	float m_topSpeed;
public:
	float TopSpeed() const { return m_topSpeed; }
#endif

#ifdef SDA_STEER_CMD_DATA
private:
	float m_steerCmd;
public:
	float SteerCmd() const { return m_steerCmd; }
#endif

#ifdef SDA_ACCEL_CMD_DATA
private:
	float m_accelCmd;
public:
	float AccelCmd() const { return m_accelCmd; }
#endif

#ifdef SDA_BRAKE_CMD_DATA
private:
	float m_brakeCmd;
public:
	float BrakeCmd() const { return m_brakeCmd; }
#endif

#ifdef SDA_CLUTCH_CMD_DATA
private:
	float m_clutchCmd;
public:
	float ClutchCmd() const { return m_clutchCmd; }
#endif

#ifdef SDA_TIME_LAST_STEER
private:
	float m_timeLastSteer;
public:
	float TimeLastSteer() const { return m_timeLastSteer; }
#endif

#ifdef SDA_OFFROAD_DATA
private:
	bool m_offroad;
public:
	float Offroad() const { return m_offroad; }
#endif

#ifdef SDA_TIME_OF_DAY_DATA
private:
	int m_timeOfDay;
public:
	int TimeOfDay() const { return m_timeOfDay; }
#endif

#ifdef SDA_CLOUDS_DATA
private:
	int m_clouds;
public:
	int Clouds() const { return m_clouds; }
#endif
};

