#ifndef GLOBLEDATA_INCLUDED
#define GLOBLEDATA_INCLUDED

#include "pthread.h"



class CServiceThread;
class CSocketItem;

//释放指针
template <typename T>
inline void SafeDelete(T *Obj)
{
    if (Obj != NULL)
    {
        delete Obj;
        Obj = NULL;
    }
}

//数据类型定义
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef unsigned long       DWORD;

//账户名长度
const int ACCOUNT_LEN = 33;
//密码串长度
const int PASS_LEN = 33;

//计算数组元素个数
#define				CountArray(Array)			(sizeof(Array)/sizeof(Array[0]))


//网络数据定义
const int SOCKET_VER     =              0x66;								//网络版本
const int SOCKET_BUFFER  =   			8192;								//网络缓冲
#define SOCKET_PACKET  (SOCKET_BUFFER-sizeof(CMD_Head)-2*sizeof(DWORD))
const int MAX_QUEUE_SIZE =              8192;                                //最大到请求指令缓冲
const int MESSAGE_BUFFER =              2000;                                //聊天消息


//内核子命令码结果
enum C_SUB_KN
{
    C_SUB_KN_DETECT_SOCKET = 1,	    					//检测命令
    C_SUB_KN_SHUT_DOWN_SOCKET	= 2,						//中断网络
};

//内核子命令码
enum S_SUB_KN
{
    S_SUB_KN_DETECT_SOCKET = 1,	    					//检测命令
    S_SUB_KN_SHUT_DOWN_SOCKET	= 2,						//中断网络
};

//网络内核
struct CMD_Info
{
	BYTE								cbVersion;							//版本标识
	BYTE								cbCheckCode;						//效验字段
	WORD								wPacketSize;						//数据大小
};

//网络命令
struct CMD_Command
{
	WORD								wMainCmdID;							//主命令码
	WORD								wSubCmdID;							//子命令码
};

//网络包头
struct CMD_Head
{
	CMD_Info							CmdInfo;							//基础结构
	CMD_Command							CommandInfo;						//命令信息
};

//网络包缓冲
struct CMD_Buffer
{
	CMD_Head							Head;								//数据包头
	BYTE								cbBuffer[SOCKET_PACKET];			//数据缓冲
};

//检测结构信息
struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;					//发送时间
	DWORD								dwRecvTickCount;					//接收时间
};

//主命令
enum S_MDM
{
    S_MDM_KN_COMMAND = 0,									//内核命令
    S_MDM_LOGON = 1,                                      //登录命令
    S_MDM_CHAT = 2,                                       //聊天主信息
};

//主命令结果
enum C_MDM
{
    C_MDM_KN_COMMAND_RESULT = 0,									//内核命令
    C_MDM_LOGON_RESULT = 1,                                      //登录命令
    C_MDM_CHAT_RESULT = 2,                                       //聊天主信息
};


//登录子命令
enum S_SUB_LOGON
{
    S_SUB_LOGON_ACCOUNTS = 1,			   //帐号登录
    S_SUB_REGISTER_ACCOUNTS = 2,		   //注册帐号
    S_SUB_LAYOUT = 3                       //注销
};

//登录结果子命令
enum C_SUB_LOGON_RESULT
{
    C_SUB_LOGON_RESULT_LAYOUT_ERROR = 1,                          //注销错误
    C_SUB_LOGON_RESULT_LAYOUT_SUCCESS = 2,                        //注销成功

    C_SUB_LOGON_RESULT_REGISTER_SUCCESS = 11,                     //用户注册成功
    C_SUB_LOGON_RESULT_REGISTER_USER_EXISTE = 12,                 //用户已经存在
    C_SUB_LOGON_RESULT_REGISTER_ERROR = 13,                       //注册用户出错

    C_SUB_LOGON_RESULT_LOGON_SUCCESS = 21,                        //登录成功
    C_SUB_LOGON_RESULT_LOGON_NO_USER = 22,                        //登录账户不存在
    C_SUB_LOGON_RESULT_LOGON_ERROR = 23,                          //登录出错
    C_SUB_LOGON_RESULT_LOGON_PASSWD_ERROR = 24,                   //登录密码错误
    C_SUB_LOGON_RESULT_LOGON_ONLINE = 25,                         //登录账户已经存在

};

//帐号登录
struct Packet_Logon_LogonByAccounts
{
	DWORD								dwPlazaVersion;					//广场版本
	char								szAccounts[ACCOUNT_LEN];			//登录帐号
	char								szPassWord[PASS_LEN];			//登录密码
};

//I D 登录
struct Packet_Logon_LogonByUserID
{
	DWORD								dwPlazaVersion;					//广场版本
	DWORD								dwUserID;						//用户 I D
	char								szPassWord[PASS_LEN];			//登录密码
};

