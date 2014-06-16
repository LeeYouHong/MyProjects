/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: Epoll.cpp
Author: 李有红
Version: 0.1
Date:2013/12/16
Description: epoll模型类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "NetworkEngine.h"
#include "SocketItem.h"
#include "../../GlobleData.h"
#include "SocketItemManager.h"
#include "Log.h"
#include "SendThread.h"
#include "GlobalParamManager.h"


CNetworkEngine::CNetworkEngine(const char *strListenPort)
:m_strListenPort(strListenPort)
,m_pListenSocketItem(NULL)
,m_pSendThread(NULL)
{

};

CNetworkEngine::~CNetworkEngine()
{
   Stop();
};

bool CNetworkEngine::Start(void)
{
    if (InitListen() == -1)
	{
        CLog::Instance().Print("InitListen error");
		return false;
	}

	m_fdEpoll = CreateEpoll(0);
	if (m_fdEpoll == -1)
	{
	    CLog::Instance().Print("CreateEpoll error");
		return false;
	}



    if(m_pListenSocketItem->AddToEpoll(m_fdEpoll, EPOLLIN | EPOLLET) == -1)
	{
        CLog::Instance().Print("AddToEpoll error");
		return false;
	}


    //初始化工作线程
    m_WorkThread.SetEpollFD(m_fdEpoll);
    m_WorkThread.SetListenSocketItem(m_pListenSocketItem);

    //启动工作线程
    if( !m_WorkThread.StartThread() )
    {
        CLog::Instance().Print("工作线程启动失败");
        return false;
    }

    //初始化发送线程
    int SizeTemp = CGlobalParamManager::Instance().m_SendRequestQueueSize;
    m_pSendThread = new CSendThread(SizeTemp);
    //启动发送线程
    m_pSendThread->StartThread();

    return true;
}

bool CNetworkEngine::Stop(void)
{
    SafeDelete(m_pListenSocketItem);

    m_pSendThread->ConcludeThread(0);
    SafeDelete(m_pSendThread);

    return true;
}

int CNetworkEngine::CreateBindedSocket()
{
    struct addrinfo hints;
    struct addrinfo *result, *temp;
    int iRet = -1;
    int Socket = -1;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    iRet = getaddrinfo (NULL, m_strListenPort, &hints, &result);
    if (iRet != 0)
    {
      CLog::Instance().Print("getaddrinfo error");
      return -1;
    }

    for (temp = result; temp != NULL; temp = temp->ai_next)
    {
      Socket = socket (temp->ai_family, temp->ai_socktype, temp->ai_protocol);
      if (Socket == -1)
        continue;

      iRet = bind (Socket, temp->ai_addr, temp->ai_addrlen);
      if (iRet == 0)
        {
          /* We managed to bind successfully! */
          break;
        }

      close (Socket);
    }

    if (temp == NULL)
    {
        CLog::Instance().Print("can't bind");
        return -1;
    }

    freeaddrinfo (result);

    return Socket;
};

int CNetworkEngine::InitListen(void)
{
    int ListenSocket = -1;
    m_pListenSocketItem = new CSocketItem(CSocketItemManager::Instance().GetServerSocketItemSink());

    ListenSocket = CreateBindedSocket();

    if (ListenSocket == -1)
    {
        CLog::Instance().Print("CreateBindedSocket");
        return -1;
    }

    if (m_pListenSocketItem->Attach(ListenSocket) == -1)
    {
        CLog::Instance().Print("Attach");
        return -1;
    }

    if (m_pListenSocketItem->SetNonBlock() == -1)
    {
        CLog::Instance().Print("MakeSocketNonBlocking");
        return -1;
    }

    if (m_pListenSocketItem->ListenClient() == -1)
    {
        CLog::Instance().Print("ListenClient");
        return -1;
    }

    return 0;
}

int CNetworkEngine::CreateEpoll( int iSize )
{
	int fdEpoll;
	fdEpoll = epoll_create1(iSize);
	if (fdEpoll == -1)
	{
		CLog::Instance().Print("epoll_create1");
		return -1;
	}

	return fdEpoll;
}

bool CNetworkEngine::SendData(int Socket, void* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID)
{
    BYTE *pNewData = NULL;
    //构造请求数据
    tagSendRequest *pRequest = new tagSendRequest;
    if (wDataSize != 0)
    {
        pNewData = new BYTE[wDataSize];
        memcpy(pNewData, pData, wDataSize);
    }

    pRequest->pData = pNewData;
    pRequest->wDataSize = wDataSize;
    pRequest->Socket = Socket;
    pRequest->wMainCmdID = wMainCmdID;
    pRequest->wSubCmdID = wSubCmdID;

    //加入发送线程队列
    m_pSendThread->Append(pRequest);

    return true;
}

