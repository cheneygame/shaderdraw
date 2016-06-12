//传入背景纹理u_texture2，和点击区域生成的u_texture1混合
//一次传入多个点
//可以选择颜色的笔刷
//可以旋转笔刷资源
//rendertexture改进测试

uniform vec2 center;
uniform vec2 resolution;
uniform int poslen;
uniform float pos[2048];  //1024*128 = 131072

//uniform int poslen = 250;
//uniform float pos[250] = {0.53,0.37,0.54,0.34,0.55,0.31,0.56,0.29,0.56,0.26,0.57,0.23,0.58,0.20,0.59,0.17,0.59,0.14,0.59,0.11,0.60,0.08,0.60,0.05,0.60,0.02,0.60,-0.02,0.59,-0.05,0.58,-0.07,0.57,-0.10,0.56,-0.13,0.55,-0.16,0.54,-0.18,0.52,-0.20,0.51,-0.22,0.49,-0.24,0.47,-0.25,0.45,-0.26,0.43,-0.28,0.41,-0.28,0.39,-0.29,0.37,-0.29,0.35,-0.30,0.33,-0.30,0.31,-0.31,0.29,-0.31,0.27,-0.32,0.25,-0.32,0.23,-0.32,0.21,-0.32,0.19,-0.32,0.17,-0.32,0.15,-0.32,0.13,-0.31,0.11,-0.30,0.09,-0.29,0.07,-0.27,0.06,-0.24,0.05,-0.22,0.03,-0.19,0.02,-0.17,0.01,-0.14,0.01,-0.11,-0.00,-0.08,-0.01,-0.05,-0.02,-0.02,-0.03,0.01,-0.04,0.03,-0.04,0.06,-0.05,0.09,-0.05,0.12,-0.06,0.15,-0.06,0.18,-0.06,0.22,-0.06,0.25,-0.06,0.28,-0.06,0.29,-0.08,0.29,-0.10,0.30,-0.12,0.30,-0.15,0.30,-0.17,0.30,-0.19,0.30,-0.21,0.29,-0.23,0.28,-0.25,0.27,-0.27,0.26,-0.28,0.24,-0.30,0.23,-0.32,0.22,-0.34,0.21,-0.36,0.20,-0.38,0.18,-0.40,0.17,-0.41,0.15,-0.43,0.12,-0.44,0.10,-0.45,0.07,-0.46,0.04,-0.47,0.02,-0.48,-0.01,-0.50,-0.03,-0.51,-0.05,-0.53,-0.07,-0.55,-0.08,-0.57,-0.10,-0.58,-0.11,-0.60,-0.13,-0.62,-0.15,-0.62,-0.18,-0.62,-0.22,-0.61,-0.24,-0.60,-0.27,-0.58,-0.28,-0.56,-0.30,-0.54,-0.31,-0.52,-0.32,-0.50,-0.32,-0.48,-0.33,-0.46,-0.34,-0.44,-0.34,-0.42,-0.34,-0.40,-0.34,-0.38,-0.34,-0.36,-0.34,-0.34,-0.34,-0.32,-0.34,-0.29,-0.34,-0.27,-0.35,-0.25,-0.35,-0.23,-0.35,-0.21,-0.35,-0.19,-0.36,-0.17,-0.36,-0.15,-0.37,-0.13,-0.37,-0.11,-0.38,-0.09,-0.39};

//uniform int poslen = 10;
//uniform float pos[10] = {-0.041600, 0.437600, -0.039520, 0.440720, -0.035360, 0.446960, -0.033280, 0.450080, -0.029120, 0.456320};


uniform vec2 mouse;
uniform sampler2D u_texture1; //额外贴图
uniform vec2 u_texture1Size;  //额外贴图大小
uniform sampler2D u_texture2; //额外贴图,整个背景的纹理
uniform vec4 scolor;

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

int LFSR_Rand_Gen(in int n)
{
  // &lt;&lt;, ^ and & require GL_EXT_gpu_shader4.
  n = (n << 13) ^ n; 
  return (n * (n*n*15731+789221) + 1376312589) & 0x7fffffff;
}
 
float LFSR_Rand_Gen_f( in int n )
{
  return float(LFSR_Rand_Gen(n));
}

