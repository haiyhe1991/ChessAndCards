#ifndef __AI_H__
#define __AI_H__

#include "chessAndCards.h"
#include "logic/GameLogic.h"

//AI的基类
class BaseAI
{
public:
	virtual void		Execute(list<Poker> handlst, Poker poker) = 0;		//AI的执行函数

protected:

	list<Poker>			samePoint(list<Poker> handlst, PokerPoint point);			//找到相同点数的牌
	int					samePointWithCount(list<Poker> handlst, PokerPoint point);	//找到相同点数的牌 返回数量
	list<Poker>			sameColor(list<Poker> handlst, PokerColor color);			//找到相同花色的牌
	int					sameColorWithCount(list<Poker> handlst, PokerColor color);	//找到相同花色的牌 返回数量
	//virtual bool		peakPoker(list<Poker> handlst, PokerPoint point);			//计算顶牌
	void				pass();														//过牌

	void				send();														//摸牌
	void				send(ActionState state, Poker poker);						//发送消息
	void				send(Poker poker, PokerColor color);						//打出特殊牌

	PokerColor			getMaxColor(list<Poker> handlst);							//取得牌最多的颜色

private:
	bool				compareColorCount(list<Poker> handlst, PokerColor color, int &count);		//比较颜色牌的数量

};

class GameAI : public BaseAI
{
public:
	virtual void		Execute(list<Poker> handlst, Poker poker);

protected:
	bool				checkSameColor(list<Poker> handlst, Poker poker);			//检查能不能相同点数的牌
	bool				checkSamePoint(list<Poker> handlst, Poker poker);			//检查有没有相同点数的牌

	Poker				getPeakPoker(list<Poker> handlst, list<Poker> samelst);		//顶牌的出牌计算

	Poker				getColorPoker(list<Poker> colorlst);						//计算出按花色出牌的牌
	Poker				getPointPoker(list<Poker> handlst, list<Poker> pointlst);	//计算出按点数出牌的牌

	void				handleChangeColor(list<Poker> handlst);						//打出改变颜色的牌

};

#endif
