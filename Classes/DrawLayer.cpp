
#include"DrawLayer.h"
#include"ShaderNode.h"
#include "../utils/Utility.h"
bool DrawLayer::init()
{
	CCLayer::init();
	if (false){
		//测试单独shader代码
		drawShader();
	}
	else
	{
		initShaderPencils();
		//bg
		auto s = Director::getInstance()->getWinSize();
		LayerColor* lc = LayerColor::create(Color4B(100,100,100,150),s.width,s.height);
		addChild(lc);
		drawNode();  //自带画点
		#ifndef BeiZerTest
		#ifdef ShowShaderLayer
				drawShader();  //调用shader
		#endif
		#endif

		#ifdef UseSendPosPool
				scheduleUpdate();
		#endif
				//drawMatrix();  //点阵
				//addUI();  //选择笔刷
	}
	
	return true;
}

static int Tag_Node = 100;  //drawNode用来画一些点和线 便于观察,ZOrder = 1
void DrawLayer::initShaderPencils()
{
	log("initShaderPencils");
	auto s = Director::getInstance()->getWinSize();
	
	//shadertoy_Draw_YH6
	pencil1 = SPencil1::SPencil1WithVertex("Shaders/example_MultiTexture.vsh", "Shaders/shadertoy_Draw_SS3.fsh"); //shadertoy_Draw_YH8 shadertoy_Draw_SS2
	log("pencil1");
	pencil1->retain();
	pencil1->setPosition(Vec2(s.width / 4, s.height / 4));
	pencil1->setContentSize(Size(s.width / 2, s.height / 2));

	//这个初始化时间较长
	szone = SZone::SZoneWithVertex("Shaders/example_MultiTexture.vsh", "Shaders/shadertoy_Draw_DL10.fsh");
	log("szone");
	szone->retain();
	szone->setPosition(Vec2(s.width / 4, s.height / 4));
	szone->setContentSize(Size(s.width / 2, s.height / 2));

	pencilClr = SPencilClr::SPencilClrWithVertex("Shaders/example_MultiTexture.vsh", "Shaders/shadertoy_Draw_YH9.fsh");
	log("pencilClr");
	pencilClr->retain();
	pencilClr->setPosition(Vec2(s.width / 4, s.height / 4));
	pencilClr->setContentSize(Size(s.width / 2, s.height / 2));
}

//这个函数不用，用initShaderPencils()
//这个函数变成演示shader用
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
	//shadertoy_Draw9,马赛克
	//shadertoy_Balloons,气球，含有线段和封闭区域填充
	//shadertoy_Draw_DL1 ,shadertoy_Draw_DL2,shadertoy_Draw_DL3 shadertoy_Draw_DL4 
	//shadertoy_Draw_DL5 ，粗细线段
	//shadertoy_Draw_DL6 ，shadertoy_Draw_DL7,shadertoy_Draw_DL7,封闭区域
	//shadertoy_Draw_YH2，线段，和自身纹理混合
	//shadertoy_Draw_YH6，用体贴采样来画线
	//shadertoy_Draw_DL10 ,封闭区域 + 边缘线段
	//shadertoy_Draw_YH9 shadertoy_Draw_YH7,clear笔刷
	//shadertoy_Draw_YH8，shadertoy_Draw_YH6，颜色笔刷
	//shadertoy_mario mario
	auto sn = ShaderNode::shaderNodeWithVertex("Shaders/example_MultiTexture.vsh", "Shaders/shadertoy_mario.fsh"); //shadertoy_Draw_YH6 shadertoy_mario shadertoy_sea

	auto s = Director::getInstance()->getWinSize();
	sn->setPosition(Vec2(s.width / 4, s.height / 4));
	sn->setContentSize(Size(s.width / 2, s.height / 2));

	auto moveto = MoveTo::create(1.0f, Vec2(s.width / 4, s.height / 4));
	auto moveto1 = MoveTo::create(1.0f, Vec2(s.width / 2, s.height / 2));
	auto sequece = Sequence::createWithTwoActions(moveto, moveto1);
	//sn->runAction(RepeatForever::create(sequece));
	addChild(sn, 1, "sn");
	this->tsn = sn;

//#ifdef UseRenderTexture
//	this->tsn = sn;
//	sn->retain();
//#else
//	addChild(sn, 1, "sn");
//	this->tsn = sn;
//#endif
	
}

void DrawLayer::resetShaderHandler()
{
	switch (shaderStrIdx)
	{
	case ShaderPencilIndex::Pencil1:
		this->tsn = pencil1;
		break;
	case ShaderPencilIndex::Zone:
		this->tsn = szone;
		break;
	case ShaderPencilIndex::PencilClr:
		this->tsn = pencilClr;
		break;
	default:
		break;
	}


}

