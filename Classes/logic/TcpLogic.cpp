#include "TcpLogic.h"
#include "tcp/LicenseServer.h"
#include "tcp/LinkServer.h"

TcpLogic *TcpLogic::_instance = NULL;
TcpLogic* TcpLogic::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new TcpLogic();
	}
	return _instance;
}
void TcpLogic::DestoryInstance()
{
	if (_instance != NULL)
	{
		delete _instance;
	}
}

TcpLogic::TcpLogic()
{
	wait = new WaitTime;
	m_pUserLoginRes = new SUserLoginRes();
	m_pQueryPartitionRes = new SUserQueryPartitionRes();
	m_pEnterPartitionRes = new SUserEnterPartitionRes();
	m_pCreateRoleRes = new SCreateRoleRes();
	m_pSEnterRoleRes = new SEnterRoleRes();
	state = TcpState::IDEL;
	SetDelayTime(TCP_DELAY_TIME);
	TickManager::GetInstance()->Reg(this);
}

TcpLogic::~TcpLogic()
{
	TickManager::GetInstance()->Unreg(this);
	delete wait;
	delete m_pUserLoginRes;
	delete m_pQueryPartitionRes;
	delete m_pEnterPartitionRes;
	delete m_pCreateRoleRes;
	delete m_pSEnterRoleRes;
}

void TcpLogic::ChangeTcpState(TcpState tcpState)
{
	state = tcpState;
	wait->StartWait(0);
}

bool TcpLogic::CreatLicenseSocket()
{
	this->ChangeTcpState(TcpState::REQ);
	return LicenseServer::GetInstance()->CreatSocket(LICENSE_IP, LICENSE_PORT);
}

void TcpLogic::ConnectLincenseRes()
{
	this->ChangeTcpState(TcpState::IDEL);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_CONNECT_LICENSE_RES, nullptr);
}

bool TcpLogic::RegistUserReq(const char*	pUsername, const char*	pPassword)
{
	SUserRegistReq* pUserRegistReq = new SUserRegistReq();
	strcpy(pUserRegistReq->devid, "12345678");
	strcpy(pUserRegistReq->loginPlat, getloginPlat());
	strcpy(pUserRegistReq->model, "tinkvision");
	strcpy(pUserRegistReq->os, "windows");
	strcpy(pUserRegistReq->username, pUsername);
	strcpy(pUserRegistReq->password, pPassword);
	strcpy(pUserRegistReq->ver, "v1.0.0");
	this->ChangeTcpState(TcpState::REQ);
	return LicenseServer::GetInstance()->RegistUserReq(pUserRegistReq);
}

bool TcpLogic::LoginUserReq(const char*	pUsername, const char*	pPassword)
{
	SUserPswLoginReq* pUserPswLoginReq = new SUserPswLoginReq();
	strcpy(pUserPswLoginReq->devid, "12345678");
	strcpy(pUserPswLoginReq->loginPlat, getloginPlat());
	strcpy(pUserPswLoginReq->model, "tinkvision");
	strcpy(pUserPswLoginReq->os, "windows");
	strcpy(pUserPswLoginReq->username, pUsername);
	strcpy(pUserPswLoginReq->password, pPassword);
	strcpy(pUserPswLoginReq->ver, "v1.0.0");
	pUserPswLoginReq->mode = 1;
	this->ChangeTcpState(TcpState::REQ);
	return LicenseServer::GetInstance()->LoginUserReq(pUserPswLoginReq);
}

const char* TcpLogic::getloginPlat()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		return "ios";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID )
	{
		return "android";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_MARMALADE)
	{
		return "marmalade";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_LINUX )
	{
		return "linux";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_BADA)
	{
		return "bada";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_BLACKBERRY)
	{
		return "blackberry";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_MAC)
	{
		return "mac";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_UNKNOWN)
	{
		return "unknown";
	}
#elif(CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	{
		return "win32";
	}
	return "unknown";
#endif
}

bool TcpLogic::QueryPartitionInfoReq(UINT16 from, UINT16 num)
{
	SUserQueryPartitionReq*	pUserQueryPartitionReq = new SUserQueryPartitionReq();
	pUserQueryPartitionReq->from = from;
	pUserQueryPartitionReq->num = num;
	this->ChangeTcpState(TcpState::REQ);
	return LicenseServer::GetInstance()->QueryPartitionInfoReq(pUserQueryPartitionReq);
}

