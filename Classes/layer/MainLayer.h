#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//主界面层
class MainLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(MainLayout);

	MainLayout();
	~MainLayout();

private:
	Node*					layout;					//主界面层资源
	Node*					recv;					//遮挡层资源

	Button*					start_game;				//开始游戏按钮
	Button*					btCancel;				//返回游戏按钮
	Button*					btCard;					//斗地主游戏按钮
	Button*					btMahjong;				//麻将游戏按钮
	Button*					btChess;				//象棋游戏按钮

	void					onStartHandler(Ref* sender);	//点击的处理事件
	void					onCancelHandler(Ref* sender);	//点击的处理事件
	void					onCardHandler(Ref* sender);		//点击的处理事件
	void					onMahjongHandler(Ref* sender);	//点击的处理事件
	void					onChessHandler(Ref* sender);	//点击的处理事件

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					FightChangeSence(float a);		//战斗切换界面回调函数

	UINT16					m_currentPartitionId;					//当前要进入的分区id

	//参数：服务器返回代码
	int						QueryPartitionInfoRes(void* pBuf);		//查询分区反馈结果

	//参数：服务器返回代码
	int						EnterPartitionRes(void* pBuf);		//进入分区反馈结果

};

class MainLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(MainLayer);
	~MainLayer();

	virtual void onEnter();							//初始化函数

	virtual list<LoadItem> getResourceList();			//取得资源加载列表

	virtual void releaseResource();					//释放资源

private:
	MainLayout*				mainLayout;		//场景的背景容器					

};

#endif
