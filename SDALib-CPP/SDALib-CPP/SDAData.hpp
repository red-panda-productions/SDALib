#pragma once
#include <string>
#include <vector>
#include <msgpack.hpp>
#include "sdalib_export.h"

//#define SDA_TIMESTAMP_DATA
//#define SDA_RAIN_DATA
//#define SDA_TO_MIDDLE_DATA
//#define SDA_TO_RIGHT_DATA
//#define SDA_TO_LEFT_DATA
//#define SDA_TO_START_DATA
//#define SDA_SPEED_DATA
//#define SDA_TOP_SPEED_DATA
//#define SDA_GEAR_DATA
//#define SDA_HEADLIGHTS_DATA
//#define SDA_STEER_CMD_DATA
//#define SDA_ACCEL_CMD_DATA
//#define SDA_BRAKE_CMD_DATA
//#define SDA_CLUTCH_CMD_DATA
//#define SDA_TIME_LAST_STEER
//#define SDA_OFFROAD_DATA
//#define SDA_TIME_OF_DAY_DATA
//#define SDA_CLOUDS_DATA

//standard private getter
#define SDADATA_GETTER(type,name,getter_name) \
	private: \
		type name; \
	public: \
		type getter_name(){return name;}

// these macros pre-compute what is needed for the SDAData class
#ifdef SDA_TICKCOUNT_DATA
#define SDA_TICKCOUNT_UNPACK m_tickCount = std::stoul(resultVec[i++]);
#define SDA_TICKCOUNT_GETTER SDADATA_GETTER(unsigned long, m_tickCount, TickCount)
#define SDA_TICKCOUNT_ORDER p_order.push_back("TickCount");
#else
#define SDA_TICKCOUNT_UNPACK
#define SDA_TICKCOUNT_GETTER
#define SDA_TICKCOUNT_ORDER
#endif

#ifdef SDA_SPEED_DATA
#define SDA_SPEED_UNPACK m_speed = std::stof(resultVec[i++]);
#define SDA_SPEED_GETTER SDADATA_GETTER(float,m_speed,Speed)
#define SDA_SPEED_ORDER p_order.push_back("Speed"); 
#else
#define SDA_SPEED_UNPACK
#define SDA_SPEED_GETTER
#define SDA_SPEED_ORDER
#endif

#ifdef SDA_TOP_SPEED_DATA
#define SDA_TOP_SPEED_UNPACK m_topSpeed = std::stof(resultVec[i++]);
#define SDA_TOP_SPEED_GETTER SDADATA_GETTER(float,m_topSpeed,TopSpeed)
#define SDA_TOP_SPEED_ORDER p_order.push_back("TopSpeed"); 
#else
#define SDA_TOP_SPEED_UNPACK
#define SDA_TOP_SPEED_GETTER
#define SDA_TOP_SPEED_ORDER
#endif

#ifdef SDA_GEAR_DATA
#define SDA_GEAR_UNPACK m_gear = std::stoi(resultVec[i++]);
#define SDA_GEAR_GETTER SDADATA_GETTER(int,m_gear,Gear)
#define SDA_GEAR_ORDER p_order.push_back("Gear");
#else
#define SDA_GEAR_UNPACK
#define SDA_GEAR_GETTER
#define SDA_GEAR_ORDER
#endif

#ifdef SDA_HEADLIGHTS_DATA
#define SDA_HEADLIGHTS_UNPACK m_headlights = resultVec[i++] == "1";
#define SDA_HEADLIGHTS_GETTER SDADATA_GETTER(bool,m_headlights,Headlights)
#define SDA_HEADLIGHTS_ORDER p_order.push_back("Headlights"); 
#else
#define SDA_HEADLIGHTS_UNPACK
#define SDA_HEADLIGHTS_GETTER
#define SDA_HEADLIGHTS_ORDER
#endif

#ifdef SDA_STEER_CMD_DATA
#define SDA_STEER_CMD_UNPACK m_steerCmd = std::stof(resultVec[i++]);
#define SDA_STEER_CMD_GETTER SDADATA_GETTER(float,m_steerCmd,SteerCmd)
#define SDA_STEER_CMD_ORDER p_order.push_back("SteerCmd"); 
#else
#define SDA_STEER_CMD_UNPACK
#define SDA_STEER_CMD_GETTER
#define SDA_STEER_CMD_ORDER
#endif

