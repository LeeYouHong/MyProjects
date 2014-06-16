#ifndef CSocketItemManager_H
#define CSocketItemManager_H

#include <vector>
#include <string>
#include "Singleton.h"
#include "interface.h"

class CSocketItemManager : public CSingleton<CSocketItemManager>
{
    public:
        CSocketItemManager();
        virtual ~CSocketItemManager();
    public:
        CSocketItem *ActiveSocketItem();
        CSocketItem *GetSocketItem(const int Socket);
        bool FreeSocketItem(WORD wIndex);
        void SetStorageCount(WORD wCount);

        CSocketItem *SearchWithName(const std::string name);
        std::vector<CSocketItem *> GetOnLine(void);

    public:
        void SetHandleMessageSink(IServerSocketItemSink *pSink);
        IServerSocketItemSink *GetServerSocketItemSink(void);

    private:
        std::vector<CSocketItem *>      m_Storage;
        std::vector<CSocketItem *>      m_Free;
        std::vector<CSocketItem *>      m_Active;

        WORD                          m_wCurrentCount;
        WORD                          m_wStorageCount;

        IServerSocketItemSink       *m_pHandleMessageSink;

};

#endif // CSocketItemManager_H
