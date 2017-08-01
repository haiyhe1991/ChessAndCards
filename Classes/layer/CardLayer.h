#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//主界面层
class CardLayout : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(CardLayout);

private:
	Node*					layout;					//主界面层资源

	ListView*				lstChatRecv;				//收信息框

	Button*					bt_clear;				//清除收到信息按钮
	Button*					bt_send;				//发送信息按钮
	Button*					btCard;					//斗地主游戏按钮
	Button*					btMahjong;				//麻将游戏按钮
	Button*					btChess;				//象棋游戏按钮

	void					onClearHandler(Ref* sender);	//点击的处理事件
	void					onSendHandler(Ref* sender);		//点击的处理事件
	void					onCardHandler(Ref* sender);		//点击的处理事件
	void					onMahjongHandler(Ref* sender);	//点击的处理事件
	void					onChessHandler(Ref* sender);	//点击的处理事件
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