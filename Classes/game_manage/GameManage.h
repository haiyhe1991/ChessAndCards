#ifndef __GAME_MANAGE_H__
#define __GAME_MANAGE_H__
#include "chessAndCards.h"

class GameManage
{
public:
	
	~GameManage();
	bool init();
	static GameManage* GetInstance();
	static void RecoverInstance();

	
	//功能函数 重启当前节点 以及所有子节点及其所有下属节点
	void RevursivelyResumeAllChildren(Node *node);
	// 功能函数 暂停当前节点 以及所有子节点及其所有下属节点
	void RevursivelyPauseAllChildren(Node * node);


	void SetSavePassword(bool bSavePassword);					//设置是否保存密码
	bool GetSavePassword();										//获取是否保存密码

	void SetCurUserName(string userName);						//设置用户名
	string GetCurUserName();									//获取用户名

	void SetCurPassword(string password);						//设置密码
	string GetCurPassword();									//获取密码

	void SetCurOccuId(int curOccuId);							//设置角色id
	int GetCurOccuId();											//获取角色id

	void SetCurCoinValue(int iCoinNum);
	int GetCurCoinValue();
	void AlterCoinValue(int iChangeCoinNum,bool bRecord);
	void SetCurGemsValue(int iGemsNum);
	int GetCurGemsValue();
	void AlterGemsValue(int iChangeGemsNum,bool bRecord);

private:
	GameManage();
	bool m_bSavePassword;
	string m_userName;
	string m_password;
	int m_curOccuId;

	std::string m_nCurCoinValue;
	std::string m_nCurGemsValue;

	static GameManage* m_pSingleInstance;
	
	
};
#endif
