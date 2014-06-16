#ifndef CDATABASETHREAD_H
#define CDATABASETHREAD_H

/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: DataBaseThread.h
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 数据库线程
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <pthread.h>
#include <queue>
#include <list>
#include "ServiceThread.h"
#include "interface.h"
#include "Locker.h"

class CSocketItem;

class CDataBaseThread : public CServiceThread
{
public:
    CDataBaseThread(unsigned int MaxRequests);
    virtual ~CDataBaseThread();

public:
    virtual bool OnEventThreadRun(void);
    virtual bool OnEventThreadStrat(void);
    virtual bool OnEventThreadConclude(void);

public:
    int SetDataEngineSink(IDataEngineSink *pDataEngineSink);

public:
    bool Append(tagDBRequest *Request);

private:
    std::list<tagDBRequest *>         m_DBRequestQueue;
    CLocker                           m_QueueLocker;
    CSem                              m_QueueState;
    unsigned int                      m_MaxRequests;

private:
    IDataEngineSink                   *m_pDataBaseEngineSink;
};

#endif // CDATABASETHREAD_H
