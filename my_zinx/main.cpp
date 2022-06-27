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
//	// ͨ�� AZinxHandler �̳�
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
//	// ͨ�� AZinxHandler �̳�
//	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override
//	{
//		testMsg* pmsg = dynamic_cast<testMsg*>(_inputMsg);
//		printf("%d\n", pmsg->data[0]);
//
//		//ԭ��������һ������ 
//		//��ΪʲôҪnewһ���� ��Ϊ������ͬһ�������л������������
//		//�����new �Ļ� ����һ������һ����Ϣ����ͬһ�� �ظ������ᵼ�±���
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
//	// ͨ�� AZinxHandler �̳�
//
//	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override
//	{
//		testMsg* pmsg = dynamic_cast<testMsg*>(_inputMsg);
//		std::string output("a");
//		std::transform(pmsg->data.begin(), pmsg->data.end(), output.begin(), ::toupper);
//		
//		std::cout << output << std::endl;
//		
//		//new �µ���Ϣ���󷵻�
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



//3 -3 ����


class Echo :public AZinxHandler {
public:
	IChannel* m_out = nullptr;
	// ͨ�� AZinxHandler �̳�
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
	// ͨ�� TcpDataChannel �̳�
	virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override
	{
		echo.m_out = this;
		return &echo;
	}
};


class myFact : public TcpDataChannelFactory
{
public:

	// ͨ�� TcpDataChannelFactory �̳�
	virtual TcpDataChannel* CreateChannel(int _fd) override
	{
		return new myTcpData(_fd);
	}
	
};


int main()
{
	//3-1 3-2 �Ĳ���
	//test_chain();
	//StdInChannel in_channel;
	//StdOutChannel out_channel;
	//ProcessFunc pf;


	//pf.m_out = &out_channel;
	//in_channel.m_proc = &pf;

	//ZinxKernel &kernel = ZinxKernel::GetInstance();
	//kernel.AddChannel(&in_channel);
	//kernel.AddChannel(&out_channel);

	////�ļ�ͨ���������
	////FIFO_Channel fifo_input("input", true);
	////FIFO_Channel fifo_output("output", false);
	////fifo_input.m_out = &fifo_output;
	////kernel.AddChannel(&fifo_input);
	////kernel.AddChannel(&fifo_output);

	//kernel.run();


	//3-3 �Ĳ���
	ZinxKernel& kernel = ZinxKernel::GetInstance();
	
	kernel.AddChannel((new TcpListenChannel(51111, new myFact())));
	
	
	kernel.run();

	return 0;
}