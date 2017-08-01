#ifndef __SHADE_LAYER_H__
#define __SHADE_LAYER_H__
#include "chessAndCards.h"
//一般性的遮挡层
class ShadeLayer :public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(ShadeLayer);
	~ShadeLayer();

	//virtual void onEnter();							//初始化函数

	//virtual list<LoadItem> getResourceList();			//取得资源加载列表

	//virtual void releaseResource();					//释放资源
//
//private:
//	MainLayout*				mainLayout;		//场景的背景容器		

};

//加载场景
class RecvingLayer : public ShadeLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RecvingLayer);

	~RecvingLayer();										//析构函数

private:
	Node*					layout;					//主界面层资源
};



#endif