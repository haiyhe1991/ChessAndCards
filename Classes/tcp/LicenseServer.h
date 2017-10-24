#ifndef __LICENSE_SERVER_H__
#define __LICENSE_SERVER_H__

#include "chessAndCards.h"
#include "CGameSocket.h"
#include "tcp/ServiceHeader.h"
#include "logic/TcpLogic.h"

class LicenseServer
{
private:
	static LicenseServer* _instance;
	CGameSocket* m_sock;
public:
	~LicenseServer();																					//����
	static LicenseServer*					GetInstance();												//����ģʽ
	static void								RecoverInstance();											//���ٵ���

	bool									CreatSocket(const char* ip, int port);	

	bool									CheckSocket();							//�������״̬

	void									RecvData();													//���������̻߳ص�

	bool									RegistUserReq(SUserRegistReq*	pUserRegistReq);			//ע���û�����

	bool									LoginUserReq(SUserPswLoginReq*	pUserPswLoginReq);			//��¼�û�����

	bool									QueryPartitionInfoReq(SUserQueryPartitionReq*	pUserQueryPartitionReq);	//��ѯ��������

	bool									EnterPartitionReq(SUserEnterPartitionReq*	pUserEnterPartitionReq);		//�����������

	void									CloseLicense();											//�ر���֤����������

private:
	LicenseServer();																		//����
	char									m_key[DEFAULT_KEY_SIZE + 1];

	typedef int (TcpLogic::*fnLgProcessHandler)(SGSResPayload* payload, int nPayloadLen);	//��֤�������������ݶ�Ӧ����
	map<UINT16, fnLgProcessHandler>			m_lgProcHandler;								//��֤�������������ݶ�Ӧ��������

	thread*									threadRecv;										//��֤���������������߳�
	bool									RecvThreadState;								//��֤���������������߳̿���
};



#endif