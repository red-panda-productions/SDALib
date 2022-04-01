#include "Mocks/SDADriverMock.h"
#include <gtest/gtest.h>
#include "ServerSocket.h"
#include <thread>
#include "Utils.h"
#include "msgpack.hpp"

#define TEST_BUFFER_SIZE 8192

/// @brief Runs a mock driver as the test needs to run in parallel to this
void DriverSide()
{
	SDADriverMock driver;
	driver.Run();
}

/// @brief			Tests if the order is expected
/// @param  p_order The order
void TestOrder(std::vector<std::string> p_order)
{
	ASSERT_EQ(p_order.size(), 3);
	ASSERT_EQ(p_order[0], "ACTIONORDER");
	ASSERT_EQ(p_order[1], "Steer");
	ASSERT_EQ(p_order[2], "Brake");
}

/// @brief Tests a full session with a server
TEST(DriverTests, DriverTest)
{
	// connect server and black box
	ServerSocket server;
	server.Initialize();
	server.ConnectAsync();
	std::thread t = std::thread(DriverSide);
	t.detach();

	ASSERT_DURATION_LE(1, while (!server.Connected()) {});
	char buffer[TEST_BUFFER_SIZE];

	// wait untill client sends AI ACTIVE
	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	TestMessageEqual(buffer, "AI ACTIVE", 9);

	// accept the connection
	server.SendData("OK", 2);

	// wait for order and test
	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	std::vector<std::string> order;
	GetMsgVector(buffer, TEST_BUFFER_SIZE, order);
	TestOrder(order);

	// send amount of tests
	char* tests[]{ "1" };
	msgpack::sbuffer sbuffer(TEST_BUFFER_SIZE);
	msgpack::pack(sbuffer, tests);
	sbufferCopy(sbuffer, buffer, TEST_BUFFER_SIZE);
	server.SendData(buffer, sbuffer.size());

	// create data
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

	// send data
	sbuffer.clear();
	msgpack::pack(sbuffer, data);
	sbufferCopy(sbuffer, buffer, TEST_BUFFER_SIZE);
	server.SendData(buffer, sbuffer.size());

	// return result
	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	std::vector<std::string> actionData;
	GetMsgVector(buffer, TEST_BUFFER_SIZE, actionData);
	ASSERT_ALMOST_EQ(1, stof(actionData[0]));
	ASSERT_ALMOST_EQ(2, stof(actionData[1]));

	// stop the connection
	server.SendData("STOP", 4);
	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	ASSERT_TRUE(buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '\0');
	std::this_thread::sleep_for(std::chrono::milliseconds(10)); // wait to disconnect client
	server.CloseServer();
}