#ifdef SDA_ACCEL_CMD_DATA
#define SDA_ACCEL_CMD_UNPACK m_accelCmd = std::stof(resultVec[i++]);
#define SDA_ACCEL_CMD_GETTER SDADATA_GETTER(float,m_accelCmd,AccelCmd)
#define SDA_ACCEL_CMD_ORDER p_order.push_back("AccelCmd"); 
#else
#define SDA_ACCEL_CMD_UNPACK
#define SDA_ACCEL_CMD_GETTER
#define SDA_ACCEL_CMD_ORDER
#endif

#ifdef SDA_BRAKE_CMD_DATA
#define SDA_BRAKE_CMD_UNPACK m_brakeCmd = std::stof(resultVec[i++]);
#define SDA_BRAKE_CMD_GETTER SDADATA_GETTER(float,m_brakeCmd,BrakeCmd)
#define SDA_BRAKE_CMD_ORDER p_order.push_back("BrakeCmd"); 
#else
#define SDA_BRAKE_CMD_UNPACK
#define SDA_BRAKE_CMD_GETTER
#define SDA_BRAKE_CMD_ORDER
#endif

#ifdef SDA_CLUTCH_CMD_DATA
#define SDA_CLUTCH_CMD_UNPACK m_clutchCmd = std::stof(resultVec[i++]);
#define SDA_CLUTCH_CMD_GETTER SDADATA_GETTER(float,m_clutchCmd,ClutchCmd)
#define SDA_CLUTCH_CMD_ORDER p_order.push_back("ClutchCmd"); 
#else
#define SDA_CLUTCH_CMD_UNPACK
#define SDA_CLUTCH_CMD_GETTER
#define SDA_CLUTCH_CMD_ORDER
#endif

#ifdef SDA_OFFROAD_DATA
#define SDA_OFFROAD_UNPACK m_offroad = resultVec[i++] == "1";
#define SDA_OFFROAD_GETTER SDADATA_GETTER(bool,m_offroad,Offroad)
#define SDA_OFFROAD_ORDER p_order.push_back("Offroad"); 
#else
#define SDA_OFFROAD_UNPACK
#define SDA_OFFROAD_GETTER
#define SDA_OFFROAD_ORDER
#endif

#ifdef SDA_TO_MIDDLE_DATA
#define SDA_TO_MIDDLE_UNPACK m_toMiddle = std::stof(resultVec[i++]);
#define SDA_TO_MIDDLE_GETTER SDADATA_GETTER(float, m_toMiddle,ToMiddle)
#define SDA_TO_MIDDLE_ORDER p_order.push_back("ToMiddle");
#else
#define SDA_TO_MIDDLE_UNPACK
#define SDA_TO_MIDDLE_GETTER
#define SDA_TO_MIDDLE_ORDER
#endif

#ifdef SDA_TO_LEFT_DATA
#define SDA_TO_LEFT_UNPACK m_toLeft = std::stof(resultVec[i++]);
#define SDA_TO_LEFT_GETTER SDADATA_GETTER(float, m_toLeft,ToLeft)
#define SDA_TO_LEFT_ORDER p_order.push_back("ToLeft");
#else
#define SDA_TO_LEFT_UNPACK
#define SDA_TO_LEFT_GETTER
#define SDA_TO_LEFT_ORDER
#endif

#ifdef SDA_TO_RIGHT_DATA
#define SDA_TO_RIGHT_UNPACK m_toRight = std::stof(resultVec[i++]);
#define SDA_TO_RIGHT_GETTER SDADATA_GETTER(float, m_toRight,ToRight)
#define SDA_TO_RIGHT_ORDER p_order.push_back("ToRight");
#else
#define SDA_TO_RIGHT_UNPACK
#define SDA_TO_RIGHT_GETTER
#define SDA_TO_RIGHT_ORDER
#endif

#ifdef SDA_TO_START_DATA
#define SDA_TO_START_UNPACK m_toStart = std::stof(resultVec[i++]);
#define SDA_TO_START_GETTER SDADATA_GETTER(float, m_toStart,ToStart)
#define SDA_TO_START_ORDER p_order.push_back("ToStart");
#else
#define SDA_TO_START_UNPACK
#define SDA_TO_START_GETTER
#define SDA_TO_START_ORDER
#endif

#ifdef SDA_TIME_OF_DAY_DATA
#define SDA_TIME_OF_DAY_UNPACK m_timeOfDay = std::stof(resultVec[i++]);
#define SDA_TIME_OF_DAY_GETTER SDADATA_GETTER(float,m_timeOfDay,TimeOfDay)
#define SDA_TIME_OF_DAY_ORDER p_order.push_back("TimeOfDay");
#else
#define SDA_TIME_OF_DAY_UNPACK
#define SDA_TIME_OF_DAY_GETTER
#define SDA_TIME_OF_DAY_ORDER
#endif

