#ifndef REQUESTTHREAD_H
#define REQUESTTHREAD_H

#include "ServiceThread.h"


class CRequestThread : public CServiceThread
{
public:
    CRequestThread();
    virtual ~CRequestThread();

public:
    virtual bool OnEventThreadRun(void);
    virtual bool OnEventThreadStrat(void);
    virtual bool OnEventThreadConclude(void);

public:
    int SetDataEngineSink(IDataEngineSink *pDataEngineSink);

public:
    bool AddToQueue(WORD wIdentifier, CSocketItem *pSocketItem, void *pData, WORD wDataSize);

private:
    pthread_cond_t                    m_Cond;
    pthread_mutex_t                   m_Mutex;
    std::queue<tagRequest>            m_RequestQueue;

private:
    IDataEngineSink                   *m_pDataEngineSink;
};

#endif // REQUESTTHREAD_H
