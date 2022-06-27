#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include "AZinxHandler.h"
//创建单例实例
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
		//1 等传入 (epoll) 
		struct epoll_event astEvents[200];
		int ready_count = epoll_wait(m_epollfd, astEvents, 200, -1);
		if (0 == ready_count)
		{
			if (EINTR == errno)
			{
				// 被信号打断 则继续循环
				continue;
			}
		}
		//2 调用通道的读取函数或者写出函数
		for (int i = 0; i < ready_count; ++i)
		{
			//1 读取数据
				//将void* 强转成 IChannel*
			IChannel* pchannel = static_cast<IChannel*>(astEvents[i].data.ptr);
			if (0 != (astEvents[i].events & EPOLLIN))
			{
				//需要处理外部输入的数据

				//触发处理责任链
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
				//需要向外输出的数据
				//IChannel* pchannel = static_cast<IChannel*>(astEvents[i].data.ptr);
				
				//触发处理责任链 输出的
				pchannel->FlushOut();
				ModChannel_DelOut(pchannel);
			}
			

			//3 回显到标志输出
			//std::cout << content << std::endl;
		}
	}
	
	
}



void ZinxKernel::AddChannel(IChannel* _pChannel)
{
	//先初始化
	if (true == _pChannel->Init())
	{
		//将参数和文件描述符 关联起来 (epoll_ctl)

		struct epoll_event stEvent;
		stEvent.events = EPOLLIN;
		stEvent.data.ptr = _pChannel;


		epoll_ctl(m_epollfd, EPOLL_CTL_ADD, _pChannel->GetFd(), &stEvent);

	}
	
}

void ZinxKernel::DelChannel(IChannel* _pChannel)
{
	epoll_ctl(m_epollfd, EPOLL_CTL_DEL, _pChannel->GetFd(), nullptr);

	//删除通道后 去初始化
	_pChannel->Fini();
}


//当输入的通道得到数据后 会把数据传给输出通道的缓冲区 同时 把epoll添加输出信号
void ZinxKernel::ModChannel_AddOut(IChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN | EPOLLOUT;
	stEvent.data.ptr = _pChannel;
	
	epoll_ctl(m_epollfd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

//得到输出信号后 输出数据后 修改为输入信号
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
