/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: InputThread.cpp
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 输入线程类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include "string.h"
#include <iostream>
#include "SendThread.h"
#include "GlobalManager.h"
#include "interface.h"

using namespace std;

CSendThread::CSendThread()
{
    //ctor
}

CSendThread::~CSendThread()
{
    //dtor
}

bool CSendThread::OnEventThreadRun(void)
{
    //运行视图
    m_pViewSink->Run();

    return true;
}

bool CSendThread::OnEventThreadStrat(void)
{
    return true;
}

bool CSendThread::OnEventThreadConclude(void)
{
    return true;
}

bool CSendThread::SetSocket(int socket)
{
    m_Socket = socket;
    return true;
}

bool CSendThread::SendData(WORD wMainCmdID, WORD wSubCmdID, void* pData, WORD wDataSize)
{
    //效验状态
	if (m_Socket==-1) return false;

	//效验大小
	if (wDataSize>SOCKET_PACKET) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;

	//填写信息头
	pHead->CmdInfo.cbCheckCode=0;
	pHead->CmdInfo.wPacketSize=sizeof(CMD_Head)+wDataSize;
	pHead->CmdInfo.cbVersion=SOCKET_VER;

	if (wDataSize>0)
	{
		memcpy(pHead+1,pData,wDataSize);
	}

	WORD wSendSize = sizeof(CMD_Head) + wDataSize;
	//发送数据
	return SendDataBuffer(m_Socket, cbDataBuffer,wSendSize);
}

bool CSendThread::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
    //效验状态
	if (m_Socket == -1) return false;


	//构造数据
	BYTE cbDataBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;

	//填写信息头
	pHead->CmdInfo.cbCheckCode=0;
	pHead->CmdInfo.wPacketSize=sizeof(CMD_Head);
	pHead->CmdInfo.cbVersion=SOCKET_VER;

	WORD wSendSize = sizeof(CMD_Head);
	//发送数据
	return SendDataBuffer(m_Socket, cbDataBuffer,wSendSize);
}

DWORD CSendThread::SendDataBuffer(int socket, void* pBuffer, WORD wSendSize)
{
    //发送数据
	WORD wSended=0;
	while (wSended<wSendSize)
	{
		int iRet = write(socket,(char *)pBuffer+wSended,wSendSize-wSended);

		if (iRet == -1)
		{
		    std::cout<<"CSendThread::SendDataBuffer:ErrorCode:"<<errno<<std::endl;
			return false;
		}
		wSended += iRet;
	}
	return true;
}

bool CSendThread::SendLogonMessage(string name, string passwd)
{
    //存储登录名
    strncpy(CGlobalManager::Instance()->m_szTempAccount, name.c_str(),
            sizeof(CGlobalManager::Instance()->m_szTempAccount));
    BYTE cbBuffer[SOCKET_BUFFER];
    Packet_Logon_LogonByAccounts *pSend = (Packet_Logon_LogonByAccounts *)cbBuffer;
    pSend->dwPlazaVersion = 0;
    strncpy(pSend->szAccounts, name.c_str(), sizeof(pSend->szAccounts));
    strncpy(pSend->szPassWord, passwd.c_str(), sizeof(pSend->szPassWord));
    WORD wSize = sizeof(Packet_Logon_LogonByAccounts);

    return SendData(S_MDM_LOGON, S_SUB_LOGON_ACCOUNTS, cbBuffer, wSize);
}

bool CSendThread::SendRegisterMessage(string name, string passwd)
{
    BYTE cbBuffer[SOCKET_BUFFER];
    Packet_Logon_RegisterAccounts *pSend = (Packet_Logon_RegisterAccounts *)cbBuffer;
    strncpy(pSend->szAccounts, name.c_str(), sizeof(pSend->szAccounts));
    strncpy(pSend->szPassWord, passwd.c_str(), sizeof(pSend->szPassWord));
    WORD wSize = sizeof(Packet_Logon_RegisterAccounts);

    return SendData(S_MDM_LOGON, S_SUB_REGISTER_ACCOUNTS, cbBuffer, wSize);
}

bool CSendThread::SendChatToSingleMessage(string recver, string message)
{
    BYTE cbBuffer[SOCKET_BUFFER];
    Packet_Chat_ChatBySingle *pSend = (Packet_Chat_ChatBySingle *)cbBuffer;
    strncpy(pSend->szSender,CGlobalManager::Instance()->m_szTempAccount, sizeof(pSend->szSender));
    strncpy(pSend->szRecver, recver.c_str(), sizeof(pSend->szRecver));
    strncpy(pSend->szMessage, message.c_str(), sizeof(pSend->szMessage));
    WORD wSize = sizeof(Packet_Chat_ChatBySingle);

    return SendData(S_MDM_CHAT, S_SUB_CHAT_SINGLE, cbBuffer, wSize);
}

bool CSendThread::SendChatToAllMessage(string message)
{
    BYTE cbBuffer[SOCKET_BUFFER];
    Packet_Chat_ChatByAll *pSend = (Packet_Chat_ChatByAll *)cbBuffer;
    strncpy(pSend->szSender,CGlobalManager::Instance()->m_szTempAccount, sizeof(pSend->szSender));
    strncpy(pSend->szMessage, message.c_str(), sizeof(pSend->szMessage));
    WORD wSize = sizeof(Packet_Chat_ChatByAll);

    return SendData(S_MDM_CHAT, S_SUB_CHAT_ALL, cbBuffer, wSize);
}

bool CSendThread::SetViewSink(IViewSink* pSink)
{
    m_pViewSink = pSink;
    return true;
}

bool CSendThread::OnLogonEvent(string name, string passwd)
{
    SendLogonMessage(name, passwd);
    return true;
}

bool CSendThread::OnRegisterEvent(string name, string passwd)
{
    if ( CGlobalManager::Instance()->m_bOnline )
    {
        m_pViewSink->Print("在线状态不能注册");
    }
    else
    {
        SendRegisterMessage(name, passwd);
    }
    return true;
}

bool CSendThread::OnChatToSingle(string recver, string message)
{
    SendChatToSingleMessage(recver, message);
    return true;
}

bool CSendThread::OnChatToAll(string message)
{
    SendChatToAllMessage(message);
    return true;
}

bool CSendThread::OnLayout()
{
    SendLayoutMessage();
    return true;
}

bool CSendThread::SendLayoutMessage()
{
    BYTE cbBuffer[SOCKET_BUFFER];
    Packet_Layout *pSend = (Packet_Layout *)cbBuffer;
    strncpy(pSend->szAccount, CGlobalManager::Instance()->m_szAccount, sizeof(pSend->szAccount));
    WORD wSize = sizeof(Packet_Layout);

    SendData(S_MDM_LOGON, S_SUB_LAYOUT, cbBuffer, wSize);

    return true;
}
