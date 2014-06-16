#include <string.h>
#include "Client.h"
#include "SocketItem.h"

CClient::CClient()
:m_bOnLine(false)
{

}

CClient::~CClient()
{

}

bool CClient::Quit()
{
    //在线判断
    if ( !m_bOnLine )
    {
        return false;
    }

    //Reset data
    ResetData();

    return true;
}

void CClient::ResetData()
{
    m_bOnLine = false;
    m_strUserName.clear();
}
