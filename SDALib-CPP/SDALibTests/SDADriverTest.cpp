#include "Mocks/SDADriverMock.h"
#include <gtest/gtest.h>
#include "ServerSocket.h"
#include <thread>
#include "Utils.h"
#include "msgpack.hpp"

#define TEST_BUFFER_SIZE 8192

void DriverSide()
{
	SDADriverMock driver;
	driver.Run();
}

void TestOrder(std::vector<std::string> p_order)
{
	
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