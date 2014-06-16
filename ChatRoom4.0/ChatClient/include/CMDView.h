#ifndef CCMDVIEW_H
#define CCMDVIEW_H

#include <string>
#include "interface.h"


class ISendMessageSink;

class CCMDView : public IViewSink
{
public:
    CCMDView();
    virtual ~CCMDView();
public:
    virtual void Run(void);
    virtual void Print(string message);
    virtual void Print(const char *message);
public:
    bool SetCommandEventSink(ICommandEventSink *pSink);

private:
    ICommandEventSink          *m_pCommandEventSink;
    std::string               m_szInput;
};

#endif // CCMDVIEW_H
