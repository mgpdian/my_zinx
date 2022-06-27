#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include "AZinxHandler.h"
//��������ʵ��
ZinxKernel ZinxKernel::g_kernel;


ZinxKernel::ZinxKernel()
{
	int fd = epoll_create(1);
	if (fd >= 0)
	{
		m_epollfd = fd;
	}
}

ZinxKernel::~ZinxKernel()
{
	if (-1 != m_epollfd)
	{
		close(m_epollfd);
	}
	
}

void ZinxKernel::run()
{

	while (true)
	{
		//1 �ȴ��� (epoll) 
		struct epoll_event astEvents[200];
		int ready_count = epoll_wait(m_epollfd, astEvents, 200, -1);
		if (0 == ready_count)
		{
			if (EINTR == errno)
			{
				// ���źŴ�� �����ѭ��
				continue;
			}
		}
		//2 ����ͨ���Ķ�ȡ��������д������
		for (int i = 0; i < ready_count; ++i)
		{
			//1 ��ȡ����
				//��void* ǿת�� IChannel*
			IChannel* pchannel = static_cast<IChannel*>(astEvents[i].data.ptr);
			if (0 != (astEvents[i].events & EPOLLIN))
			{
				//��Ҫ�����ⲿ���������

				//��������������
				sysIODicMsg dic(sysIODicMsg::IO_IN);
				pchannel->Handle(&dic);
				if (true == pchannel->ChannelNeedClose())
				{
					ZinxKernel::DelChannel(pchannel);
					delete pchannel;
					break;
				}

			}
			if (0 != (astEvents[i].events & EPOLLOUT))
			{
				//��Ҫ�������������
				//IChannel* pchannel = static_cast<IChannel*>(astEvents[i].data.ptr);
				
				//�������������� �����
				pchannel->FlushOut();
				ModChannel_DelOut(pchannel);
			}
			

			//3 ���Ե���־���
			//std::cout << content << std::endl;
		}
	}
	
	
}



void ZinxKernel::AddChannel(IChannel* _pChannel)
{
	//�ȳ�ʼ��
	if (true == _pChannel->Init())
	{
		//���������ļ������� �������� (epoll_ctl)

		struct epoll_event stEvent;
		stEvent.events = EPOLLIN;
		stEvent.data.ptr = _pChannel;


		epoll_ctl(m_epollfd, EPOLL_CTL_ADD, _pChannel->GetFd(), &stEvent);

	}
	
}

void ZinxKernel::DelChannel(IChannel* _pChannel)
{
	epoll_ctl(m_epollfd, EPOLL_CTL_DEL, _pChannel->GetFd(), nullptr);

	//ɾ��ͨ���� ȥ��ʼ��
	_pChannel->Fini();
}


//�������ͨ���õ����ݺ� ������ݴ������ͨ���Ļ����� ͬʱ ��epoll�������ź�
void ZinxKernel::ModChannel_AddOut(IChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN | EPOLLOUT;
	stEvent.data.ptr = _pChannel;
	
	epoll_ctl(m_epollfd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

//�õ�����źź� ������ݺ� �޸�Ϊ�����ź�
void ZinxKernel::ModChannel_DelOut(IChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN;
	stEvent.data.ptr = _pChannel;
	
	//stEvent.data.fd = _pChannel->GetFd();
	epoll_ctl(m_epollfd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

void ZinxKernel::zin_sendout(std::string _output, IChannel* _pchannel)
{
	sysIODicMsg iodic(sysIODicMsg::IO_OUT);
	ByteMsg byte(_output, iodic);
	_pchannel->Handle(&byte);
}
