#ifndef __CHESS_AND_CARDS_H__
#define __CHESS_AND_CARDS_H__

//一般性的头文件，用于引用一些常规引用的头文件

using namespace std;
//cocos2d
#include "cocos2d.h"

////editbox
//#include "cocos-ext.h"
//USING_NS_CC_EXT;
//#include "cocostudio/CCArmatureDataManager.h"
//#include "cocostudio/CCArmature.h"

//cocosstudio
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

//ui
#include "ui/UILoadingBar.h"
#include "ui/UIButton.h"
#include "ui/UITextField.h"
#include "ui/UICheckBox.h"
#include "ui/UIListView.h"
#include "ui/UIText.h"
#include "ui/UIPageView.h"


using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocostudio::timeline;

#include <string>
#include <vector>
#include <list>
#include <map>

//custom
#include "tools/ConfigFileMan.h"
#include "tools/TickManager.h"
#include "tools/TimeTools.h"
#include "tools/StringTools.h"
#include "spriteButton/SpriteButton.h"

//管理
#include "game_manage/GameManage.h"
#include "game_record/GameRecord.h"

#include "msg/message.h"

#define POKER_PILE_ID				-1
#define MAU_PLAYER_NUMBER			4
#define POKER_COLOR_NUMBER			4
#define POKER_POINT_COLOR			13
#define TOTAL_POKER_NUMBER			104	
#define FIRST_HAND_NUMBER			5
#define POKER_TAG(count,color,point)	(((count) - 1) * POKER_COLOR_NUMBER + (color)) * 100 + (point)
#define POKER_COUNT(tag)			(tag)/500+1
#define POKER_COLOR(tag)			((tag)/100)%5 + (tag)/500
#define POKER_POINT(tag)			(tag)%100
#define POKER_VALUE(tag)			(POKER_COLOR((tag)))*1000+(POKER_POINT((tag)))*10+POKER_COUNT((tag))

#endif 
