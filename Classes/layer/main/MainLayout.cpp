#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool MainLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/main/MainScene.csb");
	this->addChild(layout);

	start_game = (Button*)layout->getChildByName("btStart");
	//����¼�����
	start_game->addClickEventListener(CC_CALLBACK_1(MainLayout::onStartHandler, this));

	btCancel = (Button*)layout->getChildByName("btCancel");
	//����¼�����
	btCancel->addClickEventListener(CC_CALLBACK_1(MainLayout::onCancelHandler, this));

	btCard = (Button*)layout->getChildByName("btCard");
	//����¼�����
	btCard->addClickEventListener(CC_CALLBACK_1(MainLayout::onCardHandler, this));

	btMahjong = (Button*)layout->getChildByName("btMahjong");
	//����¼�����
	btMahjong->addClickEventListener(CC_CALLBACK_1(MainLayout::onMahjongHandler, this));

	btChess = (Button*)layout->getChildByName("btChess");
	//����¼�����
	btChess->addClickEventListener(CC_CALLBACK_1(MainLayout::onChessHandler, this));

	m_currentPartitionId = 0;

	this->registerMessage();

	if (TcpLogic::GetInstance()->QueryPartitionInfoReq(0, INT16_MAX))
	{
		//RecvingLayer*  recv = RecvingLayer::create();
		//this->addChild(recv, INT_MAX, 10);
		MsgManager::GetInstance()->Dispather(MessageHead::MSG_QUERY_PARTITION_REQ, nullptr);
	}

	return true;
}
MainLayout::~MainLayout()
{
	this->unregisterMessage();
}

void MainLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_QUERY_PARTITION_RES, this);
	manager->Reg(MSG_ENTER_PARTITION_RES, this);
}

void MainLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_QUERY_PARTITION_RES, this);
	manager->Unreg(MSG_ENTER_PARTITION_RES, this);
}

void MainLayout::removeRecvingLayer()
{
	//this->getChildByTag(10)->removeAllChildrenWithCleanup(false);
	//this->getChildByTag(10)->removeFromParentAndCleanup(false);
}

void MainLayout::onStartHandler(Ref* sender)
{
	//������Ϸ���ؽ��棬����ս��������Դ
	CCLOG("fight layer change");
	//�������
	if (TcpLogic::GetInstance()->EnterPartitionReq(m_currentPartitionId))
	{
		CardLayer* cardLayer = CardLayer::create();
		cardLayer->retain();
		MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, cardLayer);
	}
}

void MainLayout::onCancelHandler(Ref* sender)
{
	//������Ϸ���ؽ��棬����ս��������Դ
	CCLOG("onCancelHandler");
	//GameLayer* gameLayer = GameLayer::create();
	//gameLayer->retain();
	//MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, gameLayer);
}


void MainLayout::onCardHandler(Ref* sender)
{
	m_currentPartitionId = 0; 
}


void MainLayout::onMahjongHandler(Ref* sender)
{
	m_currentPartitionId = 1;
}

void MainLayout::onChessHandler(Ref* sender)
{
	m_currentPartitionId = 2;
}

int MainLayout::QueryPartitionInfoRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "��ѯ��������,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	return LCS_OK;
}

int MainLayout::EnterPartitionRes(void* pBuf)
{
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "�����������,�����룺%d", ret);
		MessageBox(str, "��ʾ");
		return -1;
	}
	CardLayer* cardLayer = CardLayer::create();
	cardLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, cardLayer);
	return LCS_OK;
}

void MainLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("���ӳ�ʱ", "��ʾ");
		break;
	case MSG_QUERY_PARTITION_RES:
		QueryPartitionInfoRes(data);
		this->removeRecvingLayer();
		break;
	case MSG_ENTER_PARTITION_RES:
		EnterPartitionRes(data);
		this->removeRecvingLayer();
		break;
	default:
		break;
	}
}
