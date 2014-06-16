#include "tinyxml.h"
#include "Log.h"
#include "GlobalParamManager.h"

CGlobalParamManager::CGlobalParamManager()
{

}

CGlobalParamManager::~CGlobalParamManager()
{

}

bool CGlobalParamManager::LoadConfig()
{
    TiXmlDocument Document("./config.xml");

    if ( !Document.LoadFile() )
    {
        char log[255];
        snprintf(log, 255, "加载%s文件错误", "./config.xml");
        CLog::Instance().Print(log);
    }

    TiXmlElement *root = Document.RootElement();

    m_strXMLFilePath = root->Attribute("XmlFilePath");
    m_strServerPort = root->Attribute("ServerPort");

    root->Attribute("MaxPeople", &m_MaxPeople);

    root->Attribute("SendRequestQueue", &m_SendRequestQueueSize);

    root->Attribute("DBRequestQueue", &m_DBRequestQueueSize);

    Document.SaveFile();

    return true;
}
