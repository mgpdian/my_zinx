#pragma once
#include "IChannel.h"
class StdOutChannel:public IChannel
{
public:
	StdOutChannel();
	virtual ~StdOutChannel();



	// ͨ�� IChannel �̳�
	virtual std::string ReadFd() override;

	virtual void WriteFd(std::string _output) override;

	virtual int GetFd() override;

	////������������ ����Ҫ���������ݽ��д���ת��
	//virtual void data_process(std::string _input) override;


	// ͨ�� IChannel �̳�
	virtual bool Init() override;

	virtual void Fini() override;


	// ͨ�� IChannel �̳�
	virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;

};

