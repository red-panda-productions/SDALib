#pragma once
#include "ClientSocket.h"
#include "SDAData.hpp"
#include "SDAAction.hpp"
#include "msgpack.hpp"
#include "sdalib_export.h"
#include "SDA_portability.h"
#include <thread>
#include <chrono>
#include "IPCPointerManager.h"

//#define SDA_LOG_INFO(p_info) std::cout << p_info << std::endl;

#ifndef SDA_LOG_INFO(p_info)

#define SDA_LOG_INFO(p_info)

#endif

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

#define SDA_BUFFER_SIZE 512

/// @brief The driver class from which the AI should inherit
template <class PointerManager>
class SDALIB_EXPORT AIInterface
{
public:
    /// @brief Runs the driver
    void Run()
    {
        InitAI();
        SetupSocket();
        Loop();
    }

protected:
    explicit AIInterface(IPC_IP_TYPE p_ip = LOCALHOST, int p_port = 8888);

    virtual void InitAI() = 0;
    virtual SDAAction UpdateAI(SDAData& p_data) = 0;

private:
    /// @brief  Updates the ai when data is received
    /// @return Whether the simulation is still running
    bool Update()
    {
        SDA_LOG_INFO("Updating...");
        const int err = m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);  // can change to GetData
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

        const SDAAction action = UpdateAI(*data);

        int serializeSize;
        action.Serialize(m_buffer, SDA_BUFFER_SIZE, serializeSize);

        IPC_OK(m_client.SendData(m_buffer, serializeSize), "[SDA] Could not send action");

        SDA_LOG_INFO("Updated")
        return true;
    }

    /// @brief Loops the ai until the simulation stops
    void Loop()
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
    void SetupSocket()
    {
        std::cerr << "Trying to connect to Speed Dreams" << std::endl;
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

    ClientSocket m_client;
    char m_buffer[SDA_BUFFER_SIZE];
    PointerManager m_pointerManager;
};

#define SDADriver AIInterface<IPCPointerManager>