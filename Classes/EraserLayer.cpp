
#include"EraserLayer.h"
#include "../utils/Utility.h"
//主要用来测试利用RenderTexture, visit 一张底图和 一个移动的笔刷图片不断混合的结果，
//RenderTexture 背后还有背景图片等等
EraserLayer::~EraserLayer()
{
	eraserOther->release();
	spriteBG->release();
}

bool EraserLayer::init()
{
	CCLayer::init();
	auto visibleSize = Director::getInstance()->getWinSize();
	//bg
	
	auto bgSP = Sprite::create("Images/landscape-1024x1024.png");
	this->addChild(bgSP);

	//创建一个橡皮擦  
	eraserOther = Sprite::create("hole_effect.png"); //hole_effect eraser
	eraserOther->retain();

	//创建画布，并显示  
	rTex = RenderTexture::create(visibleSize.width, visibleSize.height);
	rTex->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(rTex);

	//创建需要被擦除的内容  
	spriteBG = Sprite::create("assetMgrBackground2.png");  //assetMgrBackground2 HelloWorld
	spriteBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	spriteBG->retain();
	//将内容渲染到画布上  
	/*rTex->begin();
	spriteBG->visit();
	rTex->end();*/

	//检测触摸
	auto el = EventListenerTouchOneByOne::create();
	el->setSwallowTouches(true);

	el->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto pos1 = touch->getLocation();
		log("onTouchBegan:%f,%f", pos1.x, pos1.y);


		return true;
	};

	el->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto pos1 = touch->getLocation();
		
	};

	el->onTouchMoved = [=](Touch* touch, Event* event)
	{

		auto pos1 = touch->getLocation();
		log("moved1:%f,%f", pos1.x, pos1.y);

		//将橡皮设置到点击的位置  
		Vec2 touchPoint = touch->getLocation();
		eraserOther->setPosition(touchPoint);

		//设置混合方式  
		//BlendFunc blendFunc = { GL_ONE_MINUS_SRC_ALPHA, GL_ZERO }; //GL_ONE, GL_ZERO  | GL_ZERO, GL_SRC_ALPHA |GL_ONE_MINUS_SRC_ALPHA,GL_ZERO
		//eraserOther->setBlendFunc(blendFunc);

		BlendFunc blendFunc1 = { GL_DST_ALPHA, GL_ZERO }; //GL_ONE, GL_ZERO  | GL_ZERO, GL_SRC_ALPHA |GL_ONE_MINUS_SRC_ALPHA,GL_ZERO
		spriteBG->setBlendFunc(blendFunc1);

		//开擦！  
		rTex->begin();
		eraserOther->visit();
		spriteBG->visit();
		rTex->end();
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(el, this);
	return true;
}