bool TcpLogic::EnterPartitionReq(UINT16	partitionId)
{
	SUserEnterPartitionReq* pUserEnterPartitionReq = new SUserEnterPartitionReq();
	list<SPartitionInfo*>::iterator lstIter = container.begin();
	while (lstIter != container.end())
	{
		if (partitionId == (*lstIter)->partitionId)
		{
			pUserEnterPartitionReq->partitionId = partitionId;
			pUserEnterPartitionReq->userId = m_pUserLoginRes->userId;
			this->ChangeTcpState(TcpState::REQ);
			return LicenseServer::GetInstance()->EnterPartitionReq(pUserEnterPartitionReq);
		}
		lstIter++;
	}
	this->ChangeTcpState(TcpState::REQ);
	return false;
}

void TcpLogic::OnTick(int time)
{
	if (wait->OnTime(time))
	{
		switch (state)
		{
		case TcpState::IDEL:
			break;
		case TcpState::REQ:
			state = TcpState::OUT_TIME;
			wait->StartWait(TCP_ANSWER_TIME_OUT);
			break;
		case TcpState::OUT_TIME:
			state = TcpState::IDEL;
			MsgManager::GetInstance()->Dispather(MessageHead::MSG_REQ_OUT_TIME, nullptr);
			break;
		default:
			break;
		}
	}

}

int TcpLogic::RegistUserRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_REGIST_RES, &(pBuf->retCode));
	return pBuf->retCode;
}
int TcpLogic::LoginUserRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	if (pBuf->retCode == LCS_OK)
	{
		SUserLoginRes* pUserLoginRes = (SUserLoginRes*)pBuf->data;
		pUserLoginRes->ntoh();
		m_pUserLoginRes->userId = pUserLoginRes->userId;
		strcpy(m_pUserLoginRes->username, pUserLoginRes->username);
	}
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_LOGIN_RES, &(pBuf->retCode));
	return pBuf->retCode;
}
int TcpLogic::QueryPartitionInfoRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	if (pBuf->retCode == LCS_OK)
	{
		SUserQueryPartitionRes* pQueryPartitionRes = (SUserQueryPartitionRes*)pBuf->data;
		pQueryPartitionRes->ntoh();
		m_pQueryPartitionRes->num = pQueryPartitionRes->num;
		m_pQueryPartitionRes->totals = pQueryPartitionRes->totals;

		for (int i = 0; i < pQueryPartitionRes->num; ++i)
		{
			container.push_back(pQueryPartitionRes->data + i);
		}
	}
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_QUERY_PARTITION_RES, &(pBuf->retCode));
	return pBuf->retCode;
}
int TcpLogic::EnterPartitionRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	if (pBuf->retCode == LCS_OK)
	{
		SUserEnterPartitionRes* pEnterPartitionRes = (SUserEnterPartitionRes*)pBuf->data;
		pEnterPartitionRes->ntoh();
		m_pEnterPartitionRes->linkerIp = pEnterPartitionRes->linkerIp;
		m_pEnterPartitionRes->linkerPort = pEnterPartitionRes->linkerPort;
		m_pEnterPartitionRes->partitionId = pEnterPartitionRes->partitionId;
		strcpy(m_pEnterPartitionRes->token, pEnterPartitionRes->token);
		m_pEnterPartitionRes->userId = pEnterPartitionRes->userId;

	}
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_ENTER_PARTITION_RES, &(pBuf->retCode));
	return pBuf->retCode;
}

bool TcpLogic::CreatLinkerSocket()
{
	LicenseServer::GetInstance()->CloseLicense();//断开验证服务器
	this->ChangeTcpState(TcpState::REQ);
	return LinkServer::GetInstance()->CreatSocket(m_pEnterPartitionRes->linkerIp, m_pEnterPartitionRes->linkerPort);
}

bool TcpLogic::LoginLinkReq()
{
	SLoginLinkerReq pLoginLinkerReq;
	pLoginLinkerReq.userId = m_pEnterPartitionRes->userId;
	strcpy(pLoginLinkerReq.token, m_pEnterPartitionRes->token);
	this->ChangeTcpState(TcpState::REQ);
	return LinkServer::GetInstance()->LoginLinkerReq(&pLoginLinkerReq);
}

