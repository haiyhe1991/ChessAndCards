#ifndef __GAME_RECORD_H__
#define __GAME_RECORD_H__

#include "chessAndCards.h"

#define DB_NAME  "ChessAndCards"


enum MOOD_FILE_WRITE_OR_READ
{
	fileReadbasical,
	fileWritebasical
};

class GameRecord
{
public:
	
	~GameRecord();
	static GameRecord* getInstance();
	static void recoverInstance();
	
	/* 功能：读取并判断文件是否存在，
	参数：文件名
	返回值：文件存在关闭文件并且返回真，文件不存在返回假。
	**/
	bool JudgeFileExist(const char* name);
	/*****函数功能：如果二进制文件不存在，则创建一个二进制文件，，如存在就直接返回改文件指针
	参数：文件名，读写模式，
	返回值：对应文件名的文件指针
	*****/
	FILE* FileWriteOrRead(const char* name, MOOD_FILE_WRITE_OR_READ op);
	//加载数据
	void LoadData();
	//保存数据
	void SaveData();
	//加载string类型数据
	std::string LoadDataOfString(FILE *pfile);
	//保存string类型的数据
	void SaveDataOfString(FILE* pfile, const std::string& str);
	//加载int类型的数据
	int LoadDataOfInt(FILE *pfile);
	//保存int类型的数据
	void SaveDataOfInt(FILE* pfile, const int& num);
	//加载float类型的数据
	float LoadDataOfFloat(FILE *pfile);
	//保存float类型的数据
	void SaveDataOfFloat(FILE* pfile, const float& num);
	//加载bool类型的数据
	bool LoadDataOfBool(FILE *pfile);
	//保存bool类型的数据
	void SaveDataOfBool(FILE* pfile, const bool& num);
	//把resouce下的文件拷贝到能读写的数据区
	const char* GetDataPath(const char* path);
	
private:
	GameRecord();
	static GameRecord *m_pSingleInstance;
};
#endif