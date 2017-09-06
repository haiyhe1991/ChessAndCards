#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"
bool LoginLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/login/LoginScene.csb");
	this->addChild(layout);

	register_game = (Button*)layout->getChildByName("btRegister");
	//����¼�����
	register_game->addClickEventListener(CC_CALLBACK_1(LoginLayout::onRegHandler, this));

	login_game = (Button*)layout->getChildByName("btLogin");
	//����¼�����
	login_game->addClickEventListener(CC_CALLBACK_1(LoginLayout::onLoginHandler, this));

	tbName = (TextField*)(layout->getChildByName("tbName"));

	tbPassword = (TextField*)layout->getChildByName("tbPassword");

	return true;
}

LoginLayout::LoginLayout()
{

	this->registerMessage();

	recv = nullptr;
	//this->addRecvingLayer();
	TcpLogic::GetInstance()->CreatLicenseSocket();
}

LoginLayout::~LoginLayout()
{
	this->unregisterMessage();
}

void LoginLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_REGIST_RES, this);
	manager->Reg(MSG_LOGIN_RES, this);
	manager->Reg(MSG_CONNECT_LICENSE_RES, this);
}

void LoginLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_REGIST_RES, this);
	manager->Unreg(MSG_LOGIN_RES, this);
	manager->Unreg(MSG_CONNECT_LICENSE_RES, this);
}

void LoginLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void LoginLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void LoginLayout::onRegHandler(Ref* sender)
{
	if (tbName->getString() != ""&&tbPassword->getString() != "")
	{
		this->addRecvingLayer();
		TcpLogic::GetInstance()->RegistUserReq(tbName->getString().c_str(), tbPassword->getString().c_str());
	}
	else
	{
		MessageBox("�˺����벻��Ϊ��", "��ʾ");
	}
}

void LoginLayout::onLoginHandler(Ref* sender)
{
	if (tbName->getString() != ""&&tbPassword->getString() != "")
	{
		this->addRecvingLayer();
		TcpLogic::GetInstance()->LoginUserReq(tbName->getString().c_str(), tbPassword->getString().c_str());
	}
	else
	{
		MessageBox("�˺����벻��Ϊ��", "��ʾ");
	}
}

int LoginLayout::RegistUserRes(void* pBuf)
{
	this->removeRecvingLayer();

	UINT16 ret = *(UINT16*)pBuf;
	if (ret == LCS_EXIST_ACCOUNT_NAME)
	{
		MessageBox("�Ѿ������˻�", "��ʾ");
	}
	else if (ret == LCS_OK)
	{
		MessageBox("ע��ɹ�", "��ʾ");
	}
	else
	{
		char str[48];
		sprintf(str, "ע�����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}

	return ret;
}

int LoginLayout::LoginUserRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret == LCS_ACCOUNT_PSW_WRONG)
	{
		this->removeRecvingLayer();
		MessageBox("�˺��������", "��ʾ");
	}
	else if (ret == LCS_OK)
	{
		//�����ֱ��д�ص�������Ϊ���л���һ֡ȥ�л����棬����Ҫ����
		this->scheduleOnce(schedule_selector(LoginLayout::LoginChangeSence), 0.1f);
	}
	else
	{
		this->removeRecvingLayer();
		char str[48];
		sprintf(str, "��¼����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	
	return ret;
}

void LoginLayout::LoginChangeSence(float a)
{
	this->removeRecvingLayer();
	MainLayer* mainLayer = MainLayer::create();
	mainLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, mainLayer);
}

void LoginLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("���ӳ�ʱ", "��ʾ");
		break;
	case MSG_REGIST_RES:
		RegistUserRes(data);
		break;
	case MSG_LOGIN_RES:
		LoginUserRes(data);
		break;
	case MSG_CONNECT_LICENSE_RES:
		//this->removeRecvingLayer();
		break;
	default:
		break;
	}
}