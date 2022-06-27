#pragma once
#include <string>
#include "IChannel.h"
#include "AZinxHandler.h"
class ProcessFunc : public AZinxHandler
{
public:

	IChannel* m_out = nullptr;
	ProcessFunc();
	virtual ~ProcessFunc();



	// Í¨¹ý AZinxHandler ¼Ì³Ð
	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override;
	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override;
};

