#pragma once


/*����handlerʵ��֮�䴫�ݵ���Ϣ����*/
class ZinxMessage{
public:
	ZinxMessage() {}
	virtual ~ZinxMessage() {}
};



//������handler�����࣬��Ҫ��д������Ϣ�����ͻ�ȡ��һ�������߷���
//���ĺ����߼�Ϊ������
class AZinxHandler
{
public:
	AZinxHandler();
	virtual ~AZinxHandler();
	
	//���� ����ִ�е�ǰ���ڵĴ��� 
	//��ִ����һ�׶εĴ�������ֱ��û����һ����
	void Handle(ZinxMessage* _inputMsg);
	
protected:
	//������Ҫ��д����
	// ���� ���ݵĴ��� �ɼ̳е�����ȥ����ʵ��
	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) = 0;

	//_next_input �� internel_handle�ķ���ֵ
	//��ȡ��һ�������ں���
	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) = 0;
	
	
	


};

