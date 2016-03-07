#ifndef __DrawLayer_H__
#define __DrawLayer_H__

#include "cocos2d.h"
USING_NS_CC;

class DrawLayer:public Layer
{
	enum Dir{
		Right = 1,
		Right_UP,
		Up,
		Up_Left,
		Left,
		Left_Down,
		Down,
		Down_Right,
	};
public:
	CREATE_FUNC(DrawLayer);
	virtual bool init();
protected:
	void drawDemo();
	void drawShader();
	void drawPoint(Vec2 pos,bool draw = false);
	void drawMatrix();
	void drawByDir(int dir, Vec2 mousepos);
	void drawByAngle( Vec2 mousepos); //根据任意角度画平均分布的点
	void drawByFixLen(Vec2 mousepos);
	void sendSnPos(Vec2 pos);
	/*virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);*/

	const float cellw = 6.0f;
	const float cellh = cellw;
	std::set<Vec2> poses; //存放idx,网格模式用的
	Vec2 lastidx;  //上次点击的idx,网格模式用的
	Vec2 lastDrawPos;  //上次画的位置
	Vec2 lastMousePos; //上次鼠标移动位置
	Vec2 lastMousePosWhenDraw; //上次画点的时候鼠标移动位置
	float posLen = 10.0f;
	float minMouseLen = 10.0f;  //鼠标最小移动长度
};

#endif