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
    // ͨ�� IChannel �̳�
    virtual bool Init() override;
    virtual void Fini() override;
    virtual std::string ReadFd() override;
    virtual void WriteFd(std::string _output) override;
    virtual int GetFd() override;

    virtual std::string GetChannelInfo()override;
    
    
    //virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;
    //�����д ��������ȥд
   
   
};


//������ ���������� TCPͨ����
class TcpDataChannelFactory
{
public:
    virtual TcpDataChannel* CreateChannel(int _fd) = 0;
};


//����ͨ����
class TcpListenChannel : public IChannel
{
private:
    TcpDataChannelFactory* m_factory = nullptr;
    unsigned short m_port = 0;
    int m_iListenFd = -1;
public:
    //ָ�������˿� �� ���յ��˿����Ӻ�Ҫ�����Ĺ���
    TcpListenChannel(unsigned short _port, TcpDataChannelFactory* _pfact);
    virtual ~TcpListenChannel();
    // ͨ�� IChannel �̳�
    virtual bool Init() override;
    
    virtual std::string ReadFd() override;
    virtual void WriteFd(std::string _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;



    // ��Ƶ�ﻹһ����� GetChannelInfo() Ŀǰ����ûɶ���Ȳ�д��
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;


    
};