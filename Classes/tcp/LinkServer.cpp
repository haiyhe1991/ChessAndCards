#include "LinkServer.h"
#include "LicenseServer.h"
#include "aes.h"
#include "logic/TcpLogic.h"

LinkServer *LinkServer::_instance = NULL;
LinkServer* LinkServer::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new LinkServer();
	}
	return _instance;
}
void LinkServer::RecoverInstance()
{
	if (_instance != NULL)
	{
		delete _instance;
	}
}

LinkServer::LinkServer()
{
	memset(m_key, 0, sizeof(m_key));
	strcpy(m_key, DEFAULT_TOKEN_KEY);
}

LinkServer::~LinkServer()
{
	this->CloseLink();
}

bool LinkServer::CreatSocket(DWORD dwIP, int port)
{
	m_sock = new CGameSocket();
	char szBuf[16];
	if (m_sock->Create(GetStringIP(szBuf,dwIP), port))
	{
		RecvThreadState = true;
		threadRecv = new thread(&LinkServer::RecvData, this);
		threadRecv->detach();
		return true;
	}
	return false;
}

void LinkServer::CloseLink()
{
	RecvThreadState = false;
	m_sock->Destroy();
}

// DWORD ip 转化为 字符串ip
// szBuf必须传递，至少为16字节长
const char* LinkServer::GetStringIP(char szBuf[16], DWORD dwIP)
{
	if (NULL == szBuf)
		return NULL;
	sprintf(szBuf, "%d.%d.%d.%d", (BYTE)dwIP, (BYTE)(dwIP >> 8), (BYTE)(dwIP >> 16), (BYTE)(dwIP >> 24));
	return szBuf;
}

void LinkServer::RecvData()
{
	while (RecvThreadState)
	{
		char pbuf[_MAX_MSGSIZE];
		int len = _MAX_MSGSIZE;
		if (m_sock->ReceiveMsg(&pbuf, len))
		{
			SGSProtocolHead *pSGSHeader = (SGSProtocolHead*)pbuf;
			pSGSHeader->ntoh();

			if (pSGSHeader->isCrypt)
				appDecryptDataWithKey((AES_BYTE*)((char*)pSGSHeader + SGS_PROTO_HEAD_LEN), pSGSHeader->pduLen, m_key);  //解码

			SGSResPayload* pResPayload = (SGSResPayload*)(pbuf + SGS_PROTO_HEAD_LEN);
			pResPayload->ntoh();

			int retCode = LCS_OK;
			//std::map<UINT16, fnLgProcessHandler>::iterator it = m_lgProcHandler.find(pResPayload->cmdCode);
			//if (it != m_lgProcHandler.end())
			//	retCode = (this->*(it->second))(pResPayload, pResPayload->rawDataBytes);

			if (pSGSHeader->pktType == ESGSPacketType::DATA_TOKEN_KEY_PACKET)
			{
				SConnectLinkerRes* pConnectLinkerRes = (SConnectLinkerRes*)(pResPayload->data);
				pConnectLinkerRes->ntoh();
				memcpy(m_key, pConnectLinkerRes->key, DEFAULT_KEY_SIZE);
			}
		}
	}
}

bool LinkServer::LoginLinkerReq(SLoginLinkerReq* pUserRegistReq)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SLoginLinkerReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = LINKER_LOGIN;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SLoginLinkerReq* UserRegistReq = (SLoginLinkerReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	UserRegistReq->userId = pUserRegistReq->userId;
	strcpy(UserRegistReq->token, pUserRegistReq->token);
	UserRegistReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}
