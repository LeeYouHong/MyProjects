#include <algorithm>
#include "Client.h"
#include "ClientManager.h"
#include "Log.h"

using namespace std;

CClientManager::CClientManager()
:m_wCurrentCount(0)
,m_wStorageCount(0)
{
    //ctor
}

CClientManager::~CClientManager()
{
    //dtor
    //删除在线客户项
    vector<CClient *>::iterator itera;
    for (itera = m_StorageClient.begin(); itera != m_StorageClient.end(); itera++)
    {
        SAFE_DELETE(*itera);
    }

    m_StorageClient.clear();
    m_FreeClient.clear();
    m_ActiveClient.clear();
}

CClient *CClientManager::ActiveClient()
{
    CClient *pClient = NULL;

    //从空闲客户项数组中获取新到客户项
    if ( !m_FreeClient.empty() )
    {
        pClient = m_FreeClient.back();
        m_FreeClient.pop_back();

        m_ActiveClient.push_back(pClient);
        return pClient;
    }

    //新建一个客户项
    if (m_wCurrentCount < m_wStorageCount)
    {
        pClient = new CClient();
        pClient->SetIndex(m_wCurrentCount);

        m_ActiveClient.push_back(pClient);
        m_StorageClient.push_back(pClient);

        m_wCurrentCount++;

        return pClient;
    }

    return pClient;
}

CClient *CClientManager::GetClient(int Socket)
{
    CClient *pClient = NULL;
    vector<CClient *>::iterator itera;
    for (itera = m_StorageClient.begin(); itera != m_StorageClient.end(); itera++)
    {
        pClient = *itera;
        if (pClient->GetSocket() == Socket)
        {
             break;
        }
    }
    return pClient;
}

bool CClientManager::FreeClient(WORD wClientIndex)
{
    CClient *pClient = m_StorageClient[wClientIndex];
    vector<CClient *>::iterator   itera;

    //查找客户项
    itera = find(m_ActiveClient.begin(), m_ActiveClient.end(), pClient);
    if (itera == m_ActiveClient.end())
    {
        CLog::Instance()->Print("CClientManager::FreeClient：在活动客户项数组中找不到该用户");
        return false;
    }

    //放入空闲客户项数组
    m_FreeClient.push_back(*itera);
    //在活动客户数组中删除
    m_ActiveClient.erase(itera);

    return false;
}

void CClientManager::SetStorageCount(WORD wCount)
{
    m_wStorageCount = wCount;
}

CClient *CClientManager::SearchWithName(const string name)
{
    std::vector<CClient *>::iterator iter;
    for (iter = m_ActiveClient.begin(); iter != m_ActiveClient.end(); iter++)
    {
        string UserName = (*iter)->m_strUserName;
        if (name == UserName)
        {
            return *iter;
        }
    }
    return NULL;
}


vector<CClient *> CClientManager::GetOnLine(void)
{
    std::vector<CClient *> temp;
    std::vector<CClient *>::iterator iter;
    for (iter = m_ActiveClient.begin(); iter != m_ActiveClient.end(); iter++)
    {
        if ( (*iter)->m_bOnLine == true )
        {
            temp.push_back(*iter);
        }
    }
    return temp;
}
void CClientManager::SetHandleMessageSink(IServerSocketItemSink* pSink)
{
    m_pHandleMessageSink = pSink;
}

IServerSocketItemSink *CClientManager::GetHandleMessageSink(void)
{
    return m_pHandleMessageSink;
}
