#ifndef CEPOLL_INCLUDED
#define CEPOLL_INCLUDED
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: Epoll.h
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


enum
{
  PORT_LEN = 5
};

/******************************************************************
@Brief:封装了epoll I/O模型
TODO:包含该头文件就可以直接用
******************************************************************/

class CEpoll
{
public:
    CEpoll(char *strListenPort);
    ~CEpoll();

public:
    /*****************************************************************
    Function:Start
    Description:开始epoll模型
    Params:
      1 void
    Return:
      bool,是否开启成功
    Throws:
      无
    Others:无
    *****************************************************************/
    bool Start(void);

    /*****************************************************************
    Function:Stop
    Description:停止epoll模型
    Params:
      1 void
    Return:
      bool,是否停止成功
    Throws:Ctrl+Shift+C注释掉当前行或选中块，Ctrl+Shift+X则解除注释
      无
    Others:无
    *****************************************************************/
    bool Stop(void);

protected:
    //初始化监听
    int InitListen(void);

    //把监听套接字放入epoll模型
    int AddSocketToEpoll(int socket, int fdEpoll, int flag);

    //创建一个绑定了本地端口到套接子
    int CreateBindedSocket();

	//设置套接字不阻塞
    int MakeSocketNonBlocking(int socket);

	//创建一个epoll
	int CreateEpoll(int iSize);

protected:
    void Run();

    //事件处理
protected:
    //错误事件处理方法
    int OnErrorEvent(int socket);

    //接受连接事件处理方法
    int OnAccepteEvent(int fdEpoll, int socket);

    //读取事件处理方法
    int OnReadEvent(int socket);

    //发送事件处理方法
    int OnSendEvent(int socket);

private:
    char                    *m_strListenPort;
    int                     m_ListenSocket;
    int                     m_fdEpoll;
};


#endif // CEPOLL_INCLUDED
