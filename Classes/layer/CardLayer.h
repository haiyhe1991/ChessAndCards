#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//主界面层
class CardLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(CardLayout);

	CardLayout();
	~CardLayout();

private:
	Node*					layout;					//主界面层资源
	Node*					recv;					//遮挡层资源

	TextField*				btSendData;				//编辑发送信息框
	ListView*				lstChatRecv;				//收信息框

	Button*					bt_clear;				//清除收到信息按钮
	Button*					bt_send;				//发送信息按钮

	void					onClearHandler(Ref* sender);	//点击的处理事件
	void					onSendHandler(Ref* sender);		//点击的处理事件

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面


	void					ConnectLinkRes();				//创建link连接反馈结果
	//参数：服务器返回代码
	int						LoginLinkRes(void* pBuf);		//登陆link反馈结果

	int						QueryRoleRes(void* pBuf);		//查询角色反馈结果

	int						CreateRoleRes(void* pBuf);		//创建角色反馈结果

	int						EnterRoleRes(void* pBuf);		//进入角色反馈结果

	int						JoinChannelRes(void* pBuf);		//加入聊天频道反馈结果
};

class CardLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(CardLayer);
	~CardLayer();

	virtual void onEnter();							//初始化函数

	virtual list<LoadItem> getResourceList();			//取得资源加载列表

	virtual void releaseResource();					//释放资源

private:
	CardLayout*				cardLayout;		//场景的背景容器					

};


#endif