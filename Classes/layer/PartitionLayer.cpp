#include "layer_header.h"
#include "resource/ResourceManager.h"

PartitionLayer::~PartitionLayer()
{
	releaseResource();
	CCLOG("partition Layer delete");
}

bool PartitionLayer::init()
{
	Layer::init();

	return true;
}

list<LoadItem> PartitionLayer::getResourceList()
{
	list<LoadItem> lst = BaseLayer::getResourceList();
	lst.push_back(LoadItem(LoadType::UI, "ui/partition/partition.csb"));
	lst.push_back(LoadItem(LoadType::UI, "animation/RecvingNode.csb"));
	lst.push_back(LoadItem(LoadType::ANIMATION, "animation/RecvingNode.csb"));
	return lst;
}

void PartitionLayer::onEnter()
{
	BaseLayer::onEnter();
	partitionLayout = PartitionLayout::create();
	this->addChild(partitionLayout);
}

void PartitionLayer::releaseResource()
{
	this->removeChild(partitionLayout);
	BaseLayer::releaseResource();
}
