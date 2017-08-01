#ifndef __RECV_LOGIC_H__
#define __RECV_LOGIC_H__

#include "chessAndCards.h"
#include "tcp/ServiceHeader.h"


#define TCP_DELAY_TIME			1000		//�ȴ���������Ӧ�ӳ�ʱ��1s

enum RecvState
{
	RECVING,		//ϴ�ƽ׶�
	RECVED,			//���ƽ׶�
	PAUSEING,		//��ͣ�׶�
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

class TcpLogic : public GameMessage, GameTick
{
public:

	static TcpLogic*				GetInstance();										//ȡ�õ���
	static void					DestoryInstance();										//�ݻٵ���
	~TcpLogic();																		//����

	virtual void							OnTick(int time);
	virtual void							OnMessage(const int head, void* data);

	void						registerMessage();

	bool						CreatLicenseSocket(const char* ip, int port);					//������֤����������

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
private:

	TcpLogic();																		//����

	static TcpLogic*				_instance;											//����

	SUserLoginRes*				m_pUserLoginRes;										//��¼������Ϣ
	SUserQueryPartitionRes*		m_pQueryPartitionRes;									//��ѯ����������Ϣ
	SUserEnterPartitionRes*		m_pEnterPartitionRes;									//��¼����������Ϣ

	list<SPartitionInfo* >		container;												//������Ϣ����

	WaitTime*					wait;

	RecvState					state;													//����״̬

};

#endif