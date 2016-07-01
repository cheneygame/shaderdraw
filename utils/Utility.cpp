#include "Utility.h"
//ֱ��б�ʹ�ʽ:k = (y2 - y1) / (x2 - x1)
//������ֱ�ֱཻ�ߵ�б����˻�Ϊ - 1��k1*k2 = -1.
//y = kx + b
//y = k1x + b1
//0 = (k1 - k )x + (b1 - b);
//p0,p1,p3��ʾֱ��
//p2,p3,p4����һ����
UPoint sysPointByLine(UPoint p0, UPoint p1, UPoint p2)
{
	float k = (p1.y - p0.y) / (p1.x - p0.x);
	float b = p0.y - k*p0.x;
	float k1 = -1 / k;
	float b1 = p2.y - k1*p2.x;
	UPoint p3 = { 0,0 }; //�е�
	p3.x = (b - b1) / (k1 - k);
	p3.y = k1*p3.x + b1;
	UPoint p4 = { 0, 0 }; //Ŀ���
	//float left = p2.x*p2.x - 2 * p2.x*p3.x + p2.y*p2.y - 2 * p2.y*p3.y;
	//float right = p4.x*p4.x - 2 * p4.x*p3.x + p4.y*p4.y - 2 * p4.y*p3.y;
	UPoint p23 = { p2.x - p3.x, p2.y - p3.y };
	p4 = { p3.x - p23.x, p3.y - p23.y };
	return p4;
}

float MetaComputing(float p0, float p1, float p2, float p3, float t)
{
	// ����һ:  
	float a, b, c;
	float tSquare, tCube;
	// �������ʽϵ��    
	c = 3.0 * (p1 - p0);
	b = 3.0 * (p2 - p1) - c;
	a = p3 - b - c - p0;

	// ����tλ�õĵ�   
	tSquare = t * t;
	tCube = t * tSquare;
	return (a * tCube) + (b * tSquare) + (c * t) + p0;

	// ������: ԭʼ�����η���ʽ  
	//  float n = 1.0 - t;  
	//  return n*n*n*p0 + 3.0*p1*t*n*n + 3.0*p2*t*t*n + p3*t*t*t;  
};