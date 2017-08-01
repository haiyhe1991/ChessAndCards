#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "chessAndCards.h"
#include "BaseLayer.h"

//�����水ť��
class LoginLayout : public Layer, GameMessage
{
public:
	virtual bool init();

	virtual void							OnMessage(const int head, void* data);

	CREATE_FUNC(LoginLayout);

	~LoginLayout();

private:
	Node*					layout;					//��ť����Դ
	Node*					recv;					//�ڵ�����Դ

	TextField*				tbName;					//���������
	TextField*				tbPassword;				//���������

	Button*					register_game;			//ע����Ϸ��ť
	Button*					login_game;				//��ʼ��Ϸ��ť

	void					onRegHandler(Ref* sender);		//����Ĵ����¼�
	void					onLoginHandler(Ref* sender);	//����Ĵ����¼�

	void					registerMessage();
	void					unregisterMessage();

	void					removeRecvingLayer();			//�Ƴ����ս���

	//���������������ش���
	int						RegistUserRes(void* pBuf);		//ע���û��������

	//���������������ش���
	int						LoginUserRes(void* pBuf);		//��¼�û��������
};

class LoginLayer : public BaseLayer
{
public:
	virtual bool init();
	CREATE_FUNC(LoginLayer);
	~LoginLayer();

	virtual void onEnter();							//��ʼ������

	virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	virtual void releaseResource();					//�ͷ���Դ

private:
	LoginLayout*				loginLayout;		//�����ı�������					

};

#endif
