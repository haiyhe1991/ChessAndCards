#include "GameRecord.h"
#include "game_manage/GameManage.h"
GameRecord *GameRecord::m_pSingleInstance = NULL;
GameRecord* GameRecord::getInstance()
{
	if (m_pSingleInstance == NULL)
	{
		m_pSingleInstance = new GameRecord();
	}
	return m_pSingleInstance;
}
void GameRecord::recoverInstance()
{
	CC_SAFE_DELETE(m_pSingleInstance);
}
GameRecord::GameRecord()
{

}
GameRecord::~GameRecord()
{

}

void GameRecord::LoadData()
{
	FILE* file = GameRecord::FileWriteOrRead(DB_NAME, fileReadbasical);
	if (file == NULL)
	{
		CCLOG("file isNULL");
		return;
	}
	else
	{
		CCLOG("file isOK");
	}

	GameManage::GetInstance()->SetSavePassword(GameRecord::LoadDataOfBool(file));
	GameManage::GetInstance()->SetCurUserName(GameRecord::LoadDataOfString(file));
	GameManage::GetInstance()->SetCurPassword(GameRecord::LoadDataOfString(file));
	GameManage::GetInstance()->SetCurOccuId(GameRecord::LoadDataOfInt(file));
	//  
	//GameManage::GetInstance()->SetPlayerProtraitNumber(IO_Operate::LoadDataOfInt(file));

	//for(int i=0;i<5;i++)
	//{
	//  GameManage::GetInstance()->m_iaHighScoreNumber[i]=IO_Operate::LoadDataOfInt(file);
	//  GameManage::GetInstance()->m_sHighScoreName[i]=IO_Operate::LoadDataOfString(file);
	//}
	//GameManage::GetInstance()->SetIsTeach(IO_Operate::LoadDataOfBool(file));
	fclose(file);
}

void GameRecord::SaveData()
{
	FILE* file = FileWriteOrRead(DB_NAME, fileWritebasical);
	if (file == NULL)
	{
		CCLOG("file isNULL");
		return;
	}
	else
	{
		CCLOG("file isOK");
	}

	GameRecord::SaveDataOfBool(file, GameManage::GetInstance()->GetSavePassword());
	GameRecord::SaveDataOfString(file, GameManage::GetInstance()->GetCurUserName());
	GameRecord::SaveDataOfString(file, GameManage::GetInstance()->GetCurPassword());
	GameRecord::SaveDataOfInt(file, GameManage::GetInstance()->GetCurOccuId());
	//IO_Operate::SaveDataOfInt(file,GameManage::GetInstance()->GetPlayerProtraitNumber());

	//for(int i=0;i<5;i++)
	//{
	//  IO_Operate::SaveDataOfInt(file,GameManage::GetInstance()->m_iaHighScoreNumber[i]);
	//  IO_Operate::SaveDataOfString(file,GameManage::GetInstance()->m_sHighScoreName[i]);
	//}

	//IO_Operate::SaveDataOfBool(file,GameManage::GetInstance()->GetIsTeach());

	fflush(file);
	fclose(file);
}
bool GameRecord::JudgeFileExist(const char* name)
{
	__String* buf = __String::createWithFormat("%s_db", name);//合成文件名
	std::string full_path = FileUtils::getInstance()->getWritablePath() + buf->getCString();//获取路径+文件名，即全路径
	// CCLog(full_path.c_str());
	FILE* pfile = fopen(full_path.c_str(), "r");//文件存在返回文件名，文件不存在返回null
	if (pfile)
	{
		fclose(pfile);
		return true;
	}
	else
	{
		return false;
	}
}
FILE* GameRecord::FileWriteOrRead(const char* name, MOOD_FILE_WRITE_OR_READ op)
{
	__String* buf = __String::createWithFormat("%s_db", name);
	std::string full_path = (FileUtils::getInstance())->getWritablePath() + buf->getCString();
	//NormalStrToUTF8::WStrToUTF8(full_path.c_str()).c_str();
	CCLOG("%s", full_path.c_str());
	FILE* pfile = fopen(full_path.c_str(), op == fileReadbasical ? "rb" : "wb");//如果没有该文件就会新建一个文件
	return pfile;
}
std::string GameRecord::LoadDataOfString(FILE *pfile)
{
	CCAssert(pfile, "ProjReader::readString(FILE *pfile)-pfile is null!");//pfile为空时，出现断言那个框，也就是像野指针一样出现对话框
	int len_num = 0;
	fread(&len_num, sizeof(int), 1, pfile);//从pfile文件流中读指定个数的字符，读取1块4个字节,放到&len_num里面
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	char buffer[1024];
#else
	char buffer[len_num];
#endif

	int len = fread(buffer, sizeof(char), len_num, pfile);//将文件流中的字符串放到buffer空间
	buffer[len] = '\0';//字符串结束标志
	std::string re(buffer);//重写
	CCLOG("%s", re.c_str());
	return re;
}

