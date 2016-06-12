#ifndef __MainLayer_H__
#define __MainLayer_H__

#include "cocos2d.h"
#include "DrawLayer.h"
#include "../../../extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GlobalDef.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class MainLayer :public Layer
{
	
public:
	CREATE_FUNC(MainLayer);
	virtual bool init();
	~MainLayer();

protected:
	//virtual void update(float dt);
	void addPencilScroll();
	void addColorScroll();
	void dnw_cb(Ref* ref, Widget::TouchEventType type);
	void dnw_idx(Ref* ref, Widget::TouchEventType type);
private:
	DrawLayer* drawLayer = nullptr;
};

#endif