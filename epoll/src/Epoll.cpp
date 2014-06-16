/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: Epoll.cpp
Author: 李有红
Version: 0.1
Date:2013/12/16
Description: epoll模型类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "Epoll.h"

#define MAXEVENTS 64

CEpoll::CEpoll(char *strListenPort)
:m_EpollEvents(NULL)
,m_strListenPort(strListenPort)
{

};

CEpoll::~CEpoll()
{

};

bool CEpoll::Start(void)
{
    if (InitListen() == -1)
	{
		abort();
	}

	m_fdEpoll = CreateEpoll(0);
	if (m_fdEpoll == EXIT_ERROR)
	{
		abort();
	}
	
    if(AddSocketToEpoll(m_ListenSocket, m_fdEpoll, EPOLLIN | EPOLLET) == EXIT_ERROR)
	{
		abort();
	}

    Run();

    return true;
}

bool CEpoll::Stop(void)
{


    return true;
}

int CEpoll::CreateBindedSocket()
{
    struct addrinfo hints;
    struct addrinfo *result, *temp;
    int iRet = -1;
    int Socket;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    iRet = getaddrinfo (NULL, m_strListenPort, &hints, &result);
    if (iRet != 0)
    {
      fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (iRet));
      return -1;
    }

    for (temp = result; temp != NULL; temp = temp->ai_next)
    {
      Socket = socket (temp->ai_family, temp->ai_socktype, temp->ai_protocol);
      if (Socket == -1)
        continue;

      iRet = bind (Socket, temp->ai_addr, temp->ai_addrlen);
      if (iRet == 0)
        {
          /* We managed to bind successfully! */
          break;
        }

      close (Socket);
    }

    if (temp == NULL)
    {
      fprintf (stderr, "Could not bind\n");
      return -1;
    }

    freeaddrinfo (result);

    return Socket;
};

int CEpoll::MakeSocketNonBlocking(int socket)
{
    int flags = 0;
    int iRet = -1;

    flags = fcntl(socket, F_GETFL, 0);
    if(flags == -1)
    {
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    iRet = fcntl(socket, F_SETFL, flags);
    if(iRet == -1)
    {
        perror("fcntl");
        return -1;
    }

    return 0;
}
int CEpoll::OnAccepteEvent(int fdEpoll, int socket)
{
    struct epoll_event event;
    for(;;)
    {
      struct sockaddr in_addr;
      socklen_t in_len;
      int ClientSocket;
      char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
      int iRet = -1;
      in_len = sizeof(in_addr);

      ClientSocket = accept (socket, &in_addr, &in_len);
      printf("accept\n");
      if (ClientSocket == -1)
        {
          if ((errno == EAGAIN) ||
              (errno == EWOULDBLOCK))
            {
              /* We have processed all incoming
                 connections. */
              break;
            }
          else
            {
              perror ("accept");
              break;
            }
        }

      iRet = getnameinfo (&in_addr, in_len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf),
                       NI_NUMERICHOST | NI_NUMERICSERV);
      if (iRet == 0)
      {
          printf("Accepted connection on descriptor %d "
                 "(host=%s, port=%s)\n", ClientSocket, hbuf, sbuf);
      }

      iRet = MakeSocketNonBlocking(ClientSocket);
      if (iRet == -1)
      {
        abort();
      }

      event.data.fd = ClientSocket;
      event.events = EPOLLIN | EPOLLET;
      iRet = epoll_ctl (fdEpoll, EPOLL_CTL_ADD, ClientSocket, &event);
      if (iRet == -1)
        {
          perror ("epoll_ctl");
          abort ();
        }
    }
    return 0;
}

int CEpoll::OnErrorEvent(int socket)
{
    fprintf(stderr, "epoll error\n");
    close(socket);
    return 0;
}

int CEpoll::OnReadEvent(int socket)
{
    bool bIsdone = 0;
    int iRet = 0;

    for(;;)
    {
      ssize_t count;
      char buf[512];

      count = read (socket, buf, sizeof(buf));
      if (count == -1)
      {
          /* If errno == EAGAIN, that means we have read all
             data. So go back to the main loop. */
          if (errno != EAGAIN)
            {
              perror ("read");
              bIsdone = true;
            }
          break;
      }
      else if (count == 0)
      {
          /* End of file. The remote has closed the
             connection. */
          bIsdone = true;
          break;
      }

      /* Write the buffer to standard output */
      iRet = write (1, buf, count);
      if (iRet == -1)
        {
          perror ("write");
          abort ();
        }
    }

    if (bIsdone)
    {
      printf ("Closed connection on descriptor %d\n", socket);

      /* Closing the descriptor will make epoll remove it
         from the set of descriptors which are monitored. */
      close (socket);
    }
    return 0;
}

int CEpoll::OnSendEvent(int socket)
{
    return 0;
}

int CEpoll::InitListen(void)
{
    int iRet = -1;

    m_ListenSocket = CreateBindedSocket();
    if (m_ListenSocket == -1)
    {
        return -1;
    }

    iRet = MakeSocketNonBlocking(m_ListenSocket);
    if (iRet == -1)
    {
        return -1;
    }

    iRet = listen(m_ListenSocket, SOMAXCONN);
    if (iRet == -1)
    {
        perror("listen");
        return -1;
    }
    return 0;
}

int CEpoll::AddSocketToEpoll(int socket, int fdEpoll, int flag)
{
    struct epoll_event EpollEvent;
    int iRet = -1;

    EpollEvent.data.fd = m_ListenSocket;
    EpollEvent.events = flag;

    iRet = epoll_ctl(fdEpoll, EPOLL_CTL_ADD, socket, &EpollEvent);
    if (iRet == -1)
    {
        perror("epoll_ctl");
        return EXIT_ERROR;
    }
    return 0;
}

void CEpoll::Run()
{
    int iCount;
	struct epoll_event      *EpollEvents;
	EpollEvents = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));

    for (;;)
    {
        iCount = epoll_wait(m_fdEpoll, EpollEvents, MAXEVENTS, -1);

        for (int i = 0; i < iCount; i++)
        {
			int CurrentSocket = EpollEvents[i].data.fd;
            if ( (EpollEvents[i].events & EPOLLERR)|| (EpollEvents[i].events & EPOLLHUP)||
                !(EpollEvents[i].events & EPOLLIN) )
            {
                OnErrorEvent(CurrentSocket);
                continue;
            }
            else if (m_ListenSocket == CurrentSocket)
            {
                OnAccepteEvent(m_fdEpoll, CurrentSocket);
                continue;
            }
            else
            {
                OnReadEvent(CurrentSocket);
            }
        }
    }

	free(m_EpollEvents);

	close(m_ListenSocket);
}

int CEpoll::CreateEpoll( int iSize )
{
	int fdEpoll;
	fdEpoll = epoll_create1(iSize);
	if (fdEpoll == -1)
	{
		perror("epoll_create1");
		return EXIT_ERROR;
	}

	return fdEpoll;
}
