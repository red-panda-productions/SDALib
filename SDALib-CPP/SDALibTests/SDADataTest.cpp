#include "Mocks/SDADataMock.h"
#include "Mocks/SDAActionMock.h"
#include <gtest/gtest.h>
#include "msgpack.hpp"
#include <string>
#include "Utils.h"

TEST(FullDataTests,FullDataTest)
{
	float speed = 2.0f;
	float topspeed = 8.0f;
	int gear = 6;
	bool headlights = true;
	float steercmd = 0.3f;
	float accelcmd = 0.5f;
	float brakecmd = 0.4f;
	float clutchcmd = 0.6f;
	bool offroad = true;
	float tomiddle = 2.3f;
	float toright = 2.5f;
	float toleft = 2.9f;
	float tostart = 2.2f;
	float timeofday = 20;
	float clouds = 49;
	float rain = 69;

	std::string data[] = {
		std::to_string(speed),
		std::to_string(topspeed),
		std::to_string(gear),
		std::to_string(headlights),
		std::to_string(steercmd),
		std::to_string(accelcmd),
		std::to_string(brakecmd),
		std::to_string(clutchcmd),
		std::to_string(offroad),
		std::to_string(tomiddle),
		std::to_string(toright),
		std::to_string(toleft),
		std::to_string(tostart),
		std::to_string(timeofday),
		std::to_string(clouds),
		std::to_string(rain)
	};
	msgpack::sbuffer sbuffer;
	msgpack::pack(sbuffer, data);

	char buffer[512];
	sbufferCopy(sbuffer, buffer, 512);

	SDAData r_data(buffer, 512);
	ASSERT_ALMOST_EQ(speed, r_data.Speed());
	ASSERT_ALMOST_EQ(topspeed, r_data.TopSpeed());
	ASSERT_ALMOST_EQ(gear, r_data.Gear());
	ASSERT_ALMOST_EQ(headlights, r_data.Headlights());
	ASSERT_ALMOST_EQ(steercmd, r_data.SteerCmd());
	ASSERT_ALMOST_EQ(accelcmd, r_data.AccelCmd());
	ASSERT_ALMOST_EQ(brakecmd, r_data.BrakeCmd());
	ASSERT_ALMOST_EQ(clutchcmd, r_data.ClutchCmd());
	ASSERT_ALMOST_EQ(offroad, r_data.Offroad());
	ASSERT_ALMOST_EQ(tomiddle, r_data.ToMiddle());
	ASSERT_ALMOST_EQ(toright, r_data.ToRight());
	ASSERT_ALMOST_EQ(toleft, r_data.ToLeft());
	ASSERT_ALMOST_EQ(tostart, r_data.ToStart());
	ASSERT_ALMOST_EQ(timeofday, r_data.TimeOfDay());
	ASSERT_ALMOST_EQ(clouds, r_data.Clouds());
	ASSERT_ALMOST_EQ(rain, r_data.Rain());
}


