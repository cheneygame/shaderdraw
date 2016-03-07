
#include"DrawLayer.h"
#include"ShaderNode.h"
bool DrawLayer::init()
{
	CCLayer::init();
	//bg
	LayerColor* lc = LayerColor::create(Color4B(100,100,100,150),960,640);
	addChild(lc);
	drawDemo();
	drawShader();
	//drawMatrix();
	return true;
}

void DrawLayer::drawShader()
{
	//shadertoy_Line1      抛物线
	//shadertoy_Glow
	//shadertoy_curvefitting  波浪
	//  shadertoy_LineSegment  一条线段
	//shadertoy_LineSegmentWave   平滑波浪
	//shadertoy_Draw1 shadertoy_AudioVisual
	//shadertoy_SD  平滑封闭区域
	//shadertoy_spline
	//shadertoy_1D_Linear  //多点连接线段
	//shadertoy_1D_Quadratic  //多点连接线段曲线
	//shadertoy_mario 超级玛丽
	//shadertoy_sea 海洋
	//shadertoy_Draw2,
	//shadertoy_Draw4,画线段
	//shadertoy_Draw5 ,纹理方面,画线段
	//shadertoy_Draw7,线段纹理混合,刮刮乐,多图片背景纹理
	//shadertoy_Balloons,气球，含有线段和封闭区域填充
	auto sn = ShaderNode::shaderNodeWithVertex("Shaders/example_MultiTexture.vsh", "Shaders/shadertoy_Draw4.fsh"); //

	auto s = Director::getInstance()->getWinSize();
	sn->setPosition(Vec2(s.width / 4, s.height / 4));
	sn->setContentSize(Size(s.width / 2, s.height / 2));

	auto moveto = MoveTo::create(1.0f, Vec2(s.width / 4, s.height / 4));
	auto moveto1 = MoveTo::create(1.0f, Vec2(s.width / 2, s.height / 2));
	auto sequece = Sequence::createWithTwoActions(moveto, moveto1);
	//sn->runAction(RepeatForever::create(sequece));
	addChild(sn, 1, "sn");
}

static int Tag_Node = 100;
void DrawLayer::drawDemo()
{
	//this->setTouchEnabled(true);
	
	Color4F color = Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1);
	DrawNode* node = DrawNode::create();
	node->drawLine(Vec2(0, 0), Vec2(100, 300), color);
	this->addChild(node, 1, Tag_Node);

	//auto s = Director::getInstance()->getWinSize();
	//DrawNode* drawNode = DrawNode::create();
	//float x = s.width * 2 - 100;
	//float y = s.height;

	//Vec2 vertices[] = { Vec2(5, 5), Vec2(x - 5, 5), Vec2(x - 5, y - 5), Vec2(5, y - 5) };
	//drawNode->drawPoly(vertices, 4, true, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	//this->addChild(drawNode);

	auto el = EventListenerTouchOneByOne::create();
	el->setSwallowTouches(true);
	//cocos2d::PointArray* pa = PointArray::create(1000);
	Vec2 from, control, to;
	int index = 0;
	el->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto pos1 = touch->getLocation();
		log("ended1:%f,%f", pos1.x, pos1.y);
		//from.x = pos1.x;
		lastDrawPos = Vec2(0,0);
		lastidx = Vec2(0, 0);
		lastMousePos = Vec2(0, 0);
		lastMousePosWhenDraw = Vec2(0, 0);
		return true;
	};

	el->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto pos1 = touch->getLocation();
		log("ended1:%f,%f", pos1.x, pos1.y);

		//node->drawCatmullRom();
	};

	el->onTouchMoved = [=](Touch* touch, Event* event)
	{
		auto pos1 = touch->getLocation();
		//log("moved1:%f,%f", pos1.x, pos1.y);
		node->drawPoint(pos1, 1, color);  //原生自带的画点
		drawPoint(pos1);
		//sendSnPos(pos1);  //发送原生自带的画点的坐标
		//node->drawCubicBezier(100, color);
		lastMousePos = pos1;
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(el, this);
	//dispatcher->addEventListenerWithSceneGraphPriority(el, this);

	/*CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *array = CCPointArray::create(20);

	array->addControlPoint(ccp(0, 0));
	array->addControlPoint(ccp(80, 80));
	array->addControlPoint(ccp(s.width - 80, 80));
	array->addControlPoint(ccp(s.width - 80, s.height - 80));
	array->addControlPoint(ccp(80, s.height - 80));
	array->addControlPoint(ccp(80, 80));
	array->addControlPoint(ccp(s.width / 2, s.height / 2));


	CCPointArray *array2 = CCPointArray::create(20);

	array2->addControlPoint(ccp(s.width / 2, 30));
	array2->addControlPoint(ccp(s.width - 80, 30));
	array2->addControlPoint(ccp(s.width - 80, s.height - 80));
	array2->addControlPoint(ccp(s.width / 2, s.height - 80));
	array2->addControlPoint(ccp(s.width / 2, 30));
	node->drawCatmullRom(array, 50,color);*/
}

