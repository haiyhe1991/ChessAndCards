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
	lst.push_back(LoadItem(LoadType::UI, "ui/card/CardScene.csb"));
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
