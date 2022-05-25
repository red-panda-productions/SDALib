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
    void Run();

protected:
    explicit AIInterface(IPC_IP_TYPE p_ip = LOCALHOST, int p_port = 8888);

    virtual void InitAI() = 0;
    virtual SDAAction UpdateAI(SDAData& p_data) = 0;

private:
    /// @brief  Updates the ai when data is received
    /// @return Whether the simulation is still running
    bool Update();

    /// @brief Loops the ai until the simulation stops
    void Loop();

    /// @brief Sets up the socket connection and transfers order data
    void SetupSocket();

    ClientSocket m_client;
    char m_buffer[SDA_BUFFER_SIZE];
    PointerManager m_pointerManager;
};

#define SDADriver AIInterface<IPCPointerManager>