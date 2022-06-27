#include "TcpChannel.h"
#include "ZinxKernel.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

TcpDataChannel::TcpDataChannel(int _socketfd) : m_socketfd(_socketfd)
{
}

TcpDataChannel::~TcpDataChannel()
{
    if (0 <= m_socketfd)
    {
        close(m_socketfd);
        m_socketfd = -1;
    }
}

bool TcpDataChannel::Init()
{
    return true;;
}

void TcpDataChannel::Fini()
{
    if (0 <= m_socketfd)
    {
        close(m_socketfd);
        m_socketfd = -1;

   }
}

std::string TcpDataChannel::ReadFd()
{
    std::string retStr;
    char szBuff[128] = { 0 };

    int irecvlen = -1;
    while (1)
    {
        //MSG_DONTWAIT 不阻塞
        irecvlen = recv(m_socketfd, szBuff, sizeof(szBuff), MSG_DONTWAIT);
        if (0 == irecvlen)
        {
           
           
           SetChannelClose();
            
            
            break;
            
        }
        else if (-1 == irecvlen)
        {
            //链接断了
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                //连接没断，只是没数据了
                break;
               // continue;
            }
            SetChannelClose();
            break;
        }
        else {
            retStr.append(szBuff, irecvlen);
        }
    }
    
    
    return retStr;
}

void TcpDataChannel::WriteFd(std::string _output)
{
    send(m_socketfd, _output.data(), _output.size(), 0);
    
}

int TcpDataChannel::GetFd()
{
    return m_socketfd;
}

std::string TcpDataChannel::GetChannelInfo()
{
    return "data_socket" + m_socketfd;
}

//AZinxHandler* TcpDataChannel::GetInputNextStage(ByteMsg* _byte)
//{
//    return nullptr;
//}

//第二参数的内存被该类析构
TcpListenChannel::TcpListenChannel(unsigned short _port, TcpDataChannelFactory* _pfact) : m_port(_port), m_factory(_pfact)
{

}

TcpListenChannel::~TcpListenChannel()
{
    if (nullptr != m_factory)
    {
        delete m_factory;
    }
}

bool TcpListenChannel::Init()
{
    bool bRet = false;
    int isocket = -1;

    isocket = socket(AF_INET, SOCK_STREAM, 0);
    if (0 <= isocket)
    {
        struct sockaddr_in stServer;
        stServer.sin_family = AF_INET;
        stServer.sin_addr.s_addr = INADDR_ANY;
        stServer.sin_port = htons(m_port);

        //设置允许重复绑定
        int iflag = 1;
        setsockopt(isocket, SOL_SOCKET, SO_REUSEADDR, &iflag, sizeof(iflag));

        if (0 == bind(isocket, (struct sockaddr*)&stServer, sizeof(stServer)))
        {
            if (0 == listen(isocket, 1000))
            {
                bRet = true;
                m_iListenFd = isocket;
            }
            else
            {
                perror(__FILE__":""__FUNC__"":""listen error");
            }
        }
        else {
            perror(__FILE__":""__FUNC__"":""bind error");
        }
    }
    else
    {
        perror(__FILE__":""__FUNC__"":""socket error");
    }
    return bRet;
}

void TcpListenChannel::Fini()
{
    if (0 <= m_iListenFd)
    {
        close(m_iListenFd);
    }
}

std::string TcpListenChannel::ReadFd()
{
    bool bRet = false;
    struct sockaddr_in stCLient;
    socklen_t socklen = sizeof(stCLient);

    int iDataFd = accept(m_iListenFd, (struct sockaddr*)&stCLient, &socklen);
    if (0 <= iDataFd)
    {
        TcpDataChannel* pDataChannel = m_factory->CreateChannel(iDataFd);
        ZinxKernel::GetInstance().AddChannel(pDataChannel);
    }
    else {
        perror(__FILE__":""__FUNC__"":""accept error");
    }
    return std::string();
}

void TcpListenChannel::WriteFd(std::string _output)
{
}

int TcpListenChannel::GetFd()
{
    return m_iListenFd;
}


std::string TcpListenChannel::GetChannelInfo()
{
    return "TCP_LISTEN";
}

AZinxHandler* TcpListenChannel::GetInputNextStage(ByteMsg* _byte)
{
    return nullptr;
}



