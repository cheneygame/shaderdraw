//带旋转角度的矩形队列
uniform vec2 center;
uniform vec2 resolution;
uniform int poslen;
uniform float pos[2048];  //1024*128 = 131072

//uniform int poslen = 250;
//uniform float pos[250] = {0.53,0.37,0.54,0.34,0.55,0.31,0.56,0.29,0.56,0.26,0.57,0.23,0.58,0.20,0.59,0.17,0.59,0.14,0.59,0.11,0.60,0.08,0.60,0.05,0.60,0.02,0.60,-0.02,0.59,-0.05,0.58,-0.07,0.57,-0.10,0.56,-0.13,0.55,-0.16,0.54,-0.18,0.52,-0.20,0.51,-0.22,0.49,-0.24,0.47,-0.25,0.45,-0.26,0.43,-0.28,0.41,-0.28,0.39,-0.29,0.37,-0.29,0.35,-0.30,0.33,-0.30,0.31,-0.31,0.29,-0.31,0.27,-0.32,0.25,-0.32,0.23,-0.32,0.21,-0.32,0.19,-0.32,0.17,-0.32,0.15,-0.32,0.13,-0.31,0.11,-0.30,0.09,-0.29,0.07,-0.27,0.06,-0.24,0.05,-0.22,0.03,-0.19,0.02,-0.17,0.01,-0.14,0.01,-0.11,-0.00,-0.08,-0.01,-0.05,-0.02,-0.02,-0.03,0.01,-0.04,0.03,-0.04,0.06,-0.05,0.09,-0.05,0.12,-0.06,0.15,-0.06,0.18,-0.06,0.22,-0.06,0.25,-0.06,0.28,-0.06,0.29,-0.08,0.29,-0.10,0.30,-0.12,0.30,-0.15,0.30,-0.17,0.30,-0.19,0.30,-0.21,0.29,-0.23,0.28,-0.25,0.27,-0.27,0.26,-0.28,0.24,-0.30,0.23,-0.32,0.22,-0.34,0.21,-0.36,0.20,-0.38,0.18,-0.40,0.17,-0.41,0.15,-0.43,0.12,-0.44,0.10,-0.45,0.07,-0.46,0.04,-0.47,0.02,-0.48,-0.01,-0.50,-0.03,-0.51,-0.05,-0.53,-0.07,-0.55,-0.08,-0.57,-0.10,-0.58,-0.11,-0.60,-0.13,-0.62,-0.15,-0.62,-0.18,-0.62,-0.22,-0.61,-0.24,-0.60,-0.27,-0.58,-0.28,-0.56,-0.30,-0.54,-0.31,-0.52,-0.32,-0.50,-0.32,-0.48,-0.33,-0.46,-0.34,-0.44,-0.34,-0.42,-0.34,-0.40,-0.34,-0.38,-0.34,-0.36,-0.34,-0.34,-0.34,-0.32,-0.34,-0.29,-0.34,-0.27,-0.35,-0.25,-0.35,-0.23,-0.35,-0.21,-0.35,-0.19,-0.36,-0.17,-0.36,-0.15,-0.37,-0.13,-0.37,-0.11,-0.38,-0.09,-0.39};

//uniform int poslen = 10;
//uniform float pos[10] = {-0.041600, 0.437600, -0.039520, 0.440720, -0.035360, 0.446960, -0.033280, 0.450080, -0.029120, 0.456320};


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
#define RAngle radians(45.0f)
#define RectHW 0.01f
#define RectHH 0.02f
bool inRect(vec2 origin,vec2 npos)
{
	npos = (npos.xy/iResolution.xy-0.5)/0.5;
	vec2 npos_offset = npos - origin;  //偏移
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); //逆矩阵
	npos_offset = npos_offset*m22;
	npos = npos_offset + origin;
	
	
	vec2 minp = vec2(origin.x - RectHW,origin.y - RectHH);
	vec2 maxp = vec2(origin.x + RectHW,origin.y + RectHH);
	bool ret = false;
	if(npos.x > minp.x && npos.y > minp.y && npos.x < maxp.x && npos.y < maxp.y)
	{
		ret = true;
	}
	return ret;
}

float inRectAndAbsDis(vec2 origin,vec2 npos)
{
	npos = (npos.xy/iResolution.xy-0.5)/0.5;
	vec2 npos_offset = npos - origin;  //偏移
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); //逆矩阵
	npos_offset = npos_offset*m22;
	npos = npos_offset + origin;
	
	
	vec2 minp = vec2(origin.x - RectHW,origin.y - RectHH);
	vec2 maxp = vec2(origin.x + RectHW,origin.y + RectHH);
	float ret = 0.0f;
	if(npos.x >= minp.x && npos.y >= minp.y && npos.x <= maxp.x && npos.y <= maxp.y)
	{
		ret = 0.0f;
	}else{
		if(npos.x < minp.x)
		{
			ret = ret + abs(npos.x - minp.x);
		}
		if(npos.x > maxp.x)
		{
			ret = ret + abs(npos.x - maxp.x);
		}
		if(npos.y < minp.y)
		{
			ret = ret + abs(npos.y - minp.y);
		}
		if(npos.y > maxp.y)
		{
			ret = ret + abs(npos.y - maxp.y);
		}
	}
	return ret;
}

#define minw 0.003
#define maxw 0.03
#define subwspeed 0.0005
#define blurw 0.01f
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
		//vec2 b = vec2(pos[idx1 + 2], pos[idx1 + 3]);
		bool ret = inRect(a,npos);
		if(ret)
		{
			v = 1.0;
			//return v;
		}
		
		float dis = inRectAndAbsDis(a,npos);
		float tv = 1.0 - smoothstep(0.0, blurw, dis);
		v = max(tv,v);
		/*
		if(dis == 0.0f)
		{
			v = 1.0f;
		}else if(dis >= blurw)
		{
			v = 0.0f;
		}else{
			
		}
		*/
	}
	return v;
}



//void mainImage( out vec4 o, in vec2 I )
//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{

	vec2 speed0 = vec2(0.0432, 0.0123);
    vec2 speed1 = vec2(0.0257, 0.0332);
	//CC_Texture0 u_texture1
	vec4 originc = v_fragmentColor*texture2D(CC_Texture0, v_texCoord);
	float v = countV();
	vec4 newc = vec4(v,v,v,1.0);
	gl_FragColor = newc;
	//gl_FragColor = max(originc,newc);//vec4(v,v,v,1.0);//vec4(v,v,v,1.0)*vec4(0.5,0.5,0,0.1);//正常情况
	/*
	bool found = countVB();
	if(found)
	{
		gl_FragColor = vec4(1,1,1,1.0);//vec4(v,v,v,1.0)*vec4(0.5,0.5,0,0.1);//正常情况
	}else
	{
		gl_FragColor = vec4(0,0,0,1.0);//vec4(v,v,v,1.0)*vec4(0.5,0.5,0,0.1);//正常情况
	}
	*/
}
