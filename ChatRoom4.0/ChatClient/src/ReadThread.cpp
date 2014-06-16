#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <error.h>
#include <string.h>
#include <stdio.h>
#include "ReadThread.h"

CReadThread::CReadThread()
:m_pHandleMessageSink(NULL)
{
    //ctor
}

CReadThread::~CReadThread()
{
    //dtor
}
void CReadThread::SetHandleMessageSink(IHandleMessageSink* pSink)
{
    m_pHandleMessageSink = pSink;
}

bool CReadThread::OnEventThreadRun(void)
{
    ssize_t iRet = -1;
    cout<<"read before"<<endl;
    iRet = read (m_Socket, m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize);
    cout<<"read after"<<endl;

    if (iRet == -1)
    {
        perror("read");
        close(m_Socket);
        return false;
    }
    else if (iRet == 0)
    {
        /* End of file. The remote has closed the
        connection. */
        cout<<"服务端关闭了套接字"<<endl;
        return false;
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
                    printf("收到检测命令\n");
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
        cout<<strError<<endl;
        return false;
    }

    cout<<"readmessage end"<<endl;
    return true;
}

bool CReadThread::OnEventThreadStrat(void)
{
    return true;
}

bool CReadThread::OnEventThreadConclude(void)
{
    return true;
}

void CReadThread::SetSocket(int Socket)
{
    m_Socket = Socket;
}
