

/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: SocketItem.h
Author: 李有红
Version: 0.1
Date:2013/12/16
Description: 套接字类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include "SocketItem.h"
#include "SocketItemManager.h"
#include "Log.h"
#include "Client.h"

CSocketItem::CSocketItem(IServerSocketItemSink *pSink)
:m_Socket(-1)
,m_pIServerSocketItemSink(pSink)
,m_wRecvSize(0)
,m_dwRecvPacketCount(0)
{
    //ctor
}

CSocketItem::~CSocketItem()
{
    //dtor
    //CloseSocket();
}

int CSocketItem::Attach(const int socket)
{
    if(m_Socket == -1)
    {
       m_Socket = socket;
    }
    else
    {
        return -1;
    }

    return 0;
}

void CSocketItem::CloseSocket(void)
{
    if(m_Socket != -1)
    {
        close(m_Socket);
    }
}

int CSocketItem::SetNonBlock(void)
{
    int flags = 0;

    flags = fcntl(m_Socket, F_GETFL, 0);
    if(flags == -1)
    {
        CloseSocket();
        CLog::Instance().Print("fcntl error");
        return -1;
    }

    flags |= O_NONBLOCK;
    if(fcntl(m_Socket, F_SETFL, flags) == -1)
    {
        CloseSocket();
        CLog::Instance().Print("fcntl error");
        return -1;
    }

    return 0;
}
int CSocketItem::PrintAddr(void)
{
    int iRet = -1;
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

    iRet = GetRemoteAddr(&m_SocketAddr, hbuf, sbuf);
    if (iRet == -1)
    {
        CLog::Instance().Print("GetRemoteAddr error");
        return -1;
    }

    char log[255];
    snprintf(log, 255,"Accepted connection on descriptor %d ""(host=%s, port=%s)"
             , m_Socket, hbuf, sbuf);
    CLog::Instance().Print(log);

//    printf("Accepted connection on descriptor %d "
//                 "(host=%s, port=%s)\n", m_Socket, hbuf, sbuf);
    return 0;
}

int CSocketItem::GetRemoteAddr(const struct sockaddr* pSocketAddr, char* strHost, char* strPort)
{
    int iRet = -1;
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

    iRet = getnameinfo (pSocketAddr, sizeof(struct sockaddr), hbuf, sizeof(hbuf), sbuf, sizeof(sbuf),
                       NI_NUMERICHOST | NI_NUMERICSERV);
    if (iRet == -1)
    {
        CLog::Instance().Print("getnameinfo");
        return -1;
    }

    strncpy(strHost, hbuf, sizeof(hbuf));
    strncpy(strPort, sbuf, sizeof(sbuf));

    return 0;
}

int CSocketItem::SetAddr(const struct sockaddr SocketAddr)
{
    m_SocketAddr = SocketAddr;
    return 0;
}

int CSocketItem::AddToEpoll(const int fdEpoll, const int flag)
{
    struct epoll_event EpollEvent;

    EpollEvent.data.ptr = this;
    EpollEvent.events = flag;

    if (epoll_ctl(fdEpoll, EPOLL_CTL_ADD, m_Socket, &EpollEvent) == -1)
    {
        CloseSocket();
        CLog::Instance().Print("epoll_ctl error");
        return -1;
    }

    return 0;
}

int CSocketItem::DelFromEpoll(const int fdEpoll, const int flag)
{
    struct epoll_event EpollEvent;

    EpollEvent.data.ptr = this;
    EpollEvent.events = flag;

    if (epoll_ctl(fdEpoll, EPOLL_CTL_DEL, m_Socket, &EpollEvent) == -1)
    {
        CloseSocket();
        CLog::Instance().Print("epoll_ctl error");
        return -1;
    }

    return 0;
}

int CSocketItem::ModifyFromEpoll(const int fdEpoll, const int flag)
{
    struct epoll_event EpollEvent;

    EpollEvent.data.ptr = this;
    EpollEvent.events = flag;

    if (epoll_ctl(fdEpoll, EPOLL_CTL_MOD, m_Socket, &EpollEvent) == -1)
    {
        CloseSocket();
        CLog::Instance().Print("epoll_ctl error");
        return -1;
    }

    return 0;
}

int CSocketItem::ListenClient(void)
{
    if (listen(m_Socket, SOMAXCONN) == -1)
    {
        CLog::Instance().Print("listen");
        return -1;
    }
    return 0;
}

