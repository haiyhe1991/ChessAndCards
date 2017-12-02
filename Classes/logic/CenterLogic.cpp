#include "GameLogic.h"
#include "msg/msgFight.h"

CenterLogic::CenterLogic()
{
	wait = new WaitTime;
	last_winner = nullptr;
	game_state = GameState::PAUSEING;
	direction = true;
	registerMessage();
	SetDelayTime(LOGIC_DELAY_TIME);
	initChess();
	initPlayer();
	changePlayer();
	TickManager::GetInstance()->Reg(this);
}

CenterLogic::~CenterLogic()
{
	delete chess_logic;
	TickManager::GetInstance()->Unreg(this);
	unregisterMessage();
	vector<PlayerLogic*>::iterator iter = player_vec.begin();
	while (iter != player_vec.end())
	{
		delete (*iter);
		iter++;
	}
}

void CenterLogic::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_START_GAME, this);
	manager->Reg(MSG_ACTION_CHESS, this);
	manager->Reg(MSG_LOGIC_PALYER_READY, this);
	manager->Reg(MSG_LOGIC_WASH_PORKS, this);
	manager->Reg(MSG_JION_TABLE_RES, this);
	manager->Reg(MSG_PALYER_READY_RES, this);
}

void CenterLogic::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_START_GAME, this);
	manager->Unreg(MSG_ACTION_CHESS, this);
	manager->Unreg(MSG_LOGIC_PALYER_READY, this);
	manager->Unreg(MSG_LOGIC_WASH_PORKS, this);
	manager->Unreg(MSG_JION_TABLE_RES, this);
	manager->Unreg(MSG_PALYER_READY_RES, this);
}

void CenterLogic::OnTick(int time)
{
	if (wait->OnTime(time))
	{
		switch (game_state)
		{
		case WASHING:
		{
			chess_logic->WashChess();
			MsgManager::GetInstance()->Dispather(MSG_LAYOUT_WASH_PORKS, nullptr);
			wait->StartWait(INT_MAX);			//�ȴ�3���Ӻ���뷢�ƽ׶Ρ��˴�Ҳ�����޵ȴ���һֱ�ȵ�UI�Ǳ߲����궯��֮���ٽ�����һ��
			game_state = GameState::DEAL;
			break;
		}
		case DEAL:
		{
			dealChess();
			break;
		}
		case ASKING:
		{
			//ֻ�л����˲Ž���AI����������ˣ��͵ȴ��˽��в���
			if ((*action_iter)->IsRobot())
			{				
				MsgManager::GetInstance()->Dispather(MSG_LAYOUT_NO_ACTION_CHESS, nullptr);
				(*action_iter)->Execute(poker);
				//wait->StartWait(1000);
			}
			else
			{
				//������Ϣ��֪ͨ��ҿ�ʼ�ж�

				MsgManager::GetInstance()->Dispather(MSG_LAYOUT_ACTION_CHESS, nullptr);
				wait->StartWait(INT_MAX);//�����ǵ��������޵ȴ��¼���Ӧ ֱ�������ӦΪֹ
			}
			
			break;
		}
		case CALCULATE:
			game_state = GameState::PAUSEING;
			calculateAction();
			break;
		case OVER:
			break;
		case PAUSEING:
			break;
		default:
			break;
		}
	}
}