void DrawLayer::addShaderNode()
{
	resetShaderHandler();
#ifdef UseRenderTexture

#else
	addChild(this->tsn, 1, "sn");
#endif
	return;
	/*
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
	//shadertoy_Draw_DL1 ,shadertoy_Draw_DL2,shadertoy_Draw_DL3 shadertoy_Draw_DL4 
	//shadertoy_Draw_DL5 ，粗细线段
	//shadertoy_Draw_DL6 ，shadertoy_Draw_DL7,封闭区域
	//shadertoy_Draw_YH2，线段，和自身纹理混合
	//shadertoy_Draw_YH6，用体贴采样来画线
	//const char* shaderstrs[] = { "Shaders/shadertoy_Draw_YH6.fsh", "Shaders/shadertoy_Draw_DL7.fsh", };
	const char* shaderstrs[] = { "Shaders/shadertoy_Draw_DL7.fsh", "Shaders/shadertoy_Draw_YH6.fsh", };  //shaderStrIdx
	auto sn = ShaderNode::shaderNodeWithVertex("Shaders/example_MultiTexture.vsh", shaderstrs[shaderStrIdx]); //

	auto s = Director::getInstance()->getWinSize();
	sn->setPosition(Vec2(s.width / 4, s.height / 4));
	sn->setContentSize(Size(s.width / 2, s.height / 2));

	auto moveto = MoveTo::create(1.0f, Vec2(s.width / 4, s.height / 4));
	auto moveto1 = MoveTo::create(1.0f, Vec2(s.width / 2, s.height / 2));
	auto sequece = Sequence::createWithTwoActions(moveto, moveto1);
	//sn->runAction(RepeatForever::create(sequece));
#ifdef UseRenderTexture
	this->tsn = sn;
	sn->retain();
#else
	addChild(sn, 1, "sn");
	this->tsn = sn;
#endif
	*/
}

void DrawLayer::removeShaderNode()
{
	return;
#ifdef UseRenderTexture
	if (this->tsn)
	{
		this->tsn->release();
		this->tsn = nullptr;
	}
#else
	if (this->tsn)
	{
		removeChildByName("sn");
		this->tsn = nullptr;
	}
#endif
}

void DrawLayer::menuCallback(Ref* sender)
{
	auto tag = ((MenuItemImage*)sender)->getTag();
	log("tag:%d",tag);
	shaderStrIdx = ShaderPencilIndex(tag - 101);
}

//ui
void DrawLayer::addUI()
{
	auto menuCallback1 = [](Ref* sender)->void
	{

	};
	auto menuCallback2 = [&](Ref* sender)
	{
		
	};
	const char* s_SendScore = "Images/SendScoreButton.png";
	// Image Item
	auto item1 = MenuItemImage::create(s_SendScore, s_SendScore, CC_CALLBACK_1(DrawLayer::menuCallback, this));
	auto item2 = MenuItemImage::create(s_SendScore, s_SendScore, CC_CALLBACK_1(DrawLayer::menuCallback, this));
	auto menu = Menu::create(item1, item2, nullptr);
	item1->setTag(101);
	item2->setTag(102);
	menu->alignItemsVertically();
	
	addChild(menu,10,"menu");
	menu->setPosition(ccp(80, 500));
}


