#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

class GameRuleLayout : public Layer
{
public:
	virtual bool init();

	CREATE_FUNC(GameRuleLayout);

	GameRuleLayout();
	~GameRuleLayout();

private:
	Node*					layout;								//���������Դ

	Button*					btClose;							//�˳�����

	void					onCloseHandler(Ref* sender);		//����Ĵ����¼�

};

class GameOverLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(GameOverLayout);

	GameOverLayout();
	~GameOverLayout();

private:

	struct ScoreUI
	{
		Text*					lbPlayer;						//��ɫ�ǳ�label
		Text*					lbScore;						//��ɫ�÷�label
	};

	Node*					layout;								//���������Դ

	vector<ScoreUI*>		scoreUI_vec;						//ʹ��vector �������

	Button*					btContinue;							//�˳�����
	Sprite*					win;
	Sprite*					winClickTip;

	void					onCloseHandler(Ref* sender);		//����Ĵ����¼�

	void					calculateScore(void *data);			//����÷�

};

class CalculateLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(CalculateLayout);

	CalculateLayout();
	~CalculateLayout();

private:

	struct ScoreUI
	{
		Text*					lbPlayer;						//��ɫ�ǳ�label
		Text*					lbScore;						//��ɫ�÷�label
	};

	Node*					layout;								//���������Դ

	vector<ScoreUI*>		scoreUI_vec;						//ʹ��vector �������

	Button*					btClose;							//�˳�����

	void					onCloseHandler(Ref* sender);		//����Ĵ����¼�

	void					calculateScore(void *data);			//����÷�

};

class GameMenuLayout : public Layer
{
public:
	virtual bool init();

	CREATE_FUNC(GameMenuLayout);

	GameMenuLayout();
	~GameMenuLayout();

private:
	Node*					layout;								//���������Դ

	Button*					btExitFight;						//�Ƴ�����
	Button*					btContinue;							//������Ϸ��ť
	Button*					btSetting;							//�Ա�ѡ����
	Button*					btRule;								//�Ա�ѡ��Ů
	Button*					btAchieve;							//�ǳ�
	GameRuleLayout*			gameRuleLayout;						//������Ϸ������Դ

	void					onCloseHandler(Ref* sender);		//����Ĵ����¼�
	void					onContinueHandler(Ref* sender);		//����Ĵ����¼�
	void					onSettingHandler(Ref* sender);		//����Ĵ����¼�
	void					onRuleHandler(Ref* sender);			//����Ĵ����¼�
	void					onAchieveHandler(Ref* sender);		//����Ĵ����¼�

};

class CenterLogic;
//�������
class CardLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(CardLayout);

	CardLayout();
	~CardLayout();

private:

	CenterLogic*				center_logic;		//�����߼�����

	struct PlayerUI
	{
		Text*					lbRoleNick;			//��ɫ�ǳ�label
		Text*					lbReady;			//׼��label
		Button*					btReady;			//׼����ť
		Button*					btUnReady;			//ȡ��׼����ť
		Node*					cardsNode;			//����λ��
		list<SpriteButton*>		chess_lst;			//����
		Sprite*					leftCards;			//ʣ����չʾ
		Text*					lbLeftCardsCount;	//ʣ��������
		Text*					lbScore;			//�÷�

		void					setEnable(bool enable)
		{
			list<SpriteButton*>::iterator iter = chess_lst.begin();
			while (iter != chess_lst.end())
			{
				(*iter)->setEnable(enable);
				iter++;
			}
		}
	};
	Node*					layout;					//���������Դ
	Layout*					player;					//��ɫ��
	ActionTimeline*			hitInvalidAction;		//������Ч����
	ActionTimeline*			cardAction;				//ϴ�ƶ���
	Node*					recv;					//�ڵ�����Դ
	GameMenuLayout*			gameMenuLayout;			//�����˵�����Դ
	CalculateLayout*		calculateLayout;		//�����������Դ
	GameOverLayout*			gameOverLayout;			//�����������Դ

	vector<PlayerUI*>		playerUI_vec;			//ʹ��vector �������
	vector<PlayerUI*>::iterator			action_iter;							//��ǰ���ڶ��������

	Button*					btChat;					//���찴ť
	Button*					btGameMenu;				//��Ϸ�˵���ť
	Button*					btPorkPile;				//�ƶѰ�ť
	vector<SpriteButton*>			poker_vec;							//�˿���  ���е�
	list<SpriteButton*>				distribute_lst;						//�ɷ�list
	list<SpriteButton*>				recycle_lst;						//����list
	Node*					PokerRecycleNode;						//�����ƶѽڵ�
	Sprite*					WindowColorTip;
	Node*					lstColorChoose;
	Sprite*					ColorTip;

	void					onChatHandler(Ref* sender);			//����Ĵ����¼�
	void					onGameMenuHandler(Ref* sender);		//����Ĵ����¼�
	void					WashChess();						//ϴ��
	void					AddChess(void* data);				//����
	void					HitChess(void* data);				//����
	void					HitChangeChess(int pokerColor);		//����ɫ
	void					ResetChess(void* data);				//���ô���
	void					onCardFrameEvent(Frame* frame);		//ϴ�ƽ�������

	void					onReadyHandler(Ref* sender);		//����Ĵ����¼�
	void					onUnReadyHandler(Ref* sender);		//����Ĵ����¼�
	void					onCardHandler(Ref* sender);			//����Ĵ����¼�
	void					onChooseColor(Ref* sender);			//����Ĵ����¼�
	void					onPorkPile(Ref* sender);			//����Ĵ����¼�

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					initAntimation();				//��ʼ������
	void					initPlayer();					//��ʼ�����
	void					initChess();					//��ʼ����
	void					ChangePlayerCards(int id);		//�ı�������Ƶ���ʾ

	int						JoinChannelRes(void* pBuf);		//��������Ƶ���������
};

class CardLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(CardLayer);
	~CardLayer();

	virtual void onEnter();							//��ʼ������

	virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	virtual void releaseResource();					//�ͷ���Դ

private:
	CardLayout*				cardLayout;		//�����ı�������					

};


#endif