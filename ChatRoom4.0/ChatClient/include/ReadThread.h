#ifndef CREADTHREAD_H
#define CREADTHREAD_H

#include "../../GlobleData.h"
#include "interface.h"
#include "ServiceThread.h"

class CReadThread : public CServiceThread
{
public:
    CReadThread();
    virtual ~CReadThread();

public:
    virtual bool OnEventThreadRun(void);
    virtual bool OnEventThreadStrat(void);
    virtual bool OnEventThreadConclude(void);

    //设置接口
public:
    void SetHandleMessageSink(IHandleMessageSink *pSink);

public:
    void SetSocket(int Socket);

private:
    WORD                 m_wRecvSize;
    BYTE                 m_cbRecvBuf[SOCKET_BUFFER];
    IHandleMessageSink   *m_pHandleMessageSink;
    int                  m_Socket;
};

#endif // CREADTHREAD_H
