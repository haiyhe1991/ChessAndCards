
#include "DealWithData.h"
#include "cocos2d.h"

//#include <iostream>
USING_NS_CC;
//using namespace std;
DealWithData::DealWithData()
{

}

DealWithData::~DealWithData()
{
	
}

void DealWithData::decodeData()
{
	//this->copyData(DATA_DECODE_ZIP_NAME);
}

//cc_timeval start;
//cc_timeval end;

void DealWithData::copyData(const char* pFileName)
{
	//long now=0;
	//now=clock();
       //time_t start=time(0);
	   //tm *pStartTime=NULL;
	   //pStartTime=localtime(&start);
	  // CCLOG("%d",pStartTime->tm_min*60+pStartTime->tm_sec);
	//CCTime::gettimeofdayCocos2d(&start, NULL);
	////CCLog("startTime----%f", start);
	//CCLog("=====start====");

//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	ssize_t len = 0;
	unsigned char* data = NULL;
	data = FileUtils::getInstance()->getFileData(strPath.c_str(), "rb", &len);
	std::string destPath = FileUtils::getInstance()->getWritablePath();
	 std::string buf = pFileName;
        {
            for (int i = 0; i < buf.size(); i++)
            {
                if (buf[i] == '/')
                    buf[i] = '\\';//'_'文件夹分割符也可以,只是这个文件夹名字不能用'_'而已
            }
        }
	destPath += pFileName;
	CCLOG("copy data from [%s] to [%s].",strPath.c_str(),destPath.c_str());
	FILE* fp = fopen(destPath.c_str(), "wb+");
	fwrite( data, sizeof(char), len, fp );
	fclose(fp);
	delete []data;
	data = NULL;
//#endif
//#if((CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
	

	/*std::string strPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName);
	std::string destPath = CCFileUtils::sharedFileUtils()->getWriteablePath();
	destPath += pFileName;
	std::ifstream file1( strPath.c_str(), std::ios::in | std::ios::binary); 
	std::ofstream file2( destPath.c_str(), std::ios::out | std::ios::binary); 
	char input; 
	while(file1.read(&input, 1)) 
	{ 
		file2.write(&input, 1); 
	} 
	file1.close(); 
	file2.close();*/

	
//#endif
	//time_t end=time(0);
	  // tm *pEndTime=NULL;
	   //pEndTime=localtime(&end);
	   //CCLOG("%lf",(clock()-now));
	/*CCTime::gettimeofdayCocos2d(&end, NULL);
	CCLog("end----%f", CCTime::timersubCocos2d(&start, &end));*/
}

void DealWithData::deleteData()
{
	std::string destPath = FileUtils::getInstance()->getWritablePath();
	//destPath += DATA_DECODE_ZIP_NAME;
	remove(destPath.c_str());
}
