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
//	//将数据回显到标准输出 
//	//调用标准输出通道对象的sendout函数
//	//m_out->data_sendout(_input);
//
//	//将数据交给process_func类处理
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


