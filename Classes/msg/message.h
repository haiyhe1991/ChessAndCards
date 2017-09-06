#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <map>
#include <list>

using namespace std;

enum MessageHead
{
	MSG_START_LOGO						= 1000,
	MSG_CHANGE_SCENE					= 1001,			//切换场景信息   封装之后，不存在场景了，只有各种层
	MSG_START_LOADING					= 1002,			//开始加载界面

	MSG_CONNECT_LICENSE_RES				= 2001,			//连接license响应
	MSG_REQ_OUT_TIME					= 2002,			//请求超时
	MSG_REGIST_RES						= 2004,			//注册响应
	MSG_LOGIN_RES						= 2006,			//登录响应
	MSG_QUERY_PARTITION_RES				= 2008,			//查询分区响应
	MSG_ENTER_PARTITION_RES				= 2010,			//进入分区响应
	MSG_CONNECT_LINK_RES				= 2012,			//连接link响应
	MSG_LOGIN_LINK_RES					= 2014,			//登陆link响应
	MSG_QUERY_ROLE_RES					= 2015,			//查询角色响应
	MSG_CREATE_ROLE_RES					= 2016,			//创建角色响应
	MSG_ENTER_ROLE_RES					= 2017,			//进入角色响应
	MSG_JOIN_CHANNEL_RES				= 2018,			//加入频道响应

	MSG_START_GAME						= 3001,			//开始游戏
	MSG_ACTION_CHESS					= 3002,			//玩家动作
};

//游戏中的通信接口
class GameMessage
{
public:

	virtual void OnMessage(const int head, void* data) = 0;			//接收到消息
};

//消息管理器
class MsgManager
{
public:
	static MsgManager*			GetInstance();
	static void					DestoryInstance();

	void						Reg(const int head, GameMessage* msg);				//注册消息

	void						Unreg(const int head, GameMessage* msg);			//注销消息

	void						Dispather(const int head, void* data);				//发送消息

	void						Clear();											//清除所有消息

private:
	static MsgManager*				_instance;

	map<int, list<GameMessage*>>	container;					//消息管理容器
};

#endif
