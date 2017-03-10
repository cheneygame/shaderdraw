#ifndef __TestLayer_H__
#define __TestLayer_H__

#include "cocos2d.h"
//#include "../../../extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GlobalDef.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class TestLayer :public Layer
{
	
public:
	CREATE_FUNC(TestLayer);
	virtual bool init();
	~TestLayer();

};

#endif
