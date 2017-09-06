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
	antAction->gotoFrameAndPlay(0, 35, true);//�ӵ�0֡��60֡ѭ�����š������������غ��������忴Դ�롣

	return true;
}
