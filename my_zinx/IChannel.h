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


//为了让数据保持之前的状态，即包含上一阶段的 IO 方向消息，避免额外的赋值操作，我们继承 SysIODirMsg 类
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

	//通道关闭标志
	bool m_NeedClose = false;
public:
	IChannel();
	virtual ~IChannel();

	//在 kernel 中 AddChannel() 中调用
	virtual bool Init() = 0;
	

	virtual std::string ReadFd() = 0;
	//不直接调用Writefd 只是存到缓存 m_buffer 并修改 epoll, 等到 epoll 返回再发, 通用函数
	virtual void WriteFd(std::string _output) = 0;

	//去初始化， 在 kernel 中 DelChannel() 中调用
	virtual void Fini() = 0;

	
	
	
	virtual int GetFd() = 0;

	//引入责任链后 不需要他来对数据进行处理转发
	//virtual void data_process(std::string _input) = 0;

	
	// 向外刷出缓存数据 内部调用 WriteFd 这个才是真正的输出函数
	//由这个来调用writefd
	void FlushOut();

	//不直接调用Writefd 只是存到缓存 m_buffer 并修改 epoll, 等到 epoll 返回再发, 通用函数
	virtual void data_sendout(std::string _output);

	// 通过 AZinxHandler 继承
	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) override;

	//因为这是转发给下一个处理类的 函数  从AZinxHandler继承而来
	//但我们现在还不知道要转发给谁 需要子类来实现  但这里我们不管做什么 子类都不能通过快捷键来实现
	// 这样会让用户很难受 使用不方便
	// 所以这里 我们 使用一个新的纯虚函数来 作为 转发函数 
	//目的是让子类可以看到
	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) override;

	/*设置通道关闭函数，若再数据处理或业务处理时，判断当前通道应该被关闭时，可以调用该函数*/
	void SetChannelClose() { m_NeedClose = true; }
	bool ChannelNeedClose() { return m_NeedClose; }

	
	/*获取通道信息函数，开发者可以在该函数中返回跟通道相关的一些特征字符串，方便后续查找和过滤*/
	virtual std::string GetChannelInfo() = 0;
	//视频中没有实现  但后面的视频例有
	//这个函数的作用是 设置通道的名称
	
	//传递给子类的 转发函数 功能和GetNext一样 为的是方便子类重写
	virtual AZinxHandler* GetInputNextStage(ByteMsg * _byte) = 0;

};

