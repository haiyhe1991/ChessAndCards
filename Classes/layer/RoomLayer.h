#ifndef __ROOM_SCENE_H__
#define __ROOM_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

class JionTableLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(JionTableLayout);

	JionTableLayout();
	~JionTableLayout();

private:
	Node*					layout;								//���������Դ
	Node*					recv;								//�ڵ�����Դ

	ListView*				lstViewNum;
	Button*					btClose;							//�Ƴ�����
	Button*					btRemove;							//������Ϸ��ť
	Button*					btReWrite;							//������Ϸ��ť
	Button*					btNum[10];

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					onCloseHandler(Ref* sender);		//����Ĵ����¼�
	void					onRemoveHandler(Ref* sender);		//����Ĵ�����
	void					onReWriteHandler(Ref* sender);		//����Ĵ�����
	void					onNumHandler(Ref* sender);			//����Ĵ�����

	void					FightChangeSence(float a);		//ս���л�����ص�����

	int						JionTableRes(void* pBuf);		//������ɫ�������

};

//�������
class RoomLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(RoomLayout);

	RoomLayout();
	~RoomLayout();

private:
	Node*					layout;					//���������Դ
	Node*					room;
	Node*					recv;					//�ڵ�����Դ
	JionTableLayout*		jionTableLayout;		//������ɫ����Դ

	Button*					btCreateTable;				//����������ť
	Button*					btJionTable;				//����������ť

	void					onCreateTableHandler(Ref* sender);		//����Ĵ����¼�
	void					onJionTableHandler(Ref* sender);	//����Ĵ����¼�

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					FightChangeSence(float a);		//ս���л�����ص�����

	int						CreateTableRes(void* pBuf);		//�����ɫ�������

};

class RoomLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RoomLayer);
	~RoomLayer();

	virtual void onEnter();							//��ʼ������

	virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	virtual void releaseResource();					//�ͷ���Դ

private:
	RoomLayout*				roomLayout;		//�����ı�������					

};

#endif
