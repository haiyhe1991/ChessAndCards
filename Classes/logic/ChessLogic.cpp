#include "GameLogic.h"

ChessLogic::ChessLogic()
{
	initPoker();
}

ChessLogic::~ChessLogic()
{
	distribute_lst.clear();
	recycle_lst.clear();
	vector<Poker*>::iterator iter = poker_vec.begin();
	while (iter != poker_vec.end())
	{
		delete (*iter);
		iter++;
	}
	poker_vec.clear();
}

/************************************************************************/
/* public method                                                        */
/************************************************************************/

void ChessLogic::WashChess()
{
	distribute_lst.clear();
	recycle_lst.clear();
	//����������ֿ��٣��ͷŵ�tick��ִ�о����ˡ�
	int count = WASH_COUNT;
	while (count > 0)
	{
		srand(timeGetTime());
		random_shuffle(poker_vec.begin(), poker_vec.end());
		count--;
	}
	vector<Poker*>::iterator iter = poker_vec.begin();
	while (iter != poker_vec.end())
	{
		(*iter)->particularity = true;//�����Ƶ���������Ϊ��Ч
		distribute_lst.push_back(*(*iter));
		iter++;
	}
	heap_iter = distribute_lst.begin();
}

Poker ChessLogic::GetPoker()
{
	list<Poker>::iterator iter = heap_iter;
	if (++iter == distribute_lst.end())
	{
		mixPoker();
	}
	return *heap_iter++;
}

void ChessLogic::RecyclePoker(Poker poker)
{
	recycle_lst.push_back(poker);
}

int ChessLogic::GetDistributeCount()
{
	return distribute_lst.size();
}

/************************************************************************/
/* private method                                                       */
/************************************************************************/

void ChessLogic::initPoker()
{
	int index = 0;
	int count = 2;

	vector<Poker*> vec(TOTAL_POKER_NUMBER);
	while (count > 0)
	{
		for (int color = 1; color <= POKER_COLOR_NUMBER; color++)
		{
			for (int point = 1; point <= POKER_POINT_COLOR; index++, point++)
			{
				Poker* poker = new Poker;
				poker->count = (PokerCount)count;
				poker->color = (PokerColor)color;
				poker->point = (PokerPoint)point;
				poker->particularity = true;
				poker->score = getPokerScore(poker->point);
				vec[index] = poker;
			}
		}
		count--;
	}
	poker_vec = vec;
}

int ChessLogic::getPokerScore(PokerPoint point)
{
	switch (point)
	{
	case CHESS_A:
		return 11;
	case CHESS_J:
		return 20;
	case CHESS_Q:
		return 3;
	case CHESS_K:
		return 4;
	}
	return int(point);
}

void ChessLogic::mixPoker()
{
	if (recycle_lst.size() > 1)
	{
		vector<Poker> vec(recycle_lst.size());
		list<Poker>::iterator iter2 = --recycle_lst.end();
		Poker poker = *iter2;
		recycle_lst.erase(iter2);//������һ�����ó�������ϡ�
		list<Poker>::iterator iter = recycle_lst.begin();
		int index = 0;
		while (iter != recycle_lst.end())
		{
			vec[index] = *iter;
			iter++;
			index++;
		}
		int count = WASH_COUNT;
		while (count > 0)
		{
			random_shuffle(vec.begin(), vec.end());
			count--;
		}
		vector<Poker>::iterator vecItor = vec.begin();
		while (vecItor != vec.end())
		{
			distribute_lst.push_back(*vecItor);
			vecItor++;
		}
		recycle_lst.clear();
		recycle_lst.push_back(poker);
	}
}

