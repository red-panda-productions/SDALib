#pragma once

/// @brief Struct for the actions the AI can do
struct SDAAction
{
public:
	SDAAction(float p_steer = 0, float p_acceleration = 0, float p_brake = 0, float p_clutch = 0);

	float Steer;
	float Acceleration;
	float Brake;
	float Clutch;
};