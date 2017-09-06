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

	MSG_CONNECT_LICENSE_RES				= 2001,			//����license��Ӧ
	MSG_REQ_OUT_TIME					= 2002,			//����ʱ
	MSG_REGIST_RES						= 2004,			//ע����Ӧ
	MSG_LOGIN_RES						= 2006,			//��¼��Ӧ
	MSG_QUERY_PARTITION_RES				= 2008,			//��ѯ������Ӧ
	MSG_ENTER_PARTITION_RES				= 2010,			//���������Ӧ
	MSG_CONNECT_LINK_RES				= 2012,			//����link��Ӧ
	MSG_LOGIN_LINK_RES					= 2014,			//��½link��Ӧ
	MSG_QUERY_ROLE_RES					= 2015,			//��ѯ��ɫ��Ӧ
	MSG_CREATE_ROLE_RES					= 2016,			//������ɫ��Ӧ
	MSG_ENTER_ROLE_RES					= 2017,			//�����ɫ��Ӧ
	MSG_JOIN_CHANNEL_RES				= 2018,			//����Ƶ����Ӧ

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
