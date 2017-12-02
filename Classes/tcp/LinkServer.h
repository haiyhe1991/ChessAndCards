#ifndef __LINK_SERVER_H__
#define __LINK_SERVER_H__

#include "chessAndCards.h"
#include "CGameSocket.h"
#include "tcp/ServiceHeader.h"
#include "logic/TcpLogic.h"

class LinkServer
{
private:
	static LinkServer* _instance;
	CGameSocket* m_sock;
public:
	~LinkServer();																	//����
	static LinkServer*						GetInstance();							//����ģʽ
	static void								RecoverInstance();						//���ٶ���

	bool									CreatSocket(DWORD dwIP, int port);		//����socket

	void									RecvData();								//������Ϣ

	void									CloseLink();							//�ر�socket

	bool									CheckSocket();							//�������״̬

	bool									LoginLinkerReq(SLoginLinkerReq* pLoginLinkerReq);//�����¼linker

	bool									QueryRolesReq(SQueryRolesReq* req);			//��ѯ��ɫ

	bool									CreateRoleReq(SCreateRoleReq* req);			//������ɫ

	bool									EnterRoleReq(SEnterRoleReq* pEnterRoleReq);	//�����ɫ

	bool									JoinChannelReq(SJoinChannelReq* req);			//��������Ƶ������

	bool									ChatMessageSend(BYTE* pChat);//����������Ϣ

	bool									CreateTableReq(SCreateTabelReq* req);			//������������

	bool									JionTableReq(SJoinTableReq* req);			//������������

	bool									PlayerReadyReq(SReadyTableReq* req);			//���׼������

private:

	// DWORD ip ת��Ϊ �ַ���ip
	// szBuf���봫�ݣ�����Ϊ16�ֽڳ�
	const char*								GetStringIP(char szBuf[16], DWORD dwIP);				//ת��Ϊ�ַ���ip

	LinkServer();
	thread*									threadRecv;										//link���������������߳�
	bool									RecvThreadState;								//link���������������߳̿���

	char									m_key[DEFAULT_KEY_SIZE + 1];

	typedef int (TcpLogic::*fnLgProcessHandler)(SGSResPayload* payload, int nPayloadLen);	//link�������������ݶ�Ӧ����
	map<UINT16, fnLgProcessHandler>			m_lgProcHandler;								//link�������������ݶ�Ӧ��������
};


#endif