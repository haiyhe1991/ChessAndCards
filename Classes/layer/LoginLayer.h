#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//主界面按钮层
class LoginLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(LoginLayout);

	~LoginLayout();

private:
	Node*					layout;					//按钮层资源
	Node*					recv;					//遮挡层资源

	TextField*				tbName;					//姓名输入框
	TextField*				tbPassword;				//密码输入框

	Button*					register_game;			//注册游戏按钮
	Button*					login_game;				//开始游戏按钮

	void					onRegHandler(Ref* sender);		//点击的处理事件
	void					onLoginHandler(Ref* sender);	//点击的处理事件

	void					registerMessage();
	void					unregisterMessage();

	void					removeRecvingLayer();			//移除接收界面

	//参数：服务器返回代码
	int						RegistUserRes(void* pBuf);		//注册用户反馈结果

	//参数：服务器返回代码
	int						LoginUserRes(void* pBuf);		//登录用户反馈结果
};

class LoginLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(LoginLayer);
	~LoginLayer();

	virtual void onEnter();							//初始化函数

	virtual list<LoadItem> getResourceList();			//取得资源加载列表

	virtual void releaseResource();					//释放资源

private:
	LoginLayout*				loginLayout;		//场景的背景容器					

};

#endif
