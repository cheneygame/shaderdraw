
#include"EraserLayer.h"
#include "../utils/Utility.h"
//��Ҫ������������RenderTexture, visit һ�ŵ�ͼ�� һ���ƶ��ı�ˢͼƬ���ϻ�ϵĽ����
//RenderTexture �����б���ͼƬ�ȵ�
EraserLayer::~EraserLayer()
{
	eraserPencil->release();
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
	eraserPencil = Sprite::create("hole_effect.png"); //hole_effect eraser
	eraserPencil->retain();

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
		eraserPencil->setPosition(touchPoint);

		//���û�Ϸ�ʽ  
		//BlendFunc blendFunc = { GL_ONE_MINUS_SRC_ALPHA, GL_ZERO }; //GL_ONE, GL_ZERO  | GL_ZERO, GL_SRC_ALPHA |GL_ONE_MINUS_SRC_ALPHA,GL_ZERO
		//eraserPencil->setBlendFunc(blendFunc);

		//GL_DST_ALPHA, GL_ZERO ��GL_DST_ALPHA*self.color  +  GL_ZERO*pencil.color
		BlendFunc blendFunc1 = { GL_DST_ALPHA, GL_ZERO }; //GL_ONE, GL_ZERO  | GL_ZERO, GL_SRC_ALPHA |GL_ONE_MINUS_SRC_ALPHA,GL_ZERO
		spriteBG->setBlendFunc(blendFunc1);

		//������  
		rTex->begin();
		eraserPencil->visit(); //target
		spriteBG->visit(); //src
		rTex->end();
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(el, this);
	return true;
}