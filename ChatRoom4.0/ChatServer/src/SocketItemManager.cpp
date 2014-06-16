#include <stdio.h>
#include <algorithm>
#include <vector>
#include "SocketItem.h"
#include "SocketItemManager.h"
#include "Log.h"


using namespace std;

CSocketItemManager::CSocketItemManager()
:m_wCurrentCount(0)
,m_wStorageCount(0)
{
    //ctor
}

CSocketItemManager::~CSocketItemManager()
{
    //dtor
    //删除在线客户项
    vector<CSocketItem *>::iterator itera;
    for (itera = m_Storage.begin(); itera != m_Storage.end(); itera++)
    {
        SafeDelete(*itera);
    }

    m_Storage.clear();
    m_Free.clear();
    m_Active.clear();
}


CSocketItem *CSocketItemManager::ActiveSocketItem()
{
    char log[100];
    snprintf(log, 100, "活跃套接字 %d", m_Active.size());
    CLog::Instance().Print(log);

    CSocketItem *pItem= NULL;

    //从空闲客户项数组中获取新到客户项
    if ( !m_Free.empty() )
    {
        pItem = m_Free.back();
        m_Free.pop_back();

        m_Active.push_back(pItem);
        return pItem;
    }

    //新建一个客户项
    if (m_wCurrentCount < m_wStorageCount)
    {
        pItem = new CSocketItem(m_pHandleMessageSink);

        pItem->m_wIndex = m_wCurrentCount;
        m_Active.push_back(pItem);
        m_Storage.push_back(pItem);

        m_wCurrentCount++;

        return pItem;
    }

    return pItem;
}

CSocketItem *CSocketItemManager::GetSocketItem(const int Socket)
{
    CSocketItem *pItem = NULL;
    vector<CSocketItem *>::iterator itera;
    for (itera = m_Storage.begin(); itera != m_Storage.end(); itera++)
    {
        pItem = *itera;
        if (pItem->GetSocket() == Socket)
        {
             break;
        }
    }
    return pItem;
}

bool CSocketItemManager::FreeSocketItem(WORD wIndex)
{
    CSocketItem *pItem = m_Storage[wIndex];
    vector<CSocketItem *>::iterator   itera;

    //查找客户项
    itera = find(m_Active.begin(), m_Active.end(), pItem);
    if (itera == m_Active.end())
    {
        CLog::Instance().Print("CSocketItemManager::FreeClient：在活动客户项数组中找不到该用户");
        return false;
    }

    //放入空闲客户项数组
    m_Free.push_back(*itera);
    //在活动客户数组中删除
    m_Active.erase(itera);

    return true;
}

void CSocketItemManager::SetStorageCount(WORD wCount)
{
    m_wStorageCount = wCount;
}

CSocketItem *CSocketItemManager::SearchWithName(const std::string name)
{
    std::vector<CSocketItem *>::iterator iter;
    for (iter = m_Active.begin(); iter != m_Active.end(); iter++)
    {
        string UserName = (*iter)->m_Client.m_strUserName;
        if (name == UserName)
        {
            return *iter;
        }
    }
    return NULL;
}

std::vector<CSocketItem *> CSocketItemManager::GetOnLine(void)
{
    std::vector<CSocketItem *> temp;
    std::vector<CSocketItem *>::iterator iter;
    for (iter = m_Active.begin(); iter != m_Active.end(); iter++)
    {
        if ( (*iter)->m_Client.m_bOnLine == true )
        {
            temp.push_back(*iter);
        }
    }
    return temp;
}

void CSocketItemManager::SetHandleMessageSink(IServerSocketItemSink* pSink)
{
    m_pHandleMessageSink = pSink;
}

IServerSocketItemSink *CSocketItemManager::GetServerSocketItemSink()
{
    return m_pHandleMessageSink;
}
