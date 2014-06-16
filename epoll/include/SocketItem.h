#ifndef CSOCKETITEM_H
#define CSOCKETITEM_H
/*****************************************************************
Copyright©,  2013-2014,  Tenone  Technology  Co.,Ltd.  ALL  RIGHTS
RESERVED
File name: SocketItem.h
Author: 李有红
Version: 0.1
Date:2013/12/16
Description: 套接字类
Others:
History：
    1.  Date：
        Author:
        Modification:
*********************************************************************/


/******************************************************************
@Brief:封装了套接字到基本属性和操作
TODO:包含该头文件就可以直接用
******************************************************************/

class CSocketItem
{
public:
    CSocketItem();
    virtual ~CSocketItem();

    /*****************************************************************
    Function:SetBlock
    Description:设置套接字为不阻塞
    Params:
      1 void
    Return:
      void
    Throws:
      无
    Others:无
    *****************************************************************/
    void SetBlock(void);

    //设置套接子
    void SetSocket(int socket);
private:
    int     m_Socket;

};

#endif // CSOCKETITEM_H
