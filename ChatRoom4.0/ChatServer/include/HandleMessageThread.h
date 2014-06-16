#ifndef CHANDLEMESSAGETHREAD_H
#define CHANDLEMESSAGETHREAD_H

/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: HandleMessageThread.h
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 处理消息线程
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <queue>
#include "pthread.h"
#include "ServiceThread.h"
#include "interface.h"

class CHandleMessageThread : public CServiceThread
{
public:
    CHandleMessageThread();
    virtual ~CHandleMessageThread();

public:
    virtual bool OnEventThreadRun(void);
    virtual bool OnEventThreadStrat(void);
    virtual bool OnEventThreadConclude(void);
private:
    pthread_cond_t                              m_Cond;
    pthread_mutex_t                             m_Mutex;
    std::queue<tagHandleMessageRequest>         m_DBRequestQueue;
};

#endif // CHANDLEMESSAGETHREAD_H
