#include "IChannel.h"
#include "ZinxKernel.h"
IChannel::IChannel()
{
}

IChannel::~IChannel()
{
}

//输出数据 将参数记录到缓存中
void IChannel::data_sendout(std::string _output)
{
	m_buffer.append(_output.begin(), _output.end());

	//修改epoll监听类型 添加out方向的监听
	ZinxKernel::GetInstance().ModChannel_AddOut(this);

}

ZinxMessage* IChannel::Internel_Handle(ZinxMessage* _inputMsg)
{
	//判断输入消息方向是啥
	sysIODicMsg* pmsg = dynamic_cast<sysIODicMsg*>(_inputMsg);
	if (nullptr != pmsg)
	{
		if (sysIODicMsg::IO_IN == pmsg->m_DIC)
		{
			//输出数据
			auto input_data = this -> ReadFd();
			return new ByteMsg(input_data, *pmsg);
		}
		else {
			//输出数据
			
			//来自用完处理的输出
			auto pout = dynamic_cast<ByteMsg*>(_inputMsg);
			if (nullptr != pout)
			{
				data_sendout(pout ->content);
			}
		}
	}
	return nullptr;
}

AZinxHandler* IChannel::GetNextHandler(ZinxMessage* _next_input)
{
	ByteMsg* pbyte = dynamic_cast<ByteMsg*>(_next_input);
	if (nullptr != pbyte)
	{
		return GetInputNextStage(pbyte);
	}
	return nullptr;
}

void IChannel::FlushOut()
{
	WriteFd(m_buffer);
	m_buffer.clear();
}
