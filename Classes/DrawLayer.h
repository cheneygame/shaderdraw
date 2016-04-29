#ifndef __DrawLayer_H__
#define __DrawLayer_H__

#include "cocos2d.h"
#include "ShaderNode.h"
USING_NS_CC;

//#define BeiZerTest 1  //开启表示测试berzer数组，点击4个点
//#define ZoneCode  //绘制封闭区域模式
//#define drawCCSMousePath  //绘制cocos鼠标点
#define UseRenderTexture  //使用RenderTexture
//#define UseSpriteList  //使用UseSpriteList
#define ShowShaderLayer  //显示shader层
//#define drawSendMousePath  //绘制优化完的点,发给shader层的平均点
#define AvgPtLen 2.0f  //绘制优化完的点间距，即笔刷密度
#define UseSendPosPool //使用发送坐标池,不立即发送，用缓存
#define UseSendPosPool_OnceAll  //一次发送池内全部内容
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
	~DrawLayer();
	
protected:
	virtual void update(float dt);
	void drawNode();
	void drawShader();
	void drawPoint(Vec2 pos,bool draw = false);
	void drawMatrix();
	void drawByDir(int dir, Vec2 mousepos);
	void drawByAngle( Vec2 mousepos); //根据任意角度画平均分布的点
	void drawByFixLen(Vec2 mousepos);
	void sendSnPos(Vec2 pos);
	void addSpriteList(float x, float y);
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
	float posLen = AvgPtLen;  //点的平均分布间距
	float minMouseLen = 10.0f;  //鼠标最小移动长度

	//计算封闭区域用
	bool isGetFirstPoses = true;  //是否圈定过第一个坐标范围
	std::vector<Vec2> firstPoses;
	int lineIdx = 0;
	std::vector<Vec2> linePos;
	//beizer部分
	std::vector<Vec2> beizerPos;
	std::deque<Vec2> sendPosPool;
private:
	//计算封闭区域用
	double direction(Vec2 p1, Vec2 p2, Vec2 p3);
	bool OnSegment(Vec2 p1, Vec2 p2, Vec2 p3);
	bool SegmentIntersect(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4);
	void drawFirstPosesLines();
	void checkPosInZone(Vec2 pos);
	//beizer部分
	void startBeizer();
	void autoCreateBeizerPos();
	void updateRenderTexture();
	ShaderNode* tsn = nullptr;
	CCRenderTexture * rt = nullptr;
};

#endif