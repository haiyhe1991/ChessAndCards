#ifndef __ROLE_SCENE_H__
#define __ROLE_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

class CreateRoleLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(CreateRoleLayout);

	CreateRoleLayout();
	~CreateRoleLayout();

private:
	Node*					layout;								//主界面层资源
	Node*					recv;								//遮挡层资源

	Button*					btClose;							//推出窗口
	Button*					btContinue;							//继续游戏按钮
	CheckBox*				ckbMan;								//性别复选框男
	CheckBox*				ckbWoman;							//性别复选框女
	TextField*				tbNick;								//昵称
	Text*					lbOccuId;							//职业名

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					onCloseHandler(Ref* sender);		//点击的处理事件
	void					onContinueHandler(Ref* sender);		//点击的处理事件
	void					ManSelectedEvent(Ref *pSender, CheckBox::EventType type);		//复选框男选择监听事件
	void					WomanSelectedEvent(Ref *pSender, CheckBox::EventType type);		//复选框女选择监听事件

	int						CreateRoleRes(void* pBuf);		//创建角色反馈结果

};

//主界面层
class RoleLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(RoleLayout);

	RoleLayout();
	~RoleLayout();

private:
	Node*					layout;					//主界面层资源
	Node*					recv;					//遮挡层资源
	CreateRoleLayout*		creatRolelayout;		//创建角色层资源
	int						roleCount;				//角色个数

	Button*					btStartGame;				//开始游戏按钮
	Button*					btCreateRole;				//创建角色按钮
	Button*					btLeft;						//左移按钮
	Button*					btRight;					//右移按钮
	PageView*				pageView_SelectPage;		//换页容器

	void					onStartHandler(Ref* sender);		//点击的处理事件
	void					onCreateRoleHandler(Ref* sender);	//点击的处理事件
	void					onLeftHandler(Ref* sender);			//点击的处理事件
	void					onRightHandler(Ref* sender);		//点击的处理事件
	void					pageViewEvent(Ref *pSender, PageView::EventType type);


	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					SenceLoad();					//场景加载回调函数
	void					PageCurIndexChange();			//页面变化改变左右button可用状态

	void					FightChangeSence(float a);		//战斗切换界面回调函数

	int						EnterRoleRes(void* pBuf);		//进入角色反馈结果

	int						JoinChannelRes(void* pBuf);		//加入聊天频道反馈结果



};

class RoleLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RoleLayer);
	~RoleLayer();

	virtual void onEnter();							//初始化函数

	virtual list<LoadItem> getResourceList();			//取得资源加载列表

	virtual void releaseResource();					//释放资源

private:
	RoleLayout*				roleLayout;		//场景的背景容器					

};

#endif
