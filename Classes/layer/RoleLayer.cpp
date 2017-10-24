#include "layer_header.h"
#include "resource/ResourceManager.h"

RoleLayer::~RoleLayer()
{
	releaseResource();
	CCLOG("main layer delete");
}

bool RoleLayer::init()
{
	Layer::init();
	
	return true;
}

list<LoadItem> RoleLayer::getResourceList()
{
	list<LoadItem> lst = BaseLayer::getResourceList();
	lst.push_back(LoadItem(LoadType::UI, "ui/role/role.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/role/CreateRoleNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "animation/RecvingNode.csb"));
	lst.push_back(LoadItem(LoadType::ANIMATION, "animation/RecvingNode.csb"));
	return lst;
}

void RoleLayer::onEnter()
{
	BaseLayer::onEnter();
	roleLayout = RoleLayout::create();
	this->addChild(roleLayout);
}

void RoleLayer::releaseResource()
{
	this->removeChild(roleLayout);
	BaseLayer::releaseResource();
}
