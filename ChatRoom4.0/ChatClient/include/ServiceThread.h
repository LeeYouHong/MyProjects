#ifndef SERVICETHREAD_H
#define SERVICETHREAD_H
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: ServiceThread.h
Author: 李有红
Version: 0.1
Date:2013/12/18
Description: 线程类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include "pthread.h"
#include "../../GlobleData.h"

class CServiceThread
{
public:
    CServiceThread();
    virtual ~CServiceThread();

	//接口函数
public:
	//获取状态
	virtual bool IsRuning(void);
	//启动线程
	virtual bool StartThread(void);
	//终止线程
	virtual bool ConcludeThread(DWORD dwMillSeconds);

	//事件函数
protected:
	//运行事件
	virtual bool OnEventThreadRun(void) { return true; }
	//开始事件
	virtual bool OnEventThreadStrat() { return true; }
	//终止事件
	virtual bool OnEventThreadConclude() { return true; }

	//内部函数
private:
	//线程函数
	static void *ThreadFunction(void *pThreadData);

private:
    volatile  bool            m_bRun;

private:
    pthread_t                 m_ThreadID;
};

#endif // SERVICETHREAD_H