void CenterLogic::OnMessage(const int head, void* data)
{
	if (head == MSG_START_GAME)
	{
		game_state = GameState::WASHING;
	}
	else if (head == MSG_ACTION_CHESS)
	{
		ActionChess action = *((ActionChess*)data);
		processAction(action);
	}
	else if (head == MSG_LOGIC_PALYER_READY)
	{
		sReady ready = *((sReady*)data);
		TcpLogic::GetInstance()->PlayerReadyReq(ready.isReady);
		//player_vec[ready.id]->SetReadyState(ready.isReady);
		//if (ready.isReady)
		//{
		//	int i = 0;
		//	for (; i < MAU_PLAYER_NUMBER; ++i)
		//	{
		//		if (!player_vec[i]->IsReady() && !player_vec[i]->IsLose())
		//			break;
		//	}
		//	//������������Ҷ�׼����Ϸ�����Ϣ����ʾ��ϴ��
		//	if (i == MAU_PLAYER_NUMBER)
		//	{
		//		for (int j = 0; j < MAU_PLAYER_NUMBER; ++j)
		//		{
		//			//û��������׼��״̬ȡ��
		//			if (!player_vec[j]->IsLose())
		//			{
		//				player_vec[j]->SetReadyState(false);
		//			}
		//		}
		//		game_state = GameState::WASHING;
		//	}
		//}
	}
	else if (head == MSG_LOGIC_WASH_PORKS)
	{
		wait->StartWait(1000);
	}
	else if (head == MSG_JION_TABLE_RES)
	{
		UINT16 ret = *(UINT16*)data;
		if (ret == MAU_OK)
		{
			changePlayer();
		}
	}
	else if (head == MSG_PALYER_READY_RES)
	{
		SReadyTableRes *ret = (SReadyTableRes*)data;
		int id = -1;
		map<int, BYTE>::iterator it = tableIdMap.begin();
		while (it != tableIdMap.end())
		{
			if (it->second == ret->tableId)
			{
				id = it->first;
				break;
			}
			it++;
		}
		if (id == -1)
		{
			UINT16 retCmd = MAU_ID_NOT_EXIST_IN_CHANNEL;
			MsgManager::GetInstance()->Dispather(MessageHead::MSG_ERROR_RES, &retCmd);
		}
		player_vec[id]->SetReadyState(ret->ready);
		char p[MSG_MAX_LENGTH] = {};
		sReady*	ready = (sReady*)p;
		ready->id = id;
		ready->isReady = ret->ready;
		MsgManager::GetInstance()->Dispather(MessageHead::MSG_LAYOUT_PALYER_READY, ready);
	}

}

void CenterLogic::Reset()
{
	direction = true;
	chess_logic->WashChess();
	//�������
	for (int i = 0; i < MAU_PLAYER_NUMBER; ++i)
	{
		player_vec[i]->Reset();
	}
}

void CenterLogic::initChess()
{
	chess_logic = new ChessLogic;
}

void CenterLogic::initPlayer()
{
	list<SJoinTableInfo*>::iterator lstIter = TcpLogic::GetInstance()->containerJoinTableInfo.begin();
	while (lstIter != TcpLogic::GetInstance()->containerJoinTableInfo.end())//����map
	{
		if ((*lstIter)->userId == TcpLogic::GetInstance()->m_pSEnterRoleRes->userId)
		{
			BYTE tableId = (*lstIter)->tableId;
			map<int, BYTE>::iterator it = tableIdMap.find(0);
			if (it == tableIdMap.end())
			{
				tableIdMap.insert(make_pair(0, tableId));
			}
			else
			{
				it->second = tableId;
			}
			it = tableIdMap.find(1);
			tableId++;
			if (tableId == MAU_PLAYER_NUMBER)
			{
				tableId = 0;
			}
			if (it == tableIdMap.end())
			{
				tableIdMap.insert(make_pair(1, tableId));
			}
			else
			{
				it->second = tableId;
			}
			it = tableIdMap.find(2);
			tableId++;
			if (tableId == MAU_PLAYER_NUMBER)
			{
				tableId = 0;
			}
			if (it == tableIdMap.end())
			{
				tableIdMap.insert(make_pair(2, tableId));
			}
			else
			{
				it->second = tableId;
			}
			it = tableIdMap.find(3);
			tableId++;
			if (tableId == MAU_PLAYER_NUMBER)
			{
				tableId = 0;
			}
			if (it == tableIdMap.end())
			{
				tableIdMap.insert(make_pair(3, tableId));
			}
			else
			{
				it->second = tableId;
			}

		}
		lstIter++;
	}
}

