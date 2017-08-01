#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//�������
class CardLayout : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(CardLayout);

private:
	Node*					layout;					//���������Դ

	ListView*				lstChatRecv;				//����Ϣ��

	Button*					bt_clear;				//����յ���Ϣ��ť
	Button*					bt_send;				//������Ϣ��ť
	Button*					btCard;					//��������Ϸ��ť
	Button*					btMahjong;				//�齫��Ϸ��ť
	Button*					btChess;				//������Ϸ��ť

	void					onClearHandler(Ref* sender);	//����Ĵ����¼�
	void					onSendHandler(Ref* sender);		//����Ĵ����¼�
	void					onCardHandler(Ref* sender);		//����Ĵ����¼�
	void					onMahjongHandler(Ref* sender);	//����Ĵ����¼�
	void					onChessHandler(Ref* sender);	//����Ĵ����¼�
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