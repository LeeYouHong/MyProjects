#ifndef CHANLDEMESSAGE_H
#define CHANLDEMESSAGE_H

#include "interface.h"

class CHanldeMessage : public IHandleMessageSink
{
public:
    CHanldeMessage();
    virtual ~CHanldeMessage();

public:
    virtual bool HandleMessage(CMD_Command command, const void *pData, WORD wDataSize);

protected:
    bool OnSocketMainLogon(WORD wSubCmdID, const void *pData, WORD wDataSize);
    bool OnSocketChat(WORD wSubCmdID, const void *pData, WORD wDataSize);

public:
    void SetViewSink(IViewSink *pSink);
private:
    IViewSink          *m_pViewSink;
};

#endif // CHANLDEMESSAGE_H
