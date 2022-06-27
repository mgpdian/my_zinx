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

	// ͨ�� IChannel �̳�
	virtual std::string ReadFd() override;
	virtual void WriteFd(std::string _output) override;
	virtual int GetFd() override;
	
	//������������ ����Ҫ��
	//virtual void data_process(std::string _input) override;

	
	

	// ͨ�� IChannel �̳�
	virtual bool Init() override;

	virtual void Fini() override;


	// ͨ�� IChannel �̳�
	virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;

};

