#include "StdInChannel.h"
#include <iostream>

StdInChannel::StdInChannel()
{
}

StdInChannel::~StdInChannel()
{
}

std::string StdInChannel::ReadFd()
{
	std::string ret;
	std::cin >> ret;

	return ret;
}

void StdInChannel::WriteFd(std::string _output)
{
}

int StdInChannel::GetFd()
{
	return 0;
}

//void StdInChannel::data_process(std::string _input)
//{
//	//�����ݻ��Ե���׼��� 
//	//���ñ�׼���ͨ�������sendout����
//	//m_out->data_sendout(_input);
//
//	//�����ݽ���process_func�ദ��
//	m_proc->DataProcess(_input);
//}

bool StdInChannel::Init()
{
	return true;
}

void StdInChannel::Fini()
{
}

AZinxHandler* StdInChannel::GetInputNextStage(ByteMsg* _byte)
{
	return m_proc;
}