//注册帐号
struct Packet_Logon_RegisterAccounts
{
	BYTE								cbGender;						//用户性别
	DWORD								dwPlazaVersion;					//广场版本
	char								szAccounts[ACCOUNT_LEN];			//登录帐号
	char								szPassWord[PASS_LEN];			//登录密码
};

//登陆失败
struct Packet_Result_Logon
{
	long								lErrorCode;						//错误代码
	char								szErrorDescribe[128];			//错误消息
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//数据库对象定义

//查询数据库错误码
enum S_DB_RESULT
{
    S_DB_RESULT_REGISTER_SUCCESS = 1,
    S_DB_RESULT_REGISTER_USER_EXISTS = 2,
    S_DB_RESULT_REGISTER_ERROR = 3,

    S_DB_RESULT_LOGON_SUCCESS = 4,
    S_DB_RESULT_LOGON_NO_EXISTS = 5,
    S_DB_RESULT_LOGON_ERROR = 6,
    S_DB_RESULT_LOGON_PASSWD_ERROR = 7,

    S_DB_RESULT_ERROR = -1,
};

//数据库请求标识
enum S_DBR
{
    S_DBR_LOGON_BY_ACCOUNTS = 1,
    S_DBR_LOGON_BY_USERID = 2,
    S_DBR_REGISTER_ACCOUNTS = 3,
};

//数据库输出标识
enum S_DBR_RESULT
{
    S_DBR_RESULT_LOGON_SUCCESS = 1,
    S_DBR_RESULT_LOGON_ERROR = 2,
};

//帐号登录
struct DBR_LogonByAccounts
{
	char							szAccounts[ACCOUNT_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
};

//ID 登录
struct DBR_LogonByUserID
{
	DWORD							dwUserID;							//用户 I D
	char							szPassWord[PASS_LEN];				//登录密码
};

//帐号注册
struct DBR_RegisterAccounts
{
	BYTE							cbGender;							//用户性别
	char							szAccounts[ACCOUNT_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
};
//////////////////////////////////////////////////////////////////////////////

//用户数据结构
struct tagUserData
{
    char							szAccounts[ACCOUNT_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
};

//线程参数
struct tagThreadParameter
{
    CServiceThread                  *pServerThread;                     //线程指针
    pthread_cond_t                  *pCondt;
};

//数据库请求队列
struct tagDBRequest
{
    WORD                            wRequestID;
    int                             Socket;
    BYTE                            *pData;
    WORD                            wDataSize;
};

//数据列头
struct tagDataHead
{
	WORD							wDataSize;							//数据大小
	WORD							wIdentifier;						//类型标识
};

struct tagHandleMessageRequest
{
    WORD                            wRequestID;
    CSocketItem                     *pSocketItem;
    BYTE                            *pData;
    WORD                            wDataSize;
};

//请求队列
struct tagRequest
{
    WORD                            wRequestID;
    CSocketItem                     *pSocketItem;
    BYTE                            *pData;
    WORD                            wDataSize;
};


struct tagSendRequest
{
    int                             Socket;
    BYTE                            *pData;
    WORD                            wDataSize;
    WORD                            wMainCmdID;
    WORD                            wSubCmdID;
};

//帐号登录
struct DBR_GP_Result_LogonByAccounts
{
	char							szAccounts[ACCOUNT_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
};

//注册成功包
struct DBR_GP_Result_Register_Success
{
	char							szAccounts[ACCOUNT_LEN];				//注册帐号
	char							szPassWord[PASS_LEN];				//注册密码
};

////////////////////////聊天消息////////////////////////////////////////////
//消息传送
enum C_SUB_CHAT
{
    C_SUB_CHAT_SINGLE_RESULT = 1,
    C_SUB_CHAT_ALL_RESULT = 2,
    C_SUB_CHAT_RESULT = 3,
};

enum S_SUB_CHAT
{
    S_SUB_CHAT_SINGLE = 1,
    S_SUB_CHAT_ALL = 2,
    S_SUB_CHAT = 3,
};

enum C_SUB_CHAT_RESULT
{
    C_SUB_CHAT_RESULT_CHATBYSINGLE_NO_ONLINE = 1,
};

//单聊
struct Packet_Chat_ChatBySingle
{
	char								szSender[ACCOUNT_LEN];			//发送者
	char								szRecver[ACCOUNT_LEN];			//接收者
	char                                szMessage[MESSAGE_BUFFER];  //聊天消息
};

//群聊
struct Packet_Chat_ChatByAll
{
	char								szSender[ACCOUNT_LEN];			//发送者
	char                                szMessage[MESSAGE_BUFFER];  //聊天消息
};

struct Packet_Result_Chat
{
    WORD                                 wResultCode;               //结果码
};
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////注销包///////////////////////////////////
struct Packet_Layout
{
    char                              szAccount[ACCOUNT_LEN];            //注销者
};


#endif // GLOBLEDATA_INCLUDED
