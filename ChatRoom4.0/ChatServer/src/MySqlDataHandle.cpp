#include <mysql/mysql.h>
#include <stdio.h>
#include "MySqlDataHandle.h"
#include "Log.h"

CMySqlDataHandle::CMySqlDataHandle()
{

}

CMySqlDataHandle::~CMySqlDataHandle()
{

}

int CMySqlDataHandle::LogonByAccounts(char *strAccount, char *strPasswd)
{
    //判断是否存在此用户
    if (IsExistsUser(strAccount))
    {
        //建立连接
        MYSQL *pConnection = ConnectMysql(0);
        char statement[255];
        sprintf(statement, "select * from user where name='%s' and pwd='%s'", strAccount, strPasswd);
        //开始查询
        if (mysql_query(pConnection, statement) != 0)
        {
            FinishWithError(pConnection);

            mysql_close(pConnection);
            return S_DB_RESULT_LOGON_PASSWD_ERROR;
        }
        MYSQL_RES *result = mysql_store_result(pConnection);
        //判断查询结果
        if (result != NULL)
        {
            char log[255];
            snprintf(log, 255, "%s登录成功", strAccount);
            CLog::Instance().Print(log);

            mysql_free_result(result);
            mysql_close(pConnection);
            return S_DB_RESULT_LOGON_SUCCESS;
        }
        else
        {
            char log[255];
            snprintf(log, 255, "%s密码错误", strAccount);
            CLog::Instance().Print(log);

            mysql_free_result(result);
            mysql_close(pConnection);
            return S_DB_RESULT_LOGON_PASSWD_ERROR;
        }
    }
    else
    {
        char log[255];
        snprintf(log, 255, "%s用户名不存在", strAccount);
        CLog::Instance().Print(log);
        return S_DB_RESULT_LOGON_NO_EXISTS;
    }


}

int CMySqlDataHandle::RegisterAccounts(char *strAccount, char *strPasswd)
{
    if ( IsExistsUser(strAccount) )
    {
        return S_DB_RESULT_REGISTER_USER_EXISTS;
    }
    else
    {
        //创建连接
        MYSQL *pConnection = ConnectMysql(0);
        char statement[255];
        sprintf(statement, "insert into user(name,pwd) values('%s','%s')", strAccount, strPasswd);
        //插入数据
        if (mysql_query(pConnection, statement) != 0)
        {
              FinishWithError(pConnection);

              mysql_close(pConnection);
              return S_DB_RESULT_REGISTER_ERROR;
        }
        return S_DB_RESULT_REGISTER_SUCCESS;
    }
    return S_DB_RESULT_REGISTER_ERROR;
}

MYSQL *CMySqlDataHandle::ConnectMysql(unsigned long flag)
{
        //初始化
    MYSQL *pConnect = mysql_init(NULL);
    if (pConnect == NULL)
    {
        CLog::Instance().Print("mysql init failed");
        return NULL;
    }

    //创建连接
    pConnect = mysql_real_connect(pConnect, "localhost", "root", "geiwo8maoqianba!", "chatroom",
                                   0, NULL, flag);
    if (pConnect == NULL)
    {
        CLog::Instance().Print("connection failed");
        mysql_close(pConnect);
        return NULL;
    }
    CLog::Instance().Print("connection success");

    return pConnect;
}

bool CMySqlDataHandle::IsExistsUser(char *strAccount)
{
    //创建连接
    MYSQL *pConnection = ConnectMysql(0);

    char statement[255];
    sprintf(statement, "select name from user where name='%s'", strAccount);

    if (mysql_query(pConnection, statement) != 0)
    {
        FinishWithError(pConnection);
        return false;
    }
    MYSQL_RES *result = mysql_store_result(pConnection);

    if (result == NULL)
    {
        FinishWithError(pConnection);

        mysql_free_result(result);
        return false;
    }

    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return false;
    }

    mysql_free_result(result);
    return true;
}

void CMySqlDataHandle::FinishWithError(MYSQL *pConnection)
{
    CLog::Instance().Print(mysql_error(pConnection));
    mysql_close(pConnection);
}
