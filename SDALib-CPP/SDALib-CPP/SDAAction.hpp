#pragma once
#include "msgpack.hpp"
#include <vector>
#include <string>
#include "sdalib_export.h"

//#define SDA_STEER_ACTION
//#define SDA_BRAKE_ACTION

// these macros pre-compute what is needed for the SDAAction class
#ifdef SDA_STEER_ACTION
#define SDA_STEER_VAR float Steer = 0;
#define SDA_STEER_PACK data.push_back(std::to_string(Steer));
#define SDA_STEER_ORDER p_order.push_back("Steer");
#else
#define SDA_STEER_VAR
#define SDA_STEER_PACK
#define SDA_STEER_ORDER
#endif

#ifdef SDA_BRAKE_ACTION
#define SDA_BRAKE_VAR float Brake = 0;
#define SDA_BRAKE_PACK data.push_back(std::to_string(Brake));
#define SDA_BRAKE_ORDER p_order.push_back("Brake");
#else
#define SDA_BRAKE_VAR
#define SDA_BRAKE_PACK
#define SDA_BRAKE_ORDER
#endif

/// @brief				   Copies a msgpack buffer
/// @param  sbuffer		   The msgpack buffer
/// @param  p_destination  The destination buffer
inline void sbufferCopy(const msgpack::sbuffer& sbuffer, char* p_destination, int p_destinationSize)
{
	int size = sbuffer.size();
	assert(size < p_destinationSize);
	const char* newData = sbuffer.data();
	for (int i = 0; i < size; i++)
	{
		p_destination[i] = newData[i];
	}
	p_destination[size] = '\0';
}

/// @brief Struct for the actions the AI can do
struct SDALIB_EXPORT SDAAction
{
public:
	SDA_STEER_VAR
	SDA_BRAKE_VAR

	void Serialize(char* p_buffer, int p_bufferSize) const
	{
		std::vector<std::string> data;
		SDA_STEER_PACK
		SDA_BRAKE_PACK

		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer,data);

		sbufferCopy(sbuffer, p_buffer, p_bufferSize);
	}

	static void GetOrder(std::vector<std::string>& p_order)
	{
		SDA_STEER_ORDER
		SDA_BRAKE_ORDER
	}
};
