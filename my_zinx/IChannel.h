#pragma once

#include <string>
#include "AZinxHandler.h"

class sysIODicMsg : public ZinxMessage
{
public:
	enum IO_DIC {
		IO_IN, IO_OUT
	}m_DIC;

	sysIODicMsg(IO_DIC _dic) :m_DIC(_dic) {}
};


//Ϊ�������ݱ���֮ǰ��״̬����������һ�׶ε� IO ������Ϣ���������ĸ�ֵ���������Ǽ̳� SysIODirMsg ��
class ByteMsg : public sysIODicMsg
{
public:
	std::string content;
	std::string szInfo;
	ByteMsg(std::string _content, sysIODicMsg &_iodic) :content(_content), sysIODicMsg(_iodic.m_DIC) {}
};

class IChannel : public AZinxHandler
{
private:
	std::string m_buffer;

	//ͨ���رձ�־
	bool m_NeedClose = false;
public:
	IChannel();
	virtual ~IChannel();

	//�� kernel �� AddChannel() �е���
	virtual bool Init() = 0;
	

	virtual std::string ReadFd() = 0;
	//��ֱ�ӵ���Writefd ֻ�Ǵ浽���� m_buffer ���޸� epoll, �ȵ� epoll �����ٷ�, ͨ�ú���
	virtual void WriteFd(std::string _output) = 0;

	//ȥ��ʼ���� �� kernel �� DelChannel() �е���
	virtual void Fini() = 0;

	
	
	
	virtual int GetFd() = 0;

	//������������ ����Ҫ���������ݽ��д���ת��
	//virtual void data_process(std::string _input) = 0;

	
	// ����ˢ���������� �ڲ����� WriteFd ��������������������
	//�����������writefd
	void FlushOut();

	//��ֱ�ӵ���Writefd ֻ�Ǵ浽���� m_buffer ���޸� epoll, �ȵ� epoll �����ٷ�, ͨ�ú���
	virtual void data_sendout(std::string _output);

	// ͨ�� AZinxHandler �̳�
	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override;

	//��Ϊ����ת������һ��������� ����  ��AZinxHandler�̳ж���
	//���������ڻ���֪��Ҫת����˭ ��Ҫ������ʵ��  ���������ǲ�����ʲô ���඼����ͨ����ݼ���ʵ��
	// ���������û������� ʹ�ò�����
	// �������� ���� ʹ��һ���µĴ��麯���� ��Ϊ ת������ 
	//Ŀ������������Կ���
	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override;

	/*����ͨ���رպ������������ݴ����ҵ����ʱ���жϵ�ǰͨ��Ӧ�ñ��ر�ʱ�����Ե��øú���*/
	void SetChannelClose() { m_NeedClose = true; }
	bool ChannelNeedClose() { return m_NeedClose; }

	
	/*��ȡͨ����Ϣ�����������߿����ڸú����з��ظ�ͨ����ص�һЩ�����ַ���������������Һ͹���*/
	virtual std::string GetChannelInfo() = 0;
	//��Ƶ��û��ʵ��  ���������Ƶ����
	//��������������� ����ͨ��������
	
	//���ݸ������ ת������ ���ܺ�GetNextһ�� Ϊ���Ƿ���������д
	virtual AZinxHandler* GetInputNextStage(ByteMsg * _byte) = 0;

};

