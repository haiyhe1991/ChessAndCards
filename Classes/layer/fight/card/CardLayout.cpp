#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"
bool CardLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/card/CardScene.csb");
	this->addChild(layout);

	btSendData = (TextField*)(layout->getChildByName("btSendData"));

	lstChatRecv = (ListView*)layout->getChildByName("LstChatRecv");

	for (int i = 0; i < 20; i++)
	{
		auto text = Text::create();
		text->setString("defef");
		lstChatRecv->addChild(text);
	}
	bt_clear = (Button*)layout->getChildByName("bt_clear");
	//����¼�����
	bt_clear->addClickEventListener(CC_CALLBACK_1(CardLayout::onClearHandler, this));

	bt_send = (Button*)layout->getChildByName("bt_send");
	//����¼�����
	bt_send->addClickEventListener(CC_CALLBACK_1(CardLayout::onSendHandler, this));

	return true;
}

CardLayout::CardLayout()
{

	recv = nullptr;

	this->registerMessage();

	//this->addRecvingLayer();
	TcpLogic::GetInstance()->CreatLinkerSocket();
}

CardLayout::~CardLayout()
{
	this->unregisterMessage();
}

void CardLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_CONNECT_LINK_RES, this);
	manager->Reg(MSG_LOGIN_LINK_RES, this);
	manager->Reg(MSG_QUERY_ROLE_RES, this);
	manager->Reg(MSG_CREATE_ROLE_RES, this);
	manager->Reg(MSG_ENTER_ROLE_RES, this);
	manager->Reg(MSG_JOIN_CHANNEL_RES, this);
}

void CardLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_CONNECT_LINK_RES, this);
	manager->Unreg(MSG_LOGIN_LINK_RES, this);
	manager->Unreg(MSG_QUERY_ROLE_RES, this);
	manager->Unreg(MSG_CREATE_ROLE_RES, this);
	manager->Unreg(MSG_ENTER_ROLE_RES, this);
	manager->Unreg(MSG_JOIN_CHANNEL_RES, this);
}

void CardLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void CardLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void CardLayout::onClearHandler(Ref* sender)
{
	//lstChatRecv->removeAllChildren();
	this->addRecvingLayer();
	TcpLogic::GetInstance()->CreateRoleReq(2,0,"iampeople");
}

void CardLayout::onSendHandler(Ref* sender)
{
	if (btSendData->getString() != "")
	{
		TcpLogic::GetInstance()->ChatMessageSend(btSendData->getString().c_str());
	}
}

void CardLayout::ConnectLinkRes()
{
	TcpLogic::GetInstance()->LoginLinkReq();
}

int CardLayout::LoginLinkRes(void* pBuf)
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
		this->addRecvingLayer();
		TcpLogic::GetInstance()->QueryRolesReq();
	}

	return ret;
}

int CardLayout::QueryRoleRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "��ѯ��ɫ����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	return ret;
}

int CardLayout::CreateRoleRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "������ɫ����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	return ret;
}

int CardLayout::EnterRoleRes(void* pBuf)
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
		this->addRecvingLayer();
		TcpLogic::GetInstance()->JoinChannelReq();
	}
	return ret;
}

int CardLayout::JoinChannelRes(void* pBuf)
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

void CardLayout::OnMessage(const int head, void* data)
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
	case MSG_CREATE_ROLE_RES:
		this->CreateRoleRes(data);
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

