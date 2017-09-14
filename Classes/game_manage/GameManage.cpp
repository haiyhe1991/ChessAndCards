#include "GameManage.h"
#include "data_encrypt/base64.h"
#include "game_record/GameRecord.h"
GameManage *GameManage::m_pSingleInstance=NULL;
GameManage* GameManage::GetInstance()
{
	if(m_pSingleInstance==NULL)
	{
		m_pSingleInstance=new GameManage();
	}
	return m_pSingleInstance;
}

GameManage::GameManage()
{
	m_bSavePassword = true;
	m_userName = "";
	m_password = "";
	m_curOccuId = 1;


	 this->SetCurCoinValue(0);
	 this->SetCurGemsValue(0);
}

GameManage::~GameManage()
{

}

void GameManage::RecoverInstance()
{
	CC_SAFE_DELETE(m_pSingleInstance);
}

void GameManage::RevursivelyPauseAllChildren(Node * node)
{
	node->pause();
	for(Vector<cocos2d::Node*>::const_iterator it = (node -> getChildren()).begin(); it != (node -> getChildren()).end(); ++it)
	{
		RevursivelyPauseAllChildren(*it); 
	}
}

void GameManage::RevursivelyResumeAllChildren(Node *node)
{
	node->resume();
	for(Vector<cocos2d::Node*>::const_iterator it = (node -> getChildren()).begin(); it != (node -> getChildren()).end(); ++it)
	{
		RevursivelyResumeAllChildren(*it); 
	}
}

void GameManage::SetSavePassword(bool bSavePassword)
{
	m_bSavePassword = bSavePassword;
}

bool GameManage::GetSavePassword()
{
	return m_bSavePassword;
}

void GameManage::SetCurUserName(string userName)
{
	m_userName = userName;
}

string GameManage::GetCurUserName()
{
	return m_userName;
}

void GameManage::SetCurPassword(string password)
{
	m_password = password;
}

string GameManage::GetCurPassword()
{
	return m_password;
}

void GameManage::SetCurOccuId(int curOccuId)
{
	m_curOccuId = curOccuId;
}

int GameManage::GetCurOccuId()
{
	return m_curOccuId;
}

void GameManage::SetCurCoinValue(int iCoinNum)
{
	char str[32];
	sprintf(str,"%d",iCoinNum);
	m_nCurCoinValue = base64_encode(reinterpret_cast<const unsigned char*>(str),iCoinNum/10+1);
}

int GameManage::GetCurCoinValue()
{
	return atoi((base64_decode(m_nCurCoinValue)).c_str());
}

void GameManage::AlterCoinValue(int iChangeCoinNum,bool bRecord)
{
	int iCurCoinValue = this->GetCurCoinValue();
	if(iCurCoinValue+iChangeCoinNum>INT_MAX)
	{
		iCurCoinValue = INT_MAX;
	}
	else if(iCurCoinValue+iChangeCoinNum<0)
	{
		iCurCoinValue = 0;
	}
	else
	{
		iCurCoinValue += iChangeCoinNum; 
	}
	this->SetCurCoinValue(iCurCoinValue);
	if(bRecord)
	{
//		GameRecord::SaveData();
	}
}

void GameManage::SetCurGemsValue(int iGemsNum)
{
	char str[32];
	sprintf(str,"%d",iGemsNum);
	m_nCurGemsValue = base64_encode(reinterpret_cast<const unsigned char*>(str),iGemsNum/10+1);
}

int GameManage::GetCurGemsValue()
{
	return atoi((base64_decode(m_nCurGemsValue)).c_str());
}

void GameManage::AlterGemsValue(int iChangeGemsNum,bool bRecord)
{
	int iCurGemsValue = this->GetCurGemsValue();
	if(iCurGemsValue+iChangeGemsNum>INT_MAX)
	{
		iCurGemsValue = INT_MAX;
	}
	else if(iCurGemsValue+iChangeGemsNum<0)
	{
		iCurGemsValue = 0;
	}
	else
	{
		iCurGemsValue += iChangeGemsNum; 
	}
	this->SetCurCoinValue(iCurGemsValue);
	if(bRecord)
	{
//		GameRecord::SaveData();
	}
}