void DrawLayer::drawNode()
{
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
		pencilAction = PencilAction::PencilAction_Began;
		addShaderNode(); 

		auto pos1 = touch->getLocation();
		log("onTouchBegan:%f,%f", pos1.x, pos1.y);
		//from.x = pos1.x;
		lastDrawPos = Vec2(0,0);
		lastidx = Vec2(0, 0);
		lastMousePos = Vec2(0, 0);
		lastMousePosWhenDraw = Vec2(0, 0);
		if (linePos.size() >= 4)  //用来测试2条相交的线
		{
			linePos.clear();
		}
		linePos.push_back(pos1);
		//TouchBegan就开始画
		drawPoint(pos1);
		lastMousePos = pos1;

		return true;
	};

	el->onTouchEnded = [=](Touch* touch, Event* event)
	{
		pencilAction = PencilAction::PencilAction_End;
		auto pos1 = touch->getLocation();
		log("ended1:%f,%f", pos1.x, pos1.y);
#ifdef BeiZerTest
		beizerPos.push_back(pos1);
		if(beizerPos.size() >= 4)
		{
			startBeizer();
			beizerPos.clear();
		}
#else

		switch (shaderStrIdx)
		{
			case ShaderPencilIndex::Zone:
			{
				this->autoCreateBeizerPos(); //创建弧线
				//this->drawFirstPosesLines();  //把所有点用直线连接 起来,画出整个区域轮廓
				//auto sn = this->getChildByName("sn");
				auto sn = this->tsn;
				if (sn)
				{
					switch (shaderStrIdx)
					{
					case ShaderPencilIndex::Pencil1:
						dynamic_cast<SPencil1*>(sn)->setzonepos(firstPoses);
						break;
					case ShaderPencilIndex::Zone:
						dynamic_cast<SZone*>(sn)->setzonepos(firstPoses);
						break;
					case ShaderPencilIndex::PencilClr:
						//this->tsn = pencilClr;
						break;

					default:
						break;
					}

				}
				firstPoses.clear();
				//zone模式是 mouseEnd结束后绘制一次，pencil模式是放在update
				updateRenderTexture();
			}
				break;
			case ShaderPencilIndex::Pencil1:
			{
				pencil1DrawEnd();
			}break;
			default:
				break;
		}
/*
#ifdef ZoneCode
		if (isGetFirstPoses) //第一次花区域
		{
			this->autoCreateBeizerPos(); //创建弧线
			this->drawFirstPosesLines();  //把所有点用直线连接 起来
			//auto sn = this->getChildByName("sn");
			auto sn = this->tsn;
			if (sn)
			{
				switch (shaderStrIdx)
				{
				case ShaderPencilIndex::Pencil1:
					dynamic_cast<SPencil1*>(sn)->setzonepos(firstPoses);
					break;
				case ShaderPencilIndex::Zone:
					dynamic_cast<SZone*>(sn)->setzonepos(firstPoses);
					break;
				default:
					break;
				}
				
			}
		}
		else
		{
			//已经划过区域了，判断封闭区域
			checkPosInZone(pos1);
		}
		isGetFirstPoses = false;
		//node->drawCatmullRom();
		linePos.push_back(pos1);
		//画出2条相交的线
		if (linePos.size() >= 2)
		{
			//node->drawLine(linePos[linePos.size() - 2], linePos[linePos.size() - 1], color);
		}
		//判断2条相交的线
		if (linePos.size() >= 4)
		{
			if (SegmentIntersect(linePos[0], linePos[1], linePos[2], linePos[3]))
				log("YES:%d", linePos.size());
			else
				log("NO:%d", linePos.size());
		}
#endif
*/
#endif
		removeShaderNode();
	};

	el->onTouchMoved = [=](Touch* touch, Event* event)
	{
		pencilAction = PencilAction::PencilAction_Move;
		auto pos1 = touch->getLocation();
		log("moved1:%f,%f", pos1.x, pos1.y);
#ifdef drawCCSMousePath
		node->drawPoint(pos1, 1, color);  //原生自带的画点	
#endif
		drawPoint(pos1);
		//sendSnPos(pos1);  //发送原生自带的画点的坐标
		//node->drawCubicBezier(100, color);
		lastMousePos = pos1;
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(el, this);
	//dispatcher->addEventListenerWithSceneGraphPriority(el, this);


}

//发送给shader的pos,
//如果开启缓存模式，pos会被放入sendPosPool，然后每次update取一个发送到shader
void DrawLayer::sendSnPos(Vec2 pos)
{
	if(! SendPosToShader)
	{
		return;
	}
#ifndef UseSendPosPool
	auto sn = this->tsn;
	if (sn)
	{
		switch (shaderStrIdx)
		{
		case ShaderPencilIndex::Pencil1:
			dynamic_cast<SPencil1*>(sn)->setmousexy(pos.x, pos.y);
			break;
		case ShaderPencilIndex::Zone:
			dynamic_cast<SZone*>(sn)->setmousexy(pos.x, pos.y);
			break;
		case ShaderPencilIndex::PencilClr:
			dynamic_cast<SPencilClr*>(sn)->setmousexy(pos.x, pos.y);
			break;
		default:
			break;
		}
		
	}
	if (isGetFirstPoses) //第一次圈定区域的代码
	{
		firstPoses.push_back(pos);
	}
#ifdef UseRenderTexture
	updateRenderTexture();
#endif

#ifdef UseSpriteList
	addSpriteList(pos.x, pos.y);
#endif

#else
	//使用发送坐标缓存
	sendPosPool.push_back(pos);
	//
	if (isGetFirstPoses) //第一次圈定区域的代码
	{
		firstPoses.push_back(pos);
	}

#endif
	
}

