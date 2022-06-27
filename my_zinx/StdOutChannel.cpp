#include "StdOutChannel.h"
#include <iostream>
StdOutChannel::StdOutChannel()
{
}

StdOutChannel::~StdOutChannel()
{
}

std::string StdOutChannel::ReadFd()
{
    return std::string();
}

void StdOutChannel::WriteFd(std::string _output)
{
    std::cout << _output << std::endl;
}

int StdOutChannel::GetFd()
{
    return 1;
}

//void StdOutChannel::data_process(std::string _input)
//{
//}

bool StdOutChannel::Init()
{
    return true;
}

void StdOutChannel::Fini()
{
}

AZinxHandler* StdOutChannel::GetInputNextStage(ByteMsg* _byte)
{
    return nullptr;
}