void GameRecord::SaveDataOfString(FILE* pfile, const std::string& str)
{
	CCAssert(pfile, "pfile is null!");
	int len_num = (int)str.length();
	fwrite(&len_num, sizeof(int), 1, pfile);
	fwrite(str.c_str(), sizeof(char), len_num, pfile);
}

int GameRecord::LoadDataOfInt(FILE *pfile)
{
	CCAssert(pfile, "ProjReader::LoadDataOfInt(FILE *pfile)-pfile is null!");
	int num = 0;
	fread(&num, sizeof(int), 1, pfile);
	return num;
}

void GameRecord::SaveDataOfInt(FILE* pfile, const int& num)
{
	CCAssert(pfile, "pfile is null!");
	if (fwrite(&num, sizeof(int), 1, pfile) == 0)
	{
		CCAssert(false, "write error!");
	}
}

float GameRecord::LoadDataOfFloat(FILE *pfile)
{
	CCAssert(pfile, "ProjReader::LoadDataOfFloat(FILE *pfile)-pfile is null!");
	float num = 0;
	fread(&num, sizeof(float), 1, pfile);
	return num;
}

void GameRecord::SaveDataOfFloat(FILE* pfile, const float& num)
{
	CCAssert(pfile, "pfile is null!");
	if (fwrite(&num, sizeof(float), 1, pfile) == 0)
	{
		CCAssert(false, "write error!");
	}
}

bool GameRecord::LoadDataOfBool(FILE *pfile)
{
	CCAssert(pfile, "ProjReader::LoadDataOfBool(FILE *pfile)-pfile is null!");
	bool num = false;
	fread(&num, sizeof(bool), 1, pfile);
	return num;
}

void GameRecord::SaveDataOfBool(FILE* pfile, const bool& num)
{
	CCAssert(pfile, "pfile is null!");
	if (fwrite(&num, sizeof(bool), 1, pfile) == 0)
	{
		CCAssert(false, "write error!");
	}
}
const char* GameRecord::GetDataPath(const char* path)
{
	const std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path);
	//CCLOG("%s",fullpath.c_str());
	unsigned char* pData = NULL;
	ssize_t len = 0;
	pData = FileUtils::getInstance()->getDataFromFile(fullpath).getBytes();
	std::string destpath = FileUtils::getInstance()->getWritablePath();
	std::string buf = path;
	{
		for (int i = 0; i < int(buf.size()); i++)
		{
			if (buf[i] == '/')
				buf[i] = '\\';//'_'文件夹分割符也可以,只是这个文件夹名字不能用'_'而已
		}
	}
	destpath += buf;
	CCLOG("copy data from [%s] to [%s].", fullpath.c_str()
		, destpath.c_str());
	FILE* fp = fopen(destpath.c_str(), "w");
	fwrite(pData, sizeof(char), len, fp);
	//CCLOG("tttttttttttttt%s",pData);
	fclose(fp);
	free(pData);
	delete pData;
	pData = NULL;
	return destpath.c_str();
}