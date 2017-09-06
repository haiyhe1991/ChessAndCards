#include "LinkServer.h"
#include "LicenseServer.h"
#include "aes.h"
#include "logic/TcpLogic.h"

#define ADD_LINK_PROC_HANDLER(cmdCode, fun) \
do \
		{   \
		m_lgProcHandler.insert(std::make_pair(cmdCode, &TcpLogic::fun));  \
		} while (0)

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
	ADD_LINK_PROC_HANDLER(LINKER_LOGIN, LoginLinkerRes);
	ADD_LINK_PROC_HANDLER(MLS_QUERY_ROLES, QueryRolesRes);
	ADD_LINK_PROC_HANDLER(MLS_CREATE_ROLE, CreateRoleRes);
	ADD_LINK_PROC_HANDLER(MLS_ENTER_ROLE, EnterRoleRes);
	ADD_LINK_PROC_HANDLER(CHAT_JOIN_CHANNEL, JoinChannelRes);
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
			std::map<UINT16, fnLgProcessHandler>::iterator it = m_lgProcHandler.find(pResPayload->cmdCode);
			if (it != m_lgProcHandler.end())
				retCode = (TcpLogic::GetInstance()->*(it->second))(pResPayload, pResPayload->rawDataBytes);

			if (pSGSHeader->pktType == ESGSPacketType::DATA_TOKEN_KEY_PACKET)
			{
				SConnectLinkerRes* pConnectLinkerRes = (SConnectLinkerRes*)(pResPayload->data);
				pConnectLinkerRes->ntoh();
				memcpy(m_key, pConnectLinkerRes->key, DEFAULT_KEY_SIZE);
				TcpLogic::GetInstance()->ConnectLinkRes();
			}
		}
	}
}

bool LinkServer::LoginLinkerReq(SLoginLinkerReq* pLoginLinkerReq)
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

	SLoginLinkerReq* LoginLinkerReq = (SLoginLinkerReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	LoginLinkerReq->userId = pLoginLinkerReq->userId;
	strcpy(LoginLinkerReq->token, pLoginLinkerReq->token);
	LoginLinkerReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LinkServer::QueryRolesReq(SQueryRolesReq* req)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SQueryRolesReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = MLS_QUERY_ROLES;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SQueryRolesReq* pCreateRoleReq = (SQueryRolesReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	pCreateRoleReq->userId = req->userId;
	pCreateRoleReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LinkServer::CreateRoleReq(SCreateRoleReq* req)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SCreateRoleReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = MLS_CREATE_ROLE;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SCreateRoleReq* pCreateRoleReq = (SCreateRoleReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	pCreateRoleReq->occuId = req->occuId;
	pCreateRoleReq->sex = req->sex;
	strcpy(pCreateRoleReq->nick, req->nick);

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LinkServer::EnterRoleReq(SEnterRoleReq * pEnterRoleReq)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SEnterRoleReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = MLS_ENTER_ROLE;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SEnterRoleReq* EnterRoleReq = (SEnterRoleReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	EnterRoleReq->roleId = pEnterRoleReq->roleId;
	EnterRoleReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LinkServer::JoinChannelReq(SJoinChannelReq* req)
{
	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SJoinChannelReq);
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = CHAT_JOIN_CHANNEL;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SJoinChannelReq* pJoinChannelReq = (SJoinChannelReq*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	pJoinChannelReq->channel = req->channel;
	pJoinChannelReq->joinId = req->joinId;
	strcpy(pJoinChannelReq->nick, req->nick);
	pJoinChannelReq->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}
	return false;
}

bool LinkServer::ChatMessageSend(BYTE* pChat)
{
	SChatMessageSend* pChatMessageSend = (SChatMessageSend*)pChat;

	char* Chatmsg = (char*)pChat + sizeof(SChatMessageSend) - 1;

	BOOL isCrypt = 1;
	int nRawPayloadLen = SGS_REQ_HEAD_LEN + sizeof(SChatMessageSend) - 1 + pChatMessageSend->msgBytes;
	int nEncryptedPayloadLen = isCrypt ? ((nRawPayloadLen + 15) / 16) * 16 : nRawPayloadLen;

	BYTE p[_MAX_MSGSIZE] = { 0 };
	SGSProtocolHead* pSGSHeader = (SGSProtocolHead*)p;
	pSGSHeader->isCrypt = isCrypt ? 1 : 0;
	pSGSHeader->pktType = DATA_PACKET;
	pSGSHeader->pduLen = nEncryptedPayloadLen;
	pSGSHeader->hton();

	SGSReqPayload* pReqPayload = (SGSReqPayload*)(p + SGS_PROTO_HEAD_LEN);
	pReqPayload->rawDataBytes = nRawPayloadLen;
	pReqPayload->cmdCode = CHAT_SEND_MESSAGE;
	pReqPayload->token = GetTickCount();
	pReqPayload->hton();

	SChatMessageSend* ChatMessage = (SChatMessageSend*)((char*)pReqPayload + SGS_REQ_HEAD_LEN);
	ChatMessage->channel = pChatMessageSend->channel;
	ChatMessage->targetId = pChatMessageSend->targetId;
	ChatMessage->msgBytes = pChatMessageSend->msgBytes;
	char* msg = (char*)ChatMessage + sizeof(SChatMessageSend) - 1;
	strcpy(msg, Chatmsg);
	ChatMessage->hton();

	if (isCrypt)
		appEncryptDataWithKey((AES_BYTE*)(p + SGS_PROTO_HEAD_LEN), nRawPayloadLen, m_key);

	if (m_sock->SendMsg(p, SGS_PROTO_HEAD_LEN + nEncryptedPayloadLen))
	{
		return m_sock->Flush();
	}

	return false;
}

