#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: InputThread.h
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

#include <string>
#include <ServiceThread.h>
#include "../../GlobleData.h"
#include "interface.h"

class IViewSink;

class CSendThread : public CServiceThread, public ICommandEventSink
{
public:
    CSendThread();
    virtual ~CSendThread();

public:
    virtual bool OnEventThreadRun(void);
    virtual bool OnEventThreadStrat(void);
    virtual bool OnEventThreadConclude(void);

protected:
    bool SendData( WORD MainCmdID, WORD wSubCmdID, void *pData, WORD wDataSize);
    bool SendData(WORD MainCmdID, WORD wSubCmdID);
    DWORD SendDataBuffer(int socket, void * pBuffer, WORD wSendSize);

protected:
    bool SendLogonMessage(string name, string passwd);
    bool SendRegisterMessage(string name, string passwd);
    bool SendChatToSingleMessage(string recver, string message);
    bool SendChatToAllMessage(string message);
    bool SendLayoutMessage();

public:
    virtual bool OnLogonEvent(string name, string passwd);
    virtual bool OnRegisterEvent(string name, string passwd);
    virtual bool OnChatToSingle(string recver, string message);
    virtual bool OnChatToAll(string message);
    virtual bool OnLayout();

public:
    bool SetSocket(int socket);
    bool SetViewSink(IViewSink *pSink);
private:
    int            m_Socket;
//    char       m_SendBuf[SOCKET_BUFFER];
    std::string    m_SendBuf;
    IViewSink      *m_pViewSink;        //视图接口
};

#endif // INPUTTHREAD_H
