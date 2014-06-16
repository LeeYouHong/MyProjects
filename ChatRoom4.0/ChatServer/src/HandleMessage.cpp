/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: HandleMessage.cpp
Author: 李有红
Version: 0.1
Date:2013/12/1
Description: 处理消息逻辑
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include <vector>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include "../../GlobleData.h"
#include "HandleMessage.h"
#include "SocketItemManager.h"
#include "SocketItem.h"
#include "Log.h"

using namespace std;

CHandleMessage::CHandleMessage()
{

}

CHandleMessage::~CHandleMessage()
{

}

int CHandleMessage::OnReadSink(CMD_Command Command, void *pBuffer, WORD wDataSize, int Socket)
{
    switch (Command.wMainCmdID)
    {
    case S_MDM_LOGON:
        {
           OnSocketMainLogon(Command.wSubCmdID, pBuffer, wDataSize, Socket);
        }
        return 0;
    case S_MDM_CHAT:
        {
           OnSocketChat(Command.wSubCmdID, pBuffer, wDataSize, Socket);
        }
        return 0;
    }
    return -1;
}

int CHandleMessage::OnAcceptSink(int fdEpoll, int socket, struct sockaddr in_addr)
{
    CSocketItem *pSocketItem = CSocketItemManager::Instance().ActiveSocketItem();
    if( pSocketItem == NULL)
    {
        return -1;
    }
    pSocketItem->Attach(socket);
    pSocketItem->SetAddr(in_addr);

    //不是致命错误，所以不返回
    if(pSocketItem->PrintAddr() == -1)
    {
        CLog::Instance().Print("PrintAddr error");
    }

    if (pSocketItem->SetNonBlock() == -1)
    {
        CLog::Instance().Print("SetNonBlock error");
        return -1;
    }

    if(pSocketItem->AddToEpoll(fdEpoll, EPOLLIN | EPOLLET) == -1)
    {
        CLog::Instance().Print("AddToPoll error");
        return -1;
    }
    return 0;
}

int CHandleMessage::onSendSink(int Socket)
{
    return 0;
}

