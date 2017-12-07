#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"
#include "logic/GameLogic.h"
#include "msg/msgFight.h"

bool CardLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/card/CardScene.csb");
	this->addChild(layout);

	btChat = (Button*)layout->getChildByName("btChat");
	//添加事件监听
	btChat->addClickEventListener(CC_CALLBACK_1(CardLayout::onChatHandler, this));

	btGameMenu = (Button*)layout->getChildByName("btGameMenu");
	//添加事件监听
	btGameMenu->addClickEventListener(CC_CALLBACK_1(CardLayout::onGameMenuHandler, this));

	btPorkPile = (Button*)layout->getChildByName("btPorkPile");
	//添加事件监听
	btPorkPile->addClickEventListener(CC_CALLBACK_1(CardLayout::onPorkPile, this));

	lbUnionId = (Text*)layout->getChildByName("lbUnionId");
	char str[48] = {};
	sprintf(str, "%d", (*TcpLogic::GetInstance()->containerJoinTableInfo.begin())->joinId);
	lbUnionId->setString(str);

	PokerRecycleNode = (Node*)layout->getChildByName("PokerRecycleNode");

	lstColorChoose = (Node*)layout->getChildByName("lstColorChoose");

	Button* btSpades = (Button*)lstColorChoose->getChildByName("btSpades");
	//添加事件监听
	btSpades->setTag(PokerColor::SPADES);
	btSpades->addClickEventListener(CC_CALLBACK_1(CardLayout::onChooseColor, this));

	Button* btHearts = (Button*)lstColorChoose->getChildByName("btHearts");
	//添加事件监听
	btHearts->setTag(PokerColor::HEARTS);
	btHearts->addClickEventListener(CC_CALLBACK_1(CardLayout::onChooseColor, this));

	Button* btClubs = (Button*)lstColorChoose->getChildByName("btClubs");
	//添加事件监听
	btClubs->setTag(PokerColor::CLUBS);
	btClubs->addClickEventListener(CC_CALLBACK_1(CardLayout::onChooseColor, this));

	Button* btDiamonds = (Button*)lstColorChoose->getChildByName("btDiamonds");
	//添加事件监听
	btDiamonds->setTag(PokerColor::DIAMONDS);
	btDiamonds->addClickEventListener(CC_CALLBACK_1(CardLayout::onChooseColor, this));

	WindowColorTip = (Sprite*)layout->getChildByName("WindowColorTip");

	ColorTip = Sprite::createWithSpriteFrame(ResourceManager::GetInstance()->Get_resource_from_plist("fight/Colortip1.png"));
	ColorTip->setPosition(Vec2(WindowColorTip->getContentSize().width / 2, WindowColorTip->getContentSize().height / 2));
	WindowColorTip->addChild(ColorTip);

	player = (Layout*)layout->getChildByName("ProjectNode_Player");

	this->initAntimation();

	this->initPlayer();

	this->initChess();


	center_logic = new CenterLogic();

	return true;
}

CardLayout::CardLayout()
{
	recv = nullptr;

	this->registerMessage();
}

CardLayout::~CardLayout()
{
	this->unregisterMessage();

	delete center_logic;
	distribute_lst.clear();
	recycle_lst.clear();
	poker_vec.clear();
}

void CardLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_LAYOUT_PALYER_INFO, this);
	manager->Reg(MSG_LAYOUT_PALYER_READY, this);
	manager->Reg(MSG_LAYOUT_WASH_PORKS, this);
	manager->Reg(MSG_LAYOUT_DEAL_PORK, this);
	manager->Reg(MSG_LAYOUT_HIT_PORK, this);
	manager->Reg(MSG_LAYOUT_HIT_CHANGE, this);
	manager->Reg(MSG_LAYOUT_ACTION_CHESS, this);
	manager->Reg(MSG_LAYOUT_NO_ACTION_CHESS, this);
	manager->Reg(MSG_LAYOUT_VALIAD, this);
	manager->Reg(MSG_LAYOUT_INVALIAD, this);
	manager->Reg(MSG_LAYOUT_CALCULATE, this);
	manager->Reg(MSG_JOIN_CHANNEL_RES, this);
	manager->Reg(MSG_ERROR_RES, this);
}

void CardLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_LAYOUT_PALYER_INFO, this);
	manager->Unreg(MSG_LAYOUT_PALYER_READY, this);
	manager->Unreg(MSG_LAYOUT_WASH_PORKS, this);
	manager->Unreg(MSG_LAYOUT_DEAL_PORK, this);
	manager->Unreg(MSG_LAYOUT_HIT_PORK, this);
	manager->Unreg(MSG_LAYOUT_HIT_CHANGE, this);
	manager->Unreg(MSG_LAYOUT_ACTION_CHESS, this);
	manager->Unreg(MSG_LAYOUT_NO_ACTION_CHESS, this);
	manager->Unreg(MSG_LAYOUT_VALIAD, this);
	manager->Unreg(MSG_LAYOUT_INVALIAD, this);
	manager->Unreg(MSG_LAYOUT_CALCULATE, this);
	manager->Unreg(MSG_JOIN_CHANNEL_RES, this);
	manager->Unreg(MSG_ERROR_RES, this);
}

void CardLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void CardLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void CardLayout::initAntimation()
{
	Layout* hitInvalid = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("animation/hitInvalid.csb");
	this->addChild(hitInvalid);
	//hitInvalid->setVisible(false);

	hitInvalidAction = (ActionTimeline*)ResourceManager::GetInstance()->Get_ani_resource("animation/hitInvalid.csb");
	hitInvalid->runAction(hitInvalidAction);


	Layout* wash = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("animation/aniCard.csb");
	this->addChild(wash);

	cardAction = (ActionTimeline*)ResourceManager::GetInstance()->Get_ani_resource("animation/aniCard.csb");
	wash->runAction(cardAction);

	cardAction->setFrameEventCallFunc(CC_CALLBACK_1(CardLayout::onCardFrameEvent, this));
	//cardAction->setLastFrameCallFunc(CC_CALLBACK_0(CardLayout::onWashOverEvent, this));


}

void CardLayout::initPlayer()
{

	//由于暂时只是单机模式，所以这里写死，收到信息之后，初始化本地玩家以及机器人
	//玩家始终是在0的位置
	vector<PlayerUI*> vec(MAU_PLAYER_NUMBER);
	vec[0] = new PlayerUI();
	vec[0]->btReady = (Button*)(player->getChildByName("btReady"));
	vec[0]->btReady->addClickEventListener(CC_CALLBACK_1(CardLayout::onReadyHandler, this));
	vec[0]->btUnReady = (Button*)(player->getChildByName("btUnReady"));
	vec[0]->btUnReady->addClickEventListener(CC_CALLBACK_1(CardLayout::onUnReadyHandler, this));
	vec[0]->lbRoleNick = (Text*)(player->getChildByName("lbRoleNick"));
	vec[0]->lbScore = (Text*)(player->getChildByName("lbScore1"));
	vec[0]->cardsNode = (Node*)(player->getChildByName("PlayerNode"));
	//vec[0]->lbRoleNick->setString(TcpLogic::GetInstance()->m_pSEnterRoleRes->nick);
	//测试   直接4个机器人
	//vec[0] = new PlayerLogic(true);
	for (int i = 1; i < MAU_PLAYER_NUMBER; ++i)
	{
		vec[i] = new PlayerUI();
		char str[48];

		sprintf(str,"lbNickRole%d",i);
		vec[i]->lbRoleNick = (Text*)(player->getChildByName(str));

		sprintf(str, "Portrait%d", i);
		vec[i]->Portrait = (Sprite*)(player->getChildByName(str));

		sprintf(str, "lbReadyRole%d", i);
		vec[i]->lbReady = (Text*)(player->getChildByName(str));

		sprintf(str, "PlayerNode%d", i);
		vec[i]->cardsNode = (Node*)(player->getChildByName(str));

		sprintf(str, "lbLeftCardsCount%d", i);
		vec[i]->lbLeftCardsCount = (Text*)(player->getChildByName(str));

		sprintf(str, "lbScore%d", i+1);
		vec[i]->lbScore = (Text*)(player->getChildByName(str));

		vec[i]->leftCards = Sprite::createWithSpriteFrame(ResourceManager::GetInstance()->Get_resource_from_plist("Card/Leftcards1.png"));
		vec[i]->leftCards->setPosition(vec[i]->cardsNode->getPosition());
		vec[i]->leftCards->setVisible(false);
		this->addChild(vec[i]->leftCards);
	}
	playerUI_vec = vec;
	action_iter = playerUI_vec.begin();
}

