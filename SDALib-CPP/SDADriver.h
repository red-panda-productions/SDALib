#pragma once
#include "ClientSocket.h"
class SDADriver
{
public:
	void Init();
protected:
	SDADriver();

	virtual void InitAI() = 0;
	virtual void UpdateAI() = 0;



private:

	void SetupSocket();

	ClientSocket m_client;
};