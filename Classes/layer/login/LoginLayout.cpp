#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"
bool LoginLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/login/LoginScene.csb");
	this->addChild(layout);

	register_game = (Button*)layout->getChildByName("btRegister");
	//添加事件监听
	register_game->addClickEventListener(CC_CALLBACK_1(LoginLayout::onRegHandler, this));

	login_game = (Button*)layout->getChildByName("btLogin");
	//添加事件监听
	login_game->addClickEventListener(CC_CALLBACK_1(LoginLayout::onLoginHandler, this));

	tbName = (TextField*)(layout->getChildByName("tbName"));

	tbPassword = (TextField*)layout->getChildByName("tbPassword");

	this->registerMessage();

	recv = nullptr;

	if (!TcpLogic::GetInstance()->CreatLicenseSocket("192.168.50.2", 10030))
	{
		MessageBox("连接验证服务器失败", "提示");
	}

	return true;
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
}

void LoginLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_REGIST_RES, this);
	manager->Unreg(MSG_LOGIN_RES, this);
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

void LoginLayout::onRegHandler(Ref* sender)
{
	if (tbName->getString() != ""&&tbPassword->getString() != "")
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
		if (TcpLogic::GetInstance()->RegistUserReq(tbName->getString().c_str(), tbPassword->getString().c_str()))
		{
			MsgManager::GetInstance()->Dispather(MessageHead::MSG_REGIST_REQ, nullptr);
		}
		else
		{
			this->removeRecvingLayer();
		}
	}
	else
	{
		MessageBox("账号密码不能为空", "提示");
	}
}

void LoginLayout::onLoginHandler(Ref* sender)
{
	if (tbName->getString() != ""&&tbPassword->getString() != "")
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
		if (TcpLogic::GetInstance()->LoginUserReq(tbName->getString().c_str(), tbPassword->getString().c_str()))
		{
			MsgManager::GetInstance()->Dispather(MessageHead::MSG_LOGIN_REQ, nullptr);
		}
		else
		{
			this->removeRecvingLayer();
		}
	}
	else
	{
		MessageBox("账号密码不能为空", "提示");
	}
}

int LoginLayout::RegistUserRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret == LCS_EXIST_ACCOUNT_NAME)
	{
		MessageBox("已经存在账户", "提示");
	}
	else if (ret == LCS_OK)
	{
		MessageBox("注册成功", "提示");
	}
	else
	{
		char str[48];
		sprintf(str, "注册错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	return LCS_OK;
}

int LoginLayout::LoginUserRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret == LCS_ACCOUNT_PSW_WRONG)
	{
		MessageBox("账号密码错误", "提示");
		return -1;
	}
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "登录错误,错误码：%d", ret);
		MessageBox(str, "提示");
		return -1;
	}
	MainLayer* mainLayer = MainLayer::create();
	mainLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, mainLayer);
	return LCS_OK;
}

void LoginLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("连接超时", "提示");
		break;
	case MSG_REGIST_RES:
		this->removeRecvingLayer();
		RegistUserRes(data);
		break;
	case MSG_LOGIN_RES:
		this->removeRecvingLayer();
		LoginUserRes(data);
		break;
	default:
		break;
	}
}