void CenterLogic::changePlayer()
{
	list<SJoinTableInfo*>::iterator lstIter = TcpLogic::GetInstance()->containerJoinTableInfo.begin();
	while (lstIter != TcpLogic::GetInstance()->containerJoinTableInfo.end())//�����ɫ
	{
		map<int, BYTE>::iterator it = tableIdMap.begin();
		while (it != tableIdMap.end())
		{
			if (it->second == (*lstIter)->tableId)
			{
				bool HavePalyer = false;
				vector<PlayerLogic*>::iterator vecIter = player_vec.begin();
				while (vecIter != player_vec.end())
				{
					if ((*vecIter)->GetId() == it->first)
					{
						HavePalyer = true;
						break;
					}
					vecIter++;
				}
				if (!HavePalyer)
				{
					PlayerLogic* player = new PlayerLogic(false, it->first, (*lstIter)->nick);
					//������ʱֻ�ǵ���ģʽ����������д�����յ���Ϣ֮�󣬳�ʼ����������Լ�������
					//���ʼ������0��λ��
					player_vec.push_back(player);// = vec;
				}
				break;
			}
			it++;
		}
		lstIter++;
	}
}

void CenterLogic::dealChess()
{
	int count = FIRST_HAND_NUMBER;
	while (count > 0)
	{
		for (int i = 0; i < int(player_vec.size()); i++)
		{
			if (!player_vec[i]->IsLose())
			{
				player_vec[i]->AddChess(chess_logic->GetPoker());
			}
		}
		count--;
	}

	actionPlayer(true);

	punish_count = 0;
	//ֱ�ӷ����Ƶ�����
	Poker pokerTemp = chess_logic->GetPoker();
	HitChess(pokerTemp);
	//������ʾ����ʾ�ƶѵõ�����
	char p[MSG_MAX_LENGTH] = {};
	sGetPork*	getPork = (sGetPork*)p;
	getPork->poker[0] = pokerTemp;
	getPork->id = POKER_PILE_ID;
	MsgManager::GetInstance()->Dispather(MSG_LAYOUT_HIT_PORK, (void*)getPork);
	if (pokerTemp.point == PokerPoint::CHESS_J)
	{
		ActionChess action;
		action.color = (PokerColor)RandomHelper::random_int(0, PokerColor::DIAMONDS - 1);
		MsgManager::GetInstance()->Dispather(MSG_LAYOUT_HIT_CHANGE, &action);
	}

	wait->StartWait(3000);
	//�л���ǰ��Ϸ״̬
	game_state = GameState::ASKING;
}

void CenterLogic::calculateAction()
{
	char p[MSG_MAX_LENGTH] = {};
	sCalculate*	calculate = (sCalculate*)p;
	calculate->rolesCount = player_vec.size();
	calculate->winnerId = 0;
	calculate->gameOverState = GameOverState::NO_OVER;
	int i = 0;
	int loser = 0;
	int winnerIndex = 0;
	vector<PlayerLogic*>::iterator iter = player_vec.begin();
	while (iter != player_vec.end())
	{
		(*iter)->AddScore();//����÷ֲ�������ܷ���
		(calculate->roles + i)->id = (*iter)->GetId();
		(calculate->roles + i)->isRobot = (*iter)->IsRobot();
		(calculate->roles + i)->isLose = (*iter)->IsLose();
		(calculate->roles + i)->name = (*iter)->GetName();
		(calculate->roles + i)->curScore = (*iter)->GetCurScore();
		(calculate->roles + i)->sumScore = (*iter)->GetAllScore();
		(*iter)->removeHandChess();//�Ƴ�����
		if ((*iter)->IsLose())
		{
			loser++;
		}
		else
		{
			winnerIndex = i;
		}
		iter++;
		i++;
	}
	if (loser == player_vec.size() - 1)
	{
		if (winnerIndex == 0)
		{
			calculate->gameOverState = GameOverState::WIN;
		}
		else
		{
			calculate->gameOverState = GameOverState::LOSE;
		}
		last_winner = player_vec[winnerIndex];
	}
	//֪ͨ��ʾ�����
	MsgManager::GetInstance()->Dispather(MSG_LAYOUT_CALCULATE, p);
}

