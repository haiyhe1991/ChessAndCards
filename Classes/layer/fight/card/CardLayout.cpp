#include "layer/layer_header.h"
#include "resource/ResourceManager.h"
#include "msg/message.h"
bool CardLayout::init()
{
	Layer::init();
	layout = (Layout*)ResourceManager::GetInstance()->Get_ui_resource("ui/card/CardScene.csb");
	this->addChild(layout);

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

	//btCard = (Button*)layout->getChildByName("btCard");
	////����¼�����
	//btCard->addClickEventListener(CC_CALLBACK_1(MainLayout::onCardHandler, this));

	//btMahjong = (Button*)layout->getChildByName("btMahjong");
	////����¼�����
	//btMahjong->addClickEventListener(CC_CALLBACK_1(MainLayout::onMahjongHandler, this));

	//btChess = (Button*)layout->getChildByName("btChess");
	////����¼�����
	//btChess->addClickEventListener(CC_CALLBACK_1(MainLayout::onChessHandler, this));

	//onCardHandler(nullptr);

	return true;
}

void CardLayout::onClearHandler(Ref* sender)
{
	lstChatRecv->removeAllChildren();
}

void CardLayout::onSendHandler(Ref* sender)
{
	//������Ϸ���ؽ��棬����ս��������Դ
	CCLOG("onCancelHandler");
	//GameLayer* gameLayer = GameLayer::create();
	//gameLayer->retain();
	//MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, gameLayer);
}


void CardLayout::onCardHandler(Ref* sender)
{
	//������Ϸ���ؽ��棬����ս��������Դ
	CCLOG("onCardHandler");
	//GameLayer* gameLayer = GameLayer::create();
	//gameLayer->retain();
	//MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, gameLayer);
}


void CardLayout::onMahjongHandler(Ref* sender)
{
	//������Ϸ���ؽ��棬����ս��������Դ
	CCLOG("onMahjongHandler");
	//GameLayer* gameLayer = GameLayer::create();
	//gameLayer->retain();
	//MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, gameLayer);
}

void CardLayout::onChessHandler(Ref* sender)
{
	//������Ϸ���ؽ��棬����ս��������Դ
	CCLOG("onChessHandler");
	//GameLayer* gameLayer = GameLayer::create();
	//gameLayer->retain();
	//MsgManager::GetInstance()->Dispather(MessageHead::MSG_START_LOADING, gameLayer);
}

