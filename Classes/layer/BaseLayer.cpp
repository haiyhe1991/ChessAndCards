#include "layer_header.h"
#include "resource/ResourceManager.h"

bool BaseLayer::init()
{
	bool flag = Layer::init();
	return flag;
}

void BaseLayer::onEnter()
{
	Layer::onEnter();
}

list<LoadItem> BaseLayer::getResourceList()
{
	list<LoadItem> lst;
	return lst;
}

void BaseLayer::releaseResource()
{
	ResourceManager::GetInstance()->Release(this->getResourceList());
}