void DrawLayer::sendSnPos(Vec2 pos)
{
	auto sn = this->getChildByName("sn");
	if (sn)
	{
		dynamic_cast<ShaderNode*>(sn)->setmousexy(pos.x, pos.y);
	}
}

void DrawLayer::drawPoint(Vec2 pos, bool draw)
{
	//log("drawPoint:%f,%f", pos.x, pos.y);
	if (draw)
	{
		DrawNode* node = (DrawNode*)(this->getChildByTag(Tag_Node));
		if (node)
		{
			node->drawPoint(pos, 1.0f, Color4F(1.0, 1.0, 0, 1.0));
		}
		sendSnPos(pos);
	}
	else
	{
		//1、atan（x）表示求的是x的反正切，其返回值为[-pi/2,+pi/2]之间的一个数。
		//2、atan2（y，x）求的是y / x的反正切，其返回值为[-pi, +pi]之间的一个数。
		drawByAngle(pos);
		return;

		Vec2 idx(int(pos.x / cellw), int(pos.y / cellh));
		Vec2 newpos(idx.x*cellw + cellw / 2, idx.y*cellh + cellh / 2);
		Vec2 subidx(idx.x - lastidx.x, idx.y - lastidx.y);
		Vec2 subpos(pos.x - lastDrawPos.x, pos.y - lastDrawPos.y);
		int dir = 0;
		if (lastidx.x == 0 && lastidx.y == 0) //第一个点
		{

		}
		else if (abs(subidx.x) < 0.000001 && abs(subidx.y) < 0.000001) //没有什么变化
		{

		}
		else //表示新的点
		{
			//log("subidx:%f,%f,%f", subidx.x, subidx.y, atan2(subidx.y, subidx.x));
			log("subpos:%f,%f,%f", subpos.x, subpos.y, atan2(subpos.y, subpos.x));
			//check dir by pos off(not idx)
			float curdir = atan2(subpos.y, subpos.x);
			float angle = 3.14*0.125;
			if (curdir >= -angle && curdir < angle) //right
			{
				dir = Right;
				log("dir:Right");
			}
			else if (curdir >= angle && curdir < angle * 3) //up
			{
				dir = Right_UP;
				log("dir:Right_UP");
			}
			else if (curdir >= angle*3 && curdir < angle*5) //up
			{
				dir = Up;
				log("dir:Up");
			}
			else if (curdir >= angle * 5 && curdir < angle * 7) //up
			{
				dir = Up_Left;
				log("dir:Up_Left");
			}
			else if (curdir < -angle && curdir >= -angle * 3) //down
			{
				dir = Down_Right;
				log("dir:Down_Right");
			}
			else if (curdir < -angle*3 && curdir >= -angle * 5) //down
			{
				dir = Down;
				log("dir:Down");
			}
			else if (curdir < -angle * 5 && curdir >= -angle * 7) //Left_Down
			{
				dir = Left_Down;
				log("dir:Left_Down");
			}
			else //left
			{
				dir = Left;
				log("dir:Left");
			}
			
		}
		drawByDir(dir,pos);
		//if (poses.find(idx) != poses.end())
		//{
		//	
		//}
		//else
		//{
		//	//没找到
		//	poses.insert(idx);
		//	drawPoint(newpos, true);
		//}

		//lastidx = idx;
		//lastDrawPos = pos;
	}
}

