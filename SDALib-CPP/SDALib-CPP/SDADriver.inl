#pragma once
#include "SDADriver.hpp"
#include "sim.h"

#define CREATE_SDDRIVER_IMPLEMENTATION(p_type) \
	template SDDriver<p_type>::SDDriver(PCWSTR p_ip, int p_port); \
	template void SDDriver<p_type>::Simulate(const SDAAction& p_action, SDAData& p_data); 

/// @brief		   Default constructor of SDA driver
/// @param  p_ip   The server ip
/// @param  p_port The server port
template <class PointerManager>
SDDriver<PointerManager>::SDDriver(PCWSTR p_ip, int p_port) : m_client(p_ip, p_port)
{
	for (int i = 0; i < SDA_BUFFER_SIZE; i++)
	{
		m_buffer[i] = '\0';
	}
}

template <class PointerManager>
void SDDriver<PointerManager>::Simulate(const SDAAction& p_action, SDAData& p_data)
{
	// simulate using simu
}
