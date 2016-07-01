#include "Utility.h"
//直线斜率公式:k = (y2 - y1) / (x2 - x1)
//两条垂直相交直线的斜率相乘积为 - 1：k1*k2 = -1.
//y = kx + b
//y = k1x + b1
//0 = (k1 - k )x + (b1 - b);
//p0,p1,p3表示直线
//p2,p3,p4是在一条线
UPoint sysPointByLine(UPoint p0, UPoint p1, UPoint p2)
{
	float k = (p1.y - p0.y) / (p1.x - p0.x);
	float b = p0.y - k*p0.x;
	float k1 = -1 / k;
	float b1 = p2.y - k1*p2.x;
	UPoint p3 = { 0,0 }; //中点
	p3.x = (b - b1) / (k1 - k);
	p3.y = k1*p3.x + b1;
	UPoint p4 = { 0, 0 }; //目标点
	//float left = p2.x*p2.x - 2 * p2.x*p3.x + p2.y*p2.y - 2 * p2.y*p3.y;
	//float right = p4.x*p4.x - 2 * p4.x*p3.x + p4.y*p4.y - 2 * p4.y*p3.y;
	UPoint p23 = { p2.x - p3.x, p2.y - p3.y };
	p4 = { p3.x - p23.x, p3.y - p23.y };
	return p4;
}

float MetaComputing(float p0, float p1, float p2, float p3, float t)
{
	// 方法一:  
	float a, b, c;
	float tSquare, tCube;
	// 计算多项式系数    
	c = 3.0 * (p1 - p0);
	b = 3.0 * (p2 - p1) - c;
	a = p3 - b - c - p0;

	// 计算t位置的点   
	tSquare = t * t;
	tCube = t * tSquare;
	return (a * tCube) + (b * tSquare) + (c * t) + p0;

	// 方法二: 原始的三次方公式  
	//  float n = 1.0 - t;  
	//  return n*n*n*p0 + 3.0*p1*t*n*n + 3.0*p2*t*t*n + p3*t*t*t;  
};