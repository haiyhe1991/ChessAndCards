#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//游戏资源管理器   
#include "chessAndCards.h"

struct LoadItem;

class ResourceManager
{
public:
	static ResourceManager*		GetInstance();								//资源管理器  单例模式
	static void					DestoryInstance();							//摧毁单例
	~ResourceManager();														//析构

	void						Sync_load_plist(const char* path);			//同步加载plist图片
	void						Sync_unload_plist(const char* path);		//同步清理plist图片

	void						Sync_load_ui(const char* path);				//同步加载ui资源
	void						Sync_unload_ui(const char* path);			//卸载ui资源

	void						Sync_load_animation(const char* path);				//同步加载动画资源
	void						Sync_unload_animation(const char* path);			//卸载动画资源

	SpriteFrame*				Get_resource_from_plist(const char* name);	//plist文件中取出资源
	Node*						Get_ui_resource(const char* path);			//获取已经加载的ui资源		
	Node*						Get_ani_resource(const char* path);			//获取已经加载的动画资源

	void						Sync_load(LoadItem item);					//加载
	void						Release(list<LoadItem> lst);				//销毁内存

private:
	static ResourceManager*		_instance;									//单例
	
	void						clear_all();								//清除所有UI资源

	map<string, Node*>			_uiMaps;									//ui资源的存储容器

	map<string, Node*>			_aniMaps;									//动画资源的存储容器

};


#endif

