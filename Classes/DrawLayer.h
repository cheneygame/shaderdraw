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
	void drawByAngle( Vec2 mousepos); //��������ǶȻ�ƽ���ֲ��ĵ�
	void drawByFixLen(Vec2 mousepos);
	void sendSnPos(Vec2 pos);
	/*virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);*/

	const float cellw = 6.0f;
	const float cellh = cellw;
	std::set<Vec2> poses; //���idx,����ģʽ�õ�
	Vec2 lastidx;  //�ϴε����idx,����ģʽ�õ�
	Vec2 lastDrawPos;  //�ϴλ���λ��
	Vec2 lastMousePos; //�ϴ�����ƶ�λ��
	Vec2 lastMousePosWhenDraw; //�ϴλ����ʱ������ƶ�λ��
	float posLen = 10.0f;
	float minMouseLen = 10.0f;  //�����С�ƶ�����
};

#endif