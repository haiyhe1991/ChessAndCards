#include "LicenseServer.h"
#include "LinkServer.h"
#include "aes.h"


#define ADD_LC_PROC_HANDLER(cmdCode, fun) \
do \
		{   \
		m_lgProcHandler.insert(std::make_pair(cmdCode, &TcpLogic::fun));  \
		} while (0)

LicenseServer *LicenseServer::_instance = NULL;
LicenseServer* LicenseServer::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new LicenseServer();
	}
	return _instance;
}
void LicenseServer::RecoverInstance()
{
	if (_instance != NULL)
	{
		delete _instance;
	}
}

LicenseServer::LicenseServer()
{
	memset(m_key, 0, sizeof(m_key));
	strcpy(m_key, DEFAULT_TOKEN_KEY);
	ADD_LC_PROC_HANDLER(LICENCE_USER_REGIST, RegistUserRes);
	ADD_LC_PROC_HANDLER(LICENCE_USER_LOGIN, LoginUserRes);
	ADD_LC_PROC_HANDLER(LICENCE_USER_QUERY_PARTITION, QueryPartitionInfoRes);
	ADD_LC_PROC_HANDLER(LICENCE_USER_ENTER_PARTITION, EnterPartitionRes);
}

LicenseServer::~LicenseServer()
{
	m_lgProcHandler.clear();
	this->CloseLicense();
}

bool LicenseServer::CreatSocket(const char* ip, int port)
{
	m_sock = new CGameSocket();
	if (m_sock->Create(ip, port))
	{
		RecvThreadState = true;
		threadRecv = new thread(&LicenseServer::RecvData, this);
		threadRecv->detach();
		return true;
	}
	return false;
}

void LicenseServer::RecvData()
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
				appDecryptDataWithKey((AES_BYTE*)((char*)pSGSHeader + SGS_PROTO_HEAD_LEN), pSGSHeader->pduLen, m_key);  //½âÂë

			SGSResPayload* pResPayload = (SGSResPayload*)(pbuf + SGS_PROTO_HEAD_LEN);
			pResPayload->ntoh();

			int retCode = LCS_OK;
			std::map<UINT16, fnLgProcessHandler>::iterator it = m_lgProcHandler.find(pResPayload->cmdCode);
			if (it != m_lgProcHandler.end())
				retCode = (TcpLogic::GetInstance()->*(it->second))(pResPayload, pResPayload->rawDataBytes);

			if (pSGSHeader->pktType == ESGSPacketType::DATA_TOKEN_KEY_PACKET)
			{
				memcpy(m_key, pResPayload->data, DEFAULT_KEY_SIZE);
				TcpLogic::GetInstance()->ConnectLincenseRes();
			}
		}
	}
}

bool LicenseServer::RegistUserReq(SUserRegistReq*	pUserRegistReq)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SUserRegistReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = LICENCE_USER_REGIST;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SUserRegistReq* UserRegistReq = (SUserRegistReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	strcpy(UserRegistReq->devid, pUserRegistReq->devid);
	strcpy(UserRegistReq->loginPlat, pUserRegistReq->loginPlat);
	strcpy(UserRegistReq->model, pUserRegistReq->model);
	strcpy(UserRegistReq->os, pUserRegistReq->os);
	strcpy(UserRegistReq->username, pUserRegistReq->username);
	strcpy(UserRegistReq->password, pUserRegistReq->password);
	strcpy(UserRegistReq->ver, pUserRegistReq->ver);

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LicenseServer::LoginUserReq(SUserPswLoginReq* pUserPswLoginReq)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SUserPswLoginReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = LICENCE_USER_LOGIN;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SUserPswLoginReq* UserPswLoginReq = (SUserPswLoginReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	strcpy(UserPswLoginReq->devid, pUserPswLoginReq->devid);
	strcpy(UserPswLoginReq->loginPlat, pUserPswLoginReq->loginPlat);
	strcpy(UserPswLoginReq->model, pUserPswLoginReq->model);
	strcpy(UserPswLoginReq->os, pUserPswLoginReq->os);
	strcpy(UserPswLoginReq->username, pUserPswLoginReq->username);
	strcpy(UserPswLoginReq->password, pUserPswLoginReq->password);
	strcpy(UserPswLoginReq->ver, pUserPswLoginReq->ver);
	UserPswLoginReq->mode = pUserPswLoginReq->mode;

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LicenseServer::QueryPartitionInfoReq(SUserQueryPartitionReq*	pUserQueryPartitionReq)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SUserQueryPartitionReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = LICENCE_USER_QUERY_PARTITION;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SUserQueryPartitionReq*	UserQueryPartitionReq = (SUserQueryPartitionReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	UserQueryPartitionReq->from = pUserQueryPartitionReq->from;
	UserQueryPartitionReq->num = pUserQueryPartitionReq->num;
	UserQueryPartitionReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LicenseServer::EnterPartitionReq(SUserEnterPartitionReq*	pUserEnterPartitionReq)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SUserEnterPartitionReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = LICENCE_USER_ENTER_PARTITION;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SUserEnterPartitionReq* UserEnterPartitionReq = (SUserEnterPartitionReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	UserEnterPartitionReq->partitionId = pUserEnterPartitionReq->partitionId;
	UserEnterPartitionReq->userId = pUserEnterPartitionReq->userId;
	UserEnterPartitionReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

void LicenseServer::CloseLicense()
{
	RecvThreadState = false;
	m_sock->Destroy();
}