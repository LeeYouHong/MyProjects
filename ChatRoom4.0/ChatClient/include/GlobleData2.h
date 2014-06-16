#ifndef GLOBLEDATA_INCLUDED
#define GLOBLEDATA_INCLUDED

#include "pthread.h"



class CServiceThread;
class CSocketItem;

#define SAFE_DELETE(obj)    if(obj)  {delete (obj);(obj) = NULL;}

//数据类型定义
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef unsigned long       DWORD;

#define NAME_LEN            33
#define PASS_LEN            33

//数组元素数
#define				CountArray(Array)			(sizeof(Array)/sizeof(Array[0]))

//网络数据定义
#define SOCKET_VER						0x66								//网络版本
#define SOCKET_BUFFER					8192								//网络缓冲
#define SOCKET_PACKET					(SOCKET_BUFFER-sizeof(CMD_Head)-2*sizeof(DWORD))
#define MAX_QUEUE_SIZE                  8192                                //最大到请求指令缓冲
#define MESSAGE_BUFFER                  2000                                //聊天消息


//内核命令码
#define MDM_KN_COMMAND					0									//内核命令
#define SUB_KN_DETECT_SOCKET			1									//检测命令
#define SUB_KN_SHUT_DOWN_SOCKET			2									//中断网络


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

//登录命令码

#define MDM_LOGON					1								//广场登录

#define SUB_LOGON_ACCOUNTS			1								//帐号登录
#define SUB_LOGON_USERID		    2								//I D 登录
#define SUB_REGISTER_ACCOUNTS		3								//注册帐号
#define SUB_LAYOUT                  4                               //注销

#define Result_LOGON_SUCCESS			100								//登陆成功
#define Result_LOGON_ERROR				101								//登陆失败
#define Result_LOGON_FINISH				102								//登陆完成

#define RESULT_LAYOUT_ERROR             103                          //注销错误
#define RESULT_LAYOUT_SUCCESS           104                          //注销成功

#define RESULT_REGISTER_SUCCESS         1                                   //用户在数据库中注册成功
#define RESULT_REGISTER_USER_EXISTE     2                                   //用户已经存在
#define RESULT_REGISTER_ERROR           3                                   //数据库注册用户出错

#define RESULT_LOGON_SUCCESS            4                                   //数据库验证成功
#define RESULT_LOGON_NO_USER            5                                   //数据库无此账户
#define RESULT_LOGON_ERROR              6                                   //数据库验证出错
#define RESULT_LOGON_PASSWD_ERROR       7                                   //数据库验证成功
#define RESULT_LOGON_ONLINE             8                                   //已经在线

//帐号登录
struct Packet_Logon_LogonByAccounts
{
	DWORD								dwPlazaVersion;					//广场版本
	char								szAccounts[NAME_LEN];			//登录帐号
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
	char								szAccounts[NAME_LEN];			//登录帐号
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
#define DB_REGISTER_SUCCESS         1                                   //用户在数据库中注册成功
#define DB_REGISTER_USER_EXISTE     2                                   //用户已经存在
#define DB_REGISTER_ERROR           3                                   //数据库注册用户出错

#define DB_LOGON_SUCCESS            4                                   //数据库验证成功
#define DB_LOGON_NO_EXISTE          5                                   //数据库无此账户
#define DB_LOGON_ERROR              6                                   //数据库验证出错
#define DB_LOGON_PASSWD_ERROR       7                                   //数据库验证成功

#define DB_ERROR                    -1                                    //数据库错误

//数据库请求标识
#define	DBR_LOGON_BY_ACCOUNTS	1									//帐号登录
#define	DBR_LOGON_BY_USERID		2									//I D 登录
#define DBR_REGISTER_ACCOUNTS	3									//注册帐号


//数据库输出标识
#define DBR_LOGON_SUCCESS		100									//登录成功
#define DBR_LOGON_ERROR			101									//登录失败

//帐号登录
struct DBR_LogonByAccounts
{
	char							szAccounts[NAME_LEN];				//登录帐号
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
	char							szAccounts[NAME_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
};
//////////////////////////////////////////////////////////////////////////////

//用户数据结构
struct tagUserData
{
    char							szAccounts[NAME_LEN];				//登录帐号
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

//////////数据库结果
#define DB_RESULT_LOGON_NO_EXIST            1
#define DB_RESULT_LOGON_ERROR               2
#define DB_RESULT_LOGON_SUCCESS             3
#define DB_RESULT_LOGON_PASSWORD_ERROR      4


#define DB_RESULT_REGISTER_EXIST            11
#define DB_RESULT_REGISTER_ERROR            12
#define DB_RESULT_REGISTER_SUCCESS          13

//帐号登录
struct DBR_GP_Result_LogonByAccounts
{
	char							szAccounts[NAME_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
};

//注册成功包
struct DBR_GP_Result_Register_Success
{
	char							szAccounts[NAME_LEN];				//注册帐号
	char							szPassWord[PASS_LEN];				//注册密码
};

////////////////////////聊天消息////////////////////////////////////////////
//消息传送
#define  MDM_CHAT                    2                                  //聊天主信息
#define  SUB_CHAT_SINGLE             1                                  //单个聊天
#define  SUB_CHAT_ALL                2                                  //群发
#define  SUB_CHAT_RESULT             3                                  //聊天结果

#define  Result_ChatBySingle_NO_PEOPLE  4                               //没有这个人
#define  Result_ChatBySingle_NO_ONLINE  5                               //已经掉线
//单聊
struct Packet_Chat_ChatBySingle
{
	char								szSender[NAME_LEN];			//发送者
	char								szRecver[NAME_LEN];			//接收者
	char                                szMessage[MESSAGE_BUFFER];  //聊天消息
};

//群聊
struct Packet_Chat_ChatByAll
{
	char								szSender[NAME_LEN];			//发送者
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
    char                              szAccount[NAME_LEN];            //注销者
};


#endif // GLOBLEDATA_INCLUDED
