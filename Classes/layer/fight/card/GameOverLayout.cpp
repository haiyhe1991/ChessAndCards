#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"
#include "msg/msgFight.h"

bool GameOverLayout::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/card/GameOverNode.csb");
	this->addChild(layout);

	btContinue = (Button*)layout->getChildByName("btContinue");
	//Ìí¼ÓÊÂ¼þ¼àÌý
	btContinue->addClickEventListener(CC_CALLBACK_1(GameOverLayout::onCloseHandler, this));

	win = (Sprite*)layout->getChildByName("win");

	winClickTip = (Sprite*)layout->getChildByName("winClickTip");

	vector<ScoreUI*> vec(MAU_PLAYER_NUMBER);
	for (int i = 0; i < MAU_PLAYER_NUMBER; ++i)
	{
		vec[i] = new ScoreUI();
		char str[48];

		sprintf(str, "lbPlayer%d", i + 1);
		vec[i]->lbPlayer = (Text*)(layout->getChildByName(str));

		sprintf(str, "lbScore%d", i + 1);
		vec[i]->lbScore = (Text*)(layout->getChildByName(str));
	}
	scoreUI_vec = vec;
	return true;
}

GameOverLayout::GameOverLayout()
{
	MsgManager::GetInstance()->Reg(MSG_LAYOUT_CALCULATE_SCORE, this);
}

GameOverLayout::~GameOverLayout()
{
	MsgManager::GetInstance()->Unreg(MSG_LAYOUT_CALCULATE_SCORE, this);
}

void GameOverLayout::onCloseHandler(Ref* sender)
{
	RoomLayer* roomLayer = RoomLayer::create();
	roomLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, roomLayer);
}

void GameOverLayout::calculateScore(void *data)
{
	char str[48];
	for (int i = 0; i < ((sCalculate*)data)->rolesCount; ++i)
	{
		scoreUI_vec[(((sCalculate*)data)->roles + i)->id]->lbPlayer->setString((((sCalculate*)data)->roles + i)->name.c_str());
		sprintf(str, "%d + %d = %d", (((sCalculate*)data)->roles + i)->curScore, (((sCalculate*)data)->roles + i)->sumScore - (((sCalculate*)data)->roles + i)->curScore, (((sCalculate*)data)->roles + i)->sumScore);
		scoreUI_vec[(((sCalculate*)data)->roles + i)->id]->lbScore->setString(str);
	}
	if (((sCalculate*)data)->gameOverState == GameOverState::LOSE)
	{
		win->setVisible(false);
		winClickTip->setVisible(false);
	}

}

void GameOverLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_LAYOUT_CALCULATE_SCORE:
		this->calculateScore(data);
		break;
	default:
		break;
	}
}