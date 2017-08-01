#include "csv.h"
#include "tools/StringTools.h"
#include <fstream>
#include <iostream>
#include "cocos2d.h"

void CSV::init(const string fileName, const int skip)
{
	string csvStr = cocos2d::FileUtils::getInstance()->getStringFromFile(fileName);
	list<string> lst;
	StringTools::split(csvStr, "\r", lst);
	//´íÎóÅĞ¶Ï
	if ((int)lst.size() <= skip)	return;
	int num = skip;
	//´¦ÀíÌø¹ı
	list<string>::iterator iter = lst.begin();
	while (num > 0)
	{
		iter++;
		num--;
	}
	while (iter != lst.end())
	{
		if ((*iter) != "")
		{
			parserData((*iter));
		}
		iter++;
	}
}

void CSV::readIntList(string& str, string delim, list<int>& lst)
{
	if (str == "null")		return;
	size_t last = 0;
	size_t index = str.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		lst.push_back(atoi((str.substr(last, index - last).c_str())));
		last = index + 1;
		index = str.find_first_of(delim, last);
	}
	if (index - last>0)		lst.push_back(atoi((str.substr(last, index - last).c_str())));
}

void CSV::readStringList(string& str, string delim, list<string>& lst)
{
	if (str == "null")		return;
	size_t last = 0;
	size_t index = str.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		lst.push_back((str.substr(last, index - last).c_str()));
		last = index + 1;
		index = str.find_first_of(delim, last);
	}
	if (index - last>0)		lst.push_back((str.substr(last, index - last).c_str()));
}

