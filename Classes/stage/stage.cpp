#include "stage.h"
#include "layer/layer_header.h"
#include "tools/TickManager.h"

//单例模式
Stage* Stage::_instance = nullptr;

Stage* Stage::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = Stage::create();
		_instance->retain();
	}
	return _instance;
}

bool Stage::init()
{
	bool flag = Scene::init();

	current = nullptr;

	asyncLayer = nullptr;

	//注册tick事件
	this->schedule(schedule_selector(Stage::onScheduleHandler));

	//注册监听事件
	MsgManager::GetInstance()->Reg(MessageHead::MSG_START_LOGO, this);
	MsgManager::GetInstance()->Reg(MessageHead::MSG_CHANGE_SCENE, this);
	MsgManager::GetInstance()->Reg(MessageHead::MSG_START_LOADING, this);

	return flag;
}

void Stage::OnMessage(const int head, void* data)
{
	asyncLayer = current;
	if (head == MessageHead::MSG_CHANGE_SCENE)
	{
		current = (Layer*)data;
		this->addChild(current);
		current->release();
	}
	else if (head == MessageHead::MSG_START_LOGO)
	{
		current = LogoLayer::create();
		this->addChild(current);
	}
	else if (head == MessageHead::MSG_START_LOADING)
	{
		current = LoadingLayer::create();
		this->addChild(current);
		((LoadingLayer*)current)->StartLoading((BaseLayer*)data);
	}
}

void Stage::async_clearChild()
{
	if (asyncLayer != nullptr)
	{
		this->removeChild(asyncLayer);
		asyncLayer = nullptr;
	}
}

void Stage::onScheduleHandler(float dt)
{
	int time = int(dt * 1000);	//间隔毫秒数
	TickManager::GetInstance()->OnTickHandler(time);
	async_clearChild();
}