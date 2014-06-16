#ifndef CCHATROOM_H
#define CCHATROOM_H

#include "SendThread.h"
#include "interface.h"
#include "netinet/in.h"
#include "ReadThread.h"
#include <sys/socket.h>

class CCMDView;
class CHanldeMessage;

class CChatRoom : public ILocalCMDSink
{
public:
    CChatRoom(const char *ip, const char *port);
    virtual ~CChatRoom();
public:
    bool Start();
    bool Stop();

public:
    bool SetHandleMessageSink(IHandleMessageSink *pSink);

public:
    bool StopConnect(void);

protected:
    bool Init(void);
    bool ReadMessage(void);

private:
    char                 m_ServerIp[17];
    char                 m_Port[6];
    int                  m_Socket;
    CSendThread          m_SendThread;
    CReadThread          m_ReadThread;
    BYTE                 m_cbRecvBuf[SOCKET_BUFFER];
    struct sockaddr_in   m_ServerAddr;
    IHandleMessageSink   *m_pHandleMessageSink;
    WORD                 m_wRecvSize;
    CCMDView             *m_pCMDView;
    bool                 m_bRun;
    CHanldeMessage       *m_pHandleMessage;
    IViewSink            *m_pViewSink;
    bool                 m_bIsStop;
};

#endif // CCHATROOM_H
