#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

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
	Node*					layout;					//���������Դ
	Node*					recv;					//�ڵ�����Դ

	TextField*				btSendData;				//�༭������Ϣ��
	ListView*				lstChatRecv;				//����Ϣ��

	Button*					bt_clear;				//����յ���Ϣ��ť
	Button*					bt_send;				//������Ϣ��ť

	void					onClearHandler(Ref* sender);	//����Ĵ����¼�
	void					onSendHandler(Ref* sender);		//����Ĵ����¼�

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���


	void					ConnectLinkRes();				//����link���ӷ������
	//���������������ش���
	int						LoginLinkRes(void* pBuf);		//��½link�������

	int						QueryRoleRes(void* pBuf);		//��ѯ��ɫ�������

	int						CreateRoleRes(void* pBuf);		//������ɫ�������

	int						EnterRoleRes(void* pBuf);		//�����ɫ�������

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