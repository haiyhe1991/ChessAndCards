#include "ai.h"

void GameAI::Execute(list<Poker> handlst, Poker poker)
{
	//if (peakPoker(handlst, poker.point))
	//{
	//	//可以顶牌，直接顶牌
	//	list<Poker> same = samePoint(handlst, poker.point);
	//	send(ActionState::HIT, getPeakPoker(handlst, same));
	//	return;
	//}
	//前一张是特殊牌
	if (poker.particularity && (poker.point == PokerPoint::CHESS_7 || poker.point == PokerPoint::CHESS_8))
	{
		list<Poker> same = samePoint(handlst, poker.point);
		if (same.size() != 0)
		{
			//可以顶牌，直接顶牌
			send(ActionState::HIT, getPeakPoker(handlst, same));
			return;
		}
		else
		{
			//不能顶牌，那么直接接受惩罚
			pass();
			return;
		}
	}
	else
	{
		//到此 特殊情况处理完毕
		//由于策划没有要求，先进行同花色的出牌。   即如果有相同花色的牌，优先出相同花色的牌，没有，再找相同点数的牌，最后再考虑J
		if (checkSameColor(handlst, poker))
		{
			send(ActionState::HIT, getColorPoker(sameColor(handlst, poker.color)));
			return;
		}
		//AI到此  没有相同花色的牌  那么就开始寻找相同点数的牌
		if (checkSamePoint(handlst, poker))
		{
			send(ActionState::HIT, getPointPoker(handlst, samePoint(handlst, poker.point)));
			return;
		}
		//AI到此，只能打J出去了，前提是上一张不是出的J
		if (poker.point != PokerPoint::CHESS_J)
		{
			if (samePointWithCount(handlst, PokerPoint::CHESS_J) > 0)
			{
				handleChangeColor(handlst);
				return;
			}
		}
		//完全没有可以出的牌了，只能摸牌了
		send();
	}
}

//计算顶牌
Poker GameAI::getPeakPoker(list<Poker> handlst, list<Poker> samelst)
{
	//只有一张，直接打
	if (samelst.size() == 1)
	{
		return *samelst.begin();
	}

	Poker result;
	int	count = 0;

	list<Poker>::iterator iter = samelst.begin();
	while (iter != samelst.end())
	{
		int same_count = sameColorWithCount(handlst, (*iter).color);
		if (same_count > count)
		{
			count = same_count;
			result = *iter;
		}
		iter++;
	}

	return result;
}

bool GameAI::checkSameColor(list<Poker> handlst, Poker poker)
{
	list<Poker> sameLst = sameColor(handlst, poker.color);
	if (sameLst.size() == 0)
	{
		return false;
	}
	list<Poker>::iterator iter = sameLst.begin();
	while (iter != sameLst.end())
	{
		if ((*iter).point != PokerPoint::CHESS_J)	//只要不是特殊的牌，就可以视为合理出牌
		{
			return true;
		}
		iter++;
	}
	return false;
}

Poker GameAI::getColorPoker(list<Poker> colorlst)
{
	//根据牌的分数出牌   优先出分数大的牌
	Poker result;
	int score = 0;
	list<Poker>::iterator iter = colorlst.begin();
	while (iter != colorlst.end())
	{
		if ((*iter).score > score)
		{
			result = (*iter);
			score = result.score;
		}
		iter++;
	}
	return result;
}

bool GameAI::checkSamePoint(list<Poker> handlst, Poker poker)
{
	list<Poker> sameLst = samePoint(handlst, poker.point);
	if (sameLst.size() == 0)
	{
		return false;
	}
	list<Poker>::iterator iter = sameLst.begin();
	while (iter != sameLst.end())
	{
		if ((*iter).point != PokerPoint::CHESS_J)	//只要不是特殊的牌，就可以视为合理出牌
		{
			return true;
		}
		iter++;
	}
	return false;
}

Poker GameAI::getPointPoker(list<Poker> handlst, list<Poker> pointlst)
{
	Poker result;
	int count = 0;
	list<Poker>::iterator iter = pointlst.begin();
	while (iter != pointlst.end())
	{
		int color_count = sameColorWithCount(handlst, (*iter).color);
		if (color_count > count)
		{
			count = color_count;
			result = (*iter);
		}
		iter++;
	}
	return result;
}

void GameAI::handleChangeColor(list<Poker> handlst)
{
	PokerColor color = getMaxColor(handlst);
	list<Poker>::iterator iter = handlst.begin();
	while (iter != handlst.end())
	{
		if ((*iter).point == PokerPoint::CHESS_J)
		{
			send((*iter), color);
			return;
		}
		iter++;
	}
}