void TcpLogic::ConnectLinkRes()
{
	this->ChangeTcpState(TcpState::IDEL);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_CONNECT_LINK_RES, nullptr);
}

int TcpLogic::LoginLinkerRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_LOGIN_LINK_RES, &(pBuf->retCode));
	return pBuf->retCode;
}

bool TcpLogic::QueryRolesReq()
{
	SQueryRolesReq pQueryRolesReq;
	pQueryRolesReq.userId = m_pUserLoginRes->userId;
	return LinkServer::GetInstance()->QueryRolesReq(&pQueryRolesReq);
}

int TcpLogic::QueryRolesRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	if (pBuf->retCode == LCS_OK)
	{
		SQueryRolesRes* pQueryRolesRes = (SQueryRolesRes*)pBuf->data;
		pQueryRolesRes->roles->ntoh();
		for (int i = 0; i < pQueryRolesRes->rolesCount; ++i)
		{
			containerRoleAttr.push_back(pQueryRolesRes->roles + i);
		}
	}
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_QUERY_ROLE_RES, &(pBuf->retCode));
	return pBuf->retCode;
}

bool TcpLogic::CreateRoleReq(BYTE occuId, BYTE sex, const char* nick)
{
	SCreateRoleReq req;
	req.occuId = occuId;
	req.sex = sex;
	strcpy(req.nick, nick);
	//list<SQueryRoleAttr*>::iterator lstIter = containerRoleAttr.begin();
	//while (lstIter != containerRoleAttr.end())
	//{
	//	if (roleId == (*lstIter)->id)
	//	{
	//		req.occuId = (*lstIter)->occuId;
	//		req.sex = (*lstIter)->sex;
	//		strcpy(req.nick, (*lstIter)->nick);
	//		this->ChangeTcpState(TcpState::REQ);
	//		return LinkServer::GetInstance()->CreateRoleReq(&req);
	//	}
	//	lstIter++;
	//}
	this->ChangeTcpState(TcpState::REQ);
	return LinkServer::GetInstance()->CreateRoleReq(&req);
}

int TcpLogic::CreateRoleRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	
	if (pBuf->retCode == LCS_OK)
	{
		SCreateRoleRes* pCreateRoleRes = (SCreateRoleRes*)pBuf->data;
		pCreateRoleRes->ntoh();

		m_pCreateRoleRes->id = pCreateRoleRes->id;
		m_pCreateRoleRes->occuId = pCreateRoleRes->occuId;
		m_pCreateRoleRes->sex = pCreateRoleRes->sex;
		m_pCreateRoleRes->dress = pCreateRoleRes->dress;
		m_pCreateRoleRes->mainWeapon = pCreateRoleRes->mainWeapon;
		m_pCreateRoleRes->subWeapon = pCreateRoleRes->subWeapon;
		strcpy(m_pCreateRoleRes->nick, pCreateRoleRes->nick);
		m_pCreateRoleRes->level = pCreateRoleRes->level;
		m_pCreateRoleRes->gold = pCreateRoleRes->gold;
		m_pCreateRoleRes->rpcoin = pCreateRoleRes->rpcoin;
		m_pCreateRoleRes->exp = pCreateRoleRes->exp;
		m_pCreateRoleRes->owncopy = pCreateRoleRes->owncopy;
	}

	MsgManager::GetInstance()->Dispather(MessageHead::MSG_CREATE_ROLE_RES, &(pBuf->retCode));
	return pBuf->retCode;
}

bool TcpLogic::EnterRoleReq(BYTE occuId)
{
	SEnterRoleReq pSEnterRoleReq;
	list<SQueryRoleAttr*>::iterator lstIter = containerRoleAttr.begin();
	while (lstIter != containerRoleAttr.end())
	{
		if (occuId == (*lstIter)->occuId)
		{
			pSEnterRoleReq.roleId = (*lstIter)->id;
			this->ChangeTcpState(TcpState::REQ);
			return LinkServer::GetInstance()->EnterRoleReq(&pSEnterRoleReq);
		}
		lstIter++;
	}

	if (m_pCreateRoleRes->occuId == occuId)
	{
		pSEnterRoleReq.roleId = m_pCreateRoleRes->id;
		this->ChangeTcpState(TcpState::REQ);
		return LinkServer::GetInstance()->EnterRoleReq(&pSEnterRoleReq);
	}

	return false;
	//return LinkServer::GetInstance()->EnterRoleReq(&pSEnterRoleReq);
}

