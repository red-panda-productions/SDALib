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
    ASSERT_EQ(p_order[2], "Accel");
    ASSERT_EQ(p_order[3], "Brake");
    ASSERT_EQ(p_order[4], "Gear");
}

/// @brief			  Tests if the AI is able to send the 'AI active message'
/// @param  p_server  The server that receives the message
/// @param  p_buffer  The buffer to store a message
void TestActive(ServerSocket& p_server, char* p_buffer)
{
    // wait untill client sends AI ACTIVE
    ASSERT_DURATION_LE(1, p_server.AwaitData(p_buffer, TEST_BUFFER_SIZE));
    TestMessageEqual(p_buffer, "AI ACTIVE", 9);

    // accept the connection
    p_server.ReceiveDataAsync();
    ASSERT_EQ(p_server.SendData("OK", 2), IPCLIB_SUCCEED);
}

/// @brief			  Tests if the AI can send its order to the server
/// @param  p_server  The server that receives the message
/// @param  p_buffer  The buffer to store a message
void TestOrder(ServerSocket& p_server, char* p_buffer)
{
    // wait for order and test
    p_server.AwaitData(p_buffer, TEST_BUFFER_SIZE);
    std::vector<std::string> order;
    GetMsgVector(p_buffer, TEST_BUFFER_SIZE, order);
    CheckOrder(order);
}

/// @brief			  Tests if the AI is able to receive the test amount message
/// @param  p_server  The server that sends the message
/// @param  p_buffer  The buffer to send/receive a message
void TestTestAmount(ServerSocket& p_server, char* p_buffer)
{
    // send amount of tests
    char* tests[]{"1"};
    msgpack::sbuffer sbuffer(TEST_BUFFER_SIZE);
    msgpack::pack(sbuffer, tests);
    sbufferCopy(sbuffer, p_buffer, TEST_BUFFER_SIZE);
    p_server.ReceiveDataAsync();
    ASSERT_EQ(p_server.SendData(p_buffer, sbuffer.size()), IPCLIB_SUCCEED);

    ASSERT_DURATION_LE(1, p_server.AwaitData(p_buffer, TEST_BUFFER_SIZE));
    ASSERT_TRUE(p_buffer[0] == 'O' && p_buffer[1] == 'K');
}

/// @brief		      Tests if the first test can be sent, and if the expected result is correct
/// @param  p_server  The server that receives the message
/// @param  p_buffer  The buffer to store a message
void TestSituation(ServerSocket& p_server, char* p_buffer)
{
    p_server.ReceiveDataAsync();
    ASSERT_EQ(p_server.SendData("1", 1), IPCLIB_SUCCEED);

    // return result
    ASSERT_DURATION_LE(1, p_server.AwaitData(p_buffer, TEST_BUFFER_SIZE));
    std::vector<std::string> actionData;
    GetMsgVector(p_buffer, TEST_BUFFER_SIZE, actionData);
    ASSERT_ALMOST_EQ(0.75, stof(actionData[0]));
    ASSERT_ALMOST_EQ(0.5, stof(actionData[1]));
    ASSERT_ALMOST_EQ(2, stof(actionData[2]));
    ASSERT_EQ(1, stoi(actionData[3]));
}

/// @brief            Tests if the connection can be stopped, so that the AI can shutdown
/// @param  p_server  The server that sends the message
/// @param  p_buffer  The buffer to store/receive a message
void TestStopConnection(ServerSocket& p_server, char* p_buffer)
{
    // stop the connection
    p_server.ReceiveDataAsync();
    ASSERT_EQ(p_server.SendData("STOP", 4), IPCLIB_SUCCEED);
    ASSERT_DURATION_LE(1, p_server.AwaitData(p_buffer, TEST_BUFFER_SIZE));
    ASSERT_TRUE(p_buffer[0] == 'O' && p_buffer[1] == 'K' && p_buffer[2] == '\0');
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // wait to disconnect client
    p_server.CloseServer();
}

/// @brief				 The server side of the tests without the stop function
/// @param  p_server	 The server socket that has a connection to the driver
/// @param  p_situations The amount of situations the driver has to handle
void ServerSideNoStop(ServerSocket& p_server, int p_situations)
{
    ASSERT_DURATION_LE(3, while (!p_server.Connected()){});
    char buffer[TEST_BUFFER_SIZE];

    TestActive(p_server, buffer);

    TestOrder(p_server, buffer);

    TestTestAmount(p_server, buffer);

    TestSituation(p_server, buffer);  // 1 test situation

    for (int i = 0; i < p_situations; i++)
    {
        TestSituation(p_server, buffer);
    }
}

/// @brief				 The server side of the tests
/// @param  p_server	 The server socket that has a connection to the driver
/// @param  p_situations The amount of situations the driver has to handle
void ServerSide(ServerSocket& p_server, int p_situations)
{
    ServerSideNoStop(p_server, p_situations);

    char buffer[TEST_BUFFER_SIZE];

    TestStopConnection(p_server, buffer);
}

/// @brief				 Tests the behaviour of a driver when it does not have to wait on a connection of the server
/// @param  p_situations The amount of different situations the driver has to handle
void NoWaitTest(int p_situations)
{
    // connect server and black box
    ServerSocket server;
    ASSERT_EQ(server.Initialize(), IPCLIB_SUCCEED);
    server.ConnectAsync();
    std::thread t = std::thread(DriverSide);

    ServerSide(server, p_situations);
    t.join();
}

TEST_CASE(DriverTests, NoWaitSituations, NoWaitTest, (0))
TEST_CASE(DriverTests, NoWaitOneSituation, NoWaitTest, (1))
TEST_CASE(DriverTests, NoWaitMultipleSituations, NoWaitTest, (4))

/// @brief				 Tests the behaviour of a driver if it can't connect to a server
/// @param  p_waitAmount The time the driver has to wait
/// @param  p_situations The amount of different situations the driver has to handle
void WaitTest(int p_waitAmount, int p_situations)
{
    std::thread t = std::thread(DriverSide);

    std::this_thread::sleep_for(std::chrono::seconds(p_waitAmount));

    ServerSocket server;
    ASSERT_EQ(server.Initialize(), IPCLIB_SUCCEED);
    server.ConnectAsync();

    ServerSide(server, p_situations);
    t.join();
}

BEGIN_TEST_COMBINATORIAL(DriverTests, CombinatorialWaitTests)
int waitAmounts[2]{3, 5};
int situationAmounts[3]{0, 1, 4};
END_TEST_COMBINATORIAL2(WaitTest, waitAmounts, 2, situationAmounts, 3)

/// @brief Tests the timeout of the driver.
TEST(DriverTests, TimeoutTests)
{
    SDADriverMock driver;
    ASSERT_DURATION_LE(60, ASSERT_THROW(driver.Run(), std::exception));
}

/// @brief Tests if the program shuts down correctly when the server is shut down abruptly
TEST(DriverTests, BreakingConnectionTest)
{
    ServerSocket server;
    ASSERT_EQ(server.Initialize(), IPCLIB_SUCCEED);
    server.ConnectAsync();
    std::thread t = std::thread(DriverSide);

    ServerSideNoStop(server, 0);
    server.~ServerSocket();
    t.join();
}