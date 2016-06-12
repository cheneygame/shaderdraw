#ifndef __DrawLayer_H__
#define __DrawLayer_H__

#include "cocos2d.h"
#include "ShaderNode.h"
#include"SPencil1.h"
#include"SZone.h"
#include"SPencilClr.h"
USING_NS_CC;

//#define BeiZerTest 1  //������ʾ����berzer���飬���4����
#define ZoneCode  //���Ʒ������ģʽ
//#define drawCCSMousePath  //����cocos����
#define UseRenderTexture  //ʹ��RenderTexture
//#define UseSpriteList  //ʹ��UseSpriteList
//#define ShowShaderLayer  //��ʾshader��
//#define drawSendMousePath  //�����Ż���ĵ�,����shader���ƽ����
#define AvgPtLen 1.0f  //�����Ż���ĵ��࣬����ˢ�ܶ�
#define UseSendPosPool //ʹ�÷��������,���������ͣ��û���
#define UseSendPosPool_OnceAll  //һ�η��ͳ���ȫ������
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
	enum ShaderPencilIndex{
		Pencil1 = 0,
		Zone,
		PencilClr,
	};
public:
	CREATE_FUNC(DrawLayer);
	virtual bool init();
	~DrawLayer();
	void setBrushCF(Color4F brushColorF)
	{ 
		pencil1->setBrushCF(brushColorF); 
		szone->setBrushCF(brushColorF);
	};

	void setPencilIdx(int idx)
	{
		shaderStrIdx = (ShaderPencilIndex)(idx - 1);
	};

	
protected:
	virtual void update(float dt);
	void initShaderPencils();
	void drawNode();
	void drawShader();
	void addUI();
	void addShaderNode();
	void removeShaderNode();
	void drawPoint(Vec2 pos,bool draw = false);
	void drawMatrix();
	void drawByDir(int dir, Vec2 mousepos);
	void drawByAngle( Vec2 mousepos); //��������ǶȻ�ƽ���ֲ��ĵ�
	void drawByFixLen(Vec2 mousepos);
	void sendSnPos(Vec2 pos);
	void addSpriteList(float x, float y);
	void resetShaderHandler();
	/*virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);*/
	void menuCallback(Ref* sender);
	const float cellw = 6.0f;
	const float cellh = cellw;
	std::set<Vec2> poses; //���idx,����ģʽ�õ�
	Vec2 lastidx;  //�ϴε����idx,����ģʽ�õ�
	Vec2 lastDrawPos;  //�ϴλ���λ��
	Vec2 lastMousePos; //�ϴ�����ƶ�λ��
	Vec2 lastMousePosWhenDraw; //�ϴλ����ʱ������ƶ�λ��
	float posLen = AvgPtLen;  //���ƽ���ֲ����
	float minMouseLen = 10.0f;  //�����С�ƶ�����

	//������������
	bool isGetFirstPoses = true;  //�Ƿ�Ȧ������һ�����귶Χ
	std::vector<Vec2> firstPoses;
	int lineIdx = 0;
	std::vector<Vec2> linePos;
	//beizer����
	std::vector<Vec2> beizerPos;
	std::deque<Vec2> sendPosPool;
private:
	//������������
	double direction(Vec2 p1, Vec2 p2, Vec2 p3);
	bool OnSegment(Vec2 p1, Vec2 p2, Vec2 p3);
	bool SegmentIntersect(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4);
	void drawFirstPosesLines();
	void checkPosInZone(Vec2 pos);
	//beizer����
	void startBeizer();
	void autoCreateBeizerPos();
	void updateRenderTexture();
	Node* tsn = nullptr;
	CCRenderTexture * rt = nullptr;
	CCRenderTexture * rt2 = nullptr;
	ShaderPencilIndex shaderStrIdx = Pencil1;  //Zone Pencil1

	SPencil1* pencil1 = nullptr;
	SPencilClr*	pencilClr = nullptr;
	SZone*	szone = nullptr;
	Sprite* lastrtsp = nullptr;
	Color4F brushColorF = Color4F::BLUE;
	int updateidx = 0;
	Texture2D* newt = nullptr;
	Image* nimage = nullptr;
};

#endif