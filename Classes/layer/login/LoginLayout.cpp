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

	exit_game = (Button*)layout->getChildByName("btExit");
	//添加事件监听
	exit_game->addClickEventListener(CC_CALLBACK_1(LoginLayout::onExitHandler, this));

	tbName = (TextField*)(layout->getChildByName("tbName"));
	tbName->setString(GameManage::GetInstance()->GetCurUserName());

	tbPassword = (TextField*)layout->getChildByName("tbPassword");
	if (GameManage::GetInstance()->GetSavePassword())
	{
		tbPassword->setString(GameManage::GetInstance()->GetCurPassword());
	}

	ckbSavePassword = (CheckBox*)layout->getChildByName("ckbSavePassword");

	ckbSavePassword->setSelected(GameManage::GetInstance()->GetSavePassword());


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
	manager->Reg(MSG_QUERY_PARTITION_RES, this);
}

void LoginLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_REGIST_RES, this);
	manager->Unreg(MSG_LOGIN_RES, this);
	manager->Unreg(MSG_CONNECT_LICENSE_RES, this);
	manager->Unreg(MSG_QUERY_PARTITION_RES, this);
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
		MessageBox("账号密码不能为空", "提示");
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
		MessageBox("账号密码不能为空", "提示");
	}
}

void LoginLayout::onExitHandler(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

int LoginLayout::RegistUserRes(void* pBuf)
{
	this->removeRecvingLayer();

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

	return ret;
}

int LoginLayout::LoginUserRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret == LCS_ACCOUNT_PSW_WRONG)
	{
		this->removeRecvingLayer();
		MessageBox("账号密码错误", "提示");
	}
	else if (ret == LCS_OK)
	{
		GameManage::GetInstance()->SetSavePassword(ckbSavePassword->isSelected());
		GameManage::GetInstance()->SetCurUserName(tbName->getString());
		GameManage::GetInstance()->SetCurPassword(tbPassword->getString());

		//查询分区
		TcpLogic::GetInstance()->QueryPartitionInfoReq(0, INT16_MAX);
	}
	else
	{
		this->removeRecvingLayer();
		char str[48];
		sprintf(str, "登录错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	
	return ret;
}


int LoginLayout::QueryPartitionInfoRes(void* pBuf)
{
	//this->removeRecvingLayer();

	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "查询分区错误,错误码：%d", ret);
		MessageBox(str, "提示");
	}
	else
	{
		//这儿不直接写回调函数是为了切换下一帧去切换界面，否者要报错
		this->scheduleOnce(schedule_selector(LoginLayout::LoginChangeSence), 0.1f);
	}
	return ret;
}


void LoginLayout::LoginChangeSence(float a)
{
	this->removeRecvingLayer();
	PartitionLayer* partitionLayer = PartitionLayer::create();
	partitionLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, partitionLayer);
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
		RegistUserRes(data);
		break;
	case MSG_LOGIN_RES:
		LoginUserRes(data);
		break;
	case MSG_CONNECT_LICENSE_RES:
		//this->removeRecvingLayer();
		break;
	case MSG_QUERY_PARTITION_RES:
		this->QueryPartitionInfoRes(data);
		break;
	default:
		break;
	}
}