#pragma once
#include "IChannel.h"
class StdOutChannel:public IChannel
{
public:
	StdOutChannel();
	virtual ~StdOutChannel();



	// 通过 IChannel 继承
	virtual std::string ReadFd() override;

	virtual void WriteFd(std::string _output) override;

	virtual int GetFd() override;

	////引入责任链后 不需要他来对数据进行处理转发
	//virtual void data_process(std::string _input) override;


	// 通过 IChannel 继承
	virtual bool Init() override;

	virtual void Fini() override;


	// 通过 IChannel 继承
	virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;

};

