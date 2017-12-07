#include "GameLogic.h"
#include "ai/ai.h"
#include "msg/msgFight.h"
PlayerLogic::PlayerLogic(bool robot, int id, bool ready, char* name)
{
	this->robot = robot;
	this->id = id;
	Reset();
	//随机名字

	if (IsRobot())
	{
		nameItem = CsvData::GetInstance()->nameCsv->GetRandomName();
		game_ai = new GameAI();
		this->ready = true;
	}
	else
	{
		nameItem = new NameCsvItem;
		nameItem->name = name;// TcpLogic::GetInstance()->m_pSEnterRoleRes->nick;
		this->ready = false;
	}
	//发送消息给显示层，显示头像和名字
	char p[MSG_MAX_LENGTH] = {};
	sPlayerInfo*	playerInfo = (sPlayerInfo*)p;
	playerInfo->name = nameItem->name;
	playerInfo->isRobot = IsRobot();
	playerInfo->id = GetId();
	playerInfo->isReady = ready;
	this->ready = ready;
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_LAYOUT_PALYER_INFO, (void *)playerInfo);
}

bool PlayerLogic::IsRobot()
{
	return robot;
}

void PlayerLogic::AddChess(Poker poker)
{
	chess_lst.push_back(poker);
	//发给显示层显示玩家得到的牌
	char p[MSG_MAX_LENGTH] = {};
	sGetPork*	getPork = (sGetPork*)p;
	getPork->id = GetId();
	getPork->poker[0] = poker;
	MsgManager::GetInstance()->Dispather(MSG_LAYOUT_DEAL_PORK, (void*)getPork);
}

void PlayerLogic::DelChess(Poker poker)
{
	list<Poker>::iterator iter = chess_lst.begin();
	while (iter != chess_lst.end())
	{
		if ((*iter).count == poker.count && (*iter).color == poker.color && (*iter).point == poker.point)
		{
			chess_lst.erase(iter);
			//发给显示层显示玩家打出的牌
			char p[MSG_MAX_LENGTH] = {};
			sHitPork*	hitPork = (sHitPork*)p;
			hitPork->id = GetId();
			hitPork->poker[0] = poker;
			MsgManager::GetInstance()->Dispather(MSG_LAYOUT_HIT_PORK, (void*)hitPork);
			return;
		}
		iter++;
	}
}

bool PlayerLogic::CheckHandChess(Poker poker)
{
	list<Poker>::iterator iter = chess_lst.begin();
	while (iter != chess_lst.end())
	{
		if ((*iter).count == poker.count && (*iter).color == poker.color && (*iter).point == poker.point)
		{
			return true;
		}
		iter++;
	}
	return false;
}

bool PlayerLogic::CheckHitValid(Poker oldPoker, Poker hitPoker)
{
	switch (oldPoker.point)
	{
	case PokerPoint::CHESS_J:
		return hitPoker.point != PokerPoint::CHESS_J&&hitPoker.color == oldPoker.color;
	case PokerPoint::CHESS_7:
	case PokerPoint::CHESS_8:
		return hitPoker.point == oldPoker.point || (!oldPoker.particularity && (hitPoker.color == oldPoker.color || hitPoker.point == PokerPoint::CHESS_J));
	default:
		return hitPoker.point == oldPoker.point || hitPoker.color == oldPoker.color || hitPoker.point == PokerPoint::CHESS_J;
	}
	return false;
}

bool PlayerLogic::IsLose()
{
	return this->score >= 100;
}

int PlayerLogic::RemainCount()
{
	return this->chess_lst.size();
}

void PlayerLogic::removeHandChess()
{
	this->chess_lst.clear();
}

void PlayerLogic::Reset()
{
	this->removeHandChess();
	this->score = 0;
}

void PlayerLogic::Execute(Poker poker)
{
	//如果需要给AI加上等待时间，可以在这里添加
	game_ai->Execute(chess_lst, poker);
}

bool PlayerLogic::IsReady()
{
	return this->ready;
}

void PlayerLogic::SetReadyState(bool ready)
{
	this->ready = ready;
}

int PlayerLogic::GetId()
{
	return this->id;
}

string PlayerLogic::GetName()
{
	return this->nameItem->name;
}

int PlayerLogic::GetCurScore()
{
	int score = 0;
	list<Poker>::iterator iter = chess_lst.begin();
	while (iter != chess_lst.end())
	{
		score += (*iter).score;
		iter++;
	}

	return score;
}

int PlayerLogic::GetAllScore()
{
	return this->score;
}

void PlayerLogic::AddScore()
{
	this->score += this->GetCurScore();
}