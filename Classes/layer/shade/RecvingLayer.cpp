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

	antAction->gotoFrameAndPlay(0, true);//�ӵ�0֡��60֡ѭ�����š������������غ��������忴Դ�롣

	return true;
}
