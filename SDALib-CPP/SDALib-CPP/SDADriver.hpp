#pragma once
#include "ClientSocket.h"
#include "SDAData.hpp"
#include "SDAAction.hpp"
#include "msgpack.hpp"
#include "sdalib_export.h"

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
class SDALIB_EXPORT SDADriver
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
	SDADriver(PCWSTR p_ip = L"127.0.0.1", int p_port = 8888);

	virtual void InitAI() = 0;
	virtual SDAAction UpdateAI(SDAData& p_data) = 0;

	/// @brief			 Simulates an action in the simulation
	/// @param  p_action The action that needs to be simulated
	/// @return			 The new state of the game
	void Simulate(const SDAAction& p_action, SDAData& p_data);

private:
	/// @brief  Updates the ai when data is received
	/// @return Whether the simulation is still running
	bool Update()
	{
		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // can change to GetData

		if (m_buffer[0] == 'S' && m_buffer[1] == 'T' && m_buffer[2] == 'O' && m_buffer[3] == 'P') return false;

		//todo: create car and situation from msgpack
		std::vector<std::string> resultVec;
		GetMsgVector(m_buffer, SDA_BUFFER_SIZE, resultVec);

		std::string pointerName = resultVec[0];
		int pointerValue = std::stoi(pointerName);
		SDAData* data = (SDAData*)pointerValue;

		const SDAAction action = UpdateAI(*data);

		int serializeSize;
		action.Serialize(m_buffer, SDA_BUFFER_SIZE,serializeSize);

		m_client.SendData(m_buffer, serializeSize);
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
		m_client.SendData("OK",2);
		m_client.Disconnect();
	}

	/// @brief Sets up the socket connection and transfers order data
	void SetupSocket()
	{
		m_client.Initialize();
		m_client.SendData("AI ACTIVE", 10);

		std::vector<std::string> order;
		order.push_back("ACTIONORDER");
		SDAAction::GetOrder(order);
		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer, order);

		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // receive reply
		if (m_buffer[0] != 'O' || m_buffer[1] != 'K') throw std::exception("Server send wrong reply");

		sbufferCopy(sbuffer, m_buffer, SDA_BUFFER_SIZE);

		m_client.SendData(m_buffer, sbuffer.size());
		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);

		std::vector<std::string> resultVec;
		GetMsgVector(m_buffer, SDA_BUFFER_SIZE, resultVec);

		const int amountOfTests = std::stoi(resultVec[0]); //serialize test amount from server

		for (int i = 0; i < amountOfTests; i++)
		{
			Update(); // perform tests
		}
	}

	ClientSocket m_client;
	char m_buffer[SDA_BUFFER_SIZE];
};