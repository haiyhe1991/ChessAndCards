#ifndef __CONFIG_FILE_MAN_H__
#define __CONFIG_FILE_MAN_H__
#include "cocos2d.h"

USING_NS_CC;
class ConfigFileMan
{
public:
	~ConfigFileMan();
	static ConfigFileMan*		getInstance();								//取得时间管理的唯一实例
	static void				destoryInstance();							//销毁当前实例

	bool LoadConfigFile(const char* pszFile = "language.xml");

	std::string getValue(std::string memberName);

private:
	ConfigFileMan();
	__Dictionary* m_dic;
	static ConfigFileMan* sg_cfgMan;
};


#endif