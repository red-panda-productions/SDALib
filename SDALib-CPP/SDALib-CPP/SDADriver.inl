#pragma once
#include "SDADriver.hpp"
#include "SDA_portability.h"


#define CREATE_AI_INTERFACE_IMPLEMENTATION(p_type) \
    template AIInterface<p_type>::AIInterface(IPC_IP_TYPE p_ip, int p_port);

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
