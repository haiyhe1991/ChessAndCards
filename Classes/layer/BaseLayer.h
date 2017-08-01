#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "chessAndCards.h"
#include "load.h"

//修改，所有的容器都以层的形式来
class BaseLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(BaseLayer);

	virtual void onEnter();							//真正进入到场景之中   在这个方法中，才是当前场景真正的初始化内容

	virtual void releaseResource();					//释放资源

public:
	virtual list<LoadItem> getResourceList();		//所需加载的资源列表
};


#endif