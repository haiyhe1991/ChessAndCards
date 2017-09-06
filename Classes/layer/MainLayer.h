#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//�������
class MainLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(MainLayout);

	MainLayout();
	~MainLayout();

private:
	Node*					layout;					//���������Դ
	Node*					recv;					//�ڵ�����Դ

	Button*					start_game;				//��ʼ��Ϸ��ť
	Button*					btCancel;				//������Ϸ��ť
	Button*					btCard;					//��������Ϸ��ť
	Button*					btMahjong;				//�齫��Ϸ��ť
	Button*					btChess;				//������Ϸ��ť

	void					onStartHandler(Ref* sender);	//����Ĵ����¼�
	void					onCancelHandler(Ref* sender);	//����Ĵ����¼�
	void					onCardHandler(Ref* sender);		//����Ĵ����¼�
	void					onMahjongHandler(Ref* sender);	//����Ĵ����¼�
	void					onChessHandler(Ref* sender);	//����Ĵ����¼�

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					FightChangeSence(float a);		//ս���л�����ص�����

	UINT16					m_currentPartitionId;					//��ǰҪ����ķ���id

	//���������������ش���
	int						QueryPartitionInfoRes(void* pBuf);		//��ѯ�����������

	//���������������ش���
	int						EnterPartitionRes(void* pBuf);		//��������������

};

class MainLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(MainLayer);
	~MainLayer();

	virtual void onEnter();							//��ʼ������

	virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	virtual void releaseResource();					//�ͷ���Դ

private:
	MainLayout*				mainLayout;		//�����ı�������					

};

#endif
