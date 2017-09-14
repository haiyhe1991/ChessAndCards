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

	
	//���ܺ��� ������ǰ�ڵ� �Լ������ӽڵ㼰�����������ڵ�
	void RevursivelyResumeAllChildren(Node *node);
	// ���ܺ��� ��ͣ��ǰ�ڵ� �Լ������ӽڵ㼰�����������ڵ�
	void RevursivelyPauseAllChildren(Node * node);


	void SetSavePassword(bool bSavePassword);					//�����Ƿ񱣴�����
	bool GetSavePassword();										//��ȡ�Ƿ񱣴�����

	void SetCurUserName(string userName);						//�����û���
	string GetCurUserName();									//��ȡ�û���

	void SetCurPassword(string password);						//��������
	string GetCurPassword();									//��ȡ����

	void SetCurOccuId(int curOccuId);							//���ý�ɫid
	int GetCurOccuId();											//��ȡ��ɫid

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
