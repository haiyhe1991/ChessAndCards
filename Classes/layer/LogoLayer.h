#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "chessAndCards.h"
#include "tools/TickManager.h"


//logo 界面显示时间
#define LOGO_LAYER_SHOW_TIME			3000				//暂定为3秒

//logo界面

class LogoLayer : public Layer, GameTick
{
public:
	virtual bool init();

	~LogoLayer();

	// a selector callback
	//void menuCloseCallback(cocos2d::Ref* pSender);

	// 计时器 回调函数
	virtual void OnTick(int time);

	// implement the "static create()" method manually
	CREATE_FUNC(LogoLayer);

private:
	int						count;
};

#endif
