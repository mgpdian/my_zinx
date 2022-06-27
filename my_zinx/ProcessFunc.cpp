#include "ProcessFunc.h"
#include <algorithm>
#include "ZinxKernel.h"
ProcessFunc::ProcessFunc()
{
}

ProcessFunc::~ProcessFunc()
{
}


ZinxMessage* ProcessFunc::Internel_Handle(ZinxMessage* _inputMsg)
{
	ByteMsg* byte = dynamic_cast<ByteMsg*>(_inputMsg);

	if (nullptr != byte)
	{
		std::string NextString = byte->content;
		//参数中第一个小写字母 转大写输出
		if (byte->content[0] <= 'z' && byte->content[0] >= 'a')
		{
			std::transform(NextString.begin(), NextString.end(), NextString.begin(), ::toupper);
			
		}
		//调用发送函数 (IChannel*)GetNextHandler(byte)
		ZinxKernel::zin_sendout(NextString, m_out);
	}
	
	//产生bytemsg类型的消息交给输出通道
	return nullptr;
}

AZinxHandler* ProcessFunc::GetNextHandler(ZinxMessage* _next_input)
{

	return nullptr;
}
