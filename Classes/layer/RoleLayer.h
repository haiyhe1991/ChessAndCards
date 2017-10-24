#ifndef __ROLE_SCENE_H__
#define __ROLE_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

class CreateRoleLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(CreateRoleLayout);

	CreateRoleLayout();
	~CreateRoleLayout();

private:
	Node*					layout;								//���������Դ
	Node*					recv;								//�ڵ�����Դ

	Button*					btClose;							//�Ƴ�����
	Button*					btContinue;							//������Ϸ��ť
	CheckBox*				ckbMan;								//�Ա�ѡ����
	CheckBox*				ckbWoman;							//�Ա�ѡ��Ů
	TextField*				tbNick;								//�ǳ�
	Text*					lbOccuId;							//ְҵ��

	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					onCloseHandler(Ref* sender);		//����Ĵ����¼�
	void					onContinueHandler(Ref* sender);		//����Ĵ����¼�
	void					ManSelectedEvent(Ref *pSender, CheckBox::EventType type);		//��ѡ����ѡ������¼�
	void					WomanSelectedEvent(Ref *pSender, CheckBox::EventType type);		//��ѡ��Ůѡ������¼�

	int						CreateRoleRes(void* pBuf);		//������ɫ�������

};

//�������
class RoleLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(RoleLayout);

	RoleLayout();
	~RoleLayout();

private:
	Node*					layout;					//���������Դ
	Node*					recv;					//�ڵ�����Դ
	CreateRoleLayout*		creatRolelayout;		//������ɫ����Դ
	int						roleCount;				//��ɫ����

	Button*					btStartGame;				//��ʼ��Ϸ��ť
	Button*					btCreateRole;				//������ɫ��ť
	Button*					btLeft;						//���ư�ť
	Button*					btRight;					//���ư�ť
	PageView*				pageView_SelectPage;		//��ҳ����

	void					onStartHandler(Ref* sender);		//����Ĵ����¼�
	void					onCreateRoleHandler(Ref* sender);	//����Ĵ����¼�
	void					onLeftHandler(Ref* sender);			//����Ĵ����¼�
	void					onRightHandler(Ref* sender);		//����Ĵ����¼�
	void					pageViewEvent(Ref *pSender, PageView::EventType type);


	void					registerMessage();
	void					unregisterMessage();

	void					addRecvingLayer();				//��ӽ��ս���
	void					removeRecvingLayer();			//�Ƴ����ս���

	void					SenceLoad();					//�������ػص�����
	void					PageCurIndexChange();			//ҳ��仯�ı�����button����״̬

	void					FightChangeSence(float a);		//ս���л�����ص�����

	int						EnterRoleRes(void* pBuf);		//�����ɫ�������

	int						JoinChannelRes(void* pBuf);		//��������Ƶ���������



};

class RoleLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RoleLayer);
	~RoleLayer();

	virtual void onEnter();							//��ʼ������

	virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	virtual void releaseResource();					//�ͷ���Դ

private:
	RoleLayout*				roleLayout;		//�����ı�������					

};

#endif
