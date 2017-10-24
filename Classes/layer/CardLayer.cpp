#include "layer_header.h"
#include "resource/ResourceManager.h"

CardLayer::~CardLayer()
{
	releaseResource();
	CCLOG("main layer delete");
}

bool CardLayer::init()
{
	Layer::init();

	return true;
}

list<LoadItem> CardLayer::getResourceList()
{
	list<LoadItem> lst = BaseLayer::getResourceList();
	lst.push_back(LoadItem(LoadType::UI, "animation/RecvingNode.csb"));
	lst.push_back(LoadItem(LoadType::ANIMATION, "animation/RecvingNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/card/CardScene.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/card/GameMenuNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/card/CalculateNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/card/GameOverNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/card/GameRuleNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "animation/aniCard.csb"));
	lst.push_back(LoadItem(LoadType::ANIMATION, "animation/aniCard.csb"));
	lst.push_back(LoadItem(LoadType::PLIST, "plist/CardPlist.plist"));
	lst.push_back(LoadItem(LoadType::PLIST, "plist/ColorTipPlist.plist"));
	lst.push_back(LoadItem(LoadType::UI, "animation/hitInvalid.csb"));
	lst.push_back(LoadItem(LoadType::ANIMATION, "animation/hitInvalid.csb"));
	return lst;
}

void CardLayer::onEnter()
{
	BaseLayer::onEnter();
	cardLayout = CardLayout::create();
	this->addChild(cardLayout);
}

void CardLayer::releaseResource()
{
	this->removeChild(cardLayout);
	BaseLayer::releaseResource();
}
