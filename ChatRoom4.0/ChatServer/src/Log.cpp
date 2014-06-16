#include <iostream>
#include "Log.h"
#include "../../GlobleData.h"

//#define _DAEMON_

#ifdef _DAEMON_
#include <syslog.h>
#endif

using namespace std;

CLog::CLog()
{

}

CLog::~CLog()
{

}


void CLog::Print(const char* log) const
{
#ifdef _DAEMON_
    syslog(LOG_INFO, "%s", log);
#else
    cout<<log<<endl;
#endif // _DAEMON_
}
