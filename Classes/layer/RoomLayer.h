#ifndef __ROOM_SCENE_H__
#define __ROOM_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

class JionTableLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(JionTableLayout);

	JionTableLayout();
	~JionTableLayout();

private:
	Node*					layout;								//主界面层资源
	Node*					recv;								//遮挡层资源

	ListView*				lstViewNum;
	Button*					btClose;							//推出窗口
	Button*					btRemove;							//继续游戏按钮
	Button*					btReWrite;							//继续游戏按钮
	Button*					btNum[10];

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					onCloseHandler(Ref* sender);		//点击的处理事件
	void					onRemoveHandler(Ref* sender);		//点击的处理事
	void					onReWriteHandler(Ref* sender);		//点击的处理事
	void					onNumHandler(Ref* sender);			//点击的处理事

	void					FightChangeSence(float a);		//战斗切换界面回调函数

	int						JionTableRes(void* pBuf);		//创建角色反馈结果

};

//主界面层
class RoomLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(RoomLayout);

	RoomLayout();
	~RoomLayout();

private:
	Node*					layout;					//主界面层资源
	Node*					room;
	Node*					recv;					//遮挡层资源
	JionTableLayout*		jionTableLayout;		//创建角色层资源

	Button*					btCreateTable;				//创建牌桌按钮
	Button*					btJionTable;				//加入牌桌按钮

	void					onCreateTableHandler(Ref* sender);		//点击的处理事件
	void					onJionTableHandler(Ref* sender);	//点击的处理事件

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					FightChangeSence(float a);		//战斗切换界面回调函数

	int						CreateTableRes(void* pBuf);		//进入角色反馈结果

};

class RoomLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RoomLayer);
	~RoomLayer();

	virtual void onEnter();							//初始化函数

	virtual list<LoadItem> getResourceList();			//取得资源加载列表

	virtual void releaseResource();					//释放资源

private:
	RoomLayout*				roomLayout;		//场景的背景容器					

};

#endif
