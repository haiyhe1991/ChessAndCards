#ifndef __LINK_SERVER_H__
#define __LINK_SERVER_H__

#include "chessAndCards.h"
#include "CGameSocket.h"
#include "tcp/ServiceHeader.h"
#include "logic/TcpLogic.h"

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

	bool									CheckSocket();							//检查链接状态

	bool									LoginLinkerReq(SLoginLinkerReq* pLoginLinkerReq);//请求登录linker

	bool									QueryRolesReq(SQueryRolesReq* req);			//查询角色

	bool									CreateRoleReq(SCreateRoleReq* req);			//创建角色

	bool									EnterRoleReq(SEnterRoleReq* pEnterRoleReq);	//进入角色

	bool									JoinChannelReq(SJoinChannelReq* req);			//加入聊天频道请求

	bool									ChatMessageSend(BYTE* pChat);//发送聊天信息

	bool									CreateTableReq(SCreateTabelReq* req);			//创建牌桌请求

	bool									JionTableReq(SJoinTableReq* req);			//加入牌桌请求

	bool									PlayerReadyReq(SReadyTableReq* req);			//玩家准备请求

private:

	// DWORD ip 转化为 字符串ip
	// szBuf必须传递，至少为16字节长
	const char*								GetStringIP(char szBuf[16], DWORD dwIP);				//转化为字符串ip

	LinkServer();
	thread*									threadRecv;										//link服务器接收数据线程
	bool									RecvThreadState;								//link服务器接收数据线程开关

	char									m_key[DEFAULT_KEY_SIZE + 1];

	typedef int (TcpLogic::*fnLgProcessHandler)(SGSResPayload* payload, int nPayloadLen);	//link服务器接收数据对应函数
	map<UINT16, fnLgProcessHandler>			m_lgProcHandler;								//link服务器接收数据对应函数容器
};


#endif