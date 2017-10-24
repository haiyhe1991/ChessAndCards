#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool PartitionLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/partition/partition.csb");
	this->addChild(layout);

	txtUserName = (Text*)layout->getChildByName("txtUserName");

	txtUserName->setString(TcpLogic::GetInstance()->m_pUserLoginRes->username);

	txtSelectPartition = (Text*)layout->getChildByName("txtSelectPartition");

	btCancel = (Button*)layout->getChildByName("btCancel");
	//添加事件监听
	btCancel->addClickEventListener(CC_CALLBACK_1(PartitionLayout::onCancelHandler, this));

	start_game = (Button*)layout->getChildByName("btSelectRole");
	//添加事件监听
	start_game->addClickEventListener(CC_CALLBACK_1(PartitionLayout::onStartHandler, this));

	lstPartition = (ListView*)layout->getChildByName("lstPartition");

	this->SenceLoad();

	return true;
}

PartitionLayout::PartitionLayout()
{

	m_currentPartitionId = 1;

	recv = nullptr;

	this->registerMessage();

	//this->addRecvingLayer();
}

PartitionLayout::~PartitionLayout()
{
	this->unregisterMessage();
}

void PartitionLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_ENTER_PARTITION_RES, this);
	manager->Reg(MSG_LOGIN_LINK_RES, this);
	manager->Reg(MSG_QUERY_ROLE_RES, this);
}

void PartitionLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_ENTER_PARTITION_RES, this);
	manager->Unreg(MSG_LOGIN_LINK_RES, this);
	manager->Unreg(MSG_QUERY_ROLE_RES, this);
}

void PartitionLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void PartitionLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void PartitionLayout::onStartHandler(Ref* sender)
{
	this->addRecvingLayer();
	//进入分区
	TcpLogic::GetInstance()->EnterPartitionReq(m_currentPartitionId);
}

void PartitionLayout::onCancelHandler(Ref* sender)
{

}

void PartitionLayout::onPartionHandler(Ref* sender)
{
	Button* button = ((Button*)sender);
	m_currentPartitionId = button->getTag();
	txtSelectPartition->setString(button->getTitleText());
}

int PartitionLayout::EnterPartitionRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		this->removeRecvingLayer();
		char str[48];
		sprintf(str, "进入分区错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	else
	{
		TcpLogic::GetInstance()->CreatLinkerSocket();
	}

	return ret;
}

int PartitionLayout::LoginLinkRes(void* pBuf)
{
	//this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		this->removeRecvingLayer();
		char str[48];
		sprintf(str, "登陆link错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	else//先默认选择进入一个角色
	{
		TcpLogic::GetInstance()->QueryRolesReq();
	}

	return ret;
}

int PartitionLayout::QueryRoleRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		this->removeRecvingLayer();
		char str[48];
		sprintf(str, "查询角色错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	{
		//这儿不直接写回调函数是为了切换下一帧去切换界面，否者要报错
		this->scheduleOnce(schedule_selector(PartitionLayout::FightChangeSence), 0.1f);
	}
	return ret;
}

void PartitionLayout::SenceLoad()
{
	list<SPartitionInfo*>::iterator lstIter = TcpLogic::GetInstance()->container.begin();
	string curPartitionName = "";
	while (lstIter != TcpLogic::GetInstance()->container.end())
	{
		Button* partition = Button::create();
		partition->addClickEventListener(CC_CALLBACK_1(PartitionLayout::onPartionHandler, this));
		partition->setTag((*lstIter)->partitionId);
		partition->setTitleFontSize(36);
		partition->setTitleText((*lstIter)->name);
		lstPartition->addChild(partition);
		curPartitionName = (*lstIter)->name;
		lstIter++;
	}

	txtSelectPartition->setString(curPartitionName);
}

void PartitionLayout::FightChangeSence(float a)
{
	this->removeRecvingLayer();
	RoleLayer* roleLayer = RoleLayer::create();
	roleLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, roleLayer);
}

void PartitionLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("连接超时", "提示");
		break;
	case MSG_ENTER_PARTITION_RES:
		EnterPartitionRes(data);
		break;
	case MSG_LOGIN_LINK_RES:
		this->LoginLinkRes(data);
		break;
	case MSG_QUERY_ROLE_RES:
		this->QueryRoleRes(data);
		break;
	default:
		break;
	}
}
