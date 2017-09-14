#ifndef __PARTITION_SCENE_H__
#define __PARTITION_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//主界面层
class PartitionLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(PartitionLayout);

	PartitionLayout();
	~PartitionLayout();

private:
	Node*					layout;					//主界面层资源
	Node*					recv;					//遮挡层资源

	Text*					txtUserName;			//用户名label
	Text*					txtSelectPartition;		//选择好的分区label
	Button*					start_game;				//开始游戏按钮
	Button*					btCancel;				//返回游戏按钮
	ListView*				lstPartition;			//分区按钮列表容器
	list<Button*>			btPartitionLst;			//分区按钮列表
	Button*					btMahjong;				//麻将游戏按钮
	Button*					btChess;				//象棋游戏按钮

	void					onStartHandler(Ref* sender);	//点击的处理事件
	void					onCancelHandler(Ref* sender);	//点击的处理事件
	void					onPartionHandler(Ref* sender);	//点击的处理事件

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					SenceLoad(float a);				//场景加载回调函数
	void					FightChangeSence(float a);		//战斗切换界面回调函数

	UINT16					m_currentPartitionId;					//当前要进入的分区id

	//参数：服务器返回代码
	int						QueryPartitionInfoRes(void* pBuf);		//查询分区反馈结果

	//参数：服务器返回代码
	int						EnterPartitionRes(void* pBuf);		//进入分区反馈结果

};

class PartitionLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(PartitionLayer);
	~PartitionLayer();

	virtual void onEnter();							//初始化函数

	virtual list<LoadItem> getResourceList();			//取得资源加载列表

	virtual void releaseResource();					//释放资源

private:
	PartitionLayout*				partitionLayout;		//场景的背景容器					

};

#endif
