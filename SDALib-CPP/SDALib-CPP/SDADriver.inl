/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once
#include "SDADriver.hpp"
#include "SDA_portability.h"

#define CREATE_AI_INTERFACE_IMPLEMENTATION(p_type)                           \
    template void AIInterface<p_type>::Run();                                \
    template AIInterface<p_type>::AIInterface(IPC_IP_TYPE p_ip, int p_port); \
    template bool AIInterface<p_type>::Update();                             \
    template void AIInterface<p_type>::Loop();                               \
    template void AIInterface<p_type>::SetupSocket();

/// @brief				 Retrieves the msgpack vector
/// @param  p_buffer	 The character buffer
///	@param  p_bufferSize The buffer size
/// @param  p_vector	 The result vector
inline void GetMsgVector(const char* p_buffer, int p_bufferSize, std::vector<std::string>& p_vector)
{
    msgpack::unpacked msg;
    msgpack::unpack(msg, p_buffer, p_bufferSize);
    msg->convert(p_vector);
}

/// @brief Runs the driver
template <class PointerManager>
void AIInterface<PointerManager>::Run()
{
    SET_WORKING_DIR();
    InitAI();
    SetupSocket();
    Loop();
}

/// @brief		   Default constructor of SDA driver
/// @param  p_ip   The server ip
/// @param  p_port The server port
template <class PointerManager>
AIInterface<PointerManager>::AIInterface(IPC_IP_TYPE p_ip, int p_port)
    : m_client(p_ip, p_port)
{
    for (int i = 0; i < SDA_BUFFER_SIZE; i++)
    {
        m_buffer[i] = '\0';
    }
}

/// @brief  Updates the ai when data is received
/// @return Whether the simulation is still running
template <class PointerManager>
bool AIInterface<PointerManager>::Update()
{
    SDA_LOG_INFO("Updating...");
    const int err = m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);
    SDA_LOG_INFO("Received data");

    if (err != IPCLIB_SUCCEED)
    {
        std::cout << "IPCLIB_FAIL: " << err << std::endl;
        return false;
    }
    if (m_buffer[0] == 'S' && m_buffer[1] == 'T' && m_buffer[2] == 'O' && m_buffer[3] == 'P') return false;

    m_client.ReceiveDataAsync();
    SDAData* data = m_pointerManager.GetDataPointer();

    data->Car.pub.trkPos.seg = m_pointerManager.GetSegmentPointer();
    data->SimCar.ctrl = &data->Car.ctrl;
    data->SimCar.carElt = &data->Car;
    data->SimCar.trkPos = data->Car.pub.trkPos;

    for (int i = 0; i < 4; i++)
    {
        data->SimCar.wheel[i].trkPos = data->Car.pub.trkPos;
    }

    const SDAAction action = UpdateAI(*data);

    int serializeSize;
    action.Serialize(m_buffer, SDA_BUFFER_SIZE, serializeSize);

    IPC_OK(m_client.SendData(m_buffer, serializeSize), "[SDA] Could not send action");

    SDA_LOG_INFO("Updated")
    return true;
}

/// @brief Loops the ai until the simulation stops
template <class PointerManager>
void AIInterface<PointerManager>::Loop()
{
    bool run = true;
    while (run)
    {
        run = Update();
    }
    std::cout << "STOP" << std::endl;
    IPC_OK(m_client.SendData("OK", 2), "[SDA] Could not send OK");
    m_client.Disconnect();
}

/// @brief Sets up the socket connection and transfers order data
template <class PointerManager>
void AIInterface<PointerManager>::SetupSocket()
{
    std::cerr << "Trying to connect to DAISI" << std::endl;
    int tries = 0;
    while (m_client.Initialize() != IPCLIB_SUCCEED && tries++ < 10)
    {
        std::cerr << "Failed Retrying in 2 seconds, tries: " << tries << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    if (tries >= 10)
    {
        SDA_THROW_EXCEPTION("Could not connect to speed dreams");
    }

    m_client.ReceiveDataAsync();
    IPC_OK(m_client.SendData("AI ACTIVE", 10), "[SDA] Could not send AI ACTIVE")

    std::vector<std::string> order;
    order.push_back("ACTIONORDER");
    SDAAction::GetOrder(order);
    msgpack::sbuffer sbuffer;
    msgpack::pack(sbuffer, order);

    IPC_OK(m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE), "[SDA] Failed to receive message from server");  // receive reply
    if (m_buffer[0] != 'O' || m_buffer[1] != 'K')
    {
        SDA_THROW_EXCEPTION("Server send wrong reply");
    }

    sbufferCopy(sbuffer, m_buffer, SDA_BUFFER_SIZE);

    m_client.ReceiveDataAsync();
    IPC_OK(m_client.SendData(m_buffer, sbuffer.size()), "[SDA] Could not send order data");

    IPC_OK(m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE), "[SDA] Failed to receive message from server");

    std::vector<std::string> resultVec;
    GetMsgVector(m_buffer, SDA_BUFFER_SIZE, resultVec);

    const int amountOfTests = std::stoi(resultVec[0]);  // serialize test amount from server

    m_client.ReceiveDataAsync();
    IPC_OK(m_client.SendData("OK", 2), "[SDA] Could not send OK");

    for (int i = 0; i < amountOfTests; i++)
    {
        Update();  // perform tests
    }
}
