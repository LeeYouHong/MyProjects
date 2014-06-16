#ifndef CMYSQLDATAHANDLE_H
#define CMYSQLDATAHANDLE_H

#include <mysql/mysql.h>
#include "interface.h"

class CMySqlDataHandle : public IDataHandleSink
{
    public:
        CMySqlDataHandle();
        virtual ~CMySqlDataHandle();

    public:
        virtual int LogonByAccounts(char *strAccount, char *strPasswd);
        virtual int RegisterAccounts(char *strAccount, char *strPasswd);

    private:
        MYSQL *ConnectMysql(unsigned long flag);
        bool IsExistsUser(char *strAccount);
        void FinishWithError(MYSQL *pConnection);
};

#endif // CMYSQLDATAHANDLE_H
