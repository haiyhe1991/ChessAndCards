#include "layer_header.h"
#include "resource/ResourceManager.h"

MainLayer::~MainLayer()
{
	releaseResource();
	CCLOG("main layer delete");
}

bool MainLayer::init()
{
	Layer::init();

	return true;
}

list<LoadItem> MainLayer::getResourceList()
{
	list<LoadItem> lst = BaseLayer::getResourceList();
	lst.push_back(LoadItem(LoadType::UI, "ui/main/MainScene.csb"));
	return lst;
}

void MainLayer::onEnter()
{
	BaseLayer::onEnter();
	mainLayout = MainLayout::create();
	this->addChild(mainLayout);
}

void MainLayer::releaseResource()
{
	this->removeChild(mainLayout);
	BaseLayer::releaseResource();
}
