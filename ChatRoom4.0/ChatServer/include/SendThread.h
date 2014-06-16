#ifndef CSENDTHREAD_H
#define CSENDTHREAD_H

/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: WorkThread.h
Author: 李有红
Version: 0.1
Date:2014/01/02
Description: 发送线程类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <list>
#include "ServiceThread.h"
#include "Locker.h"
#include "../../GlobleData.h"

class CSendThread : public CServiceThread
{
public:
    CSendThread(unsigned int MaxRequests);
    virtual ~CSendThread();

public:
    virtual bool OnEventThreadStrat();
    virtual bool OnEventThreadRun();
    virtual bool OnEventThreadConclude();

public:
    bool Append(tagSendRequest *Request);

private:
    std::list<tagSendRequest *>       m_SendRequestQueue;
    CLocker                           m_QueueLocker;
    CSem                              m_QueueState;
    unsigned int                      m_MaxRequests;
};

#endif // CSENDTHREAD_H
