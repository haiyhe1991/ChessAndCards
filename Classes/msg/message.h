#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <map>
#include <list>

using namespace std;

#define MSG_MAX_LENGTH		1024

enum MessageHead
{
	MSG_START_LOGO = 1000,
	MSG_CHANGE_SCENE = 1001,			//切换场景信息   封装之后，不存在场景了，只有各种层
	MSG_START_LOADING = 1002,			//开始加载界面

	MSG_CONNECT_LICENSE_RES = 2001,			//连接license响应
	MSG_REQ_OUT_TIME = 2002,			//请求超时
	MSG_REGIST_RES = 2004,			//注册响应
	MSG_LOGIN_RES = 2006,			//登录响应
	MSG_QUERY_PARTITION_RES = 2008,			//查询分区响应
	MSG_ENTER_PARTITION_RES = 2010,			//进入分区响应
	MSG_CONNECT_LINK_RES = 2012,			//连接link响应
	MSG_LOGIN_LINK_RES = 2014,			//登陆link响应
	MSG_QUERY_ROLE_RES = 2015,			//查询角色响应
	MSG_CREATE_ROLE_RES = 2016,			//创建角色响应
	MSG_ENTER_ROLE_RES = 2017,			//进入角色响应
	MSG_JOIN_CHANNEL_RES = 2018,			//加入频道响应

	MSG_START_GAME = 3001,					//开始游戏
	MSG_ACTION_CHESS = 3002,				//玩家动作
	MSG_LAYOUT_PALYER_INFO = 3003,			//展示玩家基本信息
	MSG_LOGIC_PALYER_READY = 3004,			//玩家准备游戏请求
	MSG_LAYOUT_PALYER_READY = 3005,			//玩家准备游戏响应
	MSG_LAYOUT_WASH_PORKS = 3006,			//显示层洗牌
	MSG_LOGIC_WASH_PORKS = 3007,			//逻辑层洗牌完毕
	MSG_LAYOUT_DEAL_PORK = 3008,			//显示层发牌
	MSG_LOGIC_DEAL_PORK = 3009,				//逻辑层发牌完毕
	MSG_LAYOUT_HIT_PORK = 3010,				//显示层打牌
	MSG_LOGIC_HIT_PORK = 3011,				//逻辑层打牌
	MSG_LAYOUT_HIT_CHANGE = 3012,			//显示层改变花色
	MSG_LAYOUT_ACTION_CHESS = 3013,			//玩家动作
	MSG_LAYOUT_NO_ACTION_CHESS = 3014,		//玩家禁止动作
	MSG_LAYOUT_VALIAD = 3015,				//出牌有效
	MSG_LAYOUT_INVALIAD = 3016,				//出牌无效
	MSG_LAYOUT_CALCULATE = 3017,			//显示层结算
	MSG_LAYOUT_CALCULATE_SCORE = 3018,		//显示层结算得分
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
