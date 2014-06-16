#ifndef CWORKTHREAD_H
#define CWORKTHREAD_H
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: WorkThread.h
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

#include "ServiceThread.h"
#include "SocketItem.h"

struct epoll_event;
class CLoggingAcceptor;

class CWorkThread : public CServiceThread
{
public:
    CWorkThread();
    virtual ~CWorkThread();

public:
    virtual bool OnEventThreadRun(void);
    virtual bool OnEventThreadStrat(void);
    virtual bool OnEventThreadConclude(void);

public:
    bool SetEpollFD(const int fd);
    bool SetListenSocketItem(CSocketItem *pSocketItem);
    bool SetLoggingAcceptor(CLoggingAcceptor *pAcceptor);

private:
    struct epoll_event      *m_pEpollEvents;
    int                     m_fdEpoll;
    CSocketItem             *m_pListenSocketItem;
    CLoggingAcceptor        *m_pLoggingAcceptor;
};

#endif // CWORKTHREAD_H
