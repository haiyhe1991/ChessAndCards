#include "layer_header.h"
#include "tools/TickManager.h"
#include "msg/message.h"


LogoLayer::~LogoLayer()
{
	CCLOG("logo layer delete");
}

// on "init" you need to initialize your instance
bool LogoLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	GameTick::SetDelayTime(LOGO_LAYER_SHOW_TIME);
	TickManager::GetInstance()->Reg(this);

	//ÕÒµ½UI±à¼­Æ÷ÖÐµÄ¸¸ÈÝÆ÷
	Node * logo = CSLoader::createNode("ui/logo/LogoScene.csb");
	this->addChild(logo);

	return true;
}

void LogoLayer::OnTick(int time)
{
	TickManager::GetInstance()->Unreg(this);
	LoginLayer* loginLayer = LoginLayer::create();
	loginLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, loginLayer);
}
