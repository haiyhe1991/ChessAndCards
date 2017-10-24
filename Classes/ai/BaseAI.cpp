#include "ai.h"
#include "msg/message.h"

list<Poker> BaseAI::samePoint(list<Poker> handlst, PokerPoint point)
{
	list<Poker> result;
	list<Poker>::iterator iter = handlst.begin();
	while (iter != handlst.end())
	{
		if ((*iter).point == point && (*iter).point != PokerPoint::CHESS_J)
		{
			result.push_back((*iter));
		}
		iter++;
	}
	return result;
}

int BaseAI::samePointWithCount(list<Poker> handlst, PokerPoint point)
{
	int result = 0;
	list<Poker>::iterator iter = handlst.begin();
	while (iter != handlst.end())
	{
		if ((*iter).point == point)
		{
			result++;
		}
		iter++;
	}
	return result;
}

list<Poker> BaseAI::sameColor(list<Poker> handlst, PokerColor color)
{
	list<Poker> result;
	list<Poker>::iterator iter = handlst.begin();
	while (iter != handlst.end())
	{
		if ((*iter).color == color && (*iter).point != PokerPoint::CHESS_J)
		{
			result.push_back((*iter));
		}
		iter++;
	}
	return result;
}

int BaseAI::sameColorWithCount(list<Poker> handlst, PokerColor color)
{
	int result = 0;
	list<Poker>::iterator iter = handlst.begin();
	while (iter != handlst.end())
	{
		if ((*iter).color == color && (*iter).point != PokerPoint::CHESS_J)
		{
			result++;
		}
		iter++;
	}
	return result;
}

//bool BaseAI::peakPoker(list<Poker> handlst, PokerPoint point)
//{
//	if (point == PokerPoint::CHESS_7 || point == PokerPoint::CHESS_8)
//	{
//		list<Poker> same = samePoint(handlst, point);
//		return same.size() != 0;
//	}
//	return false;
//}

void BaseAI::pass()
{
	ActionChess action;
	action.state = ActionState::PASS;
	MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);
}

void BaseAI::send()
{
	ActionChess action;
	action.state = ActionState::GET;
	MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);
}

void BaseAI::send(ActionState state, Poker poker)
{
	ActionChess action;
	action.state = state;
	action.poker = poker;
	MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);
}

void BaseAI::send(Poker poker, PokerColor color)
{
	ActionChess action;
	action.state = ActionState::HIT_CHANGE;
	action.poker = poker;
	action.color = color;
	MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);
}

PokerColor BaseAI::getMaxColor(list<Poker> handlst)
{
	PokerColor color = PokerColor::SPADES;
	int count = 0;
	if (compareColorCount(handlst, PokerColor::CLUBS, count))
	{
		color = PokerColor::CLUBS;
	}
	if (compareColorCount(handlst, PokerColor::DIAMONDS, count))
	{
		color = PokerColor::DIAMONDS;
	}
	if (compareColorCount(handlst, PokerColor::HEARTS, count))
	{
		color = PokerColor::HEARTS;
	}
	if (compareColorCount(handlst, PokerColor::SPADES, count))
	{
		color = PokerColor::SPADES;
	}
	return color;
}

bool BaseAI::compareColorCount(list<Poker> handlst, PokerColor color, int &count)
{
	bool flag = false;
	int tmp = sameColorWithCount(handlst, color);
	if (tmp > count)
	{
		count = tmp;
		flag = true;
	}
	return flag;
}

