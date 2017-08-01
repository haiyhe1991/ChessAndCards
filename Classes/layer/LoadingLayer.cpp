#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "msg/message.h"
#include "layer/load.h"

LoadingLayer::~LoadingLayer()
{
	CCLOG("loading layer delete");
}

bool LoadingLayer::init()
{
	bool flag = Layer::init();

	cur_num = 0;
	max_num = 0;

	GameTick::SetDelayTime(LOADING_TICK_TIME);

	if (flag)
	{
		resource = CSLoader::createNode("ui/loading/LoadingScene.csb");
		this->addChild(resource);

		progress = dynamic_cast<LoadingBar*>(resource->getChildByName("LoadingBar"));
		ChangeProgress();
	}
	return flag;
}


void LoadingLayer::StartLoading(BaseLayer* layer)
{
	this->base_layer = layer;
	loading_lst = base_layer->getResourceList();
	max_num = loading_lst.size();
	TickManager::GetInstance()->Reg(this);
}

void LoadingLayer::loadingMethod()
{
	list<LoadItem>::iterator iter = loading_lst.begin();
	if (iter != loading_lst.end())
	{
		ResourceManager::GetInstance()->Sync_load((*iter));
		loading_lst.pop_front();	//删除已经加载
		cur_num++;
		ChangeProgress();
	}
	else
	{
		//全部加载完成
		TickManager::GetInstance()->Unreg(this);
		//切换场景
		MsgManager::GetInstance()->Dispather(MessageHead::MSG_CHANGE_SCENE, base_layer);
	}
}

void LoadingLayer::ChangeProgress()
{
	if (max_num == 0)
	{
		progress->setPercent(0);
	}
	else
	{
		progress->setPercent((cur_num * 100 / max_num));
	}
}

void LoadingLayer::OnTick(int time)
{
	loadingMethod();
}
