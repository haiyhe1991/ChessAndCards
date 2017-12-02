#include "layer_header.h"
#include "resource/ResourceManager.h"

RoomLayer::~RoomLayer()
{
	releaseResource();
	CCLOG("main layer delete");
}

bool RoomLayer::init()
{
	Layer::init();
	
	return true;
}

list<LoadItem> RoomLayer::getResourceList()
{
	list<LoadItem> lst = BaseLayer::getResourceList();
	lst.push_back(LoadItem(LoadType::UI, "ui/room/RoomScene.csb"));
	lst.push_back(LoadItem(LoadType::UI, "ui/room/JionRoomNode.csb"));
	lst.push_back(LoadItem(LoadType::UI, "animation/RecvingNode.csb"));
	lst.push_back(LoadItem(LoadType::ANIMATION, "animation/RecvingNode.csb"));
	return lst;
}

void RoomLayer::onEnter()
{
	BaseLayer::onEnter();
	roomLayout = RoomLayout::create();
	this->addChild(roomLayout);
}

void RoomLayer::releaseResource()
{
	this->removeChild(roomLayout);
	BaseLayer::releaseResource();
}
