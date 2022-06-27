#include "IChannel.h"
#include "ZinxKernel.h"
IChannel::IChannel()
{
}

IChannel::~IChannel()
{
}

//������� ��������¼��������
void IChannel::data_sendout(std::string _output)
{
	m_buffer.append(_output.begin(), _output.end());

	//�޸�epoll�������� ���out����ļ���
	ZinxKernel::GetInstance().ModChannel_AddOut(this);

}

ZinxMessage* IChannel::Internel_Handle(ZinxMessage* _inputMsg)
{
	//�ж�������Ϣ������ɶ
	sysIODicMsg* pmsg = dynamic_cast<sysIODicMsg*>(_inputMsg);
	if (nullptr != pmsg)
	{
		if (sysIODicMsg::IO_IN == pmsg->m_DIC)
		{
			//�������
			auto input_data = this -> ReadFd();
			return new ByteMsg(input_data, *pmsg);
		}
		else {
			//�������
			
			//�������괦������
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
