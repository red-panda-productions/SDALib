#include "SDAAction.h"
#include "msgpack.hpp"

/// @brief					Constructs the action
/// @param  p_steer			Steer amount
/// @param  p_acceleration	Acceleration amount
/// @param  p_brake			Brake amount
/// @param  p_clutch		Clutch amount
SDAAction::SDAAction(float p_steer, float p_acceleration, float p_brake, float p_clutch)
{
	Steer = p_steer;
	Acceleration = p_acceleration;
	Brake = p_brake;
	Clutch = p_clutch;
}

/// @brief				Serializes the action
/// @param  p_buffer	The msg buffer
void SDAAction::Serialize(char* p_buffer) const
{
	float data[4]{ Steer,Acceleration,Brake,Clutch };
	msgpack::pack(p_buffer, data);
}

/// @brief			Sets the order of the action data
/// @param  p_order The order vector
void SDAAction::GetOrder(std::vector<std::string>& p_order)
{
	p_order.push_back("Steer");
	p_order.push_back("Acceleration");
	p_order.push_back("Brake");
	p_order.push_back("Clutch");
}


