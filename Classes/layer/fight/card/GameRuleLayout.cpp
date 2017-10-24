#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool GameRuleLayout::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/card/GameRuleNode.csb");
	this->addChild(layout);

	btClose = (Button*)layout->getChildByName("btClose");
	//Ìí¼ÓÊÂ¼þ¼àÌý
	btClose->addClickEventListener(CC_CALLBACK_1(GameRuleLayout::onCloseHandler, this));

	return true;
}

GameRuleLayout::GameRuleLayout()
{

}

GameRuleLayout::~GameRuleLayout()
{

}

void GameRuleLayout::onCloseHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}