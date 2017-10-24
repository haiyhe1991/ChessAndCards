#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

class GameRuleLayout : public Layer
{
public:
	virtual bool init();

	CREATE_FUNC(GameRuleLayout);

	GameRuleLayout();
	~GameRuleLayout();

private:
	Node*					layout;								//主界面层资源

	Button*					btClose;							//退出窗口

	void					onCloseHandler(Ref* sender);		//点击的处理事件

};

class GameOverLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(GameOverLayout);

	GameOverLayout();
	~GameOverLayout();

private:

	struct ScoreUI
	{
		Text*					lbPlayer;						//角色昵称label
		Text*					lbScore;						//角色得分label
	};

	Node*					layout;								//主界面层资源

	vector<ScoreUI*>		scoreUI_vec;						//使用vector 方便操作

	Button*					btContinue;							//退出窗口
	Sprite*					win;
	Sprite*					winClickTip;

	void					onCloseHandler(Ref* sender);		//点击的处理事件

	void					calculateScore(void *data);			//计算得分

};

class CalculateLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(CalculateLayout);

	CalculateLayout();
	~CalculateLayout();

private:

	struct ScoreUI
	{
		Text*					lbPlayer;						//角色昵称label
		Text*					lbScore;						//角色得分label
	};

	Node*					layout;								//主界面层资源

	vector<ScoreUI*>		scoreUI_vec;						//使用vector 方便操作

	Button*					btClose;							//退出窗口

	void					onCloseHandler(Ref* sender);		//点击的处理事件

	void					calculateScore(void *data);			//计算得分

};

class GameMenuLayout : public Layer
{
public:
	virtual bool init();

	CREATE_FUNC(GameMenuLayout);

	GameMenuLayout();
	~GameMenuLayout();

private:
	Node*					layout;								//主界面层资源

	Button*					btExitFight;						//推出窗口
	Button*					btContinue;							//继续游戏按钮
	Button*					btSetting;							//性别复选框男
	Button*					btRule;								//性别复选框女
	Button*					btAchieve;							//昵称
	GameRuleLayout*			gameRuleLayout;						//创建游戏规则资源

	void					onCloseHandler(Ref* sender);		//点击的处理事件
	void					onContinueHandler(Ref* sender);		//点击的处理事件
	void					onSettingHandler(Ref* sender);		//点击的处理事件
	void					onRuleHandler(Ref* sender);			//点击的处理事件
	void					onAchieveHandler(Ref* sender);		//点击的处理事件

};

class CenterLogic;
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

	CenterLogic*				center_logic;		//中心逻辑对象

	struct PlayerUI
	{
		Text*					lbRoleNick;			//角色昵称label
		Text*					lbReady;			//准备label
		Button*					btReady;			//准备按钮
		Button*					btUnReady;			//取消准备按钮
		Node*					cardsNode;			//手牌位置
		list<SpriteButton*>		chess_lst;			//手牌
		Sprite*					leftCards;			//剩余牌展示
		Text*					lbLeftCardsCount;	//剩余牌数量
		Text*					lbScore;			//得分

		void					setEnable(bool enable)
		{
			list<SpriteButton*>::iterator iter = chess_lst.begin();
			while (iter != chess_lst.end())
			{
				(*iter)->setEnable(enable);
				iter++;
			}
		}
	};
	Node*					layout;					//主界面层资源
	Layout*					player;					//角色层
	ActionTimeline*			hitInvalidAction;		//出牌无效动画
	ActionTimeline*			cardAction;				//洗牌动画
	Node*					recv;					//遮挡层资源
	GameMenuLayout*			gameMenuLayout;			//创建菜单层资源
	CalculateLayout*		calculateLayout;		//创建结算层资源
	GameOverLayout*			gameOverLayout;			//创建结算层资源

	vector<PlayerUI*>		playerUI_vec;			//使用vector 方便操作
	vector<PlayerUI*>::iterator			action_iter;							//当前正在动作的玩家

	Button*					btChat;					//聊天按钮
	Button*					btGameMenu;				//游戏菜单按钮
	Button*					btPorkPile;				//牌堆按钮
	vector<SpriteButton*>			poker_vec;							//扑克牌  所有的
	list<SpriteButton*>				distribute_lst;						//派发list
	list<SpriteButton*>				recycle_lst;						//回收list
	Node*					PokerRecycleNode;						//回收牌堆节点
	Sprite*					WindowColorTip;
	Node*					lstColorChoose;
	Sprite*					ColorTip;

	void					onChatHandler(Ref* sender);			//点击的处理事件
	void					onGameMenuHandler(Ref* sender);		//点击的处理事件
	void					WashChess();						//洗牌
	void					AddChess(void* data);				//发牌
	void					HitChess(void* data);				//打牌
	void					HitChangeChess(int pokerColor);		//变颜色
	void					ResetChess(void* data);				//重置打牌
	void					onCardFrameEvent(Frame* frame);		//洗牌结束动画

	void					onReadyHandler(Ref* sender);		//点击的处理事件
	void					onUnReadyHandler(Ref* sender);		//点击的处理事件
	void					onCardHandler(Ref* sender);			//点击的处理事件
	void					onChooseColor(Ref* sender);			//点击的处理事件
	void					onPorkPile(Ref* sender);			//点击的处理事件

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//添加接收界面
	void					removeRecvingLayer();			//移除接收界面

	void					initAntimation();				//初始化动画
	void					initPlayer();					//初始化玩家
	void					initChess();					//初始化牌
	void					ChangePlayerCards(int id);		//改变玩家手牌的显示

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