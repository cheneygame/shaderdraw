
#include"MainLayer.h"
#include "../utils/Utility.h"
bool MainLayer::init()
{
	CCLayer::init();
	drawLayer = DrawLayer::create();
	addChild(drawLayer,10,"drawLayer");
	addPencilScroll();
	addColorScroll();
	return true;
}

MainLayer::~MainLayer()
{
	
}


void MainLayer::addPencilScroll()
{
	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setContentSize(Size(30.0f, 300.0f));
	this->addChild(scrollView, 10, "scrollViewP");
	scrollView->setPosition(ccp(20, 130));

	float innerWidth = scrollView->getContentSize().width;
	float innerHeight = scrollView->getContentSize().height;

	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

	Vec2 offpos = { 15, 15 };
	float girdY = 50.0f;
	Button* button = Button::create("cocosui/yellow_edit.png", "cocosui/yellow_edit.png");  //"cocosui/yellow_edit.png", "cocosui/yellow_edit.png"
	button->setTitleText("Pencil1");
	button->setPosition(Vec2(0 + offpos.x, 0 + offpos.y));
	button->setTag(1);
	button->addTouchEventListener(CC_CALLBACK_2(MainLayer::dnw_idx, this));
	scrollView->addChild(button);

	Button* titleButton = Button::create("cocosui/yellow_edit.png", "cocosui/yellow_edit.png");  //"cocosui/yellow_edit.png", "cocosui/yellow_edit.png"
	titleButton->setTitleText("Zone");
	titleButton->setPosition(Vec2(0 + offpos.x, girdY + offpos.y));
	titleButton->setTag(2);
	titleButton->addTouchEventListener(CC_CALLBACK_2(MainLayer::dnw_idx, this));
	scrollView->addChild(titleButton);

	Button* titleButtonClr = Button::create("cocosui/yellow_edit.png", "cocosui/yellow_edit.png");  //"cocosui/yellow_edit.png", "cocosui/yellow_edit.png"
	titleButtonClr->setTitleText("PencilClr");
	titleButtonClr->setPosition(Vec2(0 + offpos.x, girdY*2 + offpos.y));
	titleButtonClr->setTag(3);
	titleButtonClr->addTouchEventListener(CC_CALLBACK_2(MainLayer::dnw_idx, this));
	scrollView->addChild(titleButtonClr);

	/*
	for (size_t i = 0; i < colorFListLen; i++)
	{
		Color4F c = colorFList[i];
		Widget* dnw = Widget::create();
		DrawNode* dn = DrawNode::create();
		dn->drawSolidRect(Vec2(0, 0), Vec2(30, 30), c);
		dnw->addChild(dn);
		dnw->setPosition(Vec2(80 + offpos.x + 50 * i, 0 + offpos.y));
		dnw->setContentSize(CCSize(30, 30));
		dnw->setTouchEnabled(true);
		dnw->addTouchEventListener(CC_CALLBACK_2(MainLayer::dnw_idx, this));
		dnw->setTag(i);
		//log("ac:%f,%f", dnw->getAnchorPoint().x, dnw->getAnchorPoint().y);
		scrollView->addChild(dnw);
	}
	*/
	scrollView->setBounceEnabled(true);
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
}

void MainLayer::addColorScroll()
{
	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setContentSize(Size(300.0f, 30.0f));
	this->addChild(scrollView,10,"scrollView");
	scrollView->setPosition(ccp(10,100));

	float innerWidth = scrollView->getContentSize().width;
	float innerHeight = scrollView->getContentSize().height;

	scrollView->setInnerContainerSize(Size(400.0f, 30.0f));  //内部容器大小

	Vec2 offpos = {15,15};
	Button* button = Button::create("cocosui/yellow_edit.png", "cocosui/yellow_edit.png");
	button->setPosition(Vec2(0 + offpos.x, 0 + offpos.y));
	//button->setTag(1);
	scrollView->addChild(button);

	Button* titleButton = Button::create("cocosui/yellow_edit.png", "cocosui/yellow_edit.png");
	//titleButton->setTitleText("Title Button");
	titleButton->setPosition(Vec2(50 + offpos.x, 0 + offpos.y));
	
	scrollView->addChild(titleButton);


	for (size_t i = 0; i < colorFListLen; i++)
	{
		Color4F c = colorFList[i];
		Widget* dnw = Widget::create();
		DrawNode* dn = DrawNode::create();
		dn->drawSolidRect(Vec2(0, 0), Vec2(30, 30), c);
		dnw->addChild(dn);
		dnw->setPosition(Vec2(80 + offpos.x + 50*i, 0 + offpos.y));
		dnw->setContentSize(CCSize(30, 30));
		dnw->setTouchEnabled(true);
		dnw->addTouchEventListener(CC_CALLBACK_2(MainLayer::dnw_cb, this));
		dnw->setTag(i);
		//log("ac:%f,%f", dnw->getAnchorPoint().x, dnw->getAnchorPoint().y);
		scrollView->addChild(dnw);
	}
	scrollView->setBounceEnabled(true);
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	//scrollView->setLayoutType(cocos2d::ui::Layout::Type::VERTICAL);
}

void MainLayer::dnw_cb(Ref* ref, Widget::TouchEventType type)
{
	
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		{

										  
			log("click:%d", ((Widget*)(ref))->getTag());
			int idx = (int)((Widget*)(ref))->getTag();
			if (drawLayer)
			{
				drawLayer->setBrushCF(colorFList[idx]);
			}
		}
		break;
	default:
		break;
	}
}

void MainLayer::dnw_idx(Ref* ref, Widget::TouchEventType type)
{

	switch (type)
	{
	case Widget::TouchEventType::ENDED:
	{


		log("click id:%d", ((Widget*)(ref))->getTag());
		int idx = (int)((Widget*)(ref))->getTag();
		if (drawLayer)
		{
			drawLayer->setPencilIdx(idx);
		}
	}
		break;
	default:
		break;
	}
}


