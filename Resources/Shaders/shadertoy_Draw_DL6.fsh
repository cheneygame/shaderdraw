//判断封闭区域
uniform vec2 center;
uniform vec2 resolution;
uniform int poslen;
uniform float pos[2048];  //1024*128 = 131072
uniform int zoneposlen; 
uniform float zonepos[2048]; 

uniform vec2 mouse;
uniform sampler2D u_texture1;
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

vec2   icenter = center;
vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
//vec4      iMouse = vec4(0,0,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click
vec4   iMouse = vec4(mouse.x,mouse.y,0,0);  

// Line Segment (bouncing) written 2015 by Jakob Thomsen

// mirror/bounce inside -1,+1
vec2 mirror(vec2 pos)
{
    return (2.0 * abs(2.0 * fract(pos) - 1.0) - 1.0);
}

float PointLineAlong2d(vec2 a, vec2 n, vec2 p)
{
    return dot(p - a, n) / dot(n, n);
}

float PointLineDist2d(vec2 a, vec2 n, vec2 p)
{
    //return length(p - (a + n * dot(p - a, n) / dot(n, n)));
    return length(p - (a + n * PointLineAlong2d(a, n, p)));
}

float PointLineSegDist2d(vec2 a, vec2 n, vec2 p)
{
    float q = PointLineAlong2d(a, n, p);
    return length(p - (a + n * clamp(q, 0., 1.)));
}

#define minw 0.003
#define maxw 0.03
#define subwspeed 0.0005
float countV()
{
	float width = maxw;
	vec2 I = vec2(gl_FragCoord.x,gl_FragCoord.y);
	vec2 npos = vec2(gl_FragCoord.x,gl_FragCoord.y);
    I = 2. * I / iResolution.xy - 1.;
	int posnum = poslen/2; //有几个pos
	float v = 0.0;
	bool found = false;
	for(int i = 0;i < posnum - 1;i++)
	{
		width = maxw - subwspeed*i;
		width = max(width,minw);
		int idx1 = i*2;
		vec2 a = vec2(pos[idx1], pos[idx1 + 1]);
		vec2 b = vec2(pos[idx1 + 2], pos[idx1 + 3]);
		
		float d = PointLineSegDist2d(a, b - a, I);
		//float tv = 1.0 - clamp(d,0.0, width);
		float tv = 1.0 - smoothstep(0.0, width, d); //step(width, d);
		//float tv = 1.0 - step(width, d);
		v = max(v, tv);
		/*
		float len1 = distance(npos , a);  //distance length
		float len2 = distance(npos , b);
		if(len1 + len2 < 3.0f)
		{
			v = 1.0;
			break;
		}
		*/
	}
	return v;
}

//////////////////////////////////
//判断封闭区域
//////////////////////////////////
float direction(vec2 p1, vec2 p2, vec2 p3)
{
	vec2 d1 = vec2(p3.x - p1.x, p3.y - p1.y);
	vec2 d2 = vec2(p2.x - p1.x, p2.y - p1.y);
	return d1.x*d2.y - d1.y*d2.x;
}

bool OnSegment(vec2 p1, vec2 p2, vec2 p3)
{
	float x_min, x_max, y_min, y_max;
	if (p1.x<p2.x){
		x_min = p1.x;
		x_max = p2.x;
	}
	else{
		x_min = p2.x;
		x_max = p1.x;
	}
	if (p1.y<p2.y){
		y_min = p1.y;
		y_max = p2.y;
	}
	else{
		y_min = p2.y;
		y_max = p1.y;
	}
	if (p3.x<x_min || p3.x>x_max || p3.y<y_min || p3.y>y_max)
		return false;
	return true;
}

bool SegmentIntersect(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
	float d1 = direction(p3, p4, p1);
	float d2 = direction(p3, p4, p2);
	float d3 = direction(p1, p2, p3);
	float d4 = direction(p1, p2, p4);

	if (d1*d2<0 && d3*d4<0)
		return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return true;
	return false;
}

vec2 getzoneposVec2(int idx)
{
	return vec2(zonepos[idx*2],zonepos[idx*2 + 1]);
}

bool checkPosInZone(vec2 pos)
{
	vec2 size = iResolution.xy;
	vec2 line1pos1 = pos;
	vec2 line1pos2 = vec2(size.x, pos.y);
	vec2 line2pos1 = vec2(0, pos.y);
	vec2 line2pos2 = pos;

	int line1SecNum = 0;
	int line2SecNum = 0;
	if (zoneposlen >= 3)
	{
		for (int i = 0; i < zoneposlen - 1; i++)
		{
			if (SegmentIntersect(line1pos1, line1pos2, getzoneposVec2(i), getzoneposVec2(i + 1)) )  //zonepos[i], zonepos[i + 1])
			{
				line1SecNum++;
			}
		}
		if (SegmentIntersect(line1pos1, line1pos2, getzoneposVec2(zoneposlen-1), getzoneposVec2(0)))  //zonepos[zoneposlen - 1], zonepos[0]
		{
			line1SecNum++;
		}

		for (int i = 0; i < zoneposlen - 1; i++)
		{
			if (SegmentIntersect(line2pos1, line2pos2, getzoneposVec2(i), getzoneposVec2(i + 1) ))
			{
				line2SecNum++;
			}
		}
		if (SegmentIntersect(line2pos1, line2pos2,getzoneposVec2(zoneposlen-1), getzoneposVec2(0)))
		{
			line2SecNum++;
		}
	}
	if (line1SecNum % 2 > 0 && line2SecNum % 2 > 0)
	{
		//log("IN ZONE");
		return true;
	}
	return false;
}

//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{

	vec2 curpos = vec2(gl_FragCoord.x,gl_FragCoord.y);
	float v = 0;
	if(checkPosInZone(curpos))
	{
		v = 1;
	}
	gl_FragColor = vec4(v,v,v,1.0);
}
