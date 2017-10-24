#ifndef __LOAD_H__
#define __LOAD_H__

#include "chessAndCards.h"

enum LoadType
{
	PLIST,					//图片资源
	UI,						//UI资源
	ANIMATION,				//动画资源
};


struct LoadItem
{

	LoadItem::LoadItem(LoadType type, const char* path)
	{
		this->type = type;
		this->path = path;
	};

	LoadType			type;			//加载类型
	const char*			path;			//路径
};

#endif

