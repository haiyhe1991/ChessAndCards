#ifndef __STAGE_H__
#define __STAGE_H__

#include "chessAndCards.h"
#include "msg/message.h"
#include "tools/TickManager.h"

class Stage : public Scene, GameMessage
{

public:

	static Stage*			GetInstance();		//取得单例
	virtual bool			init();				//初始化函数
	virtual void			OnMessage(const int head, void* data);
	CREATE_FUNC(Stage);

private:

	static Stage*			_instance;

	Layer*					current;						//当前正在显示的

	Layer*					asyncLayer;						//异步删除所用的layer存储指针

	void					onScheduleHandler(float dt);

	void					async_clearChild();				//异步删除舞台中的对象

};

#endif