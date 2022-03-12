#pragma once
#include <vector>
#include <string>

//#define SDA_STEER_ACTION
//#define SDA_BRAKE_ACTION

// these macros pre-compute what is needed for the SDAAction class
#ifdef SDA_STEER_ACTION
#define SDA_STEER_VAR float Steer = 0;
#define SDA_STEER_PACK data.push_back(Steer);
#define SDA_STEER_ORDER p_order.push_back("Steer"); s_itemCount++;
#else
#define SDA_STEER_VAR
#define SDA_STEER_PACK
#define SDA_STEER_ORDER
#endif

#ifdef SDA_BRAKE_ACTION
#define SDA_BRAKE_VAR float Brake = 0;
#define SDA_BRAKE_PACK data.push_back(Brake);
#define SDA_BRAKE_ORDER p_order.push_back("Brake"); s_itemCount++;
#else
#define SDA_BRAKE_VAR
#define SDA_BRAKE_PACK
#define SDA_BRAKE_ORDER
#endif

/// @brief Struct for the actions the AI can do
struct SDAAction
{
public:
	SDA_STEER_VAR
	SDA_BRAKE_VAR

	void Serialize(char* p_buffer, int p_bufferSize) const
	{
		if (s_itemCount == 0) return;
		std::vector<float> data(s_itemCount);
		SDA_STEER_PACK
		SDA_BRAKE_PACK

		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer,data);
		strcpy_s(p_buffer, p_bufferSize,sbuffer.data());
	}

	static void GetOrder(std::vector<std::string>& p_order)
	{
		s_itemCount = 0;
		SDA_STEER_ORDER
		SDA_BRAKE_ORDER
	}
private:
	static int s_itemCount;
};