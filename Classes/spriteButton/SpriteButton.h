#ifndef __SPRITE_BUTTON_H__
#define __SPRITE_BUTTON_H__

#include "chessAndCards.h"

class SpriteButton:public Layer
{
public:
	SpriteButton();
	~SpriteButton();
	virtual bool init();
	//根据帧来创建精灵
	static SpriteButton* createWithFrame(SpriteFrame* frameNormal,SpriteFrame* frameHighlight=NULL,SpriteFrame* frameDisable=NULL);
	//根据图片名字来创建精灵
	static SpriteButton* createWithSprite(Sprite* sprite);

	//设置按下建执行的回调函数
	virtual void setPressedDownHandler(Ref *object, SEL_MenuHandler handle);
	//设置按键松开执行的回调函数
	virtual void setPressedUpHandler(Ref* object, SEL_MenuHandler handle);
	//设置按键按着不放时并且在精灵范围内执行的回调函数
	virtual void setPressedMoveInsideHandler(Ref *object, SEL_MenuHandler handle);
	//设置按键按着不放时并且在精灵范围外执行的回调函数
	virtual void setPressedMoveOutsideHandler(Ref *object, SEL_MenuHandler handle);
	//设置按键是否震动
	virtual void setIsShock(bool bShock);
	//获取是否震动
	virtual bool getIsShock();
	//改变精灵图片普通显示时的图片帧
	virtual void setNormalFrame(SpriteFrame *frame);
	//设置按键失效时精灵变暗的颜色
	virtual void setUnableColor(Color3B color);
	//设置是否响应按键操作
	virtual void setEnable(bool bIsEnable);
	CREATE_FUNC(SpriteButton);
protected:
	//初始化按钮
	virtual void initWithFrame(SpriteFrame* frameNormal,SpriteFrame* frameHighlight,SpriteFrame* frameDisable);
	//是否是用图片名字来创建按钮
	bool m_bSpriteMode;
	//按钮正常显示时用的帧
	SpriteFrame *m_pFrameNormal;
	//按钮按下就显示的帧
	SpriteFrame *m_pFrameHighlight;
	//按钮无效时的帧
	SpriteFrame *m_pFrameDisable;
	//按钮精灵
	Sprite *m_pSprite;
	//执行按钮按下回调函数的对象
	Ref *m_pPressedDownHandleObject;
	//按钮按下的回调函数
	SEL_MenuHandler m_pPressedDownHandle;
	//执行按钮按下后弹起的回调函数的对象
	Ref *m_pPressedUpHandleObject;
	//按钮按下后弹起的回调函数
	SEL_MenuHandler m_pPressedUpHandle;
	//按钮按着不放并且在按钮范围内的对象
	Ref *m_pPressedMoveInsideHandleObject;
	//按钮按着不放并且在按钮范围内的回调函数
	SEL_MenuHandler m_pPressedMoveInsideHandle;
	//按钮按着不放并且在按钮范围外的对象
	Ref *m_pPressedMoveOutsideHandleObject;
	//按钮按着不放并且在按钮范围外的回调函数
	SEL_MenuHandler m_pPressedOutsideHandle;
	//是否在按钮范围内
	bool isInsideTouch(Touch* touch, Event* event);
	//按钮按下
	virtual void onPressedDown();
	//按钮按下后弹起
	virtual void onPressedUp();
	//按钮按着在按钮范围内
	virtual void onPressedMoveInside();
	//按钮按着在按钮范围外
	virtual void onPressedMoveOutside();
private:
	//响应监听事件
	void TouchEvent();
	//节点是否可见
	bool isAllParentVisible();
	//是否震动
	bool m_bShock;
	//是否响应按钮回调函数
	bool m_bEnable;
	//按钮失效时的颜色
	Color3B m_UnableColor;
};



#endif