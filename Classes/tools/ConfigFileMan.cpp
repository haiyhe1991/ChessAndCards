#include "ConfigFileMan.h"

ConfigFileMan* ConfigFileMan::sg_cfgMan = nullptr;

ConfigFileMan* ConfigFileMan::getInstance()
{
	if (sg_cfgMan == nullptr)
	{
		sg_cfgMan = new ConfigFileMan();
	}
	return sg_cfgMan;
}

void ConfigFileMan::destoryInstance()
{
	if (sg_cfgMan != nullptr)
		delete sg_cfgMan;
	sg_cfgMan = nullptr;
}

ConfigFileMan::ConfigFileMan()
{
	//m_rootValue = nullptr;
}

ConfigFileMan::~ConfigFileMan()
{

}

bool ConfigFileMan::LoadConfigFile(const char* pszFilePath)
{
	if (pszFilePath == NULL || strlen(pszFilePath) <= 0)
		return false;
	//std::string data = FileUtils::getInstance()->getStringFromFile("language.properties");
	//std::ifstream ifs;
	//ifs.open(pszFilePath);
	//if (!ifs.is_open())
	//	return false;	
	m_dic = Dictionary::createWithContentsOfFile(pszFilePath);
	//Json::Reader reader;
	//Json::Value rootValue;
	//if (!reader.parse(data, rootValue))
	//{
	//	//ifs.close();
	//	return false;
	//}
	//ifs.close();

	//bool ok = true;
	//if (rootValue.isMember("gws_ip") && rootValue["gws_ip"].isString())
	//	m_gwsIp = rootValue["gws_ip"].asCString();
	//else
	//	ok = false;

	//if (rootValue.isMember("gws_port") && rootValue["gws_port"].isInt())
	//	m_gwsPort = rootValue["gws_port"].asInt();
	//else
	//	ok = false;

	//if (rootValue.isMember("version") && rootValue["version"].isString())
	//	m_version = rootValue["version"].asCString();

	//if (rootValue.isMember("init_gw_connections") && rootValue["init_gw_connections"].isInt())
	//	m_initGWConnections = rootValue["init_gw_connections"].asInt();
	return true;
}

std::string ConfigFileMan::getValue(std::string memberName)
{
	std::string str = "";
	//if (m_rootValue == nullptr)
	//{
	//	return str;
	//}
	//if (m_rootValue.isMember(memberName) && m_rootValue[memberName].isString())
	//	str = m_rootValue[memberName].asCString();
	str = ((__String*)m_dic->objectForKey(memberName))->getCString();
	return str;
}