#pragma once
#include "ClientSocket.h"
#include "SDAData.h"
#include "SDAAction.h"

#define SDA_BUFFER_SIZE 512

/// @brief The driver class from which the AI should inherit
class SDADriver
{
public:
	void Run();
protected:
	SDADriver();

	virtual void InitAI() = 0;
	virtual SDAAction UpdateAI(SDAData& p_data) = 0;

	SDAData Simulate(SDAAction p_action);

private:

	void Update();

	void Loop();

	void SetupSocket();

	ClientSocket m_client;
	char m_buffer[SDA_BUFFER_SIZE];
};