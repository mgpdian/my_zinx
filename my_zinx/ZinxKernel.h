#pragma once

#include "StdInChannel.h"
#include "IChannel.h"
#include "StdOutChannel.h"
#include <list>

/*定义动态类型转换后的引用类型，若转换失败则执行返回NULL*/
#define GET_REF2DATA(type, ref, orig) type * pref = dynamic_cast<type *> (&orig); if(nullptr == pref) {return nullptr;} type &ref = dynamic_cast<type&>(orig)

//单例 让其他类可以对epoll进行修改 (指读写切换)
class ZinxKernel
{
private:
	StdInChannel* m_channel = nullptr;
	std::list<IChannel*> m_channel_list; //代表所有通道
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

	//添加通道顺便将通道 fd 和 epoll 关联起来
	void AddChannel(IChannel* _pChannel);
	//删除通道顺便将通道 fd 移除 epoll
	void DelChannel(IChannel* _pChannel);
	// 修改 epoll 的监听类型
	// 添加输出的监听方向
	void ModChannel_AddOut(IChannel* _pChannel);
	// 删除输出的监听方向
	void ModChannel_DelOut(IChannel* _pChannel);

	StdInChannel* GetChannel()
	{
		return this->m_channel;
	}

	static void zin_sendout(std::string _output, IChannel* _pchannel);
};

