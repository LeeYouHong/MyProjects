#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include "ChatRoom.h"
#include "../../GlobleData.h"

using namespace std;

#define MAXFD 64

//#define _DAEMON_
int daemon_init(void);

void sig_term(int signo);
int daemon ();


int main()
{

#ifdef _DAEMON_
    if (daemon() == -1)
    {
        cout<<"can't fork self"<<endl;
        exit(0);
    }
    openlog("daemontest", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Program Started");
    signal(SIGTERM, sig_term);
#endif
    CChatRoom ChatRoom;
    if( !ChatRoom.Start() )
    {
        exit(0);
    }


    while(1)
    {
        sleep(1);
    }
    syslog(LOG_INFO, "Program Ended");
    return 0;
}

int daemon_init()
{
    pid_t pid;
    if ( (pid = fork()) < 0 )
    {
        return -1;
    }
    else if(pid != 0)
    {
        exit(0);
    }

    if (setsid() < 0)
    {
        return -1;
    }
    signal(SIGHUP, SIG_IGN);

    if ( (pid = fork()) < 0)
    {
        return -1;
    }
    else if (pid !=0)
    {
        exit(0);
    }

    chdir("/");
    umask(0);
    for (int i = 0; i < MAXFD; i++)
    {
        close(i);
    }

    return 0;
}

int daemon ()
{
    pid_t pid;

    pid = fork ();

    /* 如果创建进程失败 */
    if (pid < 0)
    {
        return -1;
    }

    /* 父进程退出运行 */
    if (pid != 0)
        exit (0);

    /* 成为会话领头进程 */
    pid = setsid();

    if (pid < -1)
    {
        return -1;
    }

    /* 将工作目录修改成根目录 */
        chdir ("/");

    /* 将标准输入输出重定向到空设备 */
    {
        int fd;

        fd = open ("/dev/null", O_RDWR, 0);
        if (fd != -1)
        {
            dup2 (fd, STDIN_FILENO);
            dup2 (fd, STDOUT_FILENO);
            dup2 (fd, STDERR_FILENO);
            if (fd > 2)
                close (fd);
        }
    }

    umask (0027);

    return 0;
}

void sig_term(int signo)
{
    if (signo == SIGTERM)
    {
        syslog(LOG_INFO, "program terminated");
        closelog();
        exit(0);
    }
}
