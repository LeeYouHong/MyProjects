#ifndef CGLOBALPARAMMANAGER_H
#define CGLOBALPARAMMANAGER_H

#include <string>
#include "Singleton.h"

class CGlobalParamManager : public CSingleton<CGlobalParamManager>
{
public:
    CGlobalParamManager();
    virtual ~CGlobalParamManager();

public:
    bool LoadConfig();

public:
    std::string         m_strXMLFilePath;
    std::string         m_strServerPort;
    int                 m_MaxPeople;
    int                 m_SendRequestQueueSize;
    int                 m_DBRequestQueueSize;
};

#endif // CGLOBALPARAMMANAGER_H
