#include "TimeTools.h"
#include "cocos2d.h"
#include <time.h>
#include <sys/timeb.h>
#include "StringTools.h"

//单例模式
TimeTools* TimeTools::_instance = nullptr;

TimeTools::TimeTools()
{
	_time = 0;
}

TimeTools* TimeTools::getInstance()
{
	if (_instance == nullptr)
		_instance = new TimeTools();
	return _instance;
}

void TimeTools::destoryInstance()
{
	if (_instance != nullptr)
		delete _instance;
	_instance = nullptr;
}

void TimeTools::setSystemTime(int time)
{
	struct timeb tmb;
	ftime(&tmb);
	_time = time - (int)tmb.time;
}

TimeStruct TimeTools::getCurTime()
{
	TimeStruct result;
	struct tm* tTime;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//win32 取系统时间的方式
	time_t timep;
	time(&timep);
	timep += (time_t)_time;
	tTime = localtime(&timep);
#else
	//android and ios 测试是没有问题的，win10暂时不知
	struct timeval now;
	gettimeofday(&now, NULL);
	now.tv_sec += _time;
	tTime = localtime(&now.tv_sec);
#endif
	result.year = (int)tTime->tm_year + 1900;
	result.month = (int)tTime->tm_mon + 1;
	result.day = (int)tTime->tm_mday;
	result.hour = (int)tTime->tm_hour;
	result.minute = (int)tTime->tm_min;
	result.secend = (int)tTime->tm_sec;

	struct timeb tmb;
	ftime(&tmb);
	result.cur_time = (int)tmb.time + _time;
	result.cur_mtime = (long long)result.cur_time * 1000;// +(long long)(tmb.millitm);
	result.cur_mtime += (long long)tmb.millitm;

	return result;
}

int TimeTools::getSecendFormat(std::string timeFormat)
{
	list<string> lst;
	StringTools::split(timeFormat, ".", lst);
	struct tm tTime;
	list<string>::iterator iter = lst.begin();
	tTime.tm_year = StringTools::stringConvertInt((*iter)) - 1900;
	iter++;
	tTime.tm_mon = StringTools::stringConvertInt((*iter)) - 1;
	iter++;
	tTime.tm_mday = StringTools::stringConvertInt((*iter));
	tTime.tm_hour = 0;
	tTime.tm_min = 0;
	tTime.tm_sec = 0;
	return (int)mktime(&tTime);
}
