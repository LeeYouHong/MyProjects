/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: ChatRoom.cpp
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 聊听室
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/
#include <string>
#include <syslog.h>
#include "NetworkEngine.h"
#include "DataEngine.h"
#include "HandleMessage.h"
#include "SocketItemManager.h"
#include "ChatRoom.h"
#include "Log.h"
#include "GlobalParamManager.h"
#include "MySqlDataHandle.h"

CChatRoom::CChatRoom()
:m_pDataBaseEngine(NULL)
,m_pHandleMessage(NULL)
,m_pNetwork(NULL)
,m_pMysqlHandle(NULL)
{

}

CChatRoom::~CChatRoom()
{
    Stop();
}

bool CChatRoom::Start(void)
{
    CLog::Instance().Print("CChatRoom::Start  Start");

    //Load global data
    CGlobalParamManager::Instance().LoadConfig();

    m_pMysqlHandle = new CMySqlDataHandle;

    //Initial DatabaseEngine
    m_pDataBaseEngine = new CDataBaseEngine;
    m_pDataBaseEngine->SetDataHanleSink(m_pMysqlHandle);

    //Initial handle-message module
    m_pHandleMessage = new CHandleMessage;
    m_pHandleMessage->SetPostDataBaseRequestSink(m_pDataBaseEngine);

    m_pDataBaseEngine->SetDataBaseResultSink(m_pHandleMessage);


    CSocketItemManager::Instance().SetHandleMessageSink(m_pHandleMessage);

    //初始化网络引擎
    std::string PortTemp = CGlobalParamManager::Instance().m_strServerPort;
    CNetworkEngine *m_pNetworkEngine = new CNetworkEngine(PortTemp.c_str());

    //初始化聊天人数上限
    int MaxPeopleTemp = CGlobalParamManager::Instance().m_MaxPeople;
    CSocketItemManager::Instance().SetStorageCount(MaxPeopleTemp);

    //启动网络引擎
    if ( !m_pNetworkEngine->Start() )
    {
        CLog::Instance().Print("网络引擎启动失败");
        return false;
    }

    CLog::Instance().Print("网络引擎启动成功");

    //启动数据库引擎
    if( !m_pDataBaseEngine->Start() )
    {
        CLog::Instance().Print("数据库引擎启动失败");
        return false;
    }

    //设置处理消息引擎中的网络引擎接口
    m_pHandleMessage->SetNetworkSink(m_pNetwork);

    CLog::Instance().Print("数据库引擎启动成功");

    return true;
}

bool CChatRoom::Stop(void)
{
    SafeDelete(m_pNetwork);
    SafeDelete(m_pHandleMessage);
    SafeDelete(m_pDataBaseEngine);

    return true;
}
