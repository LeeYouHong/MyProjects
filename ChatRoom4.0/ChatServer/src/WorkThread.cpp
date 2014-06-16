/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: WorkThread.cpp
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 工作线程类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#define MAXEVENTS 64

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "WorkThread.h"
#include "SocketItem.h"
#include "Log.h"

CWorkThread::CWorkThread()
:m_pEpollEvents(NULL)
,m_pListenSocketItem(NULL)
{
    //ctor
}

CWorkThread::~CWorkThread()
{
    //dtor
}
bool CWorkThread::OnEventThreadStrat(void)
{
    m_pEpollEvents = (struct epoll_event *)new struct epoll_event[MAXEVENTS];
    return true;
}

bool CWorkThread::OnEventThreadConclude(void)
{
    SafeDelete(m_pEpollEvents);
    return true;
}

bool CWorkThread::SetEpollFD(const int fd)
{
    m_fdEpoll = fd;
    return true;
}

bool CWorkThread::SetListenSocketItem(CSocketItem* pSocketItem)
{
    m_pListenSocketItem = pSocketItem;
    return true;
}

bool CWorkThread::OnEventThreadRun(void)
{
    int iCount;
    CLog::Instance().Print("CWorkThread::OnEventThreadRun");

    iCount = epoll_wait(m_fdEpoll, m_pEpollEvents, MAXEVENTS, -1);

    for (int i = 0; i < iCount; i++)
    {
        CSocketItem *pSocketItem = (CSocketItem *)m_pEpollEvents[i].data.ptr;
        uint32_t events = m_pEpollEvents[i].events;
        if ( (events & EPOLLERR) || (events & EPOLLHUP) || !(events & EPOLLIN) )
        {
            CLog::Instance().Print("套接字EPOLLERR");
            pSocketItem->OnCloseEvent(m_fdEpoll);
            continue;
        }
        else if (m_pListenSocketItem == pSocketItem)
        {
            pSocketItem->OnAccepteEvent(m_fdEpoll);
            continue;
        }
        else
        {
            if(events & EPOLLIN)
            {
                pSocketItem->OnReadEvent(m_fdEpoll);
            }
            else if (events & EPOLLOUT)
            {
                pSocketItem->OnSendEvent();
            }
        }
    }
    return true;
}

bool CWorkThread::SetLoggingAcceptor(CLoggingAcceptor* pAcceptor)
{
    m_pLoggingAcceptor = pAcceptor;
    return true;
}
