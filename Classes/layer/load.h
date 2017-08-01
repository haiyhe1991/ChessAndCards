#ifndef __LOAD_H__
#define __LOAD_H__

#include "chessAndCards.h"

enum LoadType
{
	PLIST,					//图片资源
	UI,						//UI资源
	ARMATURE,				//动画资源
};


struct LoadItem
{

	LoadItem::LoadItem(LoadType type, const char* path)
	{
		this->type = type;
		this->path = path;
	};

	LoadItem::LoadItem(const char* path, const char* img, const char* plist)
	{
		this->type = LoadType::ARMATURE;
		this->path = path;
		this->img = img;
		this->plist = plist;
	};

	LoadType			type;			//加载类型
	const char*			path;			//路径

	//加载动画资源
	const char*			img;
	const char*			plist;
};

#endif

