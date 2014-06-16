#include "RequestThread.h"

CRequestThread::CRequestThread()
{
    //ctor
}

CRequestThread::~CRequestThread()
{
    //dtor
}

bool CRequestThread::OnEventThreadRun(void)
{
    pthread_mutex_lock(&m_Mutex);
    pthread_cond_wait(&m_Cond, &m_Mutex);
    pthread_mutex_unlock(&m_Mutex);

    while ( !m_RequestQueue.empty() )
    {
       tagDBRequest Request = m_RequestQueue.front();

       m_pDataEngineSink->OnDataEngineSink(Request.wRequestID, Request.pSocketItem
                                           , Request.pData, Request.wDataSize);
       m_RequestQueue.pop();
       SAFE_DELETE(Request.pData);
    }
    return true;
}

int CRequestThread::SetDataEngineSink(IDataEngineSink *pDataEngineSink)
{
    m_pDataEngineSink = pDataEngineSink;
    return 0;
}


bool CRequestThread::OnEventThreadStrat(void)
{
   return true;
}

bool CRequestThread::OnEventThreadConclude(void)
{
    return true;
}
