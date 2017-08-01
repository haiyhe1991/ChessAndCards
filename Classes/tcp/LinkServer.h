#ifndef __LINK_SERVER_H__
#define __LINK_SERVER_H__

#include "chessAndCards.h"
#include "CGameSocket.h"
#include "tcp/ServiceHeader.h"

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

	bool									LoginLinkerReq(SLoginLinkerReq* pUserRegistReq);//�����¼linker



private:

	// DWORD ip ת��Ϊ �ַ���ip
	// szBuf���봫�ݣ�����Ϊ16�ֽڳ�
	const char*								GetStringIP(char szBuf[16], DWORD dwIP);				//ת��Ϊ�ַ���ip

	LinkServer();
	thread*									threadRecv;										//link���������������߳�
	bool									RecvThreadState;								//link���������������߳̿���

	char									m_key[DEFAULT_KEY_SIZE + 1];
};


#endif