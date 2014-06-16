/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: DataBaseThread.cpp
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

#include <string.h>
#include "../../GlobleData.h"
#include "DataBaseThread.h"
#include "Log.h"

CDataBaseThread::CDataBaseThread(unsigned int MaxRequests)
:m_MaxRequests(MaxRequests)
{

}

CDataBaseThread::~CDataBaseThread()
{

}

bool CDataBaseThread::OnEventThreadRun(void)
{
    m_QueueState.Wait();
    m_QueueLocker.Lock();
    if ( m_DBRequestQueue.empty() )
    {
        m_QueueLocker.Unlock();
        return true;
    }

    tagDBRequest *Request = m_DBRequestQueue.front();
    m_DBRequestQueue.pop_front();
    m_QueueLocker.Unlock();

    if ( !Request )
    {
        return true;
    }

    //处理请求
    m_pDataBaseEngineSink->OnDataEngineSink(Request->wRequestID, Request->Socket,
                                        Request->pData, Request->wDataSize);

    //释放请求
    SafeDelete(Request->pData);
    SafeDelete(Request);
    return true;
}

int CDataBaseThread::SetDataEngineSink(IDataEngineSink *pDataEngineSink)
{
    m_pDataBaseEngineSink = pDataEngineSink;
    return 0;
}

bool CDataBaseThread::OnEventThreadStrat(void)
{
   return true;
}

bool CDataBaseThread::OnEventThreadConclude(void)
{
    return true;
}

bool CDataBaseThread::Append(tagDBRequest* Request)
{
    m_QueueLocker.Lock();
    if (m_DBRequestQueue.size() > m_MaxRequests)
    {
        m_QueueLocker.Unlock();
        return false;
    }
    m_DBRequestQueue.push_back(Request);
    m_QueueLocker.Unlock();
    m_QueueState.Post();

    return true;
}


