#include "layer/layer_header.h"
#include "resource/ResourceManager.h"

RecvingLayer::~RecvingLayer()
{
	CCLOG("recving layer delete");
}

bool RecvingLayer::init()
{

	ShadeLayer::init();


	layout = layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("animation/RecvingNode.csb");
	this->addChild(layout);

	auto antAction = (ActionTimeline*)ResourceManager::GetInstance()->Get_ani_resource("animation/RecvingNode.csb");

	layout->runAction(antAction);

	antAction->gotoFrameAndPlay(0, true);//从第0帧到60帧循环播放。还有其他重载函数，具体看源码。

	return true;
}
