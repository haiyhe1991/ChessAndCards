#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool GameMenuLayout::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/card/GameMenuNode.csb");
	this->addChild(layout);

	btContinue = (Button*)layout->getChildByName("btContinue");
	//����¼�����
	btContinue->addClickEventListener(CC_CALLBACK_1(GameMenuLayout::onContinueHandler, this));

	btSetting = (Button*)layout->getChildByName("btSetting");
	//����¼�����
	btSetting->addClickEventListener(CC_CALLBACK_1(GameMenuLayout::onSettingHandler, this));

	btExitFight = (Button*)layout->getChildByName("btExitFight");
	//����¼�����
	btExitFight->addClickEventListener(CC_CALLBACK_1(GameMenuLayout::onCloseHandler, this));

	btRule = (Button*)layout->getChildByName("btRule");
	//����¼�����
	btRule->addClickEventListener(CC_CALLBACK_1(GameMenuLayout::onRuleHandler, this));

	btAchieve = (Button*)layout->getChildByName("btAchieve");
	//����¼�����
	btAchieve->addClickEventListener(CC_CALLBACK_1(GameMenuLayout::onAchieveHandler, this));

	return true;
}

GameMenuLayout::GameMenuLayout()
{

}

GameMenuLayout::~GameMenuLayout()
{

}

void GameMenuLayout::onContinueHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}

void GameMenuLayout::onCloseHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}

void GameMenuLayout::onSettingHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}

void GameMenuLayout::onRuleHandler(Ref* sender)
{
	gameRuleLayout = GameRuleLayout::create();
	this->addChild(gameRuleLayout, INT32_MAX);
}

void GameMenuLayout::onAchieveHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}