#ifndef __LOADING_BAR_H__
#define __LOADING_BAR_H__

//资源加载

#include "chessAndCards.h"
#include "tools/TickManager.h"

#define LOADING_TICK_TIME			300		//加载的时间间隔

class BaseLayer;
struct LoadItem;

//加载场景
class LoadingLayer : public Layer, GameTick
{
public:
	virtual bool init();
	CREATE_FUNC(LoadingLayer);

	~LoadingLayer();										//析构函数
	void				StartLoading(BaseLayer* layer);
	void				ChangeProgress();
	virtual void		OnTick(int time);
private:
	BaseLayer*			base_layer;							//数据存储
	list<LoadItem>		loading_lst;						//需要加载的资源列表       
protected:

	void				loadingMethod();

	Node*				resource;							//总的资源
	LoadingBar*			progress;

	int					max_num;
	int					cur_num;

};


#endif
