#ifndef __LINK_SERVER_H__
#define __LINK_SERVER_H__

#include "chessAndCards.h"
#include "CGameSocket.h"
#include "tcp/ServiceHeader.h"

class LinkServer
{
private:
	static LinkServer* _instance;
	CGameSocket* m_sock;
public:
	~LinkServer();																	//析构
	static LinkServer*						GetInstance();							//单例模式
	static void								RecoverInstance();						//销毁对象

	bool									CreatSocket(DWORD dwIP, int port);		//创建socket

	void									RecvData();								//接收消息

	void									CloseLink();							//关闭socket

	bool									LoginLinkerReq(SLoginLinkerReq* pUserRegistReq);//请求登录linker



private:

	// DWORD ip 转化为 字符串ip
	// szBuf必须传递，至少为16字节长
	const char*								GetStringIP(char szBuf[16], DWORD dwIP);				//转化为字符串ip

	LinkServer();
	thread*									threadRecv;										//link服务器接收数据线程
	bool									RecvThreadState;								//link服务器接收数据线程开关

	char									m_key[DEFAULT_KEY_SIZE + 1];
};


#endif