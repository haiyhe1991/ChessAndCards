#ifndef __GAME_CARD_H__
#define __GAME_CARD_H__

#include "chessAndCards.h"

enum CardColor
{
	SPADES = 1,
	HEARTS,
	CLUBS,
	DIAMONDS
};

enum CardState
{
	HEAP = 1,		//在牌堆中
	HAND,			//在玩家手上
};

enum CardNumber
{
	CARD_3 = 1,
	CARD_4,
	CARD_5,
	CARD_6,
	CARD_7,
	CARD_8,
	CARD_9,
	CARD_10,
	CARD_J,
	CARD_Q,
	CARD_K,
	CARD_A,
	CARD_2,
	CARD_SJ,
	CARD_BJ
};

enum DealCardStyle
{
	StartDealCard,
	AddTempDealCard,
	PunlishCard,
};

//牌
class GameCard
{
public:
	GameCard()
	{
		state = CardState::HEAP;
	};

	void						initCard(CardColor color, CardNumber number);

	CardColor					getColor();

	CardNumber					getNumber();

	CardState					getState();

	void						changeState(CardState state);

	bool						isSameColor(CardColor color);

private:

	CardColor					color;

	CardNumber					number;

	CardState					state;

};

//牌管理器
class CardManager
{
public:
	~CardManager();

	void						createCard();		//创建牌

	void						washCard();			//洗牌

	GameCard*					deliverCard();		//发牌函数

private:
	vector<GameCard>			container;

	int							index;

};

#endif
