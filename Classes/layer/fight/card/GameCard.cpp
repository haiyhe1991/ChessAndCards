#include "GameCard.h"

void GameCard::initCard(CardColor color, CardNumber number)
{
	this->color = color;
	this->number = number;
}

CardColor GameCard::getColor()
{
	return this->color;
}

CardNumber GameCard::getNumber()
{
	return this->number;
}

bool GameCard::isSameColor(CardColor color)
{
	return this->color == color;
}


/******CardManager*******/

CardManager::~CardManager()
{
	container.clear();
}

void CardManager::createCard()
{
	//Çå¿Õ
	container.clear();

	index = 0;

	//vector<GameCard> vec(TOTAL_CARD_NUMBER);
	//int idx = 0;
	//for (int color = 1; color <= CARD_COLOR_NUMBER; color++)
	//{
	//for (int card = 1; card <= CARD_SINGLE_NUMBER; card++, idx++)
	//{
	//vec[idx].initCard(CardColor(color), CardNumber(card));
	//}
	//}
	//container = vec;
}

void CardManager::washCard()
{
	/*GameCard tmp;
	for (int i = 0; i < RANDOM_WISH_TIME; i++)
	{
	tmp.initCard(container[i].getColor(), container[i].getNumber());
	int rand = RandomHelper::random_int(0, 104);
	container[i].initCard(container[rand].getColor(), container[rand].getNumber());
	container[rand].initCard(tmp.getColor(), tmp.getNumber());
	}*/
}

GameCard* CardManager::deliverCard()
{
	//if ((index + 1) < container.size)
	//{
	return &container[index++];
	//}
}
