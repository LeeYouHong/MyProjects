#pragma once
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: HandleMessage.h
Author: 李有红
Version: 0.1
Date:2013/12/1
Description: 处理消息逻辑
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include "interface.h"


class CHandleMessage : public IServerSocketItemSink, public IDataBaseResultSink
{
public:
    CHandleMessage();
    virtual ~CHandleMessage();

public:
    virtual int OnReadSink(CMD_Command Command, void *pBuffer, WORD wDataSize, int Socket);
    virtual int OnAcceptSink(int fdEpoll, int socket, struct sockaddr in_addr);
    virtual int onSendSink(int Socket);

public:
    virtual int DataBaseResultSink(WORD wRequestID, int Socket, void *pData, WORD wDataSize);

public:
    int SetPostDataBaseRequestSink(IPostDataBaseRequestSink *pPostDataBaseRequestSink);
    int SetNetworkSink(INetworkSink *pSink);

private:
    bool OnSocketMainLogon(WORD wSubCmdID, void *pData, WORD wDataSize, int Socket);
    bool OnSocketChat(WORD wSubCmdID, void *pData, WORD wDataSize, int Socket);
    bool SingleChat(void *pData, WORD wDataSize, int Socket);
    bool AllChat(void *pData, WORD wDataSize, int Socket);

private:
    IPostDataBaseRequestSink             *m_pPostDataDataSink;
    INetworkSink                         *m_pNetworkSink;
};