void CardLayout::initChess()
{
	//牌的产生顺序是黑A，红A。。黑A。。。方k
	int index = 0;
	int count = 2;

	vector<SpriteButton*> vec(TOTAL_POKER_NUMBER);
	while (count > 0)
	{
		for (int color = 1; color <= POKER_COLOR_NUMBER; color++)
		{
			for (int point = 1; point <= POKER_POINT_COLOR; index++, point++)
			{
				char str[24];
				sprintf(str, "Card/%d%.2d.png", color, point);
				SpriteButton* button = SpriteButton::createWithFrame(ResourceManager::GetInstance()->Get_resource_from_plist(str));
				button->setPressedUpHandler(this, SEL_MenuHandler(&CardLayout::onCardHandler));
				button->setTag(POKER_TAG(count,color,point));
				button->setPosition(playerUI_vec[0]->cardsNode->getPosition());
				button->setIsShock(false);
				button->setVisible(false);
				this->addChild(button);
				vec[index] = button;
			}
		}
		count--;
	}

	poker_vec = vec;
}

void CardLayout::onChatHandler(Ref* sender)
{
	//lstChatRecv->removeAllChildren();
	//this->addRecvingLayer();
	//TcpLogic::GetInstance()->CreateRoleReq(2,0,"iampeople");
}

void CardLayout::onGameMenuHandler(Ref* sender)
{
	gameMenuLayout = GameMenuLayout::create();
	this->addChild(gameMenuLayout, INT32_MAX);
}

void CardLayout::onReadyHandler(Ref* sender)
{
	//playerUI_vec[0]->btReady->setVisible(false);
	//playerUI_vec[0]->btUnReady->setVisible(true);
	//发消息玩家请求准备
	this->addRecvingLayer();
	TcpLogic::GetInstance()->PlayerReadyReq(true);
	//char p[MSG_MAX_LENGTH] = {};
	//sReady*	ready = (sReady*)p;
	//ready->id = 0;
	//ready->isReady = true;
	//MsgManager::GetInstance()->Dispather(MSG_LOGIC_PALYER_READY, ready);
}

void CardLayout::onCardFrameEvent(Frame* frame)
{
	EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
	if (!evnt)
		return;

	std::string str = evnt->getEvent();

	if (str == "washOver")
	{
		//evnt->getNode()->setColor(Color3B(0, 0, 0));
		MsgManager::GetInstance()->Dispather(MSG_LOGIC_WASH_PORKS, nullptr);
	}
}

void CardLayout::onCardHandler(Ref* sender)
{
	SpriteButton* node = (SpriteButton*)sender;
	list<SpriteButton*> chess_lst = playerUI_vec[0]->chess_lst;
	list<SpriteButton*>::iterator iter = chess_lst.begin();
	while (iter != chess_lst.end())
	{
		if (*iter == node)
		{
			if ((*iter)->getPositionY() == playerUI_vec[0]->cardsNode->getPositionY())
			{
				(*iter)->setPositionY(playerUI_vec[0]->cardsNode->getPositionY() + 20);
			}
			else
			{
				(*iter)->setPositionY(playerUI_vec[0]->cardsNode->getPositionY());
				//发送消息给逻辑层判断能不能出这张牌
				ActionChess action;
				Poker poker;
				poker.count = (PokerCount)(POKER_COUNT((*iter)->getTag()));
				poker.color = (PokerColor)(POKER_COLOR((*iter)->getTag()));
				poker.point = (PokerPoint)(POKER_POINT((*iter)->getTag()));
				action.poker = poker;
				if (poker.point == PokerPoint::CHESS_J)
				{
					action.state = ActionState::HIT_CHANGE;
				}
				else
				{
					action.state = ActionState::HIT;
				}
				MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);
			}
		}
		else
		{
			(*iter)->setPositionY(playerUI_vec[0]->cardsNode->getPositionY());
		}
		iter++;
	}
}

void CardLayout::onChooseColor(Ref* sender)
{
	Node* node = (Node*)sender;
	lstColorChoose->setVisible(false);
	this->HitChangeChess(node->getTag());
}

void CardLayout::onPorkPile(Ref* sender)
{
	list<SpriteButton*> chess_lst = playerUI_vec[0]->chess_lst;
	list<SpriteButton*>::iterator iter = chess_lst.begin();
	while (iter != chess_lst.end())
	{
		(*iter)->setPositionY(playerUI_vec[0]->cardsNode->getPositionY());
		iter++;
	}

	playerUI_vec[0]->chess_lst = chess_lst;

	ActionChess action;
	action.state = ActionState::GET;
	MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);
}