int TcpLogic::EnterRoleRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	if (pBuf->retCode == LCS_OK)
	{
		SEnterRoleRes* pEnterRoleRes = (SEnterRoleRes*)pBuf->data;
		pEnterRoleRes->ntoh();

		m_pSEnterRoleRes->userId = pEnterRoleRes->userId;
		m_pSEnterRoleRes->roleId = pEnterRoleRes->roleId;
		strcpy(m_pSEnterRoleRes->nick, pEnterRoleRes->nick);
		m_pSEnterRoleRes->titleId = pEnterRoleRes->titleId;
		m_pSEnterRoleRes->roleLevel = pEnterRoleRes->roleLevel;
		m_pSEnterRoleRes->gold = pEnterRoleRes->gold;
		m_pSEnterRoleRes->exp = pEnterRoleRes->exp;
		m_pSEnterRoleRes->cash = pEnterRoleRes->cash;
		m_pSEnterRoleRes->occuId = pEnterRoleRes->occuId;
		m_pSEnterRoleRes->sex = pEnterRoleRes->sex;
		m_pSEnterRoleRes->cashcount = pEnterRoleRes->cashcount;
		m_pSEnterRoleRes->guildId = pEnterRoleRes->guildId;
		m_pSEnterRoleRes->hp = pEnterRoleRes->hp;
		m_pSEnterRoleRes->mp = pEnterRoleRes->mp;
		m_pSEnterRoleRes->vit = pEnterRoleRes->vit;
		m_pSEnterRoleRes->ap = pEnterRoleRes->ap;
		m_pSEnterRoleRes->attack = pEnterRoleRes->attack;
		m_pSEnterRoleRes->def = pEnterRoleRes->def;
		m_pSEnterRoleRes->packSize = pEnterRoleRes->packSize;
		m_pSEnterRoleRes->wareSize = pEnterRoleRes->wareSize;
		m_pSEnterRoleRes->unReadEmails = pEnterRoleRes->unReadEmails;
		m_pSEnterRoleRes->bt = pEnterRoleRes->bt;
		m_pSEnterRoleRes->pos = pEnterRoleRes->pos;
		m_pSEnterRoleRes->factionId = pEnterRoleRes->factionId;
		m_pSEnterRoleRes->skillNum = pEnterRoleRes->skillNum;
		for (int i = 0; i < pEnterRoleRes->skillNum; ++i)
		{
			containerSkill.push_back(pEnterRoleRes->data + i);
		}
	}

	MsgManager::GetInstance()->Dispather(MessageHead::MSG_ENTER_ROLE_RES, &(pBuf->retCode));
	return pBuf->retCode;
}

bool TcpLogic::JoinChannelReq()
{
	SJoinChannelReq req;
	req.channel = CHAT_WORLD_CHANNEL;
	req.joinId = 0;
	strcpy(req.nick, m_pSEnterRoleRes->nick);
	this->ChangeTcpState(TcpState::REQ);
	return LinkServer::GetInstance()->JoinChannelReq(&req);
}

int TcpLogic::JoinChannelRes(SGSResPayload* pBuf, int pBufLen)
{
	this->ChangeTcpState(TcpState::IDEL);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_JOIN_CHANNEL_RES, &(pBuf->retCode));
	return pBuf->retCode;
}

bool TcpLogic::ChatMessageSend(const char*	chatMsg)
{
	const int msgBytes = strlen(chatMsg);
	BYTE p[MAX_CHAT_MSG_BYTES] = { 0 };
	SChatMessageSend* pChatMessageSend = (SChatMessageSend*)p;
	pChatMessageSend->channel = CHAT_PRIVATE_CHANNEL;
	pChatMessageSend->msgBytes = strlen(chatMsg);
	pChatMessageSend->targetId = 1;

	char* msg = (char*)p + sizeof(SChatMessageSend)-1;
	strcpy(msg, chatMsg);
	return LinkServer::GetInstance()->ChatMessageSend(p);
}