//drawNow：是否立即就画
void DrawLayer::drawPoint(Vec2 pos, bool drawNow)
{
	//log("drawPoint:%f,%f", pos.x, pos.y);
	if (drawNow)
	{
		DrawNode* node = (DrawNode*)(this->getChildByTag(Tag_Node));
		if (node)
		{
			if(drawSendMousePath){
				node->drawPoint(pos, 2.0f, Color4F(1.0, 0.0, 1.0, 1.0)); //画出发送给shader的点		
			}

		}
		sendSnPos(pos);  //发送给shader
	}
	else
	{
		//1、atan（x）表示求的是x的反正切，其返回值为[-pi/2,+pi/2]之间的一个数。
		//2、atan2（y，x）求的是y / x的反正切，其返回值为[-pi, +pi]之间的一个数。
		//drawPoint(pos, true);
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


//////////////////////////////////
//判断封闭区域
//////////////////////////////////
double DrawLayer::direction(Vec2 p1, Vec2 p2, Vec2 p3)
{
	Vec2 d1 = Vec2(p3.x - p1.x, p3.y - p1.y);
	Vec2 d2 = Vec2(p2.x - p1.x, p2.y - p1.y);
	return d1.x*d2.y - d1.y*d2.x;
}

bool DrawLayer::OnSegment(Vec2 p1, Vec2 p2, Vec2 p3)
{
	double x_min, x_max, y_min, y_max;
	if (p1.x<p2.x){
		x_min = p1.x;
		x_max = p2.x;
	}
	else{
		x_min = p2.x;
		x_max = p1.x;
	}
	if (p1.y<p2.y){
		y_min = p1.y;
		y_max = p2.y;
	}
	else{
		y_min = p2.y;
		y_max = p1.y;
	}
	if (p3.x<x_min || p3.x>x_max || p3.y<y_min || p3.y>y_max)
		return false;
	else
		return true;
}

bool DrawLayer::SegmentIntersect(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4)
{
	double d1 = direction(p3, p4, p1);
	double d2 = direction(p3, p4, p2);
	double d3 = direction(p1, p2, p3);
	double d4 = direction(p1, p2, p4);

	if (d1*d2<0 && d3*d4<0)
		return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return true;
	else
		return false;
}

//把第一次的点全部画出来
void DrawLayer::drawFirstPosesLines()
{
	auto color = Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1);
	DrawNode* node = (DrawNode*)(this->getChildByTag(Tag_Node));
	if (firstPoses.size() >= 3)
	{
		for (int i = 0; i < firstPoses.size() - 1;i++)
		{
			node->drawLine(firstPoses[i], firstPoses[i + 1], color);
		}
		node->drawLine(firstPoses[firstPoses.size() - 1], firstPoses[0], color);
	}
}

//判断点是否在封闭区域内
void DrawLayer::checkPosInZone(Vec2 pos)
{
	auto size = Director::sharedDirector()->getWinSize();
	Vec2 line1pos1 = pos;
	Vec2 line1pos2 = Vec2(size.width, pos.y);
	Vec2 line2pos1 = Vec2(0, pos.y);
	Vec2 line2pos2 = pos;
	DrawNode* node = (DrawNode*)(this->getChildByTag(Tag_Node));
	int line1SecNum = 0;
	int line2SecNum = 0;
	if (firstPoses.size() >= 3)
	{
		for (int i = 0; i < firstPoses.size() - 1; i++)
		{
			if (SegmentIntersect(line1pos1, line1pos2, firstPoses[i], firstPoses[i + 1]))
			{
				line1SecNum++;
			}
		}
		if (SegmentIntersect(line1pos1, line1pos2, firstPoses[firstPoses.size() - 1], firstPoses[0]))
		{
			line1SecNum++;
		}

		for (int i = 0; i < firstPoses.size() - 1; i++)
		{
			if (SegmentIntersect(line2pos1, line2pos2, firstPoses[i], firstPoses[i + 1]))
			{
				line2SecNum++;
			}
		}
		if (SegmentIntersect(line2pos1, line2pos2, firstPoses[firstPoses.size() - 1], firstPoses[0]))
		{
			line2SecNum++;
		}
	}
	if (line1SecNum % 2 > 0 && line2SecNum % 2 > 0)
	{
		log("IN ZONE");
	}
	else
	{
		log("NOT IN ZONE");
	}
}

//beizer部分
void DrawLayer::startBeizer()
{
	float pointSize = 3.0f;

	//P0,P3,P1,P2分别表示 首末和中间2点
	Vec2 P0 = beizerPos[0];
	Vec2 P3 = beizerPos[1];
	Vec2 P1 = beizerPos[2];
	Vec2 P2 = beizerPos[3];
	//对称点
	UPoint UP0 = { P0.x, P0.y };  //俩段
	UPoint UP1 = { P3.x, P3.y };  //俩段
	UPoint UP2 = { P1.x, P1.y };
	UPoint UP21 = { P2.x, P2.y };
	UPoint FUP2 = sysPointByLine(UP0, UP1, UP2);
	UPoint FUP21 = sysPointByLine(UP0, UP1, UP21);
	Color4F color2 = Color4F(0, 0, 1, 1);
	P1 = Vec2(FUP2.x, FUP2.y);
	P2 = Vec2(FUP21.x, FUP21.y);
	//node->drawPoint(Vec2(FUP2.x, FUP2.y), pointSize, color2);
	//node->drawPoint(Vec2(FUP21.x, FUP21.y), pointSize, color2);

	std::vector<Vec2> tbeizerPoses = {};
	int num = 20;
	float grap = (float)1.0f / num;
	grap = float(int(grap * 100)) / 100;
	for (auto idx = 1; idx < num; idx++)
	{
		float x = MetaComputing(P0.x, P1.x, P2.x, P3.x, (float)(idx*grap));
		float y = MetaComputing(P0.y, P1.y, P2.y, P3.y, (float)(idx*grap));
		tbeizerPoses.push_back(Vec2(x,y));
	}
	Color4F color = Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1);
	DrawNode* node = (DrawNode*)this->getChildByTag(Tag_Node);
	for (auto pos:tbeizerPoses)
	{
		node->drawPoint(pos, 1, color);  //原生自带的画点
	}
	
	Color4F color1 = Color4F(1, 0, 0, 1);
	Color4F color11 = Color4F(0, 1, 0, 1);
	Color4F color111 = Color4F(0, 0, 1, 1);
	Color4F color1111 = Color4F(1, 1, 0, 1);
	node->drawPoint(P0, pointSize, color1);
	node->drawPoint(P1, pointSize, color11);
	node->drawPoint(P2, pointSize, color111);
	node->drawPoint(P3, pointSize, color1111);
	
}

