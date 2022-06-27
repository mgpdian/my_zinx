
#include "AZinxHandler.h"

AZinxHandler::AZinxHandler()
{
}

AZinxHandler::~AZinxHandler()
{
}

void AZinxHandler::Handle(ZinxMessage* _inputMsg)
{
	//1 ��ǰ���ڴ���
	ZinxMessage*  pNextMsg = Internel_Handle(_inputMsg);

	if (nullptr != pNextMsg) //���һ�����ڲ��������
	{
		//2 ��ȡ��һ������ �������ݲ�ͬ ����ò�ͬ����һ�����ڵĴ������
		AZinxHandler* pNextHandler = GetNextHandler(pNextMsg);
		if (nullptr != pNextHandler)
		{
			//3 ��һ�����ڴ���
			pNextHandler->Handle(pNextMsg);
		}
		delete pNextMsg;
	}

	return;
}