void CardLayout::onUnReadyHandler(Ref* sender)
{
	//发消息玩家请求准备
	this->addRecvingLayer();
	TcpLogic::GetInstance()->PlayerReadyReq(false);
	//char p[MSG_MAX_LENGTH] = {};
	//sReady*	ready = (sReady*)p;
	//ready->id = 0;
	//ready->isReady = false;
	//MsgManager::GetInstance()->Dispather(MSG_LOGIC_PALYER_READY, ready);
}

int CardLayout::JoinChannelRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "加入聊天频道错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	return ret;
}
void CardLayout::ChangePlayerCards(int id)
{
	if (id == 0)
		return;

	int leftCards = (int)playerUI_vec[id]->chess_lst.size();
	if (leftCards == 0)
	{
		playerUI_vec[id]->lbLeftCardsCount->setVisible(false);
		playerUI_vec[id]->leftCards->setVisible(false);
		return;
	}
	else
	{
		playerUI_vec[id]->lbLeftCardsCount->setVisible(true);
		playerUI_vec[id]->leftCards->setVisible(true);
	}
	char str[48];
	sprintf(str, "%d", leftCards);
	playerUI_vec[id]->lbLeftCardsCount->setString(str);
	if (leftCards >= 3)
		leftCards = 3;
	sprintf(str, "Card/Leftcards%d.png", leftCards);
	playerUI_vec[id]->leftCards->setSpriteFrame(ResourceManager::GetInstance()->Get_resource_from_plist(str));
}
void CardLayout::WashChess()
{
	//隐藏控件
	playerUI_vec[0]->btReady->setVisible(false);
	playerUI_vec[0]->btUnReady->setVisible(false);
	for (int i = 1; i != playerUI_vec.size(); ++i)
	{
		playerUI_vec[i]->lbReady->setVisible(false);
	}

	//洗牌动画
	cardAction->play("wash",false);
}

void CardLayout::AddChess(void* data)
{
	sGetPork *getPork = (sGetPork *)data;

	Poker poker = getPork->poker[0];
	SpriteButton * button = (SpriteButton *)this->getChildByTag(POKER_TAG(poker.count, poker.color, poker.point));

	if (getPork->id == 0)
	{
		button->setVisible(true);
		list<SpriteButton*> chess_lst = playerUI_vec[getPork->id]->chess_lst;
		list<SpriteButton*>::iterator iter = chess_lst.begin();
		while (iter != chess_lst.end())
		{
			if (POKER_VALUE((*iter)->getTag()) > POKER_VALUE(button->getTag()))
			{
				button->setPositionX((*iter)->getPositionX() - (*iter)->getContentSize().width / 2);
				for (list<SpriteButton*>::iterator iter2 = chess_lst.begin(); iter2 != iter; ++iter2)
				{
					(*iter2)->setPositionX((*iter2)->getPositionX() - (*iter2)->getContentSize().width / 2);
				}

				(*iter)->setPositionX((*iter)->getPositionX() + (*iter)->getContentSize().width / 2);

				for (list<SpriteButton*>::iterator iter3 = --chess_lst.end(); iter3 != iter; --iter3)
				{
					(*iter3)->setPositionX((*iter3)->getPositionX() + (*iter3)->getContentSize().width / 2);
				}

				chess_lst.insert(iter, button);
				playerUI_vec[getPork->id]->chess_lst = chess_lst;
				break;
			}
			iter++;
		}
		//新来的牌最大
		if (iter == chess_lst.end())
		{
			for (list<SpriteButton*>::iterator iter2 = chess_lst.begin(); iter2 != chess_lst.end(); ++iter2)
			{
				(*iter2)->setPositionX((*iter2)->getPositionX() - (*iter2)->getContentSize().width / 2);
			}

			if (chess_lst.size() != 0)
			{
				list<SpriteButton*>::iterator iter3 = --chess_lst.end();
				button->setPositionX((*iter3)->getPositionX() + (*iter3)->getContentSize().width);
			}

			chess_lst.push_back(button);
			playerUI_vec[getPork->id]->chess_lst = chess_lst;
		}
	}
	else
	{
		playerUI_vec[getPork->id]->chess_lst.push_back(button);
		this->ChangePlayerCards(getPork->id);
	}
	//发牌动画

	if (cardAction != nullptr&&cardAction->isPlaying())
	{
		cardAction->stop();
	}
	if (cardAction != nullptr&&!cardAction->isPlaying())
	{
		char str[24];
		sprintf(str, "get%d", getPork->id + 1);
		cardAction->play(str, false);
	}
}

