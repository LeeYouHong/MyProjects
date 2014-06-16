#pragma once
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: ChatRoom.h
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 聊听室
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

////class declaration//////////
class CDataBaseEngine;
class CHandleMessage;
class CNetworkEngine;
class CMySqlDataHandle;
///////////////////////////////

class CChatRoom
{
public:
    CChatRoom();
    ~CChatRoom();

public:
    bool Start(void);
    bool Stop(void);

private:
    CDataBaseEngine    *m_pDataBaseEngine;
    CHandleMessage     *m_pHandleMessage;
    CNetworkEngine     *m_pNetwork;
    CMySqlDataHandle   *m_pMysqlHandle;
};
