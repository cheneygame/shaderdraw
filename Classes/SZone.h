
#ifndef _SZone_H_
#define _SZone_H_
#include "ui/CocosGUI.h"

#include "cocos2d.h"

USING_NS_CC;
//USING_NS_CC_EXT;
#define OnlySendOneTime  //只发送一次就清除
class SZone : public Node
{
public:
	CREATE_FUNC(SZone);
	static SZone* SZoneWithVertex(const std::string &vert, const std::string &frag);

	virtual void update(float dt);
	virtual void setPosition(const Vec2 &newPosition);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void setmousexy(float mx_, float my_){ mx = mx_; my = my_; pushmousexy(mx, my); }
	void setmousexys(std::deque<Vec2>& pool);
	void setzonepos(const std::vector<Vec2>& param);
	void pushmousexy(float mx_, float my_);
	void clearAllMouseXY();
	void setShaderTexture(const std::string& name, Texture2D* texture);

	void setBrushCF(Color4F color){ brushColorF = color; };
protected:
	SZone();
	~SZone();

	bool initWithVertex(const std::string &vert, const std::string &frag);
	void loadShaderVertex(const std::string &vert, const std::string &frag);

	void onDraw(const Mat4 &transform, uint32_t flags);

	Vec2 _center;
	Vec2 _resolution;
	float      _time;
	std::string _vertFileName;
	std::string _fragFileName;
	CustomCommand _customCommand;
	float mx, my;
	int pushidx = 0;
	static const int maxLen = 1024 * 1;
	GLfloat pos[maxLen];  //pos里面坐标是有特殊计算过的(x/hx)/hx   0-1之前的

	static const int zpmaxLen = 1024 * 1;
	GLfloat zonepos[zpmaxLen];  //zonepos是实际坐标，0-width之间
	int zoneposlen = 0;

	Color4F brushColorF = Color4F::BLUE;// Color4F::MAGENTA;
};
#endif