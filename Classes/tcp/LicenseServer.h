#ifndef __LICENSE_SERVER_H__
#define __LICENSE_SERVER_H__

#include "chessAndCards.h"
#include "CGameSocket.h"
#include "tcp/ServiceHeader.h"
#include "logic/TcpLogic.h"

class LicenseServer
{
private:
	static LicenseServer* _instance;
	CGameSocket* m_sock;
public:
	~LicenseServer();																					//析构
	static LicenseServer*					GetInstance();												//单例模式
	static void								RecoverInstance();											//销毁单例

	bool									CreatSocket(const char* ip, int port);	

	bool									CheckSocket();							//检查链接状态

	void									RecvData();													//接收数据线程回调

	bool									RegistUserReq(SUserRegistReq*	pUserRegistReq);			//注册用户请求

	bool									LoginUserReq(SUserPswLoginReq*	pUserPswLoginReq);			//登录用户请求

	bool									QueryPartitionInfoReq(SUserQueryPartitionReq*	pUserQueryPartitionReq);	//查询分区请求

	bool									EnterPartitionReq(SUserEnterPartitionReq*	pUserEnterPartitionReq);		//进入分区请求

	void									CloseLicense();											//关闭验证服务器连接

private:
	LicenseServer();																		//构造
	char									m_key[DEFAULT_KEY_SIZE + 1];

	typedef int (TcpLogic::*fnLgProcessHandler)(SGSResPayload* payload, int nPayloadLen);	//验证服务器接收数据对应函数
	map<UINT16, fnLgProcessHandler>			m_lgProcHandler;								//验证服务器接收数据对应函数容器

	thread*									threadRecv;										//验证服务器接收数据线程
	bool									RecvThreadState;								//验证服务器接收数据线程开关
};



#endif