#ifdef SDA_CLOUDS_DATA
#define SDA_CLOUDS_UNPACK m_clouds = std::stof(resultVec[i++]);
#define SDA_CLOUDS_GETTER SDADATA_GETTER(float,m_clouds,Clouds)
#define SDA_CLOUDS_ORDER p_order.push_back("Clouds");
#else
#define SDA_CLOUDS_UNPACK 
#define SDA_CLOUDS_GETTER
#define SDA_CLOUDS_ORDER
#endif

#ifdef SDA_RAIN_DATA
#define SDA_RAIN_UNPACK m_rain = std::stoi(resultVec[i++]);
#define SDA_RAIN_GETTER SDADATA_GETTER(int,m_rain,Rain)
#define SDA_RAIN_ORDER p_order.push_back("Rain");
#else
#define SDA_RAIN_UNPACK 
#define SDA_RAIN_GETTER
#define SDA_RAIN_ORDER
#endif

/// @brief				 Retrieves the msgpack vector
/// @param  p_buffer	 The character buffer
///	@param  p_bufferSize The buffer size
/// @param  p_vector	 The result vector
inline void GetMsgVector(const char* p_buffer, int p_bufferSize, std::vector<std::string>& p_vector)
{
	msgpack::unpacked msg;
	msgpack::unpack(msg, p_buffer, p_bufferSize);
	msg->convert(p_vector);
}

/// @brief The data class that contains the data from the simulation. You can ONLY have 1 version of this
///	class in your program
struct SDALIB_EXPORT SDAData
{
public:
	/// @brief				 Deserializes the buffer and converts it into data 
	/// @param  p_buffer     The buffer
	/// @param  p_bufferSize The size of the buffer
	SDAData(const char* p_buffer, int p_bufferSize)
	{
		std::vector<std::string> resultVec;
		GetMsgVector(p_buffer, p_bufferSize, resultVec);

		int i = 0;
		// only unpacks the needed variables
        SDA_TICKCOUNT_UNPACK
		SDA_SPEED_UNPACK
		SDA_TOP_SPEED_UNPACK
		SDA_GEAR_UNPACK
		SDA_HEADLIGHTS_UNPACK
		SDA_STEER_CMD_UNPACK
		SDA_ACCEL_CMD_UNPACK
		SDA_BRAKE_CMD_UNPACK
		SDA_CLUTCH_CMD_UNPACK
		SDA_OFFROAD_UNPACK
		SDA_TO_MIDDLE_UNPACK
		SDA_TO_RIGHT_UNPACK
		SDA_TO_LEFT_UNPACK
		SDA_TO_START_UNPACK
		SDA_TIME_OF_DAY_UNPACK
		SDA_CLOUDS_UNPACK
		SDA_RAIN_UNPACK
	}

	// Getters of the needed variables
    SDA_TICKCOUNT_GETTER
	SDA_SPEED_GETTER
	SDA_TOP_SPEED_GETTER
	SDA_GEAR_GETTER
	SDA_HEADLIGHTS_GETTER
	SDA_STEER_CMD_GETTER
	SDA_ACCEL_CMD_GETTER
	SDA_BRAKE_CMD_GETTER
	SDA_CLUTCH_CMD_GETTER
	SDA_OFFROAD_GETTER
	SDA_TO_MIDDLE_GETTER
	SDA_TO_LEFT_GETTER
	SDA_TO_RIGHT_GETTER
	SDA_TO_START_GETTER
	SDA_TIME_OF_DAY_GETTER
	SDA_CLOUDS_GETTER
	SDA_RAIN_GETTER

	/// @brief			Gets the order of the data and stores it into the vector
	/// @param  p_order The order vector
	static void GetOrder(std::vector<std::string>& p_order)
	{
        SDA_TICKCOUNT_ORDER
		SDA_SPEED_ORDER
		SDA_TOP_SPEED_ORDER
		SDA_GEAR_ORDER
		SDA_HEADLIGHTS_ORDER
		SDA_STEER_CMD_ORDER
		SDA_ACCEL_CMD_ORDER
		SDA_BRAKE_CMD_ORDER
		SDA_CLUTCH_CMD_ORDER
		SDA_OFFROAD_ORDER
		SDA_TO_MIDDLE_ORDER
		SDA_TO_LEFT_ORDER
		SDA_TO_RIGHT_ORDER
		SDA_TO_START_ORDER
		SDA_TIME_OF_DAY_ORDER
		SDA_CLOUDS_ORDER
		SDA_RAIN_ORDER
	}
};

