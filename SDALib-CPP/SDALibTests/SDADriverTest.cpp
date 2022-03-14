#include "Mocks/SDADriverMock.h"
#include <gtest/gtest.h>
#include "ServerSocket.h"
#include <thread>
#include "Utils.h"
#include "msgpack.hpp"

#define TEST_BUFFER_SIZE 8192

/// @brief Runs a mock driver
void DriverSide()
{
	SDADriverMock driver;
	driver.Run();
}

/// @brief			Tests if the order is expected
/// @param  p_order The order
void TestOrder(std::vector<std::string> p_order)
{
	ASSERT_EQ(p_order.size(), 20);
	ASSERT_EQ(p_order[0], "dataorder");
	ASSERT_EQ(p_order[1], "Speed");
	ASSERT_EQ(p_order[2], "TopSpeed");
	ASSERT_EQ(p_order[3], "Gear");
	ASSERT_EQ(p_order[4], "Headlights");
	ASSERT_EQ(p_order[5], "SteerCmd");
	ASSERT_EQ(p_order[6], "AccelCmd");
	ASSERT_EQ(p_order[7], "BrakeCmd");
	ASSERT_EQ(p_order[8], "ClutchCmd");
	ASSERT_EQ(p_order[9], "Offroad");
	ASSERT_EQ(p_order[10], "ToMiddle");
	ASSERT_EQ(p_order[11], "ToLeft");
	ASSERT_EQ(p_order[12], "ToRight");
	ASSERT_EQ(p_order[13], "ToStart");
	ASSERT_EQ(p_order[14], "TimeOfDay");
	ASSERT_EQ(p_order[15], "Clouds");
	ASSERT_EQ(p_order[16], "Rain");
	ASSERT_EQ(p_order[17], "actionorder");
	ASSERT_EQ(p_order[18], "Steer");
	ASSERT_EQ(p_order[19], "Brake");
}

TEST(DriverTests, DriverTest)
{
	ServerSocket server;
	server.ConnectAsync();
	std::thread t = std::thread(DriverSide);
	t.detach();

	ASSERT_DURATION_LE(1, while (!server.Connected()) {});
	char buffer[TEST_BUFFER_SIZE];
	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	TestMessageEqual(buffer, "AI ACTIVE", 9);
	server.SendData("OK",2);

	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	std::vector<std::string> order;

	GetMsgVector(buffer, TEST_BUFFER_SIZE, order); //Crashes for unkown reason
	TestOrder(order);

	char* tests[]{ "1" };
	msgpack::sbuffer sbuffer(TEST_BUFFER_SIZE);
	msgpack::pack(sbuffer,tests);
	sbufferCopy(sbuffer, buffer, TEST_BUFFER_SIZE);

	server.SendData(buffer,sbuffer.size());

	std::string data[] = {
		std::to_string(2.0f),
		std::to_string(8.0f),
		std::to_string(6),
		std::to_string(true),
		std::to_string(0.3f),
		std::to_string(0.5f),
		std::to_string(0.4f),
		std::to_string(0.6f),
		std::to_string(true),
		std::to_string(2.3f),
		std::to_string(2.5f),
		std::to_string(2.9f),
		std::to_string(2.2f),
		std::to_string(20),
		std::to_string(49),
		std::to_string(69)
	};

	sbuffer.clear();
	msgpack::pack(sbuffer, data);
	sbufferCopy(sbuffer, buffer, TEST_BUFFER_SIZE);

	server.SendData(buffer,sbuffer.size());

	server.AwaitData(buffer, TEST_BUFFER_SIZE);

	std::vector<std::string> actionData;
	GetMsgVector(buffer, TEST_BUFFER_SIZE, actionData); // crash?
	ASSERT_ALMOST_EQ(1, stof(actionData[0]));
	ASSERT_ALMOST_EQ(2, stof(actionData[1]));

	server.SendData("STOP",4);
	server.CloseServer();
}