//手动画完点后，自动在首末节点中间创建弧线
void DrawLayer::autoCreateBeizerPos()
{
	if (firstPoses.size() >= 3)
	{

		DrawNode* node = (DrawNode*)this->getChildByTag(Tag_Node);
		Vec2 endPos = firstPoses[firstPoses.size() - 1];
		Vec2 beginPos = firstPoses[0]; 
		float pointSize = 3.0f;
		//P0,P3,P1,P2分别表示 首末和中间2点
		Vec2 P0 = beginPos;
		Vec2 P3 = endPos;

		//计算中间2个点的算法
		float totalX = 0;
		float totalY = 0;
		float count = 0;
		//这个值会影响生成的弧度
		float num = int((firstPoses.size() - 1) / 5); 
		for (int i = 0; i < num;i++)
		{
			totalX += firstPoses[i].x;
			totalY += firstPoses[i].y;
			count++;
		}
		Vec2 P1 = { totalX / count, totalY / count };
		totalX = 0;
		totalY = 0;
		count = 0;
		for (int i = firstPoses.size() - num; i < firstPoses.size(); i++)
		{
			totalX += firstPoses[i].x;
			totalY += firstPoses[i].y;
			count++;
		}
		Vec2 P2 = { totalX / count, totalY / count };

		//测试上面的函数beizerPos
		//beizerPos.clear();
		//beizerPos.push_back();

		//对称点
		UPoint UP0 = { beginPos.x, beginPos.y };  //俩段
		UPoint UP1 = { endPos.x, endPos.y };  //俩段
		UPoint UP2 = { P1.x, P1.y };
		UPoint UP21 = { P2.x, P2.y };
		UPoint FUP2 = sysPointByLine(UP0, UP1, UP2);
		UPoint FUP21 = sysPointByLine(UP0, UP1, UP21);
		Color4F color2 = Color4F(0, 0, 1, 1);
		P1 = Vec2(FUP2.x, FUP2.y);
		P2 = Vec2(FUP21.x, FUP21.y);

		Color4F color0 = Color4F(0, 0, 1, 1);
		std::vector<Vec2> tbeizerPoses = {};
		int gnum = 30;
		float grap = (float)1.0f / gnum;
		grap = float(int(grap * 100)) / 100;
		for (auto idx = gnum - 1; idx >=0; idx-- )  //倒序插入才是正确的
		{
			float x = MetaComputing(beginPos.x, P1.x, P2.x, endPos.x, (float)(idx*grap));
			float y = MetaComputing(beginPos.y, P1.y, P2.y, endPos.y, (float)(idx*grap));
			Vec2 bPos = Vec2(x, y);
			tbeizerPoses.push_back(bPos);
			drawPoint(bPos,true);
			//node->drawPoint(bPos, pointSize, color0);
		}
		/*
		Color4F color1 = Color4F(1, 0, 0, 1);
		Color4F color11 = Color4F(0, 1, 0, 1);
		Color4F color111 = Color4F(0, 0, 1, 1);
		Color4F color1111 = Color4F(1, 1, 0, 1);
		node->drawPoint(P0, pointSize, color1);
		node->drawPoint(P1, pointSize, color11);
		node->drawPoint(P2, pointSize, color111);
		node->drawPoint(P3, pointSize, color1111);
		*/
	}
}

void DrawLayer::addSpriteList(float x,float y)
{
	Sprite* png = Sprite::create("Images/pencel2.png");
	png->setPosition(ccp(x,y));
	//right1
	auto glprogram1 = GLProgram::createWithFilenames("Shaders/example_MultiTexture.vsh", "Shaders/shadertoy_Draw_sprite1.fsh");
	auto glprogramstate1 = GLProgramState::getOrCreateWithGLProgram(glprogram1);
	png->setGLProgramState(glprogramstate1);


	this->addChild(png);
}

DrawLayer::~DrawLayer()
{
	if (rt != nullptr)
	{
		rt->release();
	}
	if (rt2 != nullptr)
	{
		rt2->release();
	}
	
	if (tsn != nullptr)
	{
		tsn->release();
	}
}

