#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "chessAndCards.h"
#include "csv/csv.h"

#define LOGIC_DELAY_TIME		1000

#define WASH_COUNT				3

enum PokerCount
{
	COUNT_1 = 1,
	COUNT_2 = 2,
};

enum PokerColor
{
	SPADES = 1,
	HEARTS = 2,
	CLUBS = 3,			//梅花
	DIAMONDS = 4
};

enum PokerPoint
{
	CHESS_A = 1,
	CHESS_2,
	CHESS_3,
	CHESS_4,
	CHESS_5,
	CHESS_6,
	CHESS_7,
	CHESS_8,
	CHESS_9,
	CHESS_10,
	CHESS_J,
	CHESS_Q,
	CHESS_K,
};

struct Poker
{
	PokerCount		count;			//第几幅扑克牌
	PokerColor		color;
	PokerPoint		point;
	bool			particularity;	//特殊性
	int				score;			//扑克牌对应的分数
};

enum ActionState
{
	HIT,			//出一张牌
	GET,			//摸一张牌
	PASS,			//过牌  禁手
	HIT_CHANGE,		//特殊牌  改变花色
	CHOOSE_COLOR,	//选择花色
};

struct ActionChess
{
	Poker			poker;			//打出的牌
	ActionState		state;
	PokerColor		color;			//选择的花色
};

enum GameState
{
	WASHING,		//洗牌阶段
	DEAL,			//发牌阶段
	ASKING,			//询问阶段
	ACTION,			//玩家行动阶段
	CALCULATE,		//结算阶段
	OVER,			//游戏结束
	PAUSEING,		//暂停阶段，等待玩家选择是否继续游戏
};

class WaitTime
{
public:
	WaitTime();
	void			StartWait(int time);	//开始等待  时间毫秒   如果时间为0，那么到下一次心跳的时候，将执行后面的代码
	bool			OnTime(int time);		//记录时间			true 时间已到
private:
	int				recordTime;				//记录的时间
	bool			state;					//是否等待
};

class BaseAI;

//发牌机   对牌局中的牌堆进行管理
class ChessLogic
{
public:
	ChessLogic();
	~ChessLogic();

	void					WashChess();						//洗牌函数
	Poker					GetPoker();							//取得一张扑克牌
	void					RecyclePoker(Poker poker);			//回收扑克
	int						GetDistributeCount();				//派发牌堆剩余牌数

private:
	void					initPoker();						//初始化扑克牌
	int						getPokerScore(PokerPoint point);	//取得扑克牌对应的分数	
	void					mixPoker();							//牌堆的牌打完之后  混合牌

private:
	vector<Poker*>			poker_vec;							//扑克牌  所有的
	list<Poker>				distribute_lst;						//派发list
	list<Poker>				recycle_lst;						//回收list

	list<Poker>::iterator	heap_iter;							//牌堆迭代器
};

//玩家逻辑控制器
class PlayerLogic
{
public:
	PlayerLogic(bool robot,int id);

	void					Reset();							//重置玩家状态
	bool					IsLose();							//是否已经输掉了比赛
	void					AddChess(Poker poker);
	void					DelChess(Poker poker);				//删除一张手牌
	bool					IsRobot();
	int						RemainCount();						//剩余手牌数量
	void					removeHandChess();					//清空手牌

	void					Execute(Poker poker);				//机器人的AI执行
	bool					CheckHandChess(Poker poker);		//检查手牌
	bool					CheckHitValid(Poker oldPoker, Poker hitPoker);//检查出牌是否有效

	bool					IsReady();							//是否准备完毕
	void					SetReadyState(bool ready);			//设置准备完毕状态
	int						GetId();							//获取id
	string					GetName();							//获取名字
	int						GetCurScore();						//获取当前得分
	int						GetAllScore();						//获取总得分
	void					AddScore();							//增加分数							

protected:

	bool					robot;			//是否是机器人
	bool					ready;			//是否准备完毕
	int						id;
	int						score;			//分数
	NameCsvItem*			nameItem;		//玩家名字

	BaseAI*					game_ai;		//AI算法

	list<Poker>				chess_lst;		//手牌
};

//主逻辑控制器
class CenterLogic : public GameMessage, GameTick
{
public:
	CenterLogic();

	~CenterLogic();																		//析构

	virtual void							OnTick(int time);
	virtual void							OnMessage(const int head, void* data);


	void									Reset();								//重置游戏
	void									HitChess(Poker poker);					//打出一张牌

private:

	GameState								game_state;								//当前游戏的进行状态
	bool									direction;								//当前的方向顺序		true 正向  false 逆向

	vector<PlayerLogic*>					player_vec;								//使用vector 方便操作
	vector<PlayerLogic*>::iterator			action_iter;							//当前正在动作的玩家

	PlayerLogic*							last_winner;							//上一次获得胜利的玩家
	Poker									poker;									//当前打出来的牌
	int										punish_count;							//惩罚次数
	WaitTime*								wait;
	ChessLogic*								chess_logic;							//牌逻辑

private:																	//构造

	void									registerMessage();
	void									unregisterMessage();

	void									initChess();							//初始化牌
	void									initPlayer();							//初始化玩家和机器人

	void									dealChess();							//发牌函数
	void									actionPlayer(bool start = false);		//确定行动的玩家

	void									processAction(ActionChess action);		//处理行动
	void									punish();								//惩罚函数

	void									calculateAction();						//结算行动
};

#endif
