#ifndef __CSV_H__
#define __CSV_H__

#include <list>
#include <string>
#include <map>
#include "tools/StringTools.h"

using namespace std;

class CSV
{
protected:
	//跳过的行数
	void init(const std::string fileName, const int skip);
	virtual void parserData(string value) = 0;
	void readIntList(string& str, string delim, list<int>& lst);
	void readStringList(string& str, string delim, list<string>& lst);
};

struct NameCsvItem
{
	int				id;
	string			name;
	string			portrait;			//头像
};

class NameCsv : public CSV
{
public:
	NameCsv(){
		init("CSV_data/namelist.csv", 2);
	};

	NameCsvItem*	GetRandomName();

protected:
	list<int>		cacheLst;					//缓存已经随机过的ID，直接避免重复

	bool			hasCache(int id);
	virtual void	parserData(string value);

private:
	map<int, NameCsvItem*> container;

};

//csv数据存储类
class CsvData
{
public:
	CsvData();

	NameCsv*				nameCsv;

public:
	static CsvData*			GetInstance();
	static void				DestoryInstance();

private:
	static CsvData*			_instance;
};

#endif
