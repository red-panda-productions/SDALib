#pragma once
#include "ClientSocket.h"
#include "SDAData.hpp"
#include "SDAAction.hpp"
#include "msgpack.hpp"
#include "sdalib_export.h"

#define SDA_BUFFER_SIZE 8192

/// @brief The driver class from which the AI should inherit
class SDALIB_EXPORT SDADriver
{
public:
	void Run()
	{
		InitAI();
		SetupSocket();
		Loop();
	}
protected:
	SDADriver(PCWSTR p_ip = L"127.0.0.1", int p_port = 8888) : m_client(p_ip, p_port)
	{
		for (int i = 0; i < SDA_BUFFER_SIZE; i++)
		{
			m_buffer[i] = '\0';
		}
	}

	virtual void InitAI() = 0;
	virtual SDAAction UpdateAI(SDAData& p_data) = 0;

	SDAData Simulate(SDAAction p_action)
	{
		char request[20] = { '\0' }; //msgpack create request
		m_client.SendData(request, strlen(request));

		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);
		return { m_buffer,SDA_BUFFER_SIZE };
	}

private:

	bool Update()
	{
		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // can change to GetData

		if (m_buffer[0] == 'S' && m_buffer[1] == 'T' && m_buffer[2] == 'O' && m_buffer[3] == 'P') return false;

		SDAData tickData(m_buffer, SDA_BUFFER_SIZE);

		const SDAAction action = UpdateAI(tickData);

		action.Serialize(m_buffer, SDA_BUFFER_SIZE);

		m_client.SendData(m_buffer, SDA_BUFFER_SIZE);
		return true;
	}

	void Loop()
	{
		bool run = true;
		while (run)
		{
			run = Update();
		}
		m_client.Disconnect();
	}

	void SetupSocket()
	{
		m_client.SendData("AI ACTIVE", 10);

		std::vector<std::string> order;
		order.push_back("dataorder");
		SDAData::GetOrder(order);
		order.push_back("actionorder");
		SDAAction::GetOrder(order);
		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer, order);

		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // receive reply
		if (m_buffer[0] != 'O' && m_buffer[1] != 'K') throw std::exception("Server send wrong reply");

		sbufferCopy(sbuffer, m_buffer, SDA_BUFFER_SIZE);

		m_client.SendData(m_buffer, SDA_BUFFER_SIZE);
		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);

		std::vector<std::string> resultVec;
		GetMsgVector(m_buffer, SDA_BUFFER_SIZE, resultVec); // CRASH

		const int amountOfTests = std::stoi(resultVec[0]); //serialize test amount from server

		for (int i = 0; i < amountOfTests; i++)
		{
			Update(); // perform tests
		}
	}

	ClientSocket m_client;
	char m_buffer[SDA_BUFFER_SIZE];
};