#ifndef CCLIENTMANAGER_H
#define CCLIENTMANAGER_H

#include <vector>
#include <map>
#include <string>
#include "Singleton.h"
#include "Client.h"
#include "interface.h"

class CClient;

class CClientManager : public CSingleton<CClientManager>
{
public:
    CClientManager();
    virtual ~CClientManager();

public:
    CClient *ActiveClient();
    CClient *GetClient(int Socket);
    bool FreeClient(WORD wClientIndex);

    void SetStorageCount(WORD wCount);

    CClient *SearchWithName(const std::string name);
    std::vector<CClient *> GetOnLine(void);

public:
    void SetHandleMessageSink(IServerSocketItemSink *pSink);
    IServerSocketItemSink *GetHandleMessageSink(void);

private:
    std::vector<CClient *>      m_StorageClient;
    std::vector<CClient *>      m_FreeClient;
    std::vector<CClient *>      m_ActiveClient;
    WORD                          m_wStorageCount;
    WORD                          m_wCurrentCount;
    IServerSocketItemSink       *m_pHandleMessageSink;
//    std::map<int, CClient *>      m_StorageClient;
//    std::map<int, CClient *>      m_FreeClient;
//    std::map<int, CClient *>      m_ActiveClient;
};

#endif // CCLIENTMANAGER_H
