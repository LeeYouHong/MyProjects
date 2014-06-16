#include <iostream>
#include <string>
#include "CMDView.h"
#include "interface.h"

CCMDView::CCMDView()
{
    //ctor
}

CCMDView::~CCMDView()
{
    //dtor
}

void CCMDView::Run(void)
{
    getline(cin, m_szInput);
    string::size_type spitIndex = m_szInput.find_first_of(':');
    string cmd = m_szInput.substr(0, spitIndex);
    string content = m_szInput.substr(spitIndex + 1, m_szInput.size());

    if(cmd.compare("logon") == 0)
    {
        string::size_type spitIndex= content.find_first_of(':');
        string name = content.substr(0, spitIndex);
        string passwd = content.substr(spitIndex + 1, content.size());

        //发送登录消息
        m_pCommandEventSink->OnLogonEvent(name, passwd);
    }
    else if (cmd.compare("to") == 0)
    {
        string::size_type spitIndex= content.find_first_of(':');
        string recver = content.substr(0, spitIndex);
        string message = content.substr(spitIndex + 1, content.size());

        //发送单聊消息
        m_pCommandEventSink->OnChatToSingle(recver, message);
    }
    else if (cmd.compare("re") == 0)
    {
       string::size_type splitIndex = content.find_first_of(':');
       string name = content.substr(0, splitIndex);
       string passwd = content.substr(splitIndex + 1, content.size());

       //发送注册消息
       m_pCommandEventSink->OnRegisterEvent(name, passwd);
    }
    else if (cmd.compare("all") == 0)
    {
       //发送群聊消息
       m_pCommandEventSink->OnChatToAll(content);
    }
    else if (cmd.compare("layout") == 0)
    {
        m_pCommandEventSink->OnLayout();
    }
}

bool CCMDView::SetCommandEventSink(ICommandEventSink* pSink)
{
    m_pCommandEventSink = pSink;
    return true;
}

void CCMDView::Print(string message)
{
    cout<<message<<endl;
}

void CCMDView::Print(const char* message)
{
    cout<<message<<endl;
}

