
#include"EraserLayer.h"
#include "../utils/Utility.h"
//��Ҫ������������RenderTexture, visit һ�ŵ�ͼ�� һ���ƶ��ı�ˢͼƬ���ϻ�ϵĽ����
//RenderTexture �����б���ͼƬ�ȵ�
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

	//����һ����Ƥ��  
	eraserOther = Sprite::create("hole_effect.png"); //hole_effect eraser
	eraserOther->retain();

	//��������������ʾ  
	rTex = RenderTexture::create(visibleSize.width, visibleSize.height);
	rTex->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(rTex);

	//������Ҫ������������  
	spriteBG = Sprite::create("assetMgrBackground2.png");  //assetMgrBackground2 HelloWorld
	spriteBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	spriteBG->retain();
	//��������Ⱦ��������  
	/*rTex->begin();
	spriteBG->visit();
	rTex->end();*/

	//��ⴥ��
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

		//����Ƥ���õ������λ��  
		Vec2 touchPoint = touch->getLocation();
		eraserOther->setPosition(touchPoint);

		//���û�Ϸ�ʽ  
		//BlendFunc blendFunc = { GL_ONE_MINUS_SRC_ALPHA, GL_ZERO }; //GL_ONE, GL_ZERO  | GL_ZERO, GL_SRC_ALPHA |GL_ONE_MINUS_SRC_ALPHA,GL_ZERO
		//eraserOther->setBlendFunc(blendFunc);

		BlendFunc blendFunc1 = { GL_DST_ALPHA, GL_ZERO }; //GL_ONE, GL_ZERO  | GL_ZERO, GL_SRC_ALPHA |GL_ONE_MINUS_SRC_ALPHA,GL_ZERO
		spriteBG->setBlendFunc(blendFunc1);

		//������  
		rTex->begin();
		eraserOther->visit();
		spriteBG->visit();
		rTex->end();
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(el, this);
	return true;
}