#include <exception>
#include "Locker.h"

CLocker::CLocker()
{
    //ctor
    if (pthread_mutex_init(&m_Mutex, NULL) != 0)
    {
        throw std::exception();
    }
}

CLocker::~CLocker()
{
    //dtor
    pthread_mutex_destroy(&m_Mutex);
}

bool CLocker::Lock()
{
    return pthread_mutex_lock(&m_Mutex) == 0;
}

bool CLocker::Unlock()
{
    return pthread_mutex_unlock(&m_Mutex) == 0;
}

CCond::CCond()
{
    if (pthread_mutex_init(&m_Mutex, NULL) != 0)
    {
        throw std::exception();
    }

    if (pthread_cond_init(&m_Cond, NULL) != 0)
    {
        pthread_mutex_destroy(&m_Mutex);
        throw std::exception();
    }
}

CCond::~CCond()
{
    pthread_mutex_destroy(&m_Mutex);
    pthread_cond_destroy(&m_Cond);
}

bool CCond::Wait()
{
    int iRet = 0;
    pthread_mutex_lock(&m_Mutex);
    iRet = pthread_cond_wait(&m_Cond, &m_Mutex);
    pthread_mutex_unlock(&m_Mutex);

    return iRet == 0;
}

bool CCond::Signal()
{
    return pthread_cond_signal(&m_Cond) == 0;
}

CSem::CSem()
{
    if (sem_init(&m_Sem, 0, 0) != 0)
    {
        throw std::exception();
    }
}

CSem::~CSem()
{
    sem_destroy(&m_Sem);
}

bool CSem::Wait()
{
    return sem_wait(&m_Sem) == 0;
}

bool CSem::Post()
{
    return sem_post(&m_Sem) == 0;
}

