#ifndef DATAENGINE_H
#define DATAENGINE_H
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: DataEngine.h
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 数据库引擎
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include "interface.h"
#include "DataBaseThread.h"

////////Class Declaration//////////////
class CDataBaseThread;
//////////////////////////////////////

class CDataBaseEngine : public IDataEngineSink, public IPostDataBaseRequestSink
{
public:
    CDataBaseEngine();
    virtual ~CDataBaseEngine();

public:
    bool Start();
    bool Stop();

public:
    virtual int OnDataEngineSink(WORD wRequestID, int Socket, void *pData, WORD wDataSize);

    //投递数据库请求
public:
    virtual int PostDataBaseRequest(WORD wRequestID, int Socket, void *pData, WORD wDataSize);

public:
    int SetDataHanleSink(IDataHandleSink *pDataHandleSink);
    int SetDataBaseResultSink(IDataBaseResultSink *psink);

private:
    IDataHandleSink                  *m_pDataHandleSink;
    CDataBaseThread                  *m_pDataBaseThread;

    IDataBaseResultSink              *m_pDataBaseResultSink;
};

#endif // DATAENGINE_H
