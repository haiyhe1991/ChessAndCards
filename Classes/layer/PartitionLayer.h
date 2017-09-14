#ifndef __PARTITION_SCENE_H__
#define __PARTITION_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//�������
class PartitionLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(PartitionLayout);

	PartitionLayout();
	~PartitionLayout();

private:
	Node*					layout;					//���������Դ
	Node*					recv;					//�ڵ�����Դ

	Text*					txtUserName;			//�û���label
	Text*					txtSelectPartition;		//ѡ��õķ���label
	Button*					start_game;				//��ʼ��Ϸ��ť
	Button*					btCancel;				//������Ϸ��ť
	ListView*				lstPartition;			//������ť�б�����
	list<Button*>			btPartitionLst;			//������ť�б�
	Button*					btMahjong;				//�齫��Ϸ��ť
	Button*					btChess;				//������Ϸ��ť

	void					onStartHandler(Ref* sender);	//����Ĵ����¼�
	void					onCancelHandler(Ref* sender);	//����Ĵ����¼�
	void					onPartionHandler(Ref* sender);	//����Ĵ����¼�

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					SenceLoad(float a);				//�������ػص�����
	void					FightChangeSence(float a);		//ս���л�����ص�����

	UINT16					m_currentPartitionId;					//��ǰҪ����ķ���id

	//���������������ش���
	int						QueryPartitionInfoRes(void* pBuf);		//��ѯ�����������

	//���������������ش���
	int						EnterPartitionRes(void* pBuf);		//��������������

};

class PartitionLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(PartitionLayer);
	~PartitionLayer();

	virtual void onEnter();							//��ʼ������

	virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	virtual void releaseResource();					//�ͷ���Դ

private:
	PartitionLayout*				partitionLayout;		//�����ı�������					

};

#endif
