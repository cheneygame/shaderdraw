#pragma once
#ifndef Utility_H
#define Utility_H

struct UPoint
{
	float x;
	float y;
};
//p0,p1��ʾֱ�� ,p2��ʾĿ���෴����
UPoint sysPointByLine(UPoint p0, UPoint p1, UPoint p2);
float MetaComputing(float p0, float p1, float p2, float p3, float t);
#endif