float rand(vec2 co){
 return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

#define RAngle radians(0.0f) //45.0f ,旋转资源
#define RectHW 0.1f
#define RectHH 0.2f
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

vec4 getpixelInRect(vec2 origin,vec2 npos)
{
	vec2 nCoord = vec2(npos.x,npos.y);
	npos = (npos.xy/iResolution.xy-0.5)/0.5;
	vec2 npos_offset = npos - origin;  //偏移
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); //逆矩阵
	npos_offset = npos_offset*m22;
	npos = npos_offset + origin;
	
	
	//vec2 minp = vec2(origin.x - RectHW,origin.y - RectHH);
	//vec2 maxp = vec2(origin.x + RectHW,origin.y + RectHH);
	float w = u_texture1Size.x/iResolution.x;
	float h = u_texture1Size.y/iResolution.y;
	float hw = u_texture1Size.x/iResolution.x/2;  //一半宽度
	float hh = u_texture1Size.y/iResolution.y/2;  //一半高度
	vec2 minp = vec2(origin.x - hw,origin.y - hh);
	vec2 maxp = vec2(origin.x + hw,origin.y + hh);
	float ret = 0.0f;
	
	if(npos.x >= minp.x && npos.y >= minp.y && npos.x <= maxp.x && npos.y <= maxp.y)
	{
		float offsetx = (npos.x - minp.x);
		float offsety = (npos.y - minp.y);
		vec2 coord = vec2(offsetx/w,offsety/h);
		vec4 color2 = texture2D(u_texture1,coord );
		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y);  //需要翻转
		if(color2.x + color2.y + color2.z > 2.99)  //白色筛选为透明度，取背景色
		{
			color2 = texture2D(u_texture2,nCoord);
		}
		else if(color2.w == 0.0)  //本身就是透明像素
		{
			color2 = texture2D(u_texture2,nCoord);
		}
		else{
			//color2 = vec4(1,0,1,color2.w); //紫色
			//color2 = vec4(scolor.x,scolor.y,scolor.z,scolor.w); //来自c++的笔刷颜色	
			color2 = vec4(scolor.x,scolor.y,scolor.z,scolor.w); //来自c++的笔刷颜色,透明度来自图片资源 color2.w scolor.w
		}
		return color2;
	}else{
		//非笔刷区域内,
		//测试发现u_texture2 取出来的像素值都是透明的，取不到非透明区域
		//npos = (npos.xy/iResolution.xy-0.5)/0.5;
		//nCoord = vec2(npos.x/iResolution.x*640,);
		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y);  //需要翻转
		vec4 color3 = texture2D(u_texture2,nCoord);
		if(color3.x > 0.0 || color3.y > 0.0 || color3.z > 0.0) //有颜色部分,旧的笔刷
		{
			return color3;			
		}
		//return color3;
		return vec4(0,0,0,0);
	}
	return vec4(0,0,0,0);
}

#define minw 0.003
#define maxw 0.03
#define subwspeed 0.0005
#define blurw 0.01f

vec4 getPicPixel()
{
	float width = maxw;
	vec2 I = vec2(gl_FragCoord.x,gl_FragCoord.y);
	vec2 npos = vec2(gl_FragCoord.x,gl_FragCoord.y);
    I = 2. * I / iResolution.xy - 1.;
	int posnum = poslen/2; //有几个pos
	float v = 0.0;
	bool found = false;
	if(posnum > 0)
	{
		vec4 pixel = vec4(0,0,0,0);
		for(int i = 0;i < posnum;i++)  //允许一次传入多点，因此这里用循环
		{
			width = maxw - subwspeed*i;
			width = max(width,minw);
			int idx1 = i*2;
			vec2 a = vec2(pos[idx1], pos[idx1 + 1]);
			vec4 temp = getpixelInRect(a,npos);
			
			pixel.x = max(pixel.x,temp.x);
			pixel.y = max(pixel.y,temp.y);
			pixel.z = max(pixel.z,temp.z);
			pixel.w = max(pixel.w,temp.w);
			
			//pixel = temp;
		}
		vec2 nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y);  //需要翻转
		vec4 color3 = texture2D(u_texture2,nCoord);
		if(pixel.x == 0 && pixel.y == 0 && pixel.z == 0 && pixel.w == 0) //如果不在轨迹内，取背景色
		{
			pixel = color3;
		}
		if(pixel.w > 0)
		{
			pixel.w = 0.01;
		}
		
		return pixel;
	}
	return vec4(0,0,0,0);
}


//void mainImage( out vec4 o, in vec2 I )
//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{

	vec2 speed0 = vec2(0.0432, 0.0123);
    vec2 speed1 = vec2(0.0257, 0.0332);
	//CC_Texture0 u_texture1
	//vec4 originc = v_fragmentColor*texture2D(CC_Texture0, v_texCoord);
	//vec4 text2color = v_fragmentColor*texture2D(u_texture2, v_texCoord);
	gl_FragColor = getPicPixel();//mix(text2color,getPicPixel(),0.5);
	
}
