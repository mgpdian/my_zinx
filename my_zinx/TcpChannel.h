#pragma once
#include "IChannel.h"
class TcpDataChannel :
    public IChannel
{
private:
    int m_socketfd = -1;
public:
    TcpDataChannel(int _socketfd);
    virtual ~TcpDataChannel();
    // 通过 IChannel 继承
    virtual bool Init() override;
    virtual void Fini() override;
    virtual std::string ReadFd() override;
    virtual void WriteFd(std::string _output) override;
    virtual int GetFd() override;

    virtual std::string GetChannelInfo()override;
    
    
    //virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;
    //这个不写 交给子类去写
   
   
};


//工厂类 用他来创建 TCP通道类
class TcpDataChannelFactory
{
public:
    virtual TcpDataChannel* CreateChannel(int _fd) = 0;
};


//监听通道类
class TcpListenChannel : public IChannel
{
private:
    TcpDataChannelFactory* m_factory = nullptr;
    unsigned short m_port = 0;
    int m_iListenFd = -1;
public:
    //指定监听端口 和 接收到端口连接后要创建的工厂
    TcpListenChannel(unsigned short _port, TcpDataChannelFactory* _pfact);
    virtual ~TcpListenChannel();
    // 通过 IChannel 继承
    virtual bool Init() override;
    
    virtual std::string ReadFd() override;
    virtual void WriteFd(std::string _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;



    // 视频里还一个多的 GetChannelInfo() 目前好像没啥用先不写了
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;


    
};