bool CHandleMessage::OnSocketMainLogon(WORD wSubCmdID, void* pData, WORD wDataSize, int Socket)
{
    switch(wSubCmdID)
    {
        case S_SUB_LOGON_ACCOUNTS:
        {
            CLog::Instance().Print("登录：用账户登录");
            if (wDataSize < sizeof(Packet_Logon_LogonByAccounts)) return false;

            //处理消息
            Packet_Logon_LogonByAccounts *pLogonByAccount = (Packet_Logon_LogonByAccounts *)pData;

            //判断是否在线
            CSocketItem *pSocketItem = CSocketItemManager::Instance().SearchWithName(pLogonByAccount->szAccounts);
            if (pSocketItem != NULL && pSocketItem->m_Client.m_bOnLine)
            {
                 CSocketItem *pTemp = CSocketItemManager::Instance().GetSocketItem(Socket);
                 pTemp->SendData(NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LOGON_ONLINE);

                 return true;
            }
            pLogonByAccount->szAccounts[CountArray(pLogonByAccount->szAccounts) - 1] = 0;
            pLogonByAccount->szPassWord[CountArray(pLogonByAccount->szAccounts) - 1] = 0;

            //构造数据
            DBR_LogonByAccounts DBLogonByAccount;
            bzero(&DBLogonByAccount, sizeof(DBLogonByAccount));

            strncpy(DBLogonByAccount.szAccounts, pLogonByAccount->szAccounts, sizeof(DBLogonByAccount.szAccounts));
            strncpy(DBLogonByAccount.szPassWord, pLogonByAccount->szPassWord, sizeof(DBLogonByAccount.szPassWord));

            //投递任务到数据库中操作
            m_pPostDataDataSink->PostDataBaseRequest(S_DBR_LOGON_BY_ACCOUNTS, Socket, &DBLogonByAccount, sizeof(DBLogonByAccount));
        }
        return true;
        case S_SUB_REGISTER_ACCOUNTS:
        {
            CLog::Instance().Print("登录：注册");
            //处理消息
            Packet_Logon_RegisterAccounts *pRegisterAccounts = (Packet_Logon_RegisterAccounts *)pData;

            //判断是否在线
            CSocketItem *pSocketItem = CSocketItemManager::Instance().SearchWithName(pRegisterAccounts->szAccounts);
            if (pSocketItem != NULL && pSocketItem->m_Client.m_bOnLine)
            {
                 CSocketItem *pTemp = CSocketItemManager::Instance().GetSocketItem(Socket);
                 pTemp->SendData(NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LOGON_ONLINE);

                 return true;
            }
            pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts) - 1] = 0;
            pRegisterAccounts->szPassWord[CountArray(pRegisterAccounts->szPassWord) - 1] = 0;

            //构造数据
            DBR_RegisterAccounts DBRegisterAccounts;
            bzero(&DBRegisterAccounts, sizeof(DBRegisterAccounts));
            DBRegisterAccounts.cbGender= pRegisterAccounts->cbGender;
            strncpy(DBRegisterAccounts.szAccounts, pRegisterAccounts->szAccounts, sizeof(DBRegisterAccounts.szAccounts));
            strncpy(DBRegisterAccounts.szPassWord, pRegisterAccounts->szPassWord, sizeof(DBRegisterAccounts.szPassWord));

            //投递任务到数据库引擎中操作
            m_pPostDataDataSink->PostDataBaseRequest(S_DBR_REGISTER_ACCOUNTS, Socket, &DBRegisterAccounts, sizeof(DBRegisterAccounts));
        }
        return true;
        case S_SUB_LAYOUT:
        {
            CLog::Instance().Print("登录：注销");
            Packet_Layout *pLayout = (Packet_Layout *)pData;
            CSocketItem *pSocketItem = CSocketItemManager::Instance().GetSocketItem(Socket);

            if (pSocketItem->m_Client.m_strUserName.compare(pLayout->szAccount) == 0)
            {
                pSocketItem->UserQuit();
                //CSocketItemManager::Instance().FreeSocketItem(pSocketItem->m_wIndex);
                //发送注销成功
                m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LAYOUT_SUCCESS);
            }
        }
        return true;
        default:
        {
            CLog::Instance().Print("CHandleMessage::OnSocketMainLogon::进入到错误case");
        }
    }

    return false;
}

int CHandleMessage::SetPostDataBaseRequestSink(IPostDataBaseRequestSink* pPostDataBaseRequestSink)
{
    m_pPostDataDataSink = pPostDataBaseRequestSink;

    return 0;
}

int CHandleMessage::DataBaseResultSink(WORD wRequestID, int Socket, void* pData, WORD wDataSize)
{
    CSocketItem *pSocketItem = CSocketItemManager::Instance().GetSocketItem(Socket);
    switch(wRequestID)
    {
    case S_DB_RESULT_LOGON_SUCCESS:
        {
            //判断是否在线
            if (pSocketItem->m_Client.m_bOnLine)
            {
                //发送登录结果-已经在线
                m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LOGON_ONLINE);

                char log[255];
                snprintf(log, 255, "%s已经在线", pSocketItem->m_Client.m_strUserName.c_str());
                CLog::Instance().Print(log);
            }
            else
            {
                DBR_GP_Result_LogonByAccounts *result = (DBR_GP_Result_LogonByAccounts *)pData;

                //登录成功标记
                pSocketItem->m_Client.m_strUserName = result->szAccounts;
                pSocketItem->m_Client.m_bOnLine = true;

                //发送登录成功结果
                m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LOGON_SUCCESS);


                char log[255];
                snprintf(log, 255, "%s登录成功", pSocketItem->m_Client.m_strUserName.c_str());
                CLog::Instance().Print(log);
            }

        }
        return 0;
    case S_DB_RESULT_LOGON_NO_EXISTS:
        {
            m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LOGON_NO_USER);
        }
        return 0;
    case S_DB_RESULT_REGISTER_USER_EXISTS:
        {
            m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_REGISTER_USER_EXISTE);
        }
        return 0;
    case S_DB_RESULT_LOGON_PASSWD_ERROR:
        {
             m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_LOGON_PASSWD_ERROR);
        }
        return 0;
    case S_DB_RESULT_REGISTER_SUCCESS:
        {
            pSocketItem->m_Client.m_bOnLine = true;
            DBR_GP_Result_Register_Success *result = (DBR_GP_Result_Register_Success *)pData;
            pSocketItem->m_Client.m_strUserName = result->szAccounts;
            m_pNetworkSink->SendData(Socket, NULL, 0, C_MDM_LOGON_RESULT, C_SUB_LOGON_RESULT_REGISTER_SUCCESS);

            char log[255];
            snprintf(log, 255, "%s注册成功", pSocketItem->m_Client.m_strUserName.c_str());
            CLog::Instance().Print(log);
        }
        return 0;
    default:
        {
            CLog::Instance().Print("CHandleMessage::DataBaseResultSink:错误到case");
        }
    }
    return -1;
}

