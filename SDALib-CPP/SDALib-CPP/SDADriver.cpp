#include "SDADriver.hpp"
// .cpp needed to create .lib file

/// @brief		   Default constructor of SDA driver
/// @param  p_ip   The server ip
/// @param  p_port The server port
SDADriver::SDADriver(PCWSTR p_ip, int p_port) : m_client(p_ip, p_port)
{
	for (int i = 0; i < SDA_BUFFER_SIZE; i++)
	{
		m_buffer[i] = '\0';
	}
}