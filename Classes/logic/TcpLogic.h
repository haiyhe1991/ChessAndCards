#ifndef __RECV_LOGIC_H__
#define __RECV_LOGIC_H__

#include "chessAndCards.h"
#include "tcp/ServiceHeader.h"


#define TCP_DELAY_TIME			1000				//�ȴ���������Ӧ�ӳ�ʱ��1s
#define TCP_ANSWER_TIME_OUT		10000				//�ȴ����������Чʱ��10s
#define LICENSE_IP				"192.168.50.2"		//��֤������ip
#define LICENSE_PORT			10030				//��֤�������˿�

enum TcpState
{
	IDEL,			//���н׶�
	REQ,			//����׶�
	OUT_TIME,		//��ʱ�׶�
};

class WaitTime
{
public:
	WaitTime();
	void			StartWait(int time);	//��ʼ�ȴ�  ʱ�����   ���ʱ��Ϊ0����ô����һ��������ʱ�򣬽�ִ�к���Ĵ���
	bool			OnTime(int time);		//��¼ʱ��			true ʱ���ѵ�
private:
	int				recordTime;				//��¼��ʱ��
	bool			state;					//�Ƿ�ȴ�
};

class TcpLogic : public GameTick
{
public:

	static TcpLogic*				GetInstance();										//ȡ�õ���
	static void					DestoryInstance();										//�ݻٵ���
	~TcpLogic();																		//����

	virtual void							OnTick(int time);


	void						registerMessage();

	void						ChangeTcpState(TcpState tcpState);								//�ı�tcp״̬

	bool						CreatLicenseSocket();											//������֤����������

	void						ConnectLincenseRes();											//����������֤��������Ӧ

	const char*					getloginPlat();														//��ȡ��¼ƽ̨

	bool						RegistUserReq(const char*	pUsername, const char*	pPassword);		//ע���û�����

	bool						LoginUserReq(const char*	pUsername, const char*	pPassword);		//��¼�û�����

	bool						QueryPartitionInfoReq(UINT16 from, UINT16 num);						//��ѯ��������

	bool						EnterPartitionReq(UINT16	partitionId);							//�����������

	//��������Ч����buffer����Ч�����ܳ���
	int							RegistUserRes(SGSResPayload* pBuf, int pBufLen);		//ע���û����յ�������

	//��������Ч����buffer����Ч�����ܳ���
	int							LoginUserRes(SGSResPayload* pBuf, int pBufLen);			//��¼���յ�������

	//��������Ч����buffer����Ч�����ܳ���
	int							QueryPartitionInfoRes(SGSResPayload* pBuf, int pBufLen);//��ѯ�������յ�������

	//��������Ч����buffer����Ч�����ܳ���
	int							EnterPartitionRes(SGSResPayload* pBuf, int pBufLen);	//����������յ�������

	bool						CreatLinkerSocket();									//����linker����������,���ҶϿ���֤����������

	bool						LoginLinkReq();											//��¼linker����

	void						ConnectLinkRes();										//����linker������������Ӧ

	//��������Ч����buffer����Ч�����ܳ���
	int							LoginLinkerRes(SGSResPayload* pBuf, int pBufLen);	//����������յ�������

	bool						QueryRolesReq();										//��ѯ��ɫ��Ϣ����

	//��������Ч����buffer����Ч�����ܳ���
	int							QueryRolesRes(SGSResPayload* pBuf, int pBufLen);		//��ѯ��ɫ��Ϣ��Ӧ

	bool						CreateRoleReq(BYTE occuId,BYTE sex,const char* nick);	//������ɫ����

	//��������Ч����buffer����Ч�����ܳ���
	int							CreateRoleRes(SGSResPayload* pBuf, int pBufLen);			//������ɫ��Ӧ

	bool						EnterRoleReq(UINT32 roleId);							//�����ɫ����

	//��������Ч����buffer����Ч�����ܳ���
	int							EnterRoleRes(SGSResPayload* pBuf, int pBufLen);			//�����ɫ��Ӧ

	bool						JoinChannelReq();			//��������Ƶ������

	//��������Ч����buffer����Ч�����ܳ���
	int							JoinChannelRes(SGSResPayload* pBuf, int pBufLen);			//�����ɫ��Ӧ

	bool						ChatMessageSend(const char*	chatMsg);				//����������Ϣ

private:

	TcpLogic();																		//����

	static TcpLogic*				_instance;											//����

	SUserLoginRes*				m_pUserLoginRes;										//��¼������Ϣ
	SUserQueryPartitionRes*		m_pQueryPartitionRes;									//��ѯ����������Ϣ
	SUserEnterPartitionRes*		m_pEnterPartitionRes;									//��¼����������Ϣ
	SCreateRoleRes*				m_pCreateRoleRes;										//��ɫ����������Ϣ
	SEnterRoleRes*				m_pSEnterRoleRes;										//��ɫ����������Ϣ

	list<SPartitionInfo* >		container;												//������Ϣ����

	list<SSkillMoveInfo* >		containerSkill;											//������Ϣ����

	list<SQueryRoleAttr* >		containerRoleAttr;										//��ɫ��Ϣ

	WaitTime*					wait;

	TcpState					state;													//tcp״̬

};

#endif