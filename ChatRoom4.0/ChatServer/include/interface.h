#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "../../GlobleData.h"

////////类声明//////////
class CSocketItem;
/////////////////////


class IServerSocketItemSink
{
public:
    virtual int OnReadSink(CMD_Command Command, void *pBuffer, WORD wDataSize, int Socket) = 0;
    virtual int OnAcceptSink(int fdEpoll, int socket, struct sockaddr in_addr) = 0;
    virtual int onSendSink(int Socket) = 0;
};

class IDataEngineSink
{
public:
    virtual int OnDataEngineSink(WORD wRequestID, int Socket, void *pData, WORD wDataSize) = 0;
};

class IDataHandleSink
{
public:
    virtual int LogonByAccounts(char *strAccount, char *strPasswd) = 0;
    virtual int RegisterAccounts(char *strAccount, char *strPasswd) = 0;
};

class IPostDataBaseRequestSink
{
public:

    virtual int PostDataBaseRequest(WORD wRequestID, int Socket, void *pData, WORD wDataSize) = 0;
};

class IDataBaseResultSink
{
public:
    virtual int DataBaseResultSink(WORD wRequestID, int Socket, void *pData, WORD wDataSize) = 0;
};

class INetworkSink
{
public:
    virtual bool SendData(int Socket, void *pData, WORD wDataSize, WORD MainCmdID, WORD wSubCmdID) = 0;
};

#endif // INTERFACE_H_INCLUDED
