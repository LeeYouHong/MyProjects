#ifndef CNetworkEngine_INCLUDED
#define CNetworkEngine_INCLUDED
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: Epoll.h
Author: 李有红
Version: 0.1
Date:2013/12/16
Description: epoll模型类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/


#include <vector>
#include "../../GlobleData.h"
#include "interface.h"
#include "WorkThread.h"


///////////Clsss Declaration//////////////
class CSocketItem;
class CSendThread;
/////////////////////////////////////////


class CNetworkEngine : public INetworkSink
{
public:
    CNetworkEngine(const char *strListenPort);
    virtual ~CNetworkEngine();

public:
    bool Start(void);
    bool Stop(void);

public:
    virtual bool SendData(int Socket, void *pData, WORD wDataSize, WORD MainCmdID, WORD wSubCmdID);

private:
    //初始化监听
    int InitListen(void);

    //创建一个绑定了本地端口到套接子
    int CreateBindedSocket();

	//创建一个epoll
	int CreateEpoll(int iSize);

private:
    const char                       *m_strListenPort;
    int                              m_fdEpoll;
    CSocketItem                      *m_pListenSocketItem;
    CWorkThread                      m_WorkThread;
    CSendThread                      *m_pSendThread;
};


#endif // CNetworkEngine_INCLUDED
