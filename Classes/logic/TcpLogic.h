#ifndef __RECV_LOGIC_H__
#define __RECV_LOGIC_H__

#include "chessAndCards.h"
#include "tcp/ServiceHeader.h"


#define TCP_DELAY_TIME			1000		//等待服务器回应延迟时间1s

enum RecvState
{
	RECVING,		//洗牌阶段
	RECVED,			//发牌阶段
	PAUSEING,		//暂停阶段
};

class WaitTime
{
public:
	WaitTime();
	void			StartWait(int time);	//开始等待  时间毫秒   如果时间为0，那么到下一次心跳的时候，将执行后面的代码
	bool			OnTime(int time);		//记录时间			true 时间已到
private:
	int				recordTime;				//记录的时间
	bool			state;					//是否等待
};

class TcpLogic : public GameMessage, GameTick
{
public:

	static TcpLogic*				GetInstance();										//取得单例
	static void					DestoryInstance();										//摧毁单例
	~TcpLogic();																		//析构

	virtual void							OnTick(int time);
	virtual void							OnMessage(const int head, void* data);

	void						registerMessage();

	bool						CreatLicenseSocket(const char* ip, int port);					//创建验证服务器连接

	const char*					getloginPlat();														//获取登录平台

	bool						RegistUserReq(const char*	pUsername, const char*	pPassword);		//注册用户请求

	bool						LoginUserReq(const char*	pUsername, const char*	pPassword);		//登录用户请求

	bool						QueryPartitionInfoReq(UINT16 from, UINT16 num);						//查询分区请求

	bool						EnterPartitionReq(UINT16	partitionId);							//进入分区请求

	//参数：有效数据buffer，有效数据总长度
	int							RegistUserRes(SGSResPayload* pBuf, int pBufLen);		//注册用户接收到的数据

	//参数：有效数据buffer，有效数据总长度
	int							LoginUserRes(SGSResPayload* pBuf, int pBufLen);			//登录接收到的数据

	//参数：有效数据buffer，有效数据总长度
	int							QueryPartitionInfoRes(SGSResPayload* pBuf, int pBufLen);//查询分区接收到的数据

	//参数：有效数据buffer，有效数据总长度
	int							EnterPartitionRes(SGSResPayload* pBuf, int pBufLen);	//进入分区接收到的数据

	bool						CreatLinkerSocket();									//创建linker服务器连接,并且断开验证服务器连接
private:

	TcpLogic();																		//构造

	static TcpLogic*				_instance;											//单例

	SUserLoginRes*				m_pUserLoginRes;										//登录数据信息
	SUserQueryPartitionRes*		m_pQueryPartitionRes;									//查询分区数据信息
	SUserEnterPartitionRes*		m_pEnterPartitionRes;									//登录分区数据信息

	list<SPartitionInfo* >		container;												//分区信息容器

	WaitTime*					wait;

	RecvState					state;													//接收状态

};

#endif