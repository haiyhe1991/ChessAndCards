#include "SpriteButton.h"

SpriteButton::SpriteButton()
{
	m_pFrameDisable= NULL;
	m_pFrameHighlight= NULL;
	m_pFrameNormal = NULL;
	m_pSprite = NULL;
	m_bSpriteMode = false;
	m_pPressedMoveInsideHandleObject= NULL;
	m_pPressedMoveInsideHandle = NULL;
	m_pPressedMoveOutsideHandleObject = NULL;
	m_pPressedOutsideHandle = NULL ;
	m_pPressedUpHandleObject = NULL ;
	m_pPressedUpHandle = NULL;
	m_pPressedDownHandle = NULL;
	m_pPressedDownHandleObject = NULL;
	m_bShock = false;
	m_bEnable = true;
	m_UnableColor = Color3B::Color3B(110,110,110);
}

SpriteButton::~SpriteButton()
{
	CC_SAFE_RELEASE_NULL(m_pFrameDisable);
	CC_SAFE_RELEASE_NULL(m_pFrameHighlight);
	CC_SAFE_RELEASE_NULL(m_pFrameNormal);
}
bool SpriteButton::init()
{
	bool bRet = false;
	do
	{
	 if(!CCLayer::init())
        break;
    //this->setTouchEnabled(true);//开启触摸设置
    this->ignoreAnchorPointForPosition(false);//开启能改变layer的锚点，现在锚点默认是0.5，0.5
	this->TouchEvent();
	bRet = true;
	} while (0);

	return bRet;
}
bool SpriteButton::isInsideTouch(Touch* touch, Event* event)
{
	auto target = static_cast<Sprite *>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s =target->getContentSize();
		Rect rect = Rect(0,0,s.width,s.height);
		if(rect.containsPoint(locationInNode))
		{
			return true;
		}
		return false;
}
void SpriteButton::TouchEvent()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	//touchListener->set
	touchListener->onTouchBegan = [ = ](Touch* touch, Event* event){
		if(isInsideTouch(touch,event)&&this->isVisible()&&this->isAllParentVisible()&&m_bEnable)
		{
			this->onPressedDown();
			if(m_bShock)
			{
				this->runAction(CCScaleTo::create(0.1f,1.1f));
			}
			return true;
		}
		return false;
	};
	 touchListener->onTouchMoved = [ = ](Touch* touch, Event* event){
	 
		 if(isInsideTouch(touch,event))
		 {
			 this->onPressedMoveInside();
		 }
		 else
		 {
			 this->onPressedMoveOutside();
		 }
	 };
	 touchListener->onTouchEnded = [ = ](Touch* touch, Event* event){
		 if(m_bShock||this->getScale()>1.0f)
		 {
			 this->runAction(CCScaleTo::create(0.1f,1.0f));
		 }
		 if(isInsideTouch(touch,event))
		 {
		 this->onPressedUp();
		 }
	 };
	 touchListener->onTouchCancelled = [ = ](Touch* touch, Event *event){
	 
	 };
	 
	// Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener,120);
	 Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);
}
void SpriteButton::setNormalFrame(SpriteFrame *frame)
{
    CC_SAFE_RETAIN(frame);
    CC_SAFE_RELEASE_NULL(m_pFrameNormal);
    m_pFrameNormal = frame;
	m_pSprite->setSpriteFrame(m_pFrameNormal);
}
SpriteButton* SpriteButton::createWithFrame(SpriteFrame* frameNormal,SpriteFrame* frameHighlight,SpriteFrame* frameDisable)
{
	assert(frameNormal);
    SpriteButton* pRef = create();
    pRef->m_bSpriteMode = false;
    pRef->initWithFrame(frameNormal, frameHighlight, frameDisable);
    return pRef;
}
void SpriteButton::initWithFrame(SpriteFrame* frameNormal,SpriteFrame* frameHighlight,SpriteFrame* frameDisable)
{
	 CCAssert(frameNormal, "normal frame cannot be null!");
    m_pFrameNormal = frameNormal;
    m_pFrameHighlight = frameHighlight;
    m_pFrameDisable = frameDisable;
    CC_SAFE_RETAIN(m_pFrameNormal);
    CC_SAFE_RETAIN(m_pFrameHighlight);
    CC_SAFE_RETAIN(m_pFrameDisable);
    m_pSprite = Sprite::createWithSpriteFrame(m_pFrameNormal);
    Size size = m_pSprite->getContentSize();
    this->setContentSize(size);
	m_pSprite->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(m_pSprite);
}
SpriteButton* SpriteButton::createWithSprite(Sprite* sprite)
{
	SpriteButton* pRef = create();
	return pRef;
}
void SpriteButton::setPressedDownHandler(Ref *object, SEL_MenuHandler handle)
{
//    CC_SAFE_RELEASE_NULL(m_pPressedUpHandleObject);
    m_pPressedDownHandleObject = object;
//    CC_SAFE_RETAIN(m_pPressedUpHandleObject);
    m_pPressedDownHandle = handle;
}
void SpriteButton::setPressedUpHandler(Ref *object, SEL_MenuHandler handle)
{
//    CC_SAFE_RELEASE_NULL(m_pPressedUpHandleObject);
    m_pPressedUpHandleObject = object;
//    CC_SAFE_RETAIN(m_pPressedUpHandleObject);
    m_pPressedUpHandle = handle;
}
void SpriteButton::setPressedMoveInsideHandler(Ref *object, SEL_MenuHandler handle)
{
//    CC_SAFE_RELEASE_NULL(m_pPressedUpHandleObject);
	m_pPressedMoveInsideHandleObject = object;
//    CC_SAFE_RETAIN(m_pPressedUpHandleObject);
    m_pPressedMoveInsideHandle = handle;
}
void SpriteButton::setPressedMoveOutsideHandler(Ref *object, SEL_MenuHandler handle)
{
	m_pPressedMoveOutsideHandleObject = object;
	m_pPressedOutsideHandle = handle;
}
void SpriteButton::setIsShock(bool bShock)
{
	m_bShock = bShock;
}
bool SpriteButton::getIsShock()
{
	return m_bShock;
}
void SpriteButton::onPressedDown()
{
	if(!m_bSpriteMode&&m_pFrameHighlight)
	{
		m_pSprite->setSpriteFrame(m_pFrameHighlight);
	}
	 if(m_pPressedDownHandleObject)
    {
        (m_pPressedDownHandleObject->*m_pPressedDownHandle)(this);
    }
}
void SpriteButton::onPressedUp()
{
	if(!m_bSpriteMode)
	{
		m_pSprite->setSpriteFrame(m_pFrameNormal);
	}
	if(m_pPressedUpHandleObject)
	{
		(m_pPressedUpHandleObject->*m_pPressedUpHandle)(this);
	}
}
void SpriteButton::onPressedMoveInside()
{
	if(m_pPressedMoveInsideHandleObject)
	{
		(m_pPressedMoveInsideHandleObject->*m_pPressedMoveInsideHandle)(this);
	}
}
void SpriteButton::onPressedMoveOutside()
{
	if(m_pPressedMoveOutsideHandleObject)
	{
		(m_pPressedMoveOutsideHandleObject->*m_pPressedOutsideHandle)(this);
	}
}
bool SpriteButton::isAllParentVisible()
{
	Node *parent = getParent();
	while (parent)
	{
		if(parent->isVisible())
		{
			parent = parent->getParent();
		}
		else
		{
			return false;
		}
	}
	return true;
}
void SpriteButton::setUnableColor(Color3B color)
{
	m_UnableColor = color;
}
void SpriteButton::setEnable(bool bIsEnable)
{
	if (m_bEnable == bIsEnable)
		return;
	m_bEnable = bIsEnable;
	if (!m_bEnable)
	{
		if (m_pFrameDisable)
		{
			m_pSprite->setSpriteFrame(m_pFrameDisable);
		}
		else
		{
			m_pSprite->setColor(m_UnableColor);
		}
	}
	else
	{
		m_pSprite->setSpriteFrame(m_pFrameNormal);
		m_pSprite->setColor(Color3B::Color3B(255, 255, 255));
	}
}