void CardLayout::HitChess(void* data)
{
	sHitPork *hitPork = (sHitPork *)data;

	Poker poker = hitPork->poker[0];

	if (hitPork->id != POKER_PILE_ID)
	{
		list<SpriteButton*> chess_lst = playerUI_vec[hitPork->id]->chess_lst;
		list<SpriteButton*>::iterator iter = chess_lst.begin();
		while (iter != chess_lst.end())
		{
			if ((*iter)->getTag() == POKER_TAG(poker.count, poker.color, poker.point))
			{
				//移动打完牌剩余牌的位置
				if (hitPork->id == 0)
				{
					for (list<SpriteButton*>::iterator iter2 = chess_lst.begin(); iter2 != iter; ++iter2)
					{
						(*iter2)->setPositionX((*iter2)->getPositionX() + (*iter2)->getContentSize().width / 2);
					}

					for (list<SpriteButton*>::iterator iter3 = --chess_lst.end(); iter3 != iter; --iter3)
					{
						(*iter3)->setPositionX((*iter3)->getPositionX() - (*iter3)->getContentSize().width / 2);
					}
				}

				chess_lst.erase(iter);
				playerUI_vec[hitPork->id]->chess_lst = chess_lst;
				this->ChangePlayerCards(hitPork->id);
				//打牌动画
				if (cardAction != nullptr&&cardAction->isPlaying())
				{
					cardAction->stop();
				}
				if (cardAction != nullptr&&!cardAction->isPlaying())
				{
					char str[24];
					sprintf(str, "hit%d", hitPork->id + 1);
					cardAction->play(str, false);
				}
				break;
			}
			iter++;
		}
	}
	SpriteButton* button = (SpriteButton *)this->getChildByTag(POKER_TAG(poker.count, poker.color, poker.point));
	button->setVisible(true);
	recycle_lst.push_back(button);
	button->setLocalZOrder((int)recycle_lst.size());
	button->setPosition(PokerRecycleNode->getPosition());
}

void CardLayout::HitChangeChess(int pokerColor)
{
	char str[24];
	sprintf(str, "fight/Colortip%d.png", pokerColor);
	ColorTip->setSpriteFrame(ResourceManager::GetInstance()->Get_resource_from_plist(str));
	WindowColorTip->setVisible(true);

	ActionChess action;
	action.state = ActionState::CHOOSE_COLOR;
	action.color = (PokerColor)pokerColor;
	MsgManager::GetInstance()->Dispather(MSG_ACTION_CHESS, &action);

}

void CardLayout::ResetChess(void* data)
{
	//重置棋牌
	recycle_lst.clear();
	GameOverState gameOverState = ((sCalculate*)data)->gameOverState;
	int winnerId = ((sCalculate*)data)->winnerId;
	for (int i = 0; i != ((sCalculate*)data)->rolesCount; i++)
	{
		char str[24];
		sprintf(str, "%d", (((sCalculate*)data)->roles+i)->sumScore);
		playerUI_vec[i]->lbScore->setString(str);
	}
	if (gameOverState == GameOverState::NO_OVER)
	{
		if (!((sCalculate*)data)->roles->isLose)
		{
			calculateLayout = CalculateLayout::create();
			this->addChild(calculateLayout, INT32_MAX);
			MsgManager::GetInstance()->Dispather(MSG_LAYOUT_CALCULATE_SCORE, data);
			playerUI_vec[0]->btReady->setVisible(true);
		}
		playerUI_vec[0]->chess_lst.clear();
		for (int i = 1; i != ((sCalculate*)data)->rolesCount; i++)
		{
			playerUI_vec[i]->chess_lst.clear();
			playerUI_vec[i]->lbLeftCardsCount->setVisible(false);
			playerUI_vec[i]->leftCards->setVisible(false);
			if ((((sCalculate*)data)->roles + i)->isRobot && !(((sCalculate*)data)->roles + i)->isLose)
			{
				playerUI_vec[i]->lbReady->setVisible(true);
				char p[MSG_MAX_LENGTH] = {};
				sReady*	ready = (sReady*)p;
				ready->id = i;
				ready->isReady = true;
				//MsgManager::GetInstance()->Dispather(MSG_LOGIC_PALYER_READY, (void*)ready);
			}
		}

		vector<SpriteButton*>::iterator iter = poker_vec.begin();
		while (iter != poker_vec.end())
		{
			(*iter)->setPosition(playerUI_vec[0]->cardsNode->getPosition());
			(*iter)->setEnable(true);
			(*iter)->setVisible(false);
			iter++;
		}

		btPorkPile->setEnabled(false);
	}
	else if (gameOverState == GameOverState::LOSE)
	{
		gameOverLayout = GameOverLayout::create();
		this->addChild(gameOverLayout, INT32_MAX);
		MsgManager::GetInstance()->Dispather(MSG_LAYOUT_CALCULATE_SCORE, data);
	}
	else if (gameOverState == GameOverState::WIN)
	{
		gameOverLayout = GameOverLayout::create();
		this->addChild(gameOverLayout, INT32_MAX);
		MsgManager::GetInstance()->Dispather(MSG_LAYOUT_CALCULATE_SCORE, data);
	}
}

void CardLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("连接超时", "提示");
		break;
	case MSG_LAYOUT_PALYER_INFO:
	{
		int id = ((sPlayerInfo*)data)->id;
		bool robot = ((sPlayerInfo*)data)->isRobot;
		bool ready = ((sPlayerInfo*)data)->isReady;
		const char *name = ((sPlayerInfo*)data)->name.c_str();
		playerUI_vec[id]->lbScore->setVisible(true);
		playerUI_vec[id]->lbRoleNick->setVisible(true);
		playerUI_vec[id]->lbRoleNick->setString(name);
		if (robot)
		{
			playerUI_vec[id]->lbReady->setVisible(true);
		}
		if (id != 0)
		{
			playerUI_vec[id]->Portrait->setVisible(true);
			playerUI_vec[id]->lbReady->setVisible(ready);
		}
		else
		{
			playerUI_vec[id]->btReady->setVisible(!ready);
			playerUI_vec[id]->btUnReady->setVisible(ready);
		}
		break;
	}
	case MSG_LAYOUT_PALYER_READY:
	{
		bool ready = ((sReady*)data)->isReady;
		int id = ((sReady*)data)->id;
		if (id == 0)
		{
			this->removeRecvingLayer();
			playerUI_vec[0]->btReady->setVisible(!ready);
			playerUI_vec[0]->btUnReady->setVisible(ready);
		}
		else
		{
			playerUI_vec[id]->lbReady->setVisible(ready);
		}
		break;
	}
	case MSG_LAYOUT_WASH_PORKS:
		this->WashChess();
		break;
	case MSG_LAYOUT_DEAL_PORK:
		this->AddChess(data);
		break;
	case MSG_LAYOUT_HIT_PORK:
	{
		this->HitChess(data);
		playerUI_vec[0]->setEnable(false);
		btPorkPile->setEnabled(false);
		WindowColorTip->setVisible(false);
		break;
	}
	case MSG_LAYOUT_HIT_CHANGE:
		if (((sChangeColorPork*)data)->id != 0)
		{
			this->HitChangeChess(((sChangeColorPork*)data)->color);
		}
		else
		{
			lstColorChoose->setVisible(true);
		}
		break;
	case MSG_LAYOUT_ACTION_CHESS:
		playerUI_vec[0]->setEnable(true);
		btPorkPile->setEnabled(true);
		break;
	case MSG_LAYOUT_NO_ACTION_CHESS:
		playerUI_vec[0]->setEnable(false);
		btPorkPile->setEnabled(false);
		break;
	case MSG_LAYOUT_INVALIAD:
		hitInvalidAction->gotoFrameAndPlay(0, false);
		break;
	case MSG_LAYOUT_CALCULATE:
		this->ResetChess(data);
		break;
	case MSG_JOIN_CHANNEL_RES:
		this->JoinChannelRes(data);
		break;
	case MSG_ERROR_RES:
	{
		this->removeRecvingLayer();
		char str[48];
		sprintf(str, "操作错误,错误码：%d", *(UINT16*)data);
		MessageBox(str, "提示");
		break;
	}
	default:
		break;
	}
}

