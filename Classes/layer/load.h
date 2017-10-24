#ifndef __LOAD_H__
#define __LOAD_H__

#include "chessAndCards.h"

enum LoadType
{
	PLIST,					//ͼƬ��Դ
	UI,						//UI��Դ
	ANIMATION,				//������Դ
};


struct LoadItem
{

	LoadItem::LoadItem(LoadType type, const char* path)
	{
		this->type = type;
		this->path = path;
	};

	LoadType			type;			//��������
	const char*			path;			//·��
};

#endif

