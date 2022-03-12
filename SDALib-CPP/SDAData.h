#pragma once
#include <string>
#include <vector>
#include <msgpack.hpp>

//#define SDA_SPEED_DATA
//#define SDA_TOP_SPEED_DATA
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

// these directives pre-compute what is needed for the data class
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
#define SDA_CLUTCH_CMD_ORDER p_order.push_back("CLUTCHCmd");
#else
#define SDA_CLUTCH_CMD_UNPACK
#define SDA_CLUTCH_CMD_GETTER
#define SDA_CLUTCH_CMD_ORDER
#endif

#ifdef SDA_TIME_LAST_STEER_DATA
#define SDA_TIME_LAST_STEER_UNPACK m_timeLastSteer = std::stof(resultVec[i++]);
#define SDA_TIME_LAST_STEER_GETTER SDADATA_GETTER(float,m_timeLastSteer,TimeLastSteer)
#define SDA_TIME_LAST_STEER_ORDER p_order.push_back("TIME_LAST_STEERCmd");
#else
#define SDA_TIME_LAST_STEER_UNPACK
#define SDA_TIME_LAST_STEER_GETTER
#define SDA_TIME_LAST_STEER_ORDER
#endif

#ifdef SDA_OFFROAD_DATA
#define SDA_OFFROAD_UNPACK std::istringstream(resultVec[i++]) >> m_offroad;
#define SDA_OFFROAD_GETTER SDADATA_GETTER(bool,m_offroad,Offroad)
#define SDA_OFFROAD_ORDER p_order.push_back("OFFROADCmd");
#else
#define SDA_OFFROAD_UNPACK
#define SDA_OFFROAD_GETTER
#define SDA_OFFROAD_ORDER
#endif

#ifdef SDA_TIME_OF_DAY_DATA
#define SDA_TIME_OF_DAY_UNPACK m_timeOfDay = std::stof(resultVec[i++]);
#define SDA_TIME_OF_DAY_GETTER SDADATA_GETTER(float,m_timeOfDay,TimeOfDay)
#define SDA_TIME_OF_DAY_ORDER p_order.push_back("TIME_OF_DAYCmd");
#else
#define SDA_TIME_OF_DAY_UNPACK
#define SDA_TIME_OF_DAY_GETTER
#define SDA_TIME_OF_DAY_ORDER
#endif

#ifdef SDA_CLOUDS_DATA
#define SDA_CLOUDS_UNPACK m_clouds = std::stof(resultVec[i++]);
#define SDA_CLOUDS_GETTER SDADATA_GETTER(float,m_clouds,Clouds)
#define SDA_CLOUDS_ORDER p_order.push_back("CLOUDSCmd");
#else
#define SDA_CLOUDS_UNPACK
#define SDA_CLOUDS_GETTER
#define SDA_CLOUDS_ORDER
#endif

/// @brief The data class that contains the data from the simulation
struct SDAData
{
public:
	SDAData(const char* data, int dataSize)
	{
		msgpack::unpacked msg;
		msgpack::unpack(msg, data, dataSize);
		std::vector<std::string> resultVec;
		msg->convert(resultVec);
		int i = 0;

		SDA_SPEED_UNPACK
		SDA_TOP_SPEED_UNPACK
		SDA_STEER_CMD_UNPACK
		SDA_ACCEL_CMD_UNPACK
		SDA_BRAKE_CMD_UNPACK
		SDA_CLUTCH_CMD_UNPACK
		SDA_TIME_LAST_STEER_UNPACK
		SDA_OFFROAD_UNPACK
		SDA_TIME_OF_DAY_UNPACK
		SDA_CLOUDS_UNPACK
	}

	SDA_SPEED_GETTER
	SDA_TOP_SPEED_GETTER
	SDA_STEER_CMD_GETTER
	SDA_ACCEL_CMD_GETTER
	SDA_BRAKE_CMD_GETTER
	SDA_CLUTCH_CMD_GETTER
	SDA_TIME_LAST_STEER_GETTER
	SDA_OFFROAD_GETTER
	SDA_TIME_OF_DAY_GETTER
	SDA_CLOUDS_GETTER

	static void GetOrder(std::vector<std::string>& p_order)
	{
		SDA_SPEED_ORDER
		SDA_TOP_SPEED_ORDER
		SDA_STEER_CMD_ORDER
		SDA_ACCEL_CMD_ORDER
		SDA_BRAKE_CMD_ORDER
		SDA_CLUTCH_CMD_ORDER
		SDA_TIME_LAST_STEER_ORDER
		SDA_OFFROAD_ORDER
		SDA_TIME_OF_DAY_ORDER
		SDA_CLOUDS_ORDER
	}
};

