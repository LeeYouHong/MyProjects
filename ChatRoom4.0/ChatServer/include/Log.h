#pragma once
#include "Singleton.h"

class CLog : public CSingleton<CLog>
{
public:
    CLog();
    virtual ~CLog();

public:
    void Print(const char *log) const;
};
