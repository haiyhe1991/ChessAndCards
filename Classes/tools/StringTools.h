#ifndef __STRING_TOOLS_H__
#define __STRING_TOOLS_H__

//字符串工具类
#include <string>
#include <list>

using namespace std;

#define SPACE_KEY_CODE		32				//空格的符号
#define LIST_DELIMITER_L	"|"				//list包裹list的默认分隔符

class StringTools
{
public:
	void static			split(string& s, string delim, list<string>& lst);	//拆分字符串
	void static			replace_all(string& str, const string& old_value, const string& new_value);		//全部替换
	string static		interceptString(string str, int length);		//根据长度，拆分字符串
	list<string> static warpString(string txt, int fontSize, int width);										//自动断行字符串
	string static		intConvertString(int value);		//int转string
	int static			stringConvertInt(string value);		//string转int
	float static		stringConvertFloat(string value);
	int static			getStringWidthForShow(string txt, int fontSize);										//计算出txt字符串在显示过程中的长度    聊天使用

																												//以下是csv所需的东西
	void static			readIntList(string& str, string delim, list<int>& lst);
	void static			readStringList(string& str, string delim, list<string>& lst);
	void static			readLstList(string& str, string delim, list<list<int>>& lst);
};


#endif
