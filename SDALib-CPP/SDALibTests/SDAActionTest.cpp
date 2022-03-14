
#include "Mocks/SDAActionMock.h"

#include <gtest/gtest.h>
#include <vector>
#include "Utils.h"

/// @brief Tests if Serialization works
TEST(FullActionTests,FullActionTest)
{
	SDAAction action;
	action.Brake = 5.0f;
	action.Steer = 2.0f;
	char buffer[512] = {'\0'};
	int size = 0;
	action.Serialize(buffer, 512,size);

	msgpack::unpacked msg;
	msgpack::unpack(msg, buffer, 512);
	std::vector<std::string> rvec;
	msg->convert(rvec);

	ASSERT_ALMOST_EQ(std::stof(rvec[0]), action.Steer) << "hi";
	ASSERT_ALMOST_EQ(std::stof(rvec[1]), action.Brake) << "hello";
}