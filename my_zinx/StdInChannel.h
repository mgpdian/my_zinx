#pragma once
#include <string>
#include "IChannel.h"
#include "StdOutChannel.h"
#include "ProcessFunc.h"
class StdInChannel:public IChannel
{
private:
	
public:
	StdInChannel();
	virtual ~StdInChannel();

	//StdOutChannel* m_out = nullptr;
	ProcessFunc* m_proc = nullptr;

	// 通过 IChannel 继承
	virtual std::string ReadFd() override;
	virtual void WriteFd(std::string _output) override;
	virtual int GetFd() override;
	
	//引入责任链后 不需要了
	//virtual void data_process(std::string _input) override;

	
	

	// 通过 IChannel 继承
	virtual bool Init() override;

	virtual void Fini() override;


	// 通过 IChannel 继承
	virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;

};

