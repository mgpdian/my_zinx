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
		//�����е�һ��Сд��ĸ ת��д���
		if (byte->content[0] <= 'z' && byte->content[0] >= 'a')
		{
			std::transform(NextString.begin(), NextString.end(), NextString.begin(), ::toupper);
			
		}
		//���÷��ͺ��� (IChannel*)GetNextHandler(byte)
		ZinxKernel::zin_sendout(NextString, m_out);
	}
	
	//����bytemsg���͵���Ϣ�������ͨ��
	return nullptr;
}

AZinxHandler* ProcessFunc::GetNextHandler(ZinxMessage* _next_input)
{

	return nullptr;
}
