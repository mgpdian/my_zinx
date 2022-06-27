#pragma once

#include "StdInChannel.h"
#include "IChannel.h"
#include "StdOutChannel.h"
#include <list>

/*���嶯̬����ת������������ͣ���ת��ʧ����ִ�з���NULL*/
#define GET_REF2DATA(type, ref, orig) type * pref = dynamic_cast<type *> (&orig); if(nullptr == pref) {return nullptr;} type &ref = dynamic_cast<type&>(orig)

//���� ����������Զ�epoll�����޸� (ָ��д�л�)
class ZinxKernel
{
private:
	StdInChannel* m_channel = nullptr;
	std::list<IChannel*> m_channel_list; //��������ͨ��
	int m_epollfd = -1;
	ZinxKernel();
	virtual ~ZinxKernel();
	static ZinxKernel g_kernel;
public:
	static ZinxKernel& GetInstance()
	{
		return g_kernel;
	}

	void run();

	//���ͨ��˳�㽫ͨ�� fd �� epoll ��������
	void AddChannel(IChannel* _pChannel);
	//ɾ��ͨ��˳�㽫ͨ�� fd �Ƴ� epoll
	void DelChannel(IChannel* _pChannel);
	// �޸� epoll �ļ�������
	// �������ļ�������
	void ModChannel_AddOut(IChannel* _pChannel);
	// ɾ������ļ�������
	void ModChannel_DelOut(IChannel* _pChannel);

	StdInChannel* GetChannel()
	{
		return this->m_channel;
	}

	static void zin_sendout(std::string _output, IChannel* _pchannel);
};