bool CHandleMessage::OnSocketChat(WORD wSubCmdID, void* pData, WORD wDataSize, int Socket)
{
    switch(wSubCmdID)
    {
    case S_SUB_CHAT_SINGLE:
        {
            //解析数据
            Packet_Chat_ChatBySingle *pRecv = (Packet_Chat_ChatBySingle *)pData;

            char log[255];
            snprintf(log, 255, "%s To %s",pRecv->szSender, pRecv->szRecver);
            CLog::Instance().Print(log);

            //单聊处理
            SingleChat(pData, wDataSize, Socket);
        }
        return true;
    case S_SUB_CHAT_ALL:
        {
            AllChat(pData, wDataSize, Socket);
        }
        return true;
    default:
        {
            CLog::Instance().Print("CHandleMessage::OnSocketChat:错误的case");
        }
    }
    return false;
}


bool CHandleMessage::SingleChat(void* pData, WORD wDataSize, int Socket)
{
    //解析数据
    Packet_Chat_ChatBySingle *pRecv = (Packet_Chat_ChatBySingle *)pData;

    //查找接受消息玩家
    CSocketItem *pRecvSocketItem = CSocketItemManager::Instance().SearchWithName(pRecv->szRecver);
    if (pRecvSocketItem == NULL)
    {
        BYTE cbBuffer[SOCKET_BUFFER];
        Packet_Result_Chat *pResult = (Packet_Result_Chat *)cbBuffer;
        pResult->wResultCode = C_SUB_CHAT_RESULT_CHATBYSINGLE_NO_ONLINE;
        WORD wSize = sizeof(Packet_Result_Chat);

        //给主动发送消息方发送结果
        m_pNetworkSink->SendData(Socket, cbBuffer, wSize, C_MDM_CHAT_RESULT, C_SUB_CHAT_RESULT);

    }
    else
    {
        m_pNetworkSink->SendData(pRecvSocketItem->GetSocket(), pData, wDataSize, C_MDM_CHAT_RESULT, C_SUB_CHAT_SINGLE_RESULT);
    }

    return true;
}

bool CHandleMessage::AllChat(void* pData, WORD wDataSize, int Socket)
{
    //获取在线人数，然后群发
    std::vector<CSocketItem *> temp;
    temp = CSocketItemManager::Instance().GetOnLine();
    std::vector<CSocketItem *>::iterator iter;
    int TempSocket;
    for(iter = temp.begin(); iter != temp.end(); iter++)
    {
        TempSocket = (*iter)->GetSocket();
        m_pNetworkSink->SendData(TempSocket, pData, wDataSize, C_MDM_CHAT_RESULT, C_SUB_CHAT_ALL_RESULT);
    }

    return true;
}


int CHandleMessage::SetNetworkSink(INetworkSink* pSink)
{
    m_pNetworkSink = pSink;
    return 0;
}
