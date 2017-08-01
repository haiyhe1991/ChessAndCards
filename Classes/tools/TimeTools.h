#ifndef __TIME_TOOLS_H__
#define __TIME_TOOLS_H__

#include <string>

//时间管理

struct TimeStruct
{
	int				year;
	int				month;
	int				day;
	int				hour;
	int				minute;
	int				secend;
	int				cur_time;			//1970年到现在的秒数
	long long		cur_mtime;			//1970年到现在的毫秒数
};

class TimeTools
{
public:
	static TimeTools*		getInstance();								//取得时间管理的唯一实例
	static void				destoryInstance();							//销毁当前实例
	void					setSystemTime(int time);

	TimeStruct				getCurTime();
	int						getSecendFormat(std::string timeFormat);			//根据时间字符串，取得1970到字符串描述时间的秒数  format：2013.11.17
private:
	TimeTools();
	static TimeTools *_instance;

	//服务器更新的时间差值    1970-01-01  毫秒数
	int _time;
};

#endif
