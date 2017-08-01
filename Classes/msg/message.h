#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <map>
#include <list>

using namespace std;

enum MessageHead
{
	MSG_START_LOGO						= 1000,
	MSG_CHANGE_SCENE					= 1001,			//�л�������Ϣ   ��װ֮�󣬲����ڳ����ˣ�ֻ�и��ֲ�
	MSG_START_LOADING					= 1002,			//��ʼ���ؽ���

	MSG_REQ_OUT_TIME					= 2000,			//����ʱ
	MSG_REGIST_REQ						= 2001,			//ע������
	MSG_REGIST_RES						= 2002,			//ע����Ӧ
	MSG_LOGIN_REQ						= 2003,			//��¼����
	MSG_LOGIN_RES						= 2004,			//��¼��Ӧ
	MSG_QUERY_PARTITION_REQ				= 2005,			//��ѯ��������
	MSG_QUERY_PARTITION_RES				= 2006,			//��ѯ������Ӧ
	MSG_ENTER_PARTITION_REQ				= 2007,			//�����������
	MSG_ENTER_PARTITION_RES				= 2008,			//���������Ӧ

	MSG_START_GAME						= 3001,			//��ʼ��Ϸ
	MSG_ACTION_CHESS					= 3002,			//��Ҷ���
};

//��Ϸ�е�ͨ�Žӿ�
class GameMessage
{
public:

	virtual void OnMessage(const int head, void* data) = 0;			//���յ���Ϣ
};

//��Ϣ������
class MsgManager
{
public:
	static MsgManager*			GetInstance();
	static void					DestoryInstance();

	void						Reg(const int head, GameMessage* msg);				//ע����Ϣ

	void						Unreg(const int head, GameMessage* msg);			//ע����Ϣ

	void						Dispather(const int head, void* data);				//������Ϣ

	void						Clear();											//���������Ϣ

private:
	static MsgManager*				_instance;

	map<int, list<GameMessage*>>	container;					//��Ϣ��������
};

#endif
