#pragma once
#include "ClientSocket.h"
#include "SDAData.h"
#include "SDAAction.h"
#include "msgpack.hpp"
#include "sdalib_export.h"

#define SDA_BUFFER_SIZE 512

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
	SDADriver()
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

	void Update()
	{
		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // can change to GetData

		SDAData tickData(m_buffer, SDA_BUFFER_SIZE);

		const SDAAction action = UpdateAI(tickData);

		action.Serialize(m_buffer, SDA_BUFFER_SIZE);

		m_client.SendData(m_buffer, SDA_BUFFER_SIZE);
	}

	void Loop()
	{
		while (true)
		{
			Update();
		}
	}

	void SetupSocket()
	{
		m_client.SendData("AI ACTIVE", 10);

		std::vector<std::string> order;
		order.push_back("DATAORDER");
		SDAData::GetOrder(order);
		order.push_back("ACTIONORDER");
		SDAAction::GetOrder(order);

		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE); // receive reply

		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer, order);
		strcpy_s(m_buffer, SDA_BUFFER_SIZE, sbuffer.data());

		m_client.SendData(m_buffer, SDA_BUFFER_SIZE);
		m_client.AwaitData(m_buffer, SDA_BUFFER_SIZE);

		msgpack::unpacked msg;
		msgpack::unpack(msg, m_buffer, SDA_BUFFER_SIZE);
		std::vector<std::string> resultVec;
		msg->convert(resultVec);

		const int amountOfTests = std::stoi(resultVec[0]); //serialize test amount from server

		for (int i = 0; i < amountOfTests; i++)
		{
			Update(); // perform tests
		}
	}

	ClientSocket m_client;
	char m_buffer[SDA_BUFFER_SIZE];
};