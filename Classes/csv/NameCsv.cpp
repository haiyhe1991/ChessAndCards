#include "csv.h"
#include "cocos2d.h"

//实际的csv文件解析函数
void NameCsv::parserData(string value)
{
	list<string> lst;
	StringTools::split(value, ",", lst);
	if (lst.size() > 2)
	{
		list<string>::iterator iter = lst.begin();
		NameCsvItem* item = new NameCsvItem;
		item->id = StringTools::stringConvertInt(*iter++);
		item->name = *iter++;
		item->portrait = *iter;

		container[item->id] = item;
	}
}

NameCsvItem* NameCsv::GetRandomName()
{
	while (true)
	{
		int randomId = cocos2d::RandomHelper::random_int(1, int(container.size()));
		if (!hasCache(randomId))
		{
			cacheLst.push_back(randomId);
			return container[randomId];
		}
	}
	return nullptr;
}

bool NameCsv::hasCache(int id)
{
	list<int>::iterator iter = cacheLst.begin();
	while (iter != cacheLst.end())
	{
		if (*iter == id)
		{
			return true;
		}
		iter++;
	}
	return false;
}