void DrawLayer::drawByDir(int dir, Vec2 mousepos)
{
	Vec2 offPos(0, 0);
	Vec2 curidx = lastidx;
	if (dir == 0)
	{
		curidx = Vec2(int(mousepos.x / cellw), int(mousepos.y / cellh));
	}else if (dir == Right)
	{
		offPos.x = 1;
	}
	else if (dir == Right_UP)
	{
		offPos.x = 1;
		offPos.y = 1;
	}
	else if (dir == Up)
	{
		offPos.y = 1;
	}
	else if (dir == Up_Left)
	{
		offPos.x = -1;
		offPos.y = 1;
	}
	else if (dir == Left)
	{
		offPos.x = -1;
	}
	else if (dir == Down_Right)
	{
		offPos.x = 1;
		offPos.y = -1;
	}
	else if (dir == Down)
	{
		offPos.y = -1;
	}
	else if (dir == Left_Down)
	{
		offPos.x = -1;
		offPos.y = -1;
	}
	else if (dir == Left)
	{
		offPos.x = -1;
	}
	Vec2 newidx = curidx;
	newidx.x += offPos.x;
	newidx.y += offPos.y;
	Vec2 newpos(newidx.x*cellw + cellw / 2, newidx.y*cellh + cellh / 2);
	if (poses.find(newidx) != poses.end())
	{

	}
	else
	{
		//没找到
		poses.insert(newidx);
		drawPoint(newpos, true);
	}

	lastidx = newidx;
	lastDrawPos = newpos;
}

void DrawLayer::drawMatrix()
{
	auto winSize = Director::getInstance()->getWinSize();
	int cellwn = int(winSize.width / cellw);
	int cellhn = int(winSize.height / cellh);
	for (int i = 0; i < cellwn;i++)
	{
		for (int j = 0; j < cellhn; j++)
		{
			Vec2 pos(i*cellw + cellw / 2, j*cellh + cellh / 2);
			drawPoint(pos,true);
		}
	}
}

//根据任意角度画平均分布的点
void DrawLayer::drawByAngle(Vec2 mousepos)
{
	//newpos:实际要花的位置
	//lastMousePos上次鼠标点击位置
	float mouseofflen = abs(lastDrawPos.x - mousepos.x) + abs(lastDrawPos.y - mousepos.y);//lastDrawPos lastMousePos lastMousePosWhenDraw
	if (mouseofflen < minMouseLen)
	{
		return;
	}
	Vec2 newpos(0,0);
	Vec2 subpos(mousepos.x - lastDrawPos.x, mousepos.y - lastDrawPos.y);  //lastDrawPos,lastMousePosWhenDraw,lastMousePos
	float atan2Angle = atan2(subpos.y, subpos.x);
	float offsetx = posLen*cos(atan2Angle);
	float offsety= posLen*sin(atan2Angle);
	//log("off:%f,%f", offsetx, offsety);
	//计算等长模式的 pos和idx
	newpos.x = lastDrawPos.x + offsetx;
	newpos.y = lastDrawPos.y + offsety;
	//第一个点
	if (lastDrawPos.x == 0 && lastDrawPos.y == 0)
	{
		newpos.x = mousepos.x;
		newpos.y = mousepos.y;
	}
	drawPoint(newpos, true);
	lastDrawPos = newpos;
	lastMousePosWhenDraw = mousepos;
	//递归，自动补充
	drawByAngle(mousepos);
}

void DrawLayer::drawByFixLen(Vec2 mousepos)
{
	
}

//bool DrawLayer::onTouchBegan(Touch *touch, Event *event)
//{
//
//	return true;
//}
//
//void DrawLayer::onTouchMoved(Touch *touch, Event *event)
//{
//	auto pos = touch->getLocationInView();
//	log("moved:%f,%f",pos.x,pos.y);
//	
//}
//
//void DrawLayer::onTouchEnded(Touch *touch, Event *event)
//{
//	auto pos = touch->getLocationInView();
//	log("ended:%f,%f", pos.x, pos.y);
//}
//
//void DrawLayer::onTouchCancelled(Touch *touch, Event *event)
//{
//
//}