void DrawLayer::update(float dt)
{
	int sendPosPool_size_FirstLog = sendPosPool.size();
#ifdef UseSendPosPool
	if (sendPosPool.size() > 0)
	{
		//log("left sendPosPool size:%d", sendPosPool.size());
		
#ifdef UseSendPosPool_OnceAll
		auto sn = this->tsn;
		if (sn)
		{
			switch (shaderStrIdx)
			{
			case ShaderPencilIndex::Pencil1:
				dynamic_cast<SPencil1*>(sn)->setmousexys(sendPosPool);
				break;
			case ShaderPencilIndex::Zone:
				dynamic_cast<SZone*>(sn)->setmousexys(sendPosPool);
				break;
			case ShaderPencilIndex::PencilClr:
				dynamic_cast<SPencilClr*>(sn)->setmousexys(sendPosPool);
				break;
			default:
				break;
			}
		}
#else
		Vec2 pos = sendPosPool.at(0);
		sendPosPool.pop_front();
		auto sn = this->tsn;
		if (sn)
		{
			switch (shaderStrIdx)
			{
			case ShaderPencilIndex::Pencil1:
				dynamic_cast<SPencil1*>(sn)->setmousexy(pos.x, pos.y);
				break;
			case ShaderPencilIndex::Zone:
				dynamic_cast<SZone*>(sn)->setmousexy(pos.x, pos.y);
				break;
			case ShaderPencilIndex::PencilClr:
				dynamic_cast<SPencilClr*>(sn)->setmousexy(pos.x, pos.y);
				break;
			default:
				break;
			}
		}
		log("left sendPosPool1 size:%d", sendPosPool.size());
#endif //#ifdef UseSendPosPool_OnceAll
		
#ifdef UseRenderTexture
if (shaderStrIdx == ShaderPencilIndex::Zone)  //zong模式 不能每帧触发
{
	return;
}else if (shaderStrIdx == ShaderPencilIndex::Pencil1)  //
{
	updateRenderTexture();
}
else if (shaderStrIdx == ShaderPencilIndex::PencilClr)  //
{
	updateRenderTexture();
}
#endif //#ifdef UseRenderTexture

#ifdef UseSpriteList
		addSpriteList(pos.x, pos.y);
#endif //#ifdef UseSpriteList
		log("Pencil1:%d,%d,%d", pencilAction, sendPosPool_size_FirstLog, sendPosPool.size());
		if (shaderStrIdx == ShaderPencilIndex::Pencil1 && pencilAction == PencilAction::PencilAction_End)
		{
			if (sendPosPool_size_FirstLog > 0 && sendPosPool.size() == 0)
			{
				checkDrawPencil1Finish(); //有可能DrawEnd并非是触摸结束，而是sendPosPool缓存在之后的update里面被清空（相差一帧）
			}
		}
#endif //#ifdef UseSendPosPool
	}
}

//画笔一 mouseEnded时候,或者update sendPosPool为空的时候
void DrawLayer::pencil1DrawEnd()
{
	checkDrawPencil1Finish();
}

//判断是否已经一笔画结束了，条件：TouchEnd并且缓存没有了
void DrawLayer::checkDrawPencil1Finish()
{
	if (shaderStrIdx == ShaderPencilIndex::Pencil1 && pencilAction == PencilAction::PencilAction_End)
	{
		if ( sendPosPool.size() == 0)
		{
			drawPencil1Finish(); //有可能DrawEnd并非是触摸结束，而是sendPosPool缓存在之后的update里面被清空（相差一帧）
		}
	}
}

void DrawLayer::drawPencil1Finish()
{
	//做法1：从rt内容生成一个Sprite,显示到界面,同时抹除rt
	//打算做的：单独用一个TextureRender 给2个Sprite拍照混合，生成一个新的Texture2D->Sprite,即：每次混合2个Sprite到1个Sprite

	if (rt)
	{
		auto showingSprite = this->getChildByTag(Tag_RenderTextureSprite);
		if (showingSprite)
		{
			pencil1_spriteNum++;
			//从rt内容生成一个Sprite
			nimage = rt->newImage();
			newt = new Texture2D();
			newt->initWithImage(nimage);
			lastrtsp = Sprite::createWithTexture(newt);
			lastrtsp->setAnchorPoint(ccp(0, 0));
			//this->removeChildByTag(Tag_RenderTextureSprite);  //删除上一个，每次只显示一个 rt 的sprite
			this->addChild(lastrtsp, 0, Tag_RenderTextureSprite + pencil1_spriteNum);
			lastrtsp->setPosition(ccp(0, 0));

			//抹除rt
			rt->beginWithClear(0, 0, 0, 0);
			rt->end();
			//清除显示纹理
			this->removeChildByTag(Tag_RenderTextureSprite);  //删除用来显示的sprite
			log("remove Tag_RenderTextureSprite");
		}

	}
}

