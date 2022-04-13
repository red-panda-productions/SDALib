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
void CheckOrder(std::vector<std::string> p_order)
{
	ASSERT_EQ(p_order.size(), 5);
	ASSERT_EQ(p_order[0], "ACTIONORDER");
	ASSERT_EQ(p_order[1], "Steer");
	ASSERT_EQ(p_order[2], "Brake");
	ASSERT_EQ(p_order[3], "Gear");
	ASSERT_EQ(p_order[4], "Accel");
}

/// @brief			Tests if the AI is able to send the 'AI active message'
/// @param  server  The server that receives the message
/// @param  buffer  The buffer to store a message
void TestActive(ServerSocket& server, char* buffer)
{
	// wait untill client sends AI ACTIVE
	ASSERT_DURATION_LE(1,server.AwaitData(buffer, TEST_BUFFER_SIZE));
	TestMessageEqual(buffer, "AI ACTIVE", 9);

	// accept the connection
	server.ReceiveDataAsync();
	ASSERT_EQ(server.SendData("OK", 2), IPCLIB_SUCCEED);
}

/// @brief			Tests if the AI can send its order to the server
/// @param  server  The server that receives the message
/// @param  buffer  The buffer to store a message
void TestOrder(ServerSocket& server, char* buffer)
{
	// wait for order and test
	server.AwaitData(buffer, TEST_BUFFER_SIZE);
	std::vector<std::string> order;
	GetMsgVector(buffer, TEST_BUFFER_SIZE, order);
    CheckOrder(order);
}

/// @brief			Tests if the AI is able to receive the test amount message
/// @param  server  The server that sends the message
/// @param  buffer  The buffer to send/receive a message
void TestTestAmount(ServerSocket& server, char* buffer)
{
	// send amount of tests
	char* tests[]{ "1" };
	msgpack::sbuffer sbuffer(TEST_BUFFER_SIZE);
	msgpack::pack(sbuffer, tests);
	sbufferCopy(sbuffer, buffer, TEST_BUFFER_SIZE);
	server.ReceiveDataAsync();
	ASSERT_EQ(server.SendData(buffer, sbuffer.size()), IPCLIB_SUCCEED);

	ASSERT_DURATION_LE(1,server.AwaitData(buffer, TEST_BUFFER_SIZE));
	ASSERT_TRUE(buffer[0] == 'O' && buffer[1] == 'K');
}

/// @brief			Tests if the first test can be sent, and if the expected result is correct
/// @param  server  The server that receives the message
/// @param  buffer  The buffer to store a message
void TestTest1(ServerSocket& server, char* buffer)
{
	server.ReceiveDataAsync();
	ASSERT_EQ(server.SendData("1", 1), IPCLIB_SUCCEED);

	// return result
	ASSERT_DURATION_LE(1,server.AwaitData(buffer, TEST_BUFFER_SIZE));
	std::vector<std::string> actionData;
	GetMsgVector(buffer, TEST_BUFFER_SIZE, actionData);
	ASSERT_ALMOST_EQ(0.75, stof(actionData[0]));
	ASSERT_ALMOST_EQ(0.5, stof(actionData[1]));
	ASSERT_ALMOST_EQ(2, stof(actionData[2]));
	ASSERT_EQ(1, stoi(actionData[3]));
}

/// @brief			Tests if the connection can be stopped, so that the AI can shutdown
/// @param  server  The server that sends the message
/// @param  buffer  The buffer to store/receive a message
void TestStopConnection(ServerSocket& server, char* buffer)
{
	// stop the connection
	server.ReceiveDataAsync();
	ASSERT_EQ(server.SendData("STOP", 4), IPCLIB_SUCCEED);
	ASSERT_DURATION_LE(1,server.AwaitData(buffer, TEST_BUFFER_SIZE));
	ASSERT_TRUE(buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '\0');
	std::this_thread::sleep_for(std::chrono::milliseconds(10)); // wait to disconnect client
	server.CloseServer();
}

void ServerSide(ServerSocket& server)
{
	ASSERT_DURATION_LE(1, while (!server.Connected()) {});
	char buffer[TEST_BUFFER_SIZE];

	TestActive(server, buffer);

	TestOrder(server, buffer);

	TestTestAmount(server, buffer);

	TestTest1(server, buffer);

	TestStopConnection(server, buffer);
}

/// @brief Tests a full session with a server
TEST(DriverTests, DriverTest)
{
	// connect server and black box
	ServerSocket server;
	ASSERT_EQ(server.Initialize(), IPCLIB_SUCCEED);
	server.ConnectAsync();
	std::thread t = std::thread(DriverSide);
	t.detach();

	ServerSide(server);
}

TEST(DriverTests, WaitForConnectionTest)
{
	std::thread t = std::thread(DriverSide);
	t.detach();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	ServerSocket server;
	ASSERT_EQ(server.Initialize(), IPCLIB_SUCCEED);
	server.ConnectAsync();

	ServerSide(server);
}

TEST(DriverTests, DoubleWaitForConnectionTest)
{
	std::thread t = std::thread(DriverSide);
	t.detach();

	std::this_thread::sleep_for(std::chrono::seconds(5));

	ServerSocket server;
	ASSERT_EQ(server.Initialize(), IPCLIB_SUCCEED);
	server.ConnectAsync();

	ServerSide(server);
}