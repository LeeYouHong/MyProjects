#pragma once
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: SocketItem.h
Author: 李有红
Version: 0.1
Date:2013/12/16
Description: 套接字类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <sys/socket.h>
#include <string>
#include "interface.h"
#include "../../GlobleData.h"
#include "Client.h"

/////////Class Declaration/////////////
class IServerSocketItemSink;
/////////////////////////////////////


class CSocketItem
{
public:
    CSocketItem(IServerSocketItemSink *pSink);
    virtual ~CSocketItem();

    //IO事件处理
public:
     bool OnReadEvent(const int fdEpoll);
     bool OnSendEvent(void);
     bool OnAccepteEvent(const int fdEpoll);
     bool OnCloseEvent(const int fdEpoll);

public:
    //设置套接子
    int Attach(const int socket);
    int GetSocket(void) const;
    int SetAddr(const struct sockaddr SocketAddr);
    int AddToEpoll(const int fdEpoll, const int flag);
    int DelFromEpoll(const int fdEpoll, const int flag);
    int ModifyFromEpoll(const int fdEpoll, const int flag);
    bool ReSetData();
    bool UserQuit();

public:
    bool SendData(void *pData, WORD wDataSize, WORD MainCmdID, WORD wSubCmdID);
    bool SendData(WORD MainCmdID, WORD wSubCmdID);
    DWORD SendDataBuffer(int socket, void * pBuffer, WORD wSendSize);

public:
    void CloseSocket(void);
    int ListenClient(void);
    int SetNonBlock(void);

public:
    int PrintAddr(void);

public:
    int SetServerSocketItemSink(IServerSocketItemSink *pServerSocketItemSink);

private:
    int GetRemoteAddr(const struct sockaddr *pSocketAddr, char *strHost, char *strPort);
    bool AcceptHandle(const int fdEpoll, int Socket, struct sockaddr in_addr);

private:
    int                     m_Socket;
    struct sockaddr         m_SocketAddr;
    IServerSocketItemSink   *m_pIServerSocketItemSink;

    //状态变量
private:
    BYTE                     m_cbRecvBuf[SOCKET_BUFFER * 5];
    WORD                     m_wRecvSize;
    DWORD                    m_dwRecvPacketCount;

public:
    WORD                     m_wIndex;
    CClient                  m_Client;
};