//rendertexture
void DrawLayer::updateRenderTexture()
{

	//思路：
	//1，用一个pencil(this->tsn)计算上次纹理和这次坐标的颜色混合，用一个texturerender(rt)显示画面和保存当前的纹理给sprite作为下次渲染用
	//2,pencil并不显示到界面，每次visit被动触发，texturerender 有显示到界面，相当于：rt每次调用tsn->visit(),把visit内容映射到界面上
	//3,由于texturerender显示带透明的的有问题，因此让texturerender visible = false,每次基于texturerender生成一个sprite用来显示到界面
	//4,同时将3生成的sprite的纹理作为u_texture2参数发给pencil,以便下一帧混合使用
	//5,u_texture2的作用是：将连贯笔画的轨迹内容用纹理形式保存下来，供新的点进行混合
	//流程：每次tsn->visit基于纹理A为背景绘制新的几个或者一个坐标点，之后把整个纹理保存为A纹理进行下一轮鼠标滑动时候渲染使用
	//其中整个纹理保存为A纹理是难点，目前使用rendertexture的思路，
	//渲染流程，1:>tsn->visit,2:rt 拍照后显示到屏幕，3：将rt拍的照片的texture赋值给tsn的u_texture2 ， 4：tsn的shader代码进行颜色混合，5返回第一步
	//现在问题是rt->begin tsn->visit rt->end的效果和rt->getSprite()的效果不同,也不是第四步代码想要的效果
	//注：tsn并没有被addChild到界面上，是想通过visit() 显示在rt上面
	//beginWithClear , clear + begin , begin 3种情况效果不同
	//理论上每次都要 clear + begin ，然后tsn visit,然后获取tsn的纹理显示到屏幕上，再把纹理赋值给tsn的u_texture2,然后循环
	//现象是：
	//1,beginWithClear后rt有像素，tsn->visit后rt的getSprite有像素，但效果不同。
	//2,clear + begin后rt和tsn->visit后rt的getSprite都无像素(???是否跟第一步无像素有关系)。
	//,clear+begin模式还不行
	//做法1:tsn负责渲染，不addchild, rt只负责拍照 addchild,visible(false),每次rt->beginWithClear(0, 0, 0, 0);
	//拍照后新建一个Sprite拷贝rt内容，显示到界面上，下一帧新建之前先删除旧的显示。
	//做法2：rt又负责拍照又负责显示，addchild,visible(true),每次rt->begin（）。这种模式有问题，透明度不对，rt->getSprite()Y轴也是反的
	//新的思路(未实现)：
	//每次连贯的笔画都生成一张Sprite,结束后和之前的Sprite进行混合。即：有2张Sprite,每次画上面一张内容，底下那张Sprite不动，结束后，进行上下混合，合并到底下那张。
	//存在的问题：rt直接显示内容和rt生成的sprite内容透明度不同，sprite更加真实，目前用这种；另外每次下笔第一帧有延迟上一次最后一次坐标
	//目前采用：做法1
	if (this->tsn)
	{
		auto size = Director::sharedDirector()->getWinSize();
		if (rt == nullptr)
		{
			rt = CCRenderTexture::create(size.width, size.height);
			rt->retain();


			//CCSprite* sp = CCSprite::create("Images/background.png");
			//rt->begin();
			//sp->visit();
			//rt->end();
			//this->addChild(sp);

			this->addChild(rt, 1, Tag_RenderTexture);
			//rt->setOpacity(0.3);
			//CCRenderTexture 只负责绘制内容，本身并不用来显示
			
			//BlendFunc bf = { GL_ZERO, GL_ONE };
			//rt->getSprite()->setBlendFunc(bf);
			//GL_ZERO, GL_DST_ALPHA  GL_SRC_ALPHA
			//GL_ONE, GL_ZERO 有颜色 周边部分是黑的
			//GL_DST_ALPHA, GL_ZERO 有半透明颜色 周边是黑的
			//BlendFunc bf = { GL_DST_ALPHA, GL_ZERO };  //GL_ONE, GL_ONE_MINUS_SRC_ALPHA 
			//rt->getSprite()->setBlendFunc(bf);
			//rt2 = CCRenderTexture::create(size.width, size.height);
			//rt2->retain();

		}
		//CCRenderTexture * rt = CCRenderTexture::create(size.width, size.height);
		rt->setPosition(ccp(size.width / 2, size.height / 2));
		rt->setVisible(true);
		//传递当前已经渲染的部分,当前纹理
		switch (shaderStrIdx)
		{
		case ShaderPencilIndex::Pencil1:
		{

		 //rt->getSprite()->getTexture() ,lastrtsp
			//dynamic_cast<SPencil1*>(this->tsn)->setShaderTexture("u_texture2", lastrtsp->getTexture());
			//sp->getTexture();
			//测试切换u_texture2
			//结果的纹理可以实时动态切换
			 //之前不行是纹理映射不对
			/*auto spr_background = Sprite::create("cocosui/background.png");
			auto orange_edit = Sprite::create("Images/CyanSquare.png");
			auto green_edit = Sprite::create("Images/background.png");
			updateidx++;
			if (updateidx % 3 == 0)
			{
				dynamic_cast<SPencil1*>(this->tsn)->setShaderTexture("u_texture2", green_edit->getTexture());
			}
			else if (updateidx % 3 == 1)
			{
				dynamic_cast<SPencil1*>(this->tsn)->setShaderTexture("u_texture2", green_edit->getTexture());
			}
			else
			{
				dynamic_cast<SPencil1*>(this->tsn)->setShaderTexture("u_texture2", green_edit->getTexture());
			}*/
		
			if (lastrtsp)
			{
				//lastrtsp->release();
			}
			
			rt->setVisible(false);
			//做法1：从rt内容生成一个Sprite,显示到界面
			nimage = rt->newImage(); //默认翻转
			newt = new Texture2D();
			newt->initWithImage(nimage);
			lastrtsp = Sprite::createWithTexture(newt); //newt
			lastrtsp->setAnchorPoint(ccp(0, 0));
			this->removeChildByTag(Tag_RenderTextureSprite);  //删除上一个，每次只显示一个 rt 的sprite
			this->addChild(lastrtsp, 0, Tag_RenderTextureSprite);
			lastrtsp->setPosition(ccp(0,0));  
			log("add Tag_RenderTextureSprite");
			
			/*lastrtsp = Sprite::createWithSpriteFrame(rt->getSprite()->displayFrame());
			lastrtsp->setAnchorPoint(ccp(0, 0));
			this->removeChildByTag(Tag_RenderTextureSprite);
			this->addChild(lastrtsp, 0, Tag_RenderTextureSprite);*/

			if (lastrtsp)
			{
				dynamic_cast<SPencil1*>(this->tsn)->setShaderTexture("u_texture2", lastrtsp->getTexture());  //lastrtsp->getTexture()
			}
			else
			{
				dynamic_cast<SPencil1*>(this->tsn)->setShaderTexture("u_texture2", rt->getSprite()->getTexture());
				log("never have  lastspr");
			}
			//if (this->tsn->getParent() == nullptr)
			//{
			//	this->addChild(this->tsn);
			//}

		}
			break;
		case ShaderPencilIndex::Zone:
			dynamic_cast<SZone*>(this->tsn)->setShaderTexture("u_texture2", rt->getSprite()->getTexture());
			break;
		case ShaderPencilIndex::PencilClr:
		{
			newt = new Texture2D();
			nimage = rt->newImage();
			newt->initWithImage(nimage);
			lastrtsp = Sprite::createWithTexture(newt);
			lastrtsp->setAnchorPoint(ccp(0, 0));

			dynamic_cast<SPencilClr*>(this->tsn)->setShaderTexture("u_texture2", lastrtsp->getTexture());  //rt->getSprite()->getTexture()
			//rt->clear(0, 0, 0, 0);
		}
			break;
		default:
			break;
		}
		//blendfunc 1
		auto pencil = (dynamic_cast<Sprite*>(this->tsn));
		if (pencil)
		{
			ccBlendFunc originalBlend = pencil->getBlendFunc();
			//GL_ZERO, GL_ONE //正常
			//BlendFunc::ALPHA_NON_PREMULTIPLIED = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
			ccBlendFunc func = { GL_ONE, GL_ZERO }; //GL_SRC_ALPHA, GL_ONE,GL_ZERO
			pencil->setBlendFunc(func);
		}
		

		rt->beginWithClear(0, 0, 0, 0);
		//rt->clear(0, 0, 0, 0);
		//rt->begin();

		//if (lastrtsp)
			//lastrtsp->visit();
		//log("visit");
		this->tsn->visit();
		//log("visit end");
		//this->removeChildByTag(500);
		//this->addChild(sp, 10, 500);
		//orange_edit
		/*auto spr = Sprite::create("cocosui/orange_edit.png");
		spr->setPosition(ccp(200,200));
		spr->setOpacity(10);
		spr->visit();*/

		rt->end();

		if (nimage)
		{
			nimage->release();
		}
		if (newt)
		{
			newt->release();
		}

		//blendfunc 2
		//((Sprite*)(this->tsn))->setBlendFunc(originalBlend);


		//if (lastrtsp) lastrtsp->release();
		//if (newt) delete newt;
		//if (nimage) delete nimage;
		//this->addChild(lastrtsp);

		//rt2->begin();
		//this->tsn->visit();
		//rt2->end();

		//if (dynamic_cast<SPencil1*>(this->tsn))
		//{
		//	dynamic_cast<SPencil1*>(this->tsn)->clearAllMouseXY();
		//}

		/*
		auto s = rt->getSprite();
		auto scpy = Sprite::createWithTexture(s->getTexture());
		scpy->setFlipY(true);
		scpy->setAnchorPoint(ccp(0, 0));
		*/
		log("updateTextureRender end");
	}

}

//问题：
//为何rt颜色和addChild sprite之后颜色不同（alpha<1的情况）