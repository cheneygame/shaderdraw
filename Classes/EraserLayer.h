#ifndef __EraserLayer_H__
#define __EraserLayer_H__

#include "cocos2d.h"
#include "DrawLayer.h"
//#include "../../../extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GlobalDef.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class EraserLayer :public Layer
{

public:
	CREATE_FUNC(EraserLayer);
	virtual bool init();
	~EraserLayer();

protected:

private:
	Sprite* eraserPencil;    //ÏðÆ¤  
	RenderTexture* rTex;    //»­²¼ 
	Sprite* spriteBG;
};

#endif