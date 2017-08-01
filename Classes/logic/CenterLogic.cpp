#include "GameLogic.h"

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
	TickManager::GetInstance()->Reg(this);
}

CenterLogic::~CenterLogic()
{
	delete chess_logic;
	TickManager::GetInstance()->Unreg(this);
}

void CenterLogic::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_START_GAME, this);
	manager->Reg(MSG_ACTION_CHESS, this);
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
			wait->StartWait(3000);			//等待3秒钟后进入发牌阶段。此处也有无限等待，一直等到UI那边播放完动画之后，再进行下一步
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
			//只有机器人才进行AI处理，如果是人，就等待人进行操作
			if ((*action_iter)->IsRobot())
			{
				(*action_iter)->Execute(poker);
			}
			else
			{
				//发送消息，通知玩家开始行动
			}
			wait->StartWait(INT_MAX);//由于是单机，无限等待事件响应 直到玩家响应为止
			break;
		}
		case CALCULATE:
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
}

void CenterLogic::Reset()
{
	//direction = true;
	//chess_logic->WashChess();
	////重置玩家
	//for (int i = 0; i < MAU_PLAYER_NUMBER; ++i)
	//{
	//	player_vec[i]->Reset();
	//}
}

void CenterLogic::initChess()
{
	chess_logic = new ChessLogic;
}

void CenterLogic::initPlayer()
{
	//由于暂时只是单机模式，所以这里写死，收到信息之后，初始化本地玩家以及机器人
	//玩家始终是在0的位置
	//vector<PlayerLogic*> vec(MAU_PLAYER_NUMBER);
	////vec[0] = new PlayerLogic(false);
	////测试   直接4个机器人
	//vec[0] = new PlayerLogic(true);
	//for (int i = 1; i < MAU_PLAYER_NUMBER; ++i)
	//{
	//	vec[i] = new PlayerLogic(true);
	//}
	//player_vec = vec;
}

void CenterLogic::dealChess()
{
	//int count = FIRST_HAND_NUMBER;
	//while (count > 0)
	//{
	//	for (int i = 0; i < int(player_vec.size()); i++)
	//	{
	//		player_vec[i]->AddChess(chess_logic->GetPoker());
	//	}
	//	count--;
	//}

	//actionPlayer(true);

	//punish_count = 0;
	////直接翻张牌当公牌
	//HitChess(chess_logic->GetPoker());

	//wait->StartWait(3000);
	////切换当前游戏状态
	//game_state = GameState::ASKING;
}

void CenterLogic::actionPlayer(bool start/* = false*/)
{
	if (start)
	{
		if (last_winner == nullptr)
		{
			//发完牌之后，确定谁先进行行动
			int count = RandomHelper::random_int(0, int(player_vec.size()) - 1);
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
			action_iter--;
			if (action_iter == player_vec.end())
			{
				action_iter = player_vec.end()--;
			}
		}
	}
}

void CenterLogic::HitChess(Poker poker)
{
	this->poker = poker;
	//打过的牌 放入牌堆
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
	if (action.state == ActionState::PASS) //过牌
	{
		punish();
		actionPlayer();
	}
	else if (action.state == ActionState::HIT) //操作是打一张牌
	{
		if (player->CheckHandChess(action.poker))
		{
			player->DelChess(action.poker);
			HitChess(action.poker);
			//下一个行动的玩家
			actionPlayer();
		}
		//这里是错误的处理。。。以后再说吧  只要保证AI不出错，一般不会有问题
	}
	else if (action.state == ActionState::GET) //玩家摸牌
	{
		player->AddChess(chess_logic->GetPoker());
	}
	else if (action.state == ActionState::HIT_CHANGE)
	{

	}
	wait->StartWait(1000);
}

void CenterLogic::punish()
{
	while (this->punish_count > 0)
	{
		(*action_iter)->AddChess(chess_logic->GetPoker());
		(*action_iter)->AddChess(chess_logic->GetPoker());
		punish_count--;
	}
}
