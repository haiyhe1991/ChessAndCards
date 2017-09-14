#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool RoleLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/role/role.csb");
	this->addChild(layout);

	btStartGame = (Button*)layout->getChildByName("btStartGame");
	//����¼�����
	btStartGame->addClickEventListener(CC_CALLBACK_1(RoleLayout::onStartHandler, this));

	btCreateRole = (Button*)layout->getChildByName("btCreateRole");
	//����¼�����
	btCreateRole->addClickEventListener(CC_CALLBACK_1(RoleLayout::onCreateRoleHandler, this));

	btLeft = (Button*)layout->getChildByName("btLeft");
	//����¼�����
	btLeft->addClickEventListener(CC_CALLBACK_1(RoleLayout::onLeftHandler, this));

	btRight = (Button*)layout->getChildByName("btRight");
	//����¼�����
	btRight->addClickEventListener(CC_CALLBACK_1(RoleLayout::onRightHandler, this));

	pageView_SelectPage = (PageView*)layout->getChildByName("PageView_SelectPage");
	//����¼�����
	pageView_SelectPage->addEventListener(CC_CALLBACK_2(RoleLayout::pageViewEvent, this));

	//this->addRecvingLayer();
	TcpLogic::GetInstance()->CreatLinkerSocket();

	roleCount = pageView_SelectPage->getPages().size();

	pageView_SelectPage->setCurPageIndex(GameManage::GetInstance()->GetCurOccuId() - 1);

	this->PageCurIndexChange();

	return true;
}

RoleLayout::RoleLayout()
{

	recv = nullptr;

	this->registerMessage();

}

RoleLayout::~RoleLayout()
{
	this->unregisterMessage();
}

void RoleLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_CONNECT_LINK_RES, this);
	manager->Reg(MSG_LOGIN_LINK_RES, this);
	manager->Reg(MSG_QUERY_ROLE_RES, this);
	manager->Reg(MSG_ENTER_ROLE_RES, this);
	manager->Reg(MSG_JOIN_CHANNEL_RES, this);
}

void RoleLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_CONNECT_LINK_RES, this);
	manager->Unreg(MSG_LOGIN_LINK_RES, this);
	manager->Unreg(MSG_QUERY_ROLE_RES, this);
	manager->Unreg(MSG_ENTER_ROLE_RES, this);
	manager->Unreg(MSG_JOIN_CHANNEL_RES, this);
}

void RoleLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void RoleLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void RoleLayout::onStartHandler(Ref* sender)
{
	this->addRecvingLayer();
	//��½��ɫ

	if (!TcpLogic::GetInstance()->EnterRoleReq(pageView_SelectPage->getCurPageIndex() + 1))
	{
		this->removeRecvingLayer();
		MessageBox("��ɫδӵ�У��봴��", "��ʾ");
	}
}

void RoleLayout::onCreateRoleHandler(Ref* sender)
{
	GameManage::GetInstance()->SetCurOccuId (pageView_SelectPage->getCurPageIndex() + 1);
	creatRolelayout = CreateRoleLayout::create();
	this->addChild(creatRolelayout, INT32_MAX);
}

void RoleLayout::onLeftHandler(Ref* sender)
{
	int curIndex = pageView_SelectPage->getCurPageIndex();
	if (curIndex > 0)
		pageView_SelectPage->scrollToPage(curIndex - 1);
}

void RoleLayout::onRightHandler(Ref* sender)
{
	int curIndex = pageView_SelectPage->getCurPageIndex();
	if (curIndex < roleCount - 1)
		pageView_SelectPage->scrollToPage(curIndex + 1);
}

void RoleLayout::pageViewEvent(Ref *pSender, PageView::EventType type)
{
	switch (type)
	{
	case PageView::EventType::TURNING:
	{
		this->PageCurIndexChange();
	}
	break;

	default:
		break;
	}
}

void RoleLayout::PageCurIndexChange()
{
	if (pageView_SelectPage->getCurPageIndex() == roleCount - 1)
	{
		btLeft->setEnabled(true);
		btRight->setEnabled(false);
	}
	else if (pageView_SelectPage->getCurPageIndex() == 0)
	{
		btLeft->setEnabled(false);
		btRight->setEnabled(true);
	}
	else
	{
		btRight->setEnabled(true);
		btLeft->setEnabled(true);
	}
}

void RoleLayout::ConnectLinkRes()
{
	TcpLogic::GetInstance()->LoginLinkReq();
}

int RoleLayout::LoginLinkRes(void* pBuf)
{
	//this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "��½link����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	else//��Ĭ��ѡ�����һ����ɫ
	{
		TcpLogic::GetInstance()->QueryRolesReq();
	}

	return ret;
}

int RoleLayout::QueryRoleRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "��ѯ��ɫ����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	{
		//�����ֱ��д�ص�������Ϊ���л���һ֡ȥ�л����棬����Ҫ����
		this->scheduleOnce(schedule_selector(RoleLayout::SenceLoad), 0.1f);
	}
	return ret;
}

void RoleLayout::SenceLoad(float a)
{
	list<SQueryRoleAttr*>::iterator lstIter = TcpLogic::GetInstance()->containerRoleAttr.begin();
	string curPartitionName = "";
	while (lstIter != TcpLogic::GetInstance()->containerRoleAttr.end())
	{
		int occuId = (*(lstIter))->occuId;
		char str[48];
		sprintf(str, "Panel_Page_%d", occuId);
		Layout* layoutRole = (Layout*)pageView_SelectPage->getChildByName(str);
		sprintf(str, "txtOwnRole%d", occuId);
		Text* txtRole = (Text*)layoutRole->getChildByName(str);
		ConfigFileMan::getInstance()->LoadConfigFile("language.xml");
		txtRole->setString(ConfigFileMan::getInstance()->getValue("RoleOwn"));
		txtRole->setTextColor(Color4B(169, 255, 150, 255));
		lstIter++;
	}
}

int RoleLayout::EnterRoleRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "�����ɫ����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	else
	{
		//this->addRecvingLayer();
		//TcpLogic::GetInstance()->JoinChannelReq();
	}
	return ret;
}

int RoleLayout::JoinChannelRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "��������Ƶ������,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	return ret;
}

void RoleLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("���ӳ�ʱ", "��ʾ");
		break;
	case MSG_CONNECT_LINK_RES:
		this->ConnectLinkRes();
		break;
	case MSG_LOGIN_LINK_RES:
		this->LoginLinkRes(data);
		break;
	case MSG_QUERY_ROLE_RES:
		this->QueryRoleRes(data);
		break;
	case MSG_ENTER_ROLE_RES:
		this->EnterRoleRes(data);
		break;
	case MSG_JOIN_CHANNEL_RES:
		this->JoinChannelRes(data);
		break;
	default:
		break;
	}
}
