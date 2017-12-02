#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool RoomLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/room/RoomScene.csb");
	this->addChild(layout);

	room = (Layout*)layout->getChildByName("ProjectNode_1");

	btCreateTable = (Button*)room->getChildByName("bt_createTable");
	//添加事件监听
	btCreateTable->addClickEventListener(CC_CALLBACK_1(RoomLayout::onCreateTableHandler, this));

	btJionTable = (Button*)room->getChildByName("bt_jionTable");
	//添加事件监听
	btJionTable->addClickEventListener(CC_CALLBACK_1(RoomLayout::onJionTableHandler, this));

	return true;
}

RoomLayout::RoomLayout()
{
	recv = nullptr;

	this->registerMessage();
}

RoomLayout::~RoomLayout()
{
	this->unregisterMessage();
}

void RoomLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_CREATE_TABLE_RES, this);
}

void RoomLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_CREATE_TABLE_RES, this);
}

void RoomLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void RoomLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void RoomLayout::onCreateTableHandler(Ref* sender)
{
	this->addRecvingLayer();
	//创建房间

	if (!TcpLogic::GetInstance()->CreateTableReq())
	{
		this->removeRecvingLayer();
		MessageBox("房间创建请求失败", "提示");
	}
}

void RoomLayout::onJionTableHandler(Ref* sender)
{
	jionTableLayout = JionTableLayout::create();
	this->addChild(jionTableLayout, INT32_MAX);
}

void RoomLayout::FightChangeSence(float a)
{
	this->removeRecvingLayer();
	CardLayer* cardLayer = CardLayer::create();
	cardLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, cardLayer);
}

int RoomLayout::CreateTableRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "创建牌桌错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	else
	{
		//这儿不直接写回调函数是为了切换下一帧去切换界面，否者要报错
		this->scheduleOnce(schedule_selector(RoomLayout::FightChangeSence), 0.1f);
	}
	return ret;
}

void RoomLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("连接超时", "提示");
		break;
	case MSG_CREATE_TABLE_RES:
		this->CreateTableRes(data);
		break;
	default:
		break;
	}
}
