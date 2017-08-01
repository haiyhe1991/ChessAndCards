#include "message.h"

//µ¥ÀıÄ£Ê½
MsgManager* MsgManager::_instance = nullptr;

MsgManager* MsgManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new MsgManager();
	}
	return _instance;
}

void MsgManager::DestoryInstance()
{
	if (_instance != nullptr)
	{
		_instance->Clear();
		delete _instance;
	}
	_instance = nullptr;
}

void MsgManager::Reg(const int head, GameMessage* msg)
{
	map<int, list<GameMessage*>>::iterator iter = container.find(head);
	if (iter == container.end())
	{
		list<GameMessage*> lst;
		lst.push_back(msg);
		container[head] = lst;
	}
	else
	{
		list<GameMessage*>& lst = iter->second;
		list<GameMessage*>::iterator lstIter = lst.begin();
		while (lstIter != lst.end())
		{
			if (msg == (*lstIter))
				return;
			lstIter++;
		}
		lst.push_back(msg);
	}
}

void MsgManager::Unreg(const int head, GameMessage* msg)
{
	map<int, list<GameMessage*>>::iterator iter = container.find(head);
	if (iter == container.end())
	{
		return;
	}
	list<GameMessage*>& lst = iter->second;
	lst.remove(msg);
	if (lst.size() == 0)
	{
		container.erase(head);
	}
}

void MsgManager::Dispather(const int head, void* data)
{
	map<int, list<GameMessage*>>::iterator iter = container.find(head);
	if (iter != container.end())
	{
		list<GameMessage*> lst = iter->second;
		list<GameMessage*>::iterator lstIter = lst.begin();
		while (lstIter != lst.end())
			(*lstIter++)->OnMessage(head, data);
	}
}

void MsgManager::Clear()
{
	map<int, list<GameMessage*>>::iterator iter = container.begin();
	while (iter != container.end())
	{
		iter->second.clear();
		iter++;
	}
	container.clear();
}
