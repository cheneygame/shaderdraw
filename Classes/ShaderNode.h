
#ifndef _ShaderNode_H_
#define _ShaderNode_H_
#include "ui/CocosGUI.h"

#include "cocos2d.h"

USING_NS_CC;
//USING_NS_CC_EXT;

class ShaderNode : public Node
{
public:
	static ShaderNode* shaderNodeWithVertex(const std::string &vert, const std::string &frag);

	virtual void update(float dt);
	virtual void setPosition(const Vec2 &newPosition);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void setmousexy(float mx_, float my_){ mx = mx_; my = my_; pushmousexy(mx, my); }
	void pushmousexy(float mx_, float my_);
protected:
	ShaderNode();
	~ShaderNode();

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
	static const int maxLen = 1024 * 2;
	GLfloat pos[maxLen];
};
#endif