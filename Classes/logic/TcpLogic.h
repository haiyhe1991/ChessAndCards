#ifndef __RECV_LOGIC_H__
#define __RECV_LOGIC_H__

#include "chessAndCards.h"
#include "tcp/ServiceHeader.h"


#define TCP_DELAY_TIME			1000				//等待服务器回应延迟时间1s
#define TCP_ANSWER_TIME_OUT		10000				//等待服务器最长有效时间10s
#define LICENSE_IP				"192.168.50.2"		//验证服务器ip
#define LICENSE_PORT			10030				//验证服务器端口

enum TcpState
{
	IDEL,			//空闲阶段
	REQ,			//请求阶段
	OUT_TIME,		//超时阶段
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

class TcpLogic : public GameTick
{
public:

	static TcpLogic*				GetInstance();										//取得单例
	static void					DestoryInstance();										//摧毁单例
	~TcpLogic();																		//析构

	virtual void							OnTick(int time);


	void						registerMessage();

	void						ChangeTcpState(TcpState tcpState);								//改变tcp状态

	bool						CreatLicenseSocket();											//创建验证服务器连接

	void						ConnectLincenseRes();											//创建链接验证服务器响应

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

	bool						LoginLinkReq();											//登录linker请求

	void						ConnectLinkRes();										//创建linker服务器连接响应

	//参数：有效数据buffer，有效数据总长度
	int							LoginLinkerRes(SGSResPayload* pBuf, int pBufLen);	//进入分区接收到的数据

	bool						QueryRolesReq();										//查询角色信息请求

	//参数：有效数据buffer，有效数据总长度
	int							QueryRolesRes(SGSResPayload* pBuf, int pBufLen);		//查询角色信息响应

	bool						CreateRoleReq(BYTE occuId,BYTE sex,const char* nick);	//创建角色请求

	//参数：有效数据buffer，有效数据总长度
	int							CreateRoleRes(SGSResPayload* pBuf, int pBufLen);			//创建角色响应

	bool						EnterRoleReq(UINT32 roleId);							//进入角色请求

	//参数：有效数据buffer，有效数据总长度
	int							EnterRoleRes(SGSResPayload* pBuf, int pBufLen);			//进入角色响应

	bool						JoinChannelReq();			//加入聊天频道请求

	//参数：有效数据buffer，有效数据总长度
	int							JoinChannelRes(SGSResPayload* pBuf, int pBufLen);			//进入角色响应

	bool						ChatMessageSend(const char*	chatMsg);				//发送聊天信息

private:

	TcpLogic();																		//构造

	static TcpLogic*				_instance;											//单例

	SUserLoginRes*				m_pUserLoginRes;										//登录数据信息
	SUserQueryPartitionRes*		m_pQueryPartitionRes;									//查询分区数据信息
	SUserEnterPartitionRes*		m_pEnterPartitionRes;									//登录分区数据信息
	SCreateRoleRes*				m_pCreateRoleRes;										//角色创建数据信息
	SEnterRoleRes*				m_pSEnterRoleRes;										//角色进入数据信息

	list<SPartitionInfo* >		container;												//分区信息容器

	list<SSkillMoveInfo* >		containerSkill;											//技能信息容器

	list<SQueryRoleAttr* >		containerRoleAttr;										//角色信息

	WaitTime*					wait;

	TcpState					state;													//tcp状态

};

#endif