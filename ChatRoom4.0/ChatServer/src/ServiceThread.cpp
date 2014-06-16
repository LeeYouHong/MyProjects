/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: ServiceThread.cpp
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

#include <syslog.h>
#include "ServiceThread.h"
#include "Log.h"

CServiceThread::CServiceThread()
{
    //ctor
    m_bRun = false;
}

CServiceThread::~CServiceThread()
{
    //dtor
    ConcludeThread(-1);
}

bool CServiceThread::IsRuning(void)
{

   return m_bRun;
}

bool CServiceThread::StartThread(void)
{
    //线程等待，用于创建线程时是否初始化完成
    pthread_cond_t  condt = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


    //线程参数赋值
    tagThreadParameter ThreadParamter;
    ThreadParamter.pServerThread = this;
    ThreadParamter.pCondt = &condt;

    //启动线程
    m_bRun = true;
    int iRet = pthread_create(&m_ThreadID, NULL, ThreadFunction, &ThreadParamter);
    if (iRet != 0)
    {
        CLog::Instance().Print("线程创建失败");
        return false;
    }
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&condt, &mutex);
    pthread_mutex_unlock(&mutex);
    CLog::Instance().Print("线程创建成功");

    return true;
}

bool CServiceThread::ConcludeThread(DWORD dwMillSeconds)
{
    if (IsRuning() == true)
    {
        m_bRun = false;

        if (pthread_join(m_ThreadID, NULL) != 0)
        {
            return false;
        }
    }

    return true;
}

void *CServiceThread::ThreadFunction(void *pThreadData)
{
    tagThreadParameter *ThreadParameter = (tagThreadParameter *)pThreadData;
    pthread_cond_t  *condt = ThreadParameter->pCondt;
    CServiceThread *pServiceThread = ThreadParameter->pServerThread;
//    CServiceThread *pServiceThread = (CServiceThread *)ThreadParameter->pServerThread;

    //出发等待信号
    pthread_cond_signal(condt);

    //执行线程开始前的操作
    pServiceThread->OnEventThreadStrat();

    //执行线程体
    while ( pServiceThread->m_bRun )
    {
        if (pServiceThread->OnEventThreadRun() == false)
        {
            break;
        }
    }

    //执行线程结束前的操作
    pServiceThread->OnEventThreadConclude();

    pthread_exit( (void *)0 );

    return NULL;
}
