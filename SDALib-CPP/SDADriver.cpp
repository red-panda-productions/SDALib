#include "SDADriver.h"
#include "msgpack.hpp"
/// @brief the entry point of the class
void SDADriver::Run()
{
	InitAI();
	SetupSocket();
	Loop();
}

/// @brief The constructor of SDADriver
SDADriver::SDADriver()
{
	for(int i = 0; i < SDA_BUFFER_SIZE; i++)
	{
		m_buffer[i] = '\0';
	}
}

/// @brief			 Simulates what would happen if given current action is executed
/// @param  p_action The action that will be executed
/// @return			 The new simulation data
SDAData SDADriver::Simulate(SDAAction p_action)
{
	char request[20] = {'\0'}; //msgpack create request
	m_client.SendData(request, strlen(request));

	m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);
	return {m_buffer};
}

/// @brief The main loop of the SDADriver
void SDADriver::Loop()
{
	while (true)
	{
		Update();
	}
}

/// @brief Updates the AI for the given data
void SDADriver::Update()
{
	m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // can change to GetData
	SDAData tickData(m_buffer);
	SDAAction action = UpdateAI(tickData);
	//serialize action back into m_buffer?
	m_client.SendData(m_buffer, SDA_BUFFER_SIZE);
}

/// @brief Sets up a connection between the AI and speed dreams
void SDADriver::SetupSocket()
{
	m_client.SendData("AI ACTIVE", 10);
	m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);

	msgpack::unpacked msg;
	msgpack::unpack(msg, m_buffer, SDA_BUFFER_SIZE);
	std::vector<std::string> resultVec;
	msg->convert(resultVec);
	const int amountOfTests = 2; //serialize test amount from server

	for(int i = 0; i < amountOfTests; i++)
	{
		Update(); // perform tests
	}
}
