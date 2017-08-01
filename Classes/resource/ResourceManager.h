#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//��Ϸ��Դ������   
#include "chessAndCards.h"

struct LoadItem;

class ResourceManager
{
public:
	static ResourceManager*		GetInstance();								//��Դ������  ����ģʽ
	static void					DestoryInstance();							//�ݻٵ���
	~ResourceManager();														//����

	void						Sync_load_plist(const char* path);			//ͬ������plistͼƬ
	void						Sync_unload_plist(const char* path);		//ͬ������plistͼƬ

	void						Sync_load_ui(const char* path);				//ͬ������ui��Դ
	void						Sync_unload_ui(const char* path);			//ж��ui��Դ

	void						Sync_load_animation(const char* path);				//ͬ�����ض�����Դ
	void						Sync_unload_animation(const char* path);			//ж�ض�����Դ

	SpriteFrame*				Get_resource_from_plist(const char* name);	//plist�ļ���ȡ����Դ
	Node*						Get_ui_resource(const char* path);			//��ȡ�Ѿ����ص�ui��Դ		
	Node*						Get_ani_resource(const char* path);			//��ȡ�Ѿ����صĶ�����Դ

	void						Sync_load(LoadItem item);					//����
	void						Release(list<LoadItem> lst);				//�����ڴ�

private:
	static ResourceManager*		_instance;									//����
	
	void						clear_all();								//�������UI��Դ

	map<string, Node*>			_uiMaps;									//ui��Դ�Ĵ洢����

	map<string, Node*>			_aniMaps;									//������Դ�Ĵ洢����

};


#endif

