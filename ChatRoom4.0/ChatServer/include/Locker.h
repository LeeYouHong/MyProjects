#pragma once
#include <pthread.h>
#include <semaphore.h>

/**
@brief:封装互斥锁
*/

class CLocker
{
public:
    CLocker();
    ~CLocker();

    bool Lock();
    bool Unlock();

private:
    pthread_mutex_t m_Mutex;
};

class CCond
{
public:
    CCond();
    ~CCond();

public:
    bool Wait();
    bool Signal();

private:
    pthread_mutex_t m_Mutex;
    pthread_cond_t  m_Cond;
};

class CSem
{
public:
    CSem();
    ~CSem();

public:
    bool Wait();
    bool Post();

private:
    sem_t m_Sem;
};
