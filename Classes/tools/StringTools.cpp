#include "StringTools.h"
#include <sstream>

void StringTools::split(string& s, string delim, list<string>& lst)
{
	replace_all(s, "\n", "");
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		lst.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last>0)		lst.push_back(s.substr(last, index - last));
}

void StringTools::replace_all(string& str, const string& old_value, const string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
}

string StringTools::interceptString(string str, int length)
{
	unsigned int beginPos = 0;			//字符串的初始位置
	string resultStr;					//返回的字符串
	list<string> strLst;				//创建一个字符串类型的顺序容器
	do
	{
		strLst.push_back(str.substr(beginPos, length));		//substr函数的作用类似剪刀，将str中从beginPos到length之间的字符串剪下来，单独放入容器中 
		if (beginPos + length >str.size())
		{
			break;  //当要裁剪的长度超出str的长度，则退出循环
		}
		else
		{
			beginPos += length;
		}

	} while (true);

	list<string>::iterator iter = strLst.begin();
	while (iter != strLst.end())
	{
		resultStr.append((*iter).append("\n"));			//从容器逐一取出之前裁剪好的一段段字符串，分别在字符串后面加上换行符。append()类似胶水，将\n粘到字符串后面 
		iter++;
	}
	return resultStr;
}

string StringTools::intConvertString(int value)
{
	stringstream stm(value);
	string result;
	stm >> result;
	return result;
}

int StringTools::stringConvertInt(string value)
{
	stringstream stm(value);
	int result = 0;
	stm >> result;
	return result;
}

float StringTools::stringConvertFloat(string value)
{
	stringstream stm(value);
	float result;
	stm >> result;
	return result;
}

int StringTools::getStringWidthForShow(string txt, int fontSize)
{
	int half = ceil(fontSize * .5);		//单字节显示的宽度
	int result = 0;
	int strSize = int(txt.size());
	for (int i = 0; i < strSize; i++)
	{
		char ch = txt[i];
		if (ch >= 0 && ch <= 127)
			result += half;
		else	//全角
		{
			result += fontSize;
			i += 2;			//utf-8编码占3个字节  所以直接跳过后面
		}
	}
	return result;
}

list<string> StringTools::warpString(string txt, int fontSize, int width)
{
	int half = ceil(fontSize * .5);
	list<string> result;
	int totalWidth = getStringWidthForShow(txt, fontSize);
	if (totalWidth <= width)
	{
		result.push_back(txt);
	}
	else
	{
		//拆分
		int i = 0;
		int strSize = int(txt.size());
		string str;
		int comWidth = 0;
		while (i < strSize)
		{
			char ch = txt[i];
			if (ch >= 0 && ch <= 127)
			{
				if (comWidth + half > width)
				{
					result.push_back(str);
					str = "";
					comWidth = 0;
				}
				else
				{
					str += ch;
					comWidth += half;
					i++;
				}
			}
			else//全角
			{
				if (comWidth + fontSize > width)
				{
					result.push_back(str);
					str = "";
					comWidth = 0;
				}
				else
				{
					str += ch;
					str += txt[++i];
					str += txt[++i];
					comWidth += fontSize;
					i++;
				}
			}
		}
		result.push_back(str);
	}
	return result;
}

void StringTools::readIntList(string& str, string delim, list<int>& lst)
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

void StringTools::readStringList(string& str, string delim, list<string>& lst)
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

void StringTools::readLstList(string& str, string delim, list<list<int>>& lst)
{
	if (str == "null")		return;
	size_t last = 0;
	size_t index = str.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		list<int> childlst;
		string childstr = str.substr(last, index - last);
		readIntList(childstr, LIST_DELIMITER_L, childlst);
		lst.push_back(childlst);
		last = index + 1;
		index = str.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		list<int> childlst;
		string childstr = str.substr(last, index - last);
		readIntList(childstr, LIST_DELIMITER_L, childlst);
		lst.push_back(childlst);
	}
}
