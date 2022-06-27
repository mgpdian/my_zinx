#include <iostream>

#include "ZinxKernel.h"
#include "StdInChannel.h"

#include "FIFO_Channel.h"
#include "ProcessFunc.h"
#include "AZinxHandler.h"
#include <algorithm>
#include "TcpChannel.h"
//
//class testMsg : public ZinxMessage
//{
//public:
//	std::string data;
//};
//
//
//class output_next_letter :public AZinxHandler
//{
//	// 通过 AZinxHandler 继承
//	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override
//	{
//		testMsg* pmsg = dynamic_cast<testMsg*>(_inputMsg);
//		printf("%c\n", pmsg->data[0] + 1);
//		return nullptr;
//	}
//
//	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override
//	{
//		return nullptr;
//	}
//
//}step3;
//
//class output_ascii :public AZinxHandler
//{
//	// 通过 AZinxHandler 继承
//	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override
//	{
//		testMsg* pmsg = dynamic_cast<testMsg*>(_inputMsg);
//		printf("%d\n", pmsg->data[0]);
//
//		//原样交给下一个环节 
//		//那为什么要new一个呢 因为后面在同一个函数中会对他进行析构
//		//如果不new 的话 那上一个和这一个消息都是同一个 重复析构会导致崩溃
//		testMsg* pret = new testMsg();
//		pret->data = pmsg->data;
//
//		return pret;
//	}
//
//	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override
//	{
//		return &step3;
//	}
//
//}step2;
//
//
//
//class trancUpper : public AZinxHandler
//{
//	// 通过 AZinxHandler 继承
//
//	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override
//	{
//		testMsg* pmsg = dynamic_cast<testMsg*>(_inputMsg);
//		std::string output("a");
//		std::transform(pmsg->data.begin(), pmsg->data.end(), output.begin(), ::toupper);
//		
//		std::cout << output << std::endl;
//		
//		//new 新的消息对象返回
//		testMsg* pret = new testMsg();
//		pret->data = output;
//
//		return pret;
//	}
//
//	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override
//	{
//		
//		return &step2;
//	}
//	
//}step1;
//


//void test_chain()
//{
//	std::string tmp;
//	std::cin >> tmp;
//
//	testMsg init_msg;
//	init_msg.data = tmp;
//	step1.handle(&init_msg);
//}



//3 -3 测试


class Echo :public AZinxHandler {
public:
	IChannel* m_out = nullptr;
	// 通过 AZinxHandler 继承
	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override
	{
		//GET_REF2DATA(ByteMsg, input, *_inputMsg);
		ByteMsg* pRet = nullptr;
		ByteMsg* pmsg = dynamic_cast<ByteMsg*>(_inputMsg);

		if (nullptr != pmsg)
		{
			/*sysIODicMsg ioDir(sysIODicMsg::IO_OUT);
			pRet = new ByteMsg(pmsg->content, ioDir);
			std::cout << "echo recv \"" << pmsg->content << "\"" << std::endl;*/
			ZinxKernel::zin_sendout(pmsg->content, m_out);
			std::cout << "echo recv \"" << pmsg->content << "\"" << std::endl;
		}
		return pRet;
	}
	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override
	{
		return nullptr;
	}
} echo;

class	myTcpData :public TcpDataChannel {
public:
	myTcpData(int _fd) :TcpDataChannel(_fd) {}
	// 通过 TcpDataChannel 继承
	virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override
	{
		echo.m_out = this;
		return &echo;
	}
};


class myFact : public TcpDataChannelFactory
{
public:

	// 通过 TcpDataChannelFactory 继承
	virtual TcpDataChannel* CreateChannel(int _fd) override
	{
		return new myTcpData(_fd);
	}
	
};


int main()
{
	//3-1 3-2 的测试
	//test_chain();
	//StdInChannel in_channel;
	//StdOutChannel out_channel;
	//ProcessFunc pf;


	//pf.m_out = &out_channel;
	//in_channel.m_proc = &pf;

	//ZinxKernel &kernel = ZinxKernel::GetInstance();
	//kernel.AddChannel(&in_channel);
	//kernel.AddChannel(&out_channel);

	////文件通信类的连接
	////FIFO_Channel fifo_input("input", true);
	////FIFO_Channel fifo_output("output", false);
	////fifo_input.m_out = &fifo_output;
	////kernel.AddChannel(&fifo_input);
	////kernel.AddChannel(&fifo_output);

	//kernel.run();


	//3-3 的测试
	ZinxKernel& kernel = ZinxKernel::GetInstance();
	
	kernel.AddChannel((new TcpListenChannel(51111, new myFact())));
	
	
	kernel.run();

	return 0;
}