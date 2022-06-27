#include "FIFO_Channel.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

FIFO_Channel::FIFO_Channel(std::string _file, bool _isRead):fifo_name(_file), bIsRead(_isRead)
{
}

FIFO_Channel::~FIFO_Channel()
{
}

std::string FIFO_Channel::ReadFd()
{
    char szbuff[1024] = { 0 };

    int ilen = read(m_fd, szbuff, sizeof(szbuff));
    return std::string(szbuff, ilen);
}

void FIFO_Channel::WriteFd(std::string _output)
{
    char* pout = (char * )malloc(_output.size());
    _output.copy(pout, _output.size(), 0);
    write(m_fd, pout, _output.size());
    free(pout);
}

int FIFO_Channel::GetFd()
{
    return m_fd;
}

//void FIFO_Channel::data_process(std::string _input)
//{
//    m_out->data_sendout(_input);
//
//}

bool FIFO_Channel::Init()
{
    //打开文件
    bool bRet = false;
    int flag = O_RDONLY;
    if (false == bIsRead  )
    {
        flag = O_WRONLY;
        
    }
    int fd = open(fifo_name.c_str(), flag | O_NONBLOCK);
    if (fd >= 0)
    {

        m_fd = fd;
        bRet = true;
    }
    //std::cout << (bRet ? "success" : "fail") << std::endl;
    return bRet;
}

void FIFO_Channel::Fini()
{
    //关闭文件
    if (m_fd >= 0)
    {
        close(m_fd);
    }
}

AZinxHandler* FIFO_Channel::GetInputNextStage(ByteMsg* _byte)
{
    return nullptr;
}
