#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "logic/TcpLogic.h"

bool JionTableLayout::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/room/JionRoomNode.csb");
	this->addChild(layout);

	Node* PanelMain = (Layout*)layout->getChildByName("PanelMain");

	btRemove = (Button*)layout->getChildByName("btRemove");
	//����¼�����
	btRemove->addClickEventListener(CC_CALLBACK_1(JionTableLayout::onRemoveHandler, this));

	btClose = (Button*)layout->getChildByName("btClose");
	//����¼�����
	btClose->addClickEventListener(CC_CALLBACK_1(JionTableLayout::onCloseHandler, this));

	btReWrite = (Button*)layout->getChildByName("btReWrite");
	//����¼�����
	btReWrite->addClickEventListener(CC_CALLBACK_1(JionTableLayout::onReWriteHandler, this));
	
	lstViewNum = (ListView*)layout->getChildByName("lstViewNum");

	for (int i = 0; i < 10; i++)
	{
		char str[24] = {};
		sprintf(str, "btNum%d", i);
		btNum[i] = (Button*)layout->getChildByName(str);
		btNum[i]->setTag(i);
		btNum[i]->addClickEventListener(CC_CALLBACK_1(JionTableLayout::onNumHandler, this));
	}

	return true;
}

JionTableLayout::JionTableLayout()
{

	recv = nullptr;

	this->registerMessage();

}

JionTableLayout::~JionTableLayout()
{
	this->unregisterMessage();
}

void JionTableLayout::onRemoveHandler(Ref* sender)
{
	lstViewNum->removeLastItem();
}

void JionTableLayout::onReWriteHandler(Ref* sender)
{
	lstViewNum->removeAllChildren();
}

void JionTableLayout::onNumHandler(Ref* sender)
{
	if (lstViewNum->getChildrenCount() == 6)
	{
		return;
	}
	Node* node = (Node*)sender;
	Text* lbNum = Text::create();
	lbNum->setFontSize(36);
	char str[24] = {};
	sprintf(str, "%d", node->getTag());
	lbNum->setText(str);
	lbNum->setTag(lstViewNum->getChildrenCount());
	lstViewNum->addChild(lbNum);
	if (lstViewNum->getChildrenCount() == 6)
	{
		char strNum[8] = {};
		//��6���������ͼ�����������
		for (int i = 0; i < 6;i++)
		{ 
			strcat(strNum, ((Text*)lstViewNum->getChildByTag(i))->getString().c_str());
		}

		CCLOG(strNum);
		this->addRecvingLayer();
		if (!TcpLogic::GetInstance()->JoinTableReq(strNum))
		{
			this->removeRecvingLayer();
			MessageBox("���䴴������ʧ��", "��ʾ");
		}
	}
}

void JionTableLayout::onCloseHandler(Ref* sender)
{
	this->removeAllChildrenWithCleanup(false);
	this->removeFromParentAndCleanup(false);
}

void JionTableLayout::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REQ_OUT_TIME, this);
	manager->Reg(MSG_JION_TABLE_RES, this);
}

void JionTableLayout::unregisterMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Unreg(MSG_REQ_OUT_TIME, this);
	manager->Unreg(MSG_JION_TABLE_RES, this);
}

void JionTableLayout::removeRecvingLayer()
{
	if (recv != nullptr)
	{
		recv->removeAllChildrenWithCleanup(false);
		recv->removeFromParentAndCleanup(false);
		recv = nullptr;
	}
}

void JionTableLayout::addRecvingLayer()
{
	if (recv == nullptr)
	{
		recv = RecvingLayer::create();
		this->addChild(recv, INT_MAX);
	}
}

void JionTableLayout::FightChangeSence(float a)
{
	this->removeRecvingLayer();
	CardLayer* cardLayer = CardLayer::create();
	cardLayer->retain();
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, cardLayer);
}

int JionTableLayout::JionTableRes(void* pBuf)
{
	this->removeRecvingLayer();
	UINT16 ret = *(UINT16*)pBuf;
	if (ret != LCS_OK)
	{
		char str[48];
		sprintf(str, "������ɫ����,�����룺%d", ret);
		MessageBox(str, "��ʾ");
	}
	else
	{
		//�����ֱ��д�ص�������Ϊ���л���һ֡ȥ�л����棬����Ҫ����
		this->scheduleOnce(schedule_selector(JionTableLayout::FightChangeSence), 0.1f);
	}
	return ret;
}

void JionTableLayout::OnMessage(const int head, void* data)
{
	switch (head)
	{
	case MSG_REQ_OUT_TIME:
		this->removeRecvingLayer();
		MessageBox("���ӳ�ʱ", "��ʾ");
		break;
	case MSG_JION_TABLE_RES:
		this->JionTableRes(data);
		break;
	default:
		break;
	}
}