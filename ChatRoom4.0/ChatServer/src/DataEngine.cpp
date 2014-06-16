/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: DataEngine.cpp
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 数据库引擎
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <iostream>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include "DataEngine.h"
#include "Log.h"
#include "GlobalParamManager.h"

CDataBaseEngine::CDataBaseEngine()
{

}

CDataBaseEngine::~CDataBaseEngine()
{
    SafeDelete(m_pDataBaseThread);
}

bool CDataBaseEngine::Start()
{
    //初始化数据库操作线程
    int SizeTemp = CGlobalParamManager::Instance().m_DBRequestQueueSize;
    m_pDataBaseThread = new CDataBaseThread(SizeTemp);
    m_pDataBaseThread->SetDataEngineSink(this);

    //启动数据库操作线程
    if (m_pDataBaseThread->StartThread() == false)
    {
        CLog::Instance().Print("数据库线程启动失败");
        return false;
    }

    return true;
}

bool CDataBaseEngine::Stop()
{
    return true;
}

int CDataBaseEngine::SetDataHanleSink(IDataHandleSink* pDataHandleSink)
{
    m_pDataHandleSink = pDataHandleSink;
    return 0;
}

int CDataBaseEngine::OnDataEngineSink(WORD wRequestID, int Socket, void* pData, WORD wDataSize)
{
    switch(wRequestID)
    {
    case S_DBR_LOGON_BY_ACCOUNTS:
         {
            //校验数据
            if (wDataSize != sizeof(DBR_LogonByAccounts)) return -1;

            DBR_LogonByAccounts *pLogonByAccount = (DBR_LogonByAccounts *)pData;

            //登录数据查询
            WORD iRet =  m_pDataHandleSink->LogonByAccounts(pLogonByAccount->szAccounts, pLogonByAccount->szPassWord);

            BYTE cbBuffer[SOCKET_BUFFER];
            DBR_GP_Result_LogonByAccounts *result = (DBR_GP_Result_LogonByAccounts *)cbBuffer;
            memcpy(result->szAccounts, pLogonByAccount->szAccounts, sizeof(result->szAccounts));
            memcpy(result->szPassWord, pLogonByAccount->szPassWord, sizeof(result->szPassWord));

            WORD wSize = sizeof(DBR_GP_Result_LogonByAccounts);
            if (iRet == S_DB_RESULT_LOGON_NO_EXISTS)
            {
                m_pDataBaseResultSink->DataBaseResultSink(S_DB_RESULT_LOGON_NO_EXISTS, Socket
                                                          ,cbBuffer,wSize);
            }
            else if (iRet == S_DB_RESULT_LOGON_SUCCESS)
            {
                m_pDataBaseResultSink->DataBaseResultSink(S_DB_RESULT_LOGON_SUCCESS, Socket
                                                         ,cbBuffer,wSize);
            }
            else if (iRet == S_DB_RESULT_LOGON_PASSWD_ERROR)
            {
                m_pDataBaseResultSink->DataBaseResultSink(S_DB_RESULT_LOGON_PASSWD_ERROR, Socket
                                                          ,cbBuffer, wSize);
            }

         }
    case S_DBR_REGISTER_ACCOUNTS:
        {
            //校验数据
            if (wDataSize != sizeof(DBR_RegisterAccounts)) return -1;

            DBR_RegisterAccounts *pRegisterByAcounts = (DBR_RegisterAccounts *)pData;

            //数据库中注册
            WORD iRet = m_pDataHandleSink->RegisterAccounts(pRegisterByAcounts->szAccounts, pRegisterByAcounts->szPassWord);

            //发送结果
            BYTE cbBuffer[SOCKET_BUFFER];
            DBR_GP_Result_Register_Success *result = (DBR_GP_Result_Register_Success *)cbBuffer;
            memcpy(result->szAccounts, pRegisterByAcounts->szAccounts, sizeof(result->szAccounts));
            memcpy(result->szPassWord, pRegisterByAcounts->szPassWord, sizeof(result->szPassWord));

            WORD wSize = sizeof(DBR_GP_Result_Register_Success);

            if (iRet == S_DB_RESULT_REGISTER_USER_EXISTS)
            {
                m_pDataBaseResultSink->DataBaseResultSink(S_DB_RESULT_REGISTER_USER_EXISTS, Socket
                                                          , cbBuffer, wSize);
            }
            else if (iRet == S_DB_RESULT_REGISTER_SUCCESS)
            {
                m_pDataBaseResultSink->DataBaseResultSink(S_DB_RESULT_REGISTER_SUCCESS, Socket
                                                         ,cbBuffer, wSize);
            }
        }
    default:
        {
            CLog::Instance().Print("CDataBaseEngine::OnDataEngineSink:错误到case!");
        }
    }

    return S_DB_RESULT_ERROR;
}

int CDataBaseEngine::PostDataBaseRequest(WORD wRequestID, int Socket, void* pData, WORD wDataSize)
{
    tagDBRequest *pRequest = new tagDBRequest;
    //开辟新到内存为请求指令
    BYTE *pNewBuffer = new BYTE[wDataSize];
    memcpy(pNewBuffer, pData, wDataSize);

    pRequest->pData = pNewBuffer;
    pRequest->wDataSize = wDataSize;
    pRequest->wRequestID = wRequestID;
    pRequest->Socket = Socket;

    m_pDataBaseThread->Append(pRequest);

    return 0;
}

int CDataBaseEngine::SetDataBaseResultSink(IDataBaseResultSink* pSink)
{
    m_pDataBaseResultSink = pSink;
    return 0;
}
