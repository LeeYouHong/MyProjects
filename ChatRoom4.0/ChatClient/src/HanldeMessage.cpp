#include <iostream>
#include "HanldeMessage.h"
#include "interface.h"
#include "string.h"
#include "GlobalManager.h"

using namespace std;

CHanldeMessage::CHanldeMessage()
{
    //ctor
}

CHanldeMessage::~CHanldeMessage()
{
    //dtor
}

bool CHanldeMessage::HandleMessage(CMD_Command Command, const void* pData, WORD wDataSize)
{
    switch (Command.wMainCmdID)
    {
    case C_MDM_LOGON_RESULT:
        {
           OnSocketMainLogon(Command.wSubCmdID, pData, wDataSize);
        }
        return true;
    case C_MDM_CHAT_RESULT:
        {
           OnSocketChat(Command.wSubCmdID, pData, wDataSize);
        }
        return true;
    }
    return false;
}

bool CHanldeMessage::OnSocketMainLogon(WORD wSubCmdID, const void* pData, WORD wDataSize)
{
    switch(wSubCmdID)
    {
    case C_SUB_LOGON_RESULT_LOGON_SUCCESS:
        {
            m_pViewSink->Print("登录成功");
            CGlobalManager::Instance()->m_bOnline = true;
            strncpy(CGlobalManager::Instance()->m_szAccount,
                    CGlobalManager::Instance()->m_szTempAccount,
                    sizeof(CGlobalManager::Instance()->m_szAccount));
        }
        return true;
    case C_SUB_LOGON_RESULT_LOGON_NO_USER:
        {
            m_pViewSink->Print("用户名不存在");
        }
        return true;
    case C_SUB_LOGON_RESULT_LOGON_PASSWD_ERROR:
        {
           m_pViewSink->Print("密码错误");
        }
        return true;
    case C_SUB_LOGON_RESULT_REGISTER_USER_EXISTE:
        {
            m_pViewSink->Print("注册用户已经存在");
        }
        return true;
    case C_SUB_LOGON_RESULT_REGISTER_SUCCESS:
        {
            m_pViewSink->Print("注册成功");
            CGlobalManager::Instance()->m_bOnline = true;
        }
        return true;
    case C_SUB_LOGON_RESULT_LOGON_ONLINE:
        {
            m_pViewSink->Print("本客户端已经有一个用户已经在线");
        }
        return true;
    case C_SUB_LOGON_RESULT_LAYOUT_ERROR:
        {
            m_pViewSink->Print("注销失败");
        }
        return true;
    case C_SUB_LOGON_RESULT_LAYOUT_SUCCESS:
        {
            m_pViewSink->Print("注销成功");
        }
        return true;
    default:
        {
            m_pViewSink->Print("CHanldeMessage::OnSocketMainLogon:错误到case");
        }
    }
    return false;
}

bool CHanldeMessage::OnSocketChat(WORD wSubCmdID, const void* pData, WORD wDataSize)
{
    switch(wSubCmdID)
    {
    case C_SUB_CHAT_RESULT:
        {
            Packet_Result_Chat *pRecv = (Packet_Result_Chat *)pData;
            if (pRecv->wResultCode == C_SUB_CHAT_RESULT_CHATBYSINGLE_NO_ONLINE)
            {
                m_pViewSink->Print("不存在该用户，或该用已经掉线");
            }
        }
        return true;
    case C_SUB_CHAT_ALL_RESULT:
        {
            Packet_Chat_ChatByAll *pRecv = (Packet_Chat_ChatByAll *)pData;

            string str;
            str.append(pRecv->szSender);
            str.append(":");
            str.append(pRecv->szMessage);
            m_pViewSink->Print(str);
        }
        return true;
    case C_SUB_CHAT_SINGLE_RESULT:
        {
           Packet_Chat_ChatBySingle *pRecv = (Packet_Chat_ChatBySingle *)pData;
            string str;
            str.append(pRecv->szSender);
            str.append(":");
            str.append(pRecv->szMessage);
            m_pViewSink->Print(str);
        }
        return true;
    default:
        {
            m_pViewSink->Print("CHanldeMessage::OnSocketChat:错误到case");
        }
    }
    return false;
}

void CHanldeMessage::SetViewSink(IViewSink* pSink)
{
    m_pViewSink = pSink;
}
