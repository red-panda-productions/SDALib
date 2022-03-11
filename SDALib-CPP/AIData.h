#pragma once

#define SDA_TIME_OF_DAY_DATA

struct SDAData
{
public:
	SDAData(const char* msg)
	{
#ifdef SDA_SPEED_DATA
		//deserialize speed
#endif


#ifdef SDA_TIME_OF_DAY_DATA
		//deserialize timeOfDay
#endif
	}
#ifdef SDA_SPEED_DATA
private:
	float m_speed;
public:
	float Speed() { return m_speed; }
#endif

#ifdef SDA_TIME_OF_DAY_DATA
private:
	float m_timeOfDay;
public:
	float TimeOfDay() { return m_timeOfDay; }
#endif

};

