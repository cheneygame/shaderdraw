#ifndef __Global_H__
#define __Global_H__
#pragma once

#include "cocos2d.h"

USING_NS_CC;

static const int colorFListLen = 6;
// 这个不用static 会提示多重定义
static Color4F colorFList[colorFListLen] = { Color4F::RED, Color4F::GREEN, Color4F::BLUE, Color4F::YELLOW, Color4F::MAGENTA, Color4F::BLACK };

#endif