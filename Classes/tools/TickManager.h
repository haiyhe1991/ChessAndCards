#ifndef __TICK_MANAGER_H__
#define __TICK_MANAGER_H__

#include <list>

using namespace std;

class GameTick
{
public:
	virtual void				OnTick(int time) = 0;	//时间到了之后的回调
	void						SetDelayTime(int time);	//设置间隔回调时间   如果time=0 代表每帧都进行回调
	void						Execute(int time);

private:

	int							delayTime;				//间隔时间

	int							count;					//当前的计算时间

};

class TickManager
{
public:
	static TickManager*			GetInstance();
	static void					DestoryInstance();

	void						OnTickHandler(int delay);

	void						Clear();

	void						Reg(GameTick* tick);
	void						Unreg(GameTick* tick);

private:
	
	list<GameTick*>				container;

	static TickManager*			_instance;
};

#endif // __TICK_MANAGER_H__
