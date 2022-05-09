#pragma once
#include "ClientSocket.h"
#include "SDAData.hpp"
#include "SDAAction.hpp"
#include "msgpack.hpp"
#include "sdalib_export.h"
#include <thread>
#include <chrono>
#include "IPCPointerManager.h"

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

/// @brief		   Checks if the action was reported successfully from IPCLib
/// @param  p_stmt The action that needs to be checked
/// @param  p_msg  The message that is pushed to the standard error output when the action has failed
#define IPC_OK(p_stmt, p_msg)              \
    if ((p_stmt) != IPCLIB_SUCCEED)        \
    {                                      \
        std::cerr << (p_msg) << std::endl; \
        throw std::exception(p_msg);       \
    }

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
    explicit AIInterface(PCWSTR p_ip = L"127.0.0.1", int p_port = 8888);

    virtual void InitAI() = 0;
    virtual SDAAction UpdateAI(SDAData& p_data) = 0;

private:
    /// @brief  Updates the ai when data is received
    /// @return Whether the simulation is still running
    bool Update()
    {
        const int err = m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);  // can change to GetData

        if (err != IPCLIB_SUCCEED) return false;
        if (m_buffer[0] == 'S' && m_buffer[1] == 'T' && m_buffer[2] == 'O' && m_buffer[3] == 'P') return false;

        SDAData* data = m_pointerManager.GetDataPointer();

        data->Car.pub.trkPos.seg = m_pointerManager.GetSegmentPointer();

        const SDAAction action = UpdateAI(*data);

        int serializeSize;
        action.Serialize(m_buffer, SDA_BUFFER_SIZE, serializeSize);

        m_client.ReceiveDataAsync();
        IPC_OK(m_client.SendData(m_buffer, serializeSize), "[SDA] Could not send action");
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
            throw std::exception("Could not connect to speed dreams");

        m_client.ReceiveDataAsync();
        IPC_OK(m_client.SendData("AI ACTIVE", 10), "[SDA] Could not send AI ACTIVE")

        std::vector<std::string> order;
        order.push_back("ACTIONORDER");
        SDAAction::GetOrder(order);
        msgpack::sbuffer sbuffer;
        msgpack::pack(sbuffer, order);

        IPC_OK(m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE), "Failed to receive message from server");  // receive reply
        if (m_buffer[0] != 'O' || m_buffer[1] != 'K') throw std::exception("Server send wrong reply");

        sbufferCopy(sbuffer, m_buffer, SDA_BUFFER_SIZE);

        m_client.ReceiveDataAsync();
        IPC_OK(m_client.SendData(m_buffer, sbuffer.size()), "[SDA] Could not send order data");

        IPC_OK(m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE), "Failed to receive message from server");

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