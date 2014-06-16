#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include "ChatRoom.h"
#include "HanldeMessage.h"
#include "CMDView.h"

using namespace std;

CChatRoom::CChatRoom(const char* ip, const char* port)
:m_wRecvSize(0)
,m_pCMDView(NULL)
,m_bRun(false)
,m_bIsStop(false)
{
    strncpy(m_ServerIp, ip, sizeof(m_ServerIp));
    strncpy(m_Port, port, sizeof(m_Port));
}
CChatRoom::~CChatRoom()
{
    //dtor
    if( !m_bIsStop )
    {
        Stop();
    }
}

bool CChatRoom::Init(void)
{
    if ( (m_Socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        m_pViewSink->Print("socket error");
        return false;
    }
    bzero(&m_ServerAddr, sizeof(m_ServerAddr));
    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_port = htons(atoi(m_Port));
    if (inet_pton(AF_INET, m_ServerIp, &m_ServerAddr.sin_addr) < 0)
    {
        m_pViewSink->Print("inet_pton");
        return false;
    }
    return true;
}

bool CChatRoom::Start()
{
    //初始化CMD视图
    m_pCMDView = new CCMDView;

    this->m_pViewSink = m_pCMDView;

    //创建处理消息引擎
    m_pHandleMessage = new CHanldeMessage;
    m_pHandleMessage->SetViewSink(m_pCMDView);
    SetHandleMessageSink(m_pHandleMessage);

    if( !Init() )
    {
         m_pViewSink->Print("初始化错误");
    }

    if ( connect(m_Socket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr)) < 0)
    {
        m_pViewSink->Print(strerror(errno));
        return false;
    }

    m_pCMDView->Print("连接服务器成功！");

    m_pCMDView->Print("<使用说明>");
    m_pCMDView->Print("logon:名字:密码");
    m_pCMDView->Print("re:名字:密码");
    m_pCMDView->Print("to:名字:消息");
    m_pCMDView->Print("all:消息");

    //初始化发送线程
    m_SendThread.SetSocket(m_Socket);
    m_SendThread.SetViewSink(m_pCMDView);

    //设置CMD视图的发送消息接口
    m_pCMDView->SetCommandEventSink(&m_SendThread);

    //启动发送线程
    m_SendThread.StartThread();

    m_bRun = true;

    //读取消息
    ReadMessage();

    return true;
}

bool CChatRoom::Stop()
{
    int iRet = shutdown(m_Socket, 2);
    if (iRet < 0)
    {
        m_pViewSink->Print("shutdown 错误");
    }

    //关闭发送线程
    m_SendThread.ConcludeThread(0);

    //主线程置位
    m_bRun = false;

    //释放指针
    SafeDelete(m_pCMDView);
    SafeDelete(m_pHandleMessage);

    m_bIsStop = true;

    return true;
}

bool CChatRoom::SetHandleMessageSink(IHandleMessageSink* pSink)
{
    m_pHandleMessageSink = pSink;
    return true;
}

bool CChatRoom::ReadMessage(void)
{
    while( m_bRun )
    {
      ssize_t iRet = -1;

      iRet = read (m_Socket, m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize);
      if (iRet == -1)
      {
          m_pViewSink->Print("read");
          close(m_Socket);
          break;
      }
      else if (iRet == 0)
      {
          m_pViewSink->Print("服务端关闭了套接字");
          break;
      }

     //接收完成
      m_wRecvSize += iRet;
      BYTE cbBuffer[SOCKET_BUFFER];
      CMD_Head *pHead = (CMD_Head *)m_cbRecvBuf;

      try
      {
          while(m_wRecvSize >= sizeof(CMD_Head))
          {
              //校验数据
              WORD wPacketSize = pHead->CmdInfo.wPacketSize;
              if (wPacketSize > SOCKET_BUFFER) throw "数据包非法";
              if (wPacketSize < sizeof(CMD_Head)) throw "数据包非法";
              if (pHead->CmdInfo.cbVersion != SOCKET_VER) throw "数据包版本错误";
              if (m_wRecvSize < wPacketSize) break;

              //提取数据
              memcpy(cbBuffer, m_cbRecvBuf, wPacketSize);
              WORD wRealySize = wPacketSize;//CrevasseBuffer(cbBuffer, wPacketSize);

              //解释数据
              WORD wDataSize = wRealySize - sizeof(CMD_Head);
              void *pDataBuffer = cbBuffer + sizeof(CMD_Head);
              CMD_Command Command = ((CMD_Head *)cbBuffer)->CommandInfo;

              if (Command.wMainCmdID == C_MDM_KN_COMMAND_RESULT)
              {
                  switch (Command.wSubCmdID)
                  {
                  case C_SUB_KN_DETECT_SOCKET:
                      {
                          m_pViewSink->Print("收到检测命令");
                          break;
                      }
                  default:
                    {
                        throw "非法命令";
                    }
                  }
              }
              else
              {
                  m_pHandleMessageSink->HandleMessage(Command, pDataBuffer, wDataSize);
              }

              //删除缓存数据
              m_wRecvSize -= wPacketSize;
              memmove(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
          }
      }
      catch(char *strError)
      {
          m_pViewSink->Print(strError);
          return false;
      }
    }
    return true;
}

bool CChatRoom::StopConnect(void)
{
    if ( !m_bIsStop )
    {
        Stop();
    }
    return true;
}
