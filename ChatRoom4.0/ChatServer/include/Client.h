#pragma once
#include <string>
#include "../../GlobleData.h"

class CClient
{
public:
    CClient();
    ~CClient();

public:
    bool Quit();
    void ResetData();

public:
    std::string              m_strUserName;
    bool                     m_bOnLine;
};
