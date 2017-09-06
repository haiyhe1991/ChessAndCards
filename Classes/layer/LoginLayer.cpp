#include "layer_header.h"
#include "resource/ResourceManager.h"

LoginLayer::~LoginLayer()
{
	releaseResource();
	CCLOG("login layer delete");
}

bool LoginLayer::init()
{
	Layer::init();

	return true;
}

list<LoadItem> LoginLayer::getResourceList()
{
	list<LoadItem> lst = BaseLayer::getResourceList();
	lst.push_back(LoadItem(LoadType::UI, "ui/login/LoginScene.csb"));
	return lst;
}

void LoginLayer::onEnter()
{
	BaseLayer::onEnter();
	loginLayout = LoginLayout::create();
	this->addChild(loginLayout);
}

void LoginLayer::releaseResource()
{
	this->removeChild(loginLayout);
	BaseLayer::releaseResource();
}
