#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool CreateRoleLayout::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/role/CreateRoleNode.csb");
	this->addChild(layout);

	Node* PanelMain = (Layout*)layout->getChildByName("PanelMain");

	btContinue = (Button*)PanelMain->getChildByName("btContinue");
	//添加事件监听
	btContinue->addClickEventListener(CC_CALLBACK_1(CreateRoleLayout::onContinueHandler, this));

	btClose = (Button*)PanelMain->getChildByName("btClose");
	//添加事件监听
	btClose->addClickEventListener(CC_CALLBACK_1(CreateRoleLayout::onCloseHandler, this));

	lbOccuId = (Text*)PanelMain->getChildByName("lbOccuId");

	tbNick = (TextField*)PanelMain->getChildByName("tbNick");

	Node* PanelSex = (Layout*)PanelMain->getChildByName("PanelSex");

	ckbMan = (CheckBox*)PanelSex->getChildByName("ckbMan");
	//添加事件监听
	ckbMan->addEventListener(CC_CALLBACK_2(CreateRoleLayout::ManSelectedEvent, this));

	ckbWoman = (CheckBox*)PanelSex->getChildByName("ckbWoman");
	//添加事件监听
	ckbWoman->addEventListener(CC_CALLBACK_2(CreateRoleLayout::WomanSelectedEvent, this));

	ConfigFileMan::getInstance()->LoadConfigFile("language.xml");
	char str[48];
	sprintf(str, "lbOccuName%d", GameManage::GetInstance()->GetCurOccuId());
	lbOccuId->setString(ConfigFileMan::getInstance()->getValue(str));

	return true;
}

CreateRoleLayout::CreateRoleLayout()
{

	recv = nullptr;

	this->registerMessage();

}

CreateRoleLayout::~CreateRoleLayout()
{
	this->unregisterMessage();
}

void CreateRoleLayout::onContinueHandler(Ref* sender)
{
	BYTE sex = ckbWoman->isSelected();
	if (tbNick->getString() != "")
	{
		this->addRecvingLayer();
		TcpLogic::GetInstance()->CreateRoleReq(GameManage::GetInstance()->GetCurOccuId(), sex, tbNick->getString().c_str());
	}
	else
	{
		MessageBox("昵称不能为空", "提示");
	}
}

void CreateRoleLayout::onCloseHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}

void CreateRoleLayout::ManSelectedEvent(Ref *pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		ckbMan->setSelected(true);
		ckbWoman->setSelected(false);
		break;

	case CheckBox::EventType::UNSELECTED:
		ckbMan->setSelected(false);
		ckbWoman->setSelected(true);
		break;

	default:
		break;
	}
}

void CreateRoleLayout::WomanSelectedEvent(Ref *pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		ckbMan->setSelected(false);
		ckbWoman->setSelected(true);
		break;

	case CheckBox::EventType::UNSELECTED:
		ckbMan->setSelected(true);
		ckbWoman->setSelected(false);
		break;

	default:
		break;
	}
}

void CreateRoleLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_CREATE_ROLE_RES, this);
}

void CreateRoleLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_CREATE_ROLE_RES, this);
}

void CreateRoleLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void CreateRoleLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

int CreateRoleLayout::CreateRoleRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "创建角色错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	else
	{
		this->removeAllChildrenWithCleanup(false);
		this->removeFromParentAndCleanup(false);
	}
	return ret;
}

void CreateRoleLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("连接超时", "提示");
		break;
	case MSG_CREATE_ROLE_RES:
		this->CreateRoleRes(data);
		break;
	default:
		break;
	}
}

