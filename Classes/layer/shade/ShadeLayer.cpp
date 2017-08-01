#include "ShadeLayer.h"
#include "AppDelegate.h"

ShadeLayer::~ShadeLayer()
{
}
bool ShadeLayer::init()
{
	bool bRet = false;
	do
	{
	   if(!Layer::init())
		break;
	Layer* colorLayer = LayerColor::create(Color4B(0,0,0,150));
	colorLayer->setContentSize(Size(GAME_WIDHT, GAME_HEIGHT));
	colorLayer->ignoreAnchorPointForPosition(false);
	colorLayer->setAnchorPoint(Vec2(0.5,0.5));
	colorLayer->setPosition(Vec2(GAME_WIDHT / 2, GAME_HEIGHT / 2));
	this->addChild(colorLayer,0);
	auto listener = EventListenerTouchOneByOne::create();
     listener->onTouchBegan =  [](Touch * ,Event *)
    {
    return true;
    };
	 listener->setSwallowTouches(true);
	 _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,colorLayer);
		bRet = true;
	} while (0);

	 return bRet;
}