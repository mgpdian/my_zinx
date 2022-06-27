
#include "AZinxHandler.h"

AZinxHandler::AZinxHandler()
{
}

AZinxHandler::~AZinxHandler()
{
}

void AZinxHandler::Handle(ZinxMessage* _inputMsg)
{
	//1 当前环节处理
	ZinxMessage*  pNextMsg = Internel_Handle(_inputMsg);

	if (nullptr != pNextMsg) //最后一个环节不产生输出
	{
		//2 获取下一个环节 根据数据不同 来获得不同的下一个环节的处理对象
		AZinxHandler* pNextHandler = GetNextHandler(pNextMsg);
		if (nullptr != pNextHandler)
		{
			//3 下一个环节处理
			pNextHandler->Handle(pNextMsg);
		}
		delete pNextMsg;
	}

	return;
}
