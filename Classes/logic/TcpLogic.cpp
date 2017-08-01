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
	registerMessage();
	state = RecvState::RECVING;
	SetDelayTime(TCP_DELAY_TIME);
	TickManager::GetInstance()->Reg(this);
}

TcpLogic::~TcpLogic()
{
	TickManager::GetInstance()->Unreg(this);
	delete wait;
}

void TcpLogic::registerMessage()
{
	MsgManager* manager = MsgManager::GetInstance();
	manager->Reg(MSG_REGIST_REQ, this);
	manager->Reg(MSG_LOGIN_REQ, this);
	manager->Reg(MSG_QUERY_PARTITION_REQ, this);
	manager->Reg(MSG_ENTER_PARTITION_REQ, this);
}

bool TcpLogic::CreatLicenseSocket(const char* ip, int port)
{
	return LicenseServer::GetInstance()->CreatSocket(ip, port);
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
			return LicenseServer::GetInstance()->EnterPartitionReq(pUserEnterPartitionReq);
		}
		lstIter++;
	}
	return false;
}

void TcpLogic::OnTick(int time)
{
	if (wait->OnTime(time))
	{
		switch (state)
		{
		case RECVING:
			break;
		case RECVED:
			state = RecvState::PAUSEING;
			wait->StartWait(10000);
			break;
		case PAUSEING:
			state = RecvState::RECVING;
			MsgManager::GetInstance()->Dispather(MessageHead::MSG_REQ_OUT_TIME, nullptr);
			break;
		default:
			break;
		}
	}

}

void TcpLogic::OnMessage(const int head, void* data)
{
	if (head == MSG_REGIST_REQ || head == MSG_LOGIN_REQ || head == MSG_QUERY_PARTITION_REQ || head == MSG_ENTER_PARTITION_REQ)
	{
		state = RecvState::RECVED;
		wait->StartWait(0);
	}
}

int TcpLogic::RegistUserRes(SGSResPayload* pBuf, int pBufLen)
{
	state = RecvState::RECVING;
	wait->StartWait(0);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_REGIST_RES, &(pBuf->retCode)); 
	return LCS_OK;
}
int TcpLogic::LoginUserRes(SGSResPayload* pBuf, int pBufLen)
{
	state = RecvState::RECVING;
	wait->StartWait(0);
	if (pBuf->retCode == LCS_OK)
	{
		m_pUserLoginRes = (SUserLoginRes*)pBuf->data;
		m_pUserLoginRes->ntoh();
	}
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_LOGIN_RES, &(pBuf->retCode));
	return LCS_OK;
}
int TcpLogic::QueryPartitionInfoRes(SGSResPayload* pBuf, int pBufLen)
{
	state = RecvState::RECVING;
	wait->StartWait(0);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_QUERY_PARTITION_RES, &(pBuf->retCode));
	if (pBuf->retCode == LCS_OK)
	{
		m_pQueryPartitionRes = (SUserQueryPartitionRes*)pBuf->data;
		m_pQueryPartitionRes->ntoh();
		SPartitionInfo* pPartitionInfo = (SPartitionInfo*)m_pQueryPartitionRes->data;
		pPartitionInfo->ntoh();
		container.push_back(pPartitionInfo);
	}
	return LCS_OK;
}
int TcpLogic::EnterPartitionRes(SGSResPayload* pBuf, int pBufLen)
{
	state = RecvState::RECVING;
	wait->StartWait(0);
	MsgManager::GetInstance()->Dispather(MessageHead::MSG_ENTER_PARTITION_RES, &(pBuf->retCode));
	if (pBuf->retCode == LCS_OK)
	{
		m_pEnterPartitionRes = (SUserEnterPartitionRes*)pBuf->data;
		m_pEnterPartitionRes->ntoh();
	}
	return LCS_OK;
}

bool TcpLogic::CreatLinkerSocket()
{
	LicenseServer::GetInstance()->CloeseLicense();//断开验证服务器
	return LinkServer::GetInstance()->CreatSocket(m_pEnterPartitionRes->linkerIp, m_pEnterPartitionRes->linkerPort);
}