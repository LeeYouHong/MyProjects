#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>
#include <string>
#include "../../GlobleData.h"

using namespace std;

//处理服务端发送过来到消息接口
class IHandleMessageSink
{
public:

    virtual bool HandleMessage(CMD_Command command, const void *pData, WORD wDataSize) = 0;
};

//发送消息的接口
class ISendMessageSink
{
public:

    virtual bool SendLogonMessage(string name, string passwd) = 0;
    virtual bool SendRegisterMessage(string name, string passwd) = 0;
    virtual bool SendChatToSingleMessage(string recver, string message) = 0;
    virtual bool SendChatToAllMessage(string message) = 0;
};

//命令接口
class ICommandEventSink
{
public:
    virtual bool OnLogonEvent(string name, string passwd) = 0;
    virtual bool OnRegisterEvent(string name, string passwd) = 0;
    virtual bool OnChatToSingle(string recver, string message) = 0;
    virtual bool OnChatToAll(string message) = 0;
    virtual bool OnLayout() = 0;
};

//交互接口
class IViewSink
{
public:
    virtual void Run() = 0;
    virtual void Print(string message) = 0;
    virtual void Print(const char *message) = 0;
};

//本地操作命令
class ILocalCMDSink
{
public:

    virtual bool StopConnect(void) = 0;
};
#endif // INTERFACE_H_INCLUDED
