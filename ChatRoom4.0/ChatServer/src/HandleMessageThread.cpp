#include "HandleMessageThread.h"

CHandleMessageThread::CHandleMessageThread()
{
    //ctor
}

CHandleMessageThread::~CHandleMessageThread()
{
    //dtor
}

bool CHandleMessageThread::OnEventThreadRun(void)
{
    pthread_mutex_lock(&m_Mutex);
    pthread_cond_wait(&m_Cond, &m_Mutex);
    pthread_mutex_unlock(&m_Mutex);

    while ( !m_DBRequestQueue.empty() )
    {
       tagDBRequest DBRequest = m_DBRequestQueue.front();

       m_pDataEngineSink->OnDataEngineSink(DBRequest.wRequestID, DBRequest.pSocketItem
                                           , DBRequest.pData, DBRequest.wDataSize);
       m_DBRequestQueue.pop();
       SAFE_DELETE(DBRequest.pData);
    }
    return true;
}

bool CHandleMessageThread::OnEventThreadStrat(void)
{
    return true;
}

bool CHandleMessageThread::OnEventThreadConclude(void)
{
    return true;
}
