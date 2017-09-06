#include "layer/layer_header.h"

RecvingLayer::~RecvingLayer()
{
	CCLOG("recving layer delete");
}

bool RecvingLayer::init()
{

	ShadeLayer::init();


	layout = CSLoader::createNode("animation/recving/RecvingScene.csb");
	this->addChild(layout);

	//auto start_game = (Sprite*)layout->getChildByName("waitServer_1");
	//start_game->setVisible(true);

	auto antAction = CSLoader::createTimeline("animation/recving/RecvingScene.csb");

	layout->runAction(antAction);

	//antAction->play("aniWaitRecv", true);
	antAction->gotoFrameAndPlay(0, 35, true);//从第0帧到60帧循环播放。还有其他重载函数，具体看源码。

	return true;
}