bool CSocketItem::OnReadEvent(const int fdEpoll)
{
    bool bIsdone = 0;
    for(;;)
    {
      ssize_t iRet = -1;

      iRet = read (m_Socket, m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize);
      if (iRet == -1)
      {
          /* If errno == EAGAIN, that means we have read all
             data. So go back to the main loop. */
          if (errno != EAGAIN)
            {
              CLog::Instance().Print("read error");
              bIsdone = true;
            }
          break;
      }
      else if (iRet == 0)
      {
          /* End of file. The remote has closed the
             connection. */
          bIsdone = true;
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
              m_dwRecvPacketCount++;

              //解释数据
              WORD wDataSize = wRealySize - sizeof(CMD_Head);
              void *pDataBuffer = cbBuffer + sizeof(CMD_Head);
              CMD_Command Command = ((CMD_Head *)cbBuffer)->CommandInfo;

              if (Command.wMainCmdID == S_MDM_KN_COMMAND)
              {
                  switch (Command.wSubCmdID)
                  {
                  case S_SUB_KN_DETECT_SOCKET:
                      {
                          CLog::Instance().Print("心跳包");
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
                  m_pIServerSocketItemSink->OnReadSink(Command, pDataBuffer, wDataSize, m_Socket);
              }

              //删除缓存数据
              m_wRecvSize -= wPacketSize;
              memmove(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
          }
      }
      catch(char *strError)
      {
          CLog::Instance().Print(strError);
          return false;
      }
    }

    if (bIsdone)
    {
        char log[255];
        snprintf(log, 255, "Closed connection on descriptor %d", m_Socket);
        CLog::Instance().Print(log);

       /* Closing the descriptor will make epoll remove it
         from the set of descriptors which are monitored. */
       OnCloseEvent(fdEpoll);
    }
    return true;
}

bool CSocketItem::OnAccepteEvent(const int fdEpoll)
{
    for(;;)
    {
      struct sockaddr in_addr;
      socklen_t in_len;
      int ClientSocket;
      in_len = sizeof(in_addr);

      ClientSocket = accept (m_Socket, &in_addr, &in_len);

      if (ClientSocket == -1)
      {
          if ((errno == EAGAIN) ||
              (errno == EWOULDBLOCK))
            {
              /* We have processed all incoming
                 connections. */
              break;
            }
          else
            {
              CLog::Instance().Print("accept error");
              break;
            }
      }
      //m_pIServerSocketItemSink->OnAcceptSink(fdEpoll, ClientSocket, in_addr);
      //接受到客户后的处理
      AcceptHandle(fdEpoll, ClientSocket, in_addr);

    }

    return true;
}

bool CSocketItem::OnSendEvent(void)
{
    return true;
}

bool CSocketItem::OnCloseEvent(const int fdEpoll)
{
    CLog::Instance().Print("关闭套接字");

    //从epoll中删除套接字
    DelFromEpoll(fdEpoll, 0);

    //关闭套接字
    CloseSocket();

    //重置套接字
    ReSetData();
    //释放套接字
    CSocketItemManager::Instance().FreeSocketItem(m_wIndex);

    return true;
}

int CSocketItem::SetServerSocketItemSink(IServerSocketItemSink* pServerSocketItemSink)
{
    m_pIServerSocketItemSink = pServerSocketItemSink;
    return 0;
}

bool CSocketItem::SendData(void* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID)
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

bool CSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID)
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

DWORD CSocketItem::SendDataBuffer(int socket, void* pBuffer, WORD wSendSize)
{
    //发送数据
	WORD wSended=0;
	while (wSended<wSendSize)
	{
		int iRet = write(socket,(char *)pBuffer+wSended,wSendSize-wSended);

		if (iRet == -1)
		{
		    char log[255];
		    snprintf(log, 255, "Write error.error code:%d", errno);
		    CLog::Instance().Print(log);
			return false;
		}
		wSended += iRet;
	}
	return true;
}

int CSocketItem::GetSocket() const
{
    return m_Socket;
}

bool CSocketItem::ReSetData()
{
    m_Socket = -1;
    m_wRecvSize = 0;
    bzero(m_cbRecvBuf, sizeof(m_cbRecvBuf));
    m_Client.ResetData();
    return true;
}

bool CSocketItem::UserQuit()
{
    m_Client.Quit();
    return true;
}

bool CSocketItem::AcceptHandle(const int fdEpoll, int Socket, struct sockaddr in_addr)
{
    //分配一个套接字项
    CSocketItem *pSocketItem = CSocketItemManager::Instance().ActiveSocketItem();
    if( pSocketItem == NULL)
    {
        return false;
    }
    //绑定套接字
    pSocketItem->Attach(Socket);
    //设置客户端的地址信息
    pSocketItem->SetAddr(in_addr);

    //不是致命错误，所以不返回
    if(pSocketItem->PrintAddr() == -1)
    {
        CLog::Instance().Print("PrintAddr error");
    }
    //设置套接字为非阻塞
    if (pSocketItem->SetNonBlock() == -1)
    {
        CLog::Instance().Print("SetNonBlock error");
        return false;
    }
    //绑定套接字项消息到Epoll
    if(pSocketItem->AddToEpoll(fdEpoll, EPOLLIN | EPOLLET) == -1)
    {
        CLog::Instance().Print("AddToPoll error");
        return false;
    }
    return true;
}
