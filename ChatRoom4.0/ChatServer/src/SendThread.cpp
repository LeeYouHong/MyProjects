#include "SendThread.h"
#include "SocketItemManager.h"
#include "SocketItem.h"

CSendThread::CSendThread(unsigned int MaxRequests)
:m_MaxRequests(MaxRequests)
{
    //ctor
}

CSendThread::~CSendThread()
{
    //dtor
}

bool CSendThread::OnEventThreadStrat()
{
    return true;
}

bool CSendThread::OnEventThreadRun()
{
    m_QueueState.Wait();
    m_QueueLocker.Lock();
    if ( m_SendRequestQueue.empty() )
    {
        m_QueueLocker.Unlock();
        return true;
    }

    tagSendRequest *Request = m_SendRequestQueue.front();
    m_SendRequestQueue.pop_front();
    m_QueueLocker.Unlock();

    if ( !Request )
    {
        return false;
    }

    //发送消息
    CSocketItem *pSendSocketItem = CSocketItemManager::Instance().GetSocketItem(Request->Socket);
    pSendSocketItem->SendData(Request->pData, Request->wDataSize, Request->wMainCmdID,
                              Request->wSubCmdID);

    //释放请求
    SafeDelete(Request->pData);
    SafeDelete(Request);

    return true;
}

bool CSendThread::OnEventThreadConclude()
{
    return true;
}

bool CSendThread::Append(tagSendRequest *Request)
{
    m_QueueLocker.Lock();
    if (m_SendRequestQueue.size() > m_MaxRequests)
    {
        m_QueueLocker.Unlock();
        return false;
    }
    m_SendRequestQueue.push_back(Request);
    m_QueueLocker.Unlock();
    m_QueueState.Post();
    return true;
}
