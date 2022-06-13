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
#include "SDAUtils.hpp"

//#define SDA_LOG_INFO(p_info) std::cout << p_info << std::endl;

#ifndef SDA_LOG_INFO(p_info)

#define SDA_LOG_INFO(p_info)

#endif

#define SDA_BUFFER_SIZE 512

/// @brief The driver class from which the AI should inherit
template <class PointerManager>
class SDALIB_EXPORT AIInterface
{
public:
    void Run();

#ifdef TEST
    SDAData* GetPointer()
    {
        return m_pointerManager.GetDataPointer();
    }
#endif

protected:
    explicit AIInterface(IPC_IP_TYPE p_ip = LOCALHOST, int p_port = 8888);

    virtual void InitAI() = 0;
    virtual SDAAction UpdateAI(SDAData& p_data) = 0;

private:
    bool Update();

    void Loop();

    void SetupSocket();

    ClientSocket m_client;
    char m_buffer[SDA_BUFFER_SIZE];
    PointerManager m_pointerManager;
};

#define SDADriver AIInterface<IPCPointerManager>