void CenterLogic::actionPlayer(bool start/* = false*/)
{
	if (start)
	{
		if (last_winner == nullptr)
		{
			//������֮��ȷ��˭�Ƚ����ж�
			int count = RandomHelper::random_int(0, int(player_vec.size()) - 1);
			CCLOG("start::%d",count);
			action_iter = player_vec.begin();
			while (count > 0)
			{
				action_iter++;
				count--;
			}
		}
		else
		{
			vector<PlayerLogic*>::iterator iter = player_vec.begin();
			while (iter != player_vec.end())
			{
				if ((*iter) == last_winner)
				{
					action_iter = iter;
					return;
				}
			}
		}
	}
	else
	{
		if (direction)
		{
			action_iter++;
			if (action_iter == player_vec.end())
			{
				action_iter = player_vec.begin();
			}
		}
		else
		{
			if (action_iter == player_vec.begin())
			{
				action_iter = player_vec.end();
			}
			action_iter--;
		}
	}
	//���������Ѿ������������ô���ֵ���һ�����
	if ((*action_iter)->IsLose())
	{
		actionPlayer();
	}
}

void CenterLogic::HitChess(Poker poker)
{
	this->poker = poker;
	//������� �����ƶ�
	chess_logic->RecyclePoker(poker);

	if (poker.point == PokerPoint::CHESS_7)
	{
		punish_count += 1;
	}
	else
	{
		if (PokerPoint::CHESS_A == poker.point)
		{
			direction = !direction;
		}
		punish_count = 0;
	}
}

void CenterLogic::processAction(ActionChess action)
{
	PlayerLogic* player = *action_iter;
	if (action.state == ActionState::PASS) //����
	{
		punish();
		//��һ���ж������
		actionPlayer();
	}
	else if (action.state == ActionState::HIT) //�����Ǵ�һ����
	{
		if (player->CheckHandChess(action.poker))
		{
			if (player->CheckHitValid(this->poker, action.poker))
			{
				player->DelChess(action.poker);
				HitChess(action.poker);
				if (player->RemainCount() == 0)
				{
					//�������׶�
					game_state = GameState::CALCULATE;
				}
				else
				{
					//��һ���ж������
					actionPlayer();
				}
			}
			else
			{
				//������ʾ�������Ч
				MsgManager::GetInstance()->Dispather(MSG_LAYOUT_INVALIAD, nullptr);
			}
		}
		//�����Ǵ���Ĵ��������Ժ���˵��  ֻҪ��֤AI������һ�㲻��������
	}
	else if (action.state == ActionState::GET) //�������
	{
		if (this->poker.particularity && (this->poker.point == PokerPoint::CHESS_7 || this->poker.point == PokerPoint::CHESS_8))
		{
			punish();
		}
		else
		{
			player->AddChess(chess_logic->GetPoker());
		}
		//��һ���ж������
		actionPlayer();
	}
	else if (action.state == ActionState::HIT_CHANGE)
	{
		if (player->CheckHandChess(action.poker))
		{
			if (player->CheckHitValid(this->poker, action.poker))
			{
				player->DelChess(action.poker);
				HitChess(action.poker);
				if (player->RemainCount() == 0)
				{
					//�������׶�
					game_state = GameState::CALCULATE;
				}
				else
				{
					//������ʾ����ʾ�ı����ɫ����ѡ��ɫ
					char p[MSG_MAX_LENGTH] = {};
					sChangeColorPork*	changeColorPork = (sChangeColorPork*)p;
					changeColorPork->id = player->GetId();
					changeColorPork->color = action.color;
					MsgManager::GetInstance()->Dispather(MSG_LAYOUT_HIT_CHANGE, changeColorPork);
				}
			}
			else
			{
				//������ʾ�������Ч
				MsgManager::GetInstance()->Dispather(MSG_LAYOUT_INVALIAD, nullptr);
			}
		}
	}
	else if (action.state == ActionState::CHOOSE_COLOR)
	{
		this->poker.color = action.color;
		//��һ���ж������
		actionPlayer();
	}
	wait->StartWait(1000);
}

void CenterLogic::punish()
{
	this->poker.particularity = false;
	while (this->punish_count > 0)
	{
		(*action_iter)->AddChess(chess_logic->GetPoker());
		(*action_iter)->AddChess(chess_logic->GetPoker());
		punish_count--;
	}
}
