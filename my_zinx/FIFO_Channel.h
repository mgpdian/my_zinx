#pragma once
#include "IChannel.h"
#include <string>
class FIFO_Channel :
    public IChannel
{

private:
    std::string fifo_name; 
    int m_fd = -1;
    bool bIsRead = false;
    
public:
    FIFO_Channel* m_out = nullptr;
    FIFO_Channel(std::string _file, bool _isRead);
    virtual ~FIFO_Channel();



    // 通过 IChannel 继承
    virtual std::string ReadFd() override;

    virtual void WriteFd(std::string _output) override;

    virtual int GetFd() override;

    //virtual void data_process(std::string _input) override;


    // 通过 IChannel 继承
    virtual bool Init() override;

    virtual void Fini() override;


    // 通过 IChannel 继承
    virtual AZinxHandler* GetInputNextStage(ByteMsg* _byte) override;

};