//
//void RoleLayout::onCreateRoleHandler(Ref* sender)
//{
//	//this->addRecvingLayer();
//	//进入分区
//	//TcpLogic::GetInstance()->EnterPartitionReq(m_currentPartitionId);
//}
//
//void RoleLayout::onLeftHandler(Ref* sender)
//{
//	int curIndex = pageView_SelectPage->getCurPageIndex();
//	if (curIndex > 0)
//		pageView_SelectPage->scrollToPage(curIndex - 1);
//}
//
//void RoleLayout::onRightHandler(Ref* sender)
//{
//	int curIndex = pageView_SelectPage->getCurPageIndex();
//	if (curIndex < roleCount - 1)
//		pageView_SelectPage->scrollToPage(curIndex + 1);
//}
//
//void RoleLayout::pageViewEvent(Ref *pSender, PageView::EventType type)
//{
//	switch (type)
//	{
//	case PageView::EventType::TURNING:
//	{
//		this->PageCurIndexChange();
//	}
//	break;
//
//	default:
//		break;
//	}
//}
//
//void RoleLayout::PageCurIndexChange()
//{
//	if (pageView_SelectPage->getCurPageIndex() == roleCount - 1)
//	{
//		btLeft->setEnabled(true);
//		btRight->setEnabled(false);
//	}
//	else if (pageView_SelectPage->getCurPageIndex() == 0)
//	{
//		btLeft->setEnabled(false);
//		btRight->setEnabled(true);
//	}
//	else
//	{
//		btRight->setEnabled(true);
//		btLeft->setEnabled(true);
//	}
//}
//
//void RoleLayout::ConnectLinkRes()
//{
//	TcpLogic::GetInstance()->LoginLinkReq();
//}
//
//int RoleLayout::LoginLinkRes(void* pBuf)
//{
//	//this->removeRecvingLayer();
//	UINT16 ret = *(UINT16*)pBuf;
//	if (ret != LCS_OK)
//	{
//		char str[48];
//		sprintf(str, "登陆link错误,错误码：%d", ret);
//		MessageBox(str, "提示");
//	}
//	else//先默认选择进入一个角色
//	{
//		TcpLogic::GetInstance()->QueryRolesReq();
//	}
//
//	return ret;
//}
//
//int RoleLayout::QueryRoleRes(void* pBuf)
//{
//	UINT16 ret = *(UINT16*)pBuf;
//	if (ret != LCS_OK)
//	{
//		char str[48];
//		sprintf(str, "查询角色错误,错误码：%d", ret);
//		MessageBox(str, "提示");
//	}
//	{
//		//这儿不直接写回调函数是为了切换下一帧去切换界面，否者要报错
//		this->scheduleOnce(schedule_selector(RoleLayout::SenceLoad), 0.1f);
//	}
//	return ret;
//}
//
//void RoleLayout::SenceLoad(float a)
//{
//	list<SQueryRoleAttr*>::iterator lstIter = TcpLogic::GetInstance()->containerRoleAttr.begin();
//	string curPartitionName = "";
//	while (lstIter != TcpLogic::GetInstance()->containerRoleAttr.end())
//	{
//		int occuId = (*(lstIter))->occuId;
//		char str[48];
//		sprintf(str, "Panel_Page_%d", occuId);
//		Layout* layoutRole = (Layout*)pageView_SelectPage->getChildByName(str);
//		sprintf(str, "txtOwnRole%d", occuId);
//		Text* txtRole = (Text*)layoutRole->getChildByName(str);
//		ConfigFileMan::getInstance()->LoadConfigFile("language.xml");
//		txtRole->setString(ConfigFileMan::getInstance()->getValue("RoleOwn"));
//		txtRole->setTextColor(Color4B(169, 255, 150, 255));
//		lstIter++;
//	}
//}
//
//
//int RoleLayout::EnterRoleRes(void* pBuf)
//{
//	this->removeRecvingLayer();
//	UINT16 ret = *(UINT16*)pBuf;
//	if (ret != LCS_OK)
//	{
//		char str[48];
//		sprintf(str, "进入角色错误,错误码：%d", ret);
//		MessageBox(str, "提示");
//	}
//	else
//	{
//		//this->addRecvingLayer();
//		//TcpLogic::GetInstance()->JoinChannelReq();
//	}
//	return ret;
//}
//
//int RoleLayout::JoinChannelRes(void* pBuf)
//{
//	this->removeRecvingLayer();
//	UINT16 ret = *(UINT16*)pBuf;
//	if (ret != LCS_OK)
//	{
//		char str[48];
//		sprintf(str, "加入聊天频道错误,错误码：%d", ret);
//		MessageBox(str, "提示");
//	}
//	return ret;
//}
