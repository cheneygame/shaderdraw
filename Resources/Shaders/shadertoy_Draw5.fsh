

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_texture1;
uniform float u_interpolate;

uniform vec2 center;
uniform vec2 resolution;
uniform int poslen;
uniform float pos[2048];  //1024*128 = 131072
uniform vec2 mouse;

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

//void mainImage( out vec4 o, in vec2 I )
//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{
	float width = 0.006*3;
	vec2 I = vec2(gl_FragCoord.x,gl_FragCoord.y);
    vec2 speed0 = vec2(0.0432, 0.0123);
    vec2 speed1 = vec2(0.0257, 0.0332);
    I = 2. * I / iResolution.xy - 1.;
    //vec2 a = mirror(30 * speed0);  //iGlobalTime
    //vec2 b = mirror(30 * speed1);  //iGlobalTime
	int posnum = poslen/2; //有几个pos
	float v = 0.0;
	for(int i = 0;i < posnum - 1;i++)
	{
		//vec2 a = vec2(pos[0], pos[1]);  //iGlobalTime
		//vec2 b = vec2(pos[2], pos[3]);  //iGlobalTime
		int idx1 = i*2;
		int idx2 = i*2+1;
		int idx3 = i*2+2;
		int idx4 = i*2+3;
		vec2 a = vec2(pos[idx1], pos[idx2]);
		vec2 b = vec2(pos[idx3], pos[idx4]);
		float d = PointLineSegDist2d(a, b - a, I);
		float tv = 1.0 - smoothstep(0.0, width, d);
		//float tv = 1.0 - step(width, d);
		if(tv > v)
		{
			v = tv;
		}
		float catchnum = 0.001;  //临界值
		if(tv > catchnum)
		{
			//break;
		}else
		{
			//当前线段之外的像素，要判断是否在其他线段内
		}
	}
	////vec2(iResolution.x*0.5,iResolution.y*0.8)
	//vec2(gl_FragCoord.x/iResolution.x,gl_FragCoord.y/iResolution.y)
	//vec2(gl_FragCoord.x/iResolution.x - 0.5,gl_FragCoord.y/iResolution.y - 0.5)
	//CC_Texture0 u_texture1 vec2(gl_FragCoord.x,gl_FragCoord.y) v_texCoord
	//vec4 color2 = v_fragmentColor*texture2D(u_texture1,vec2(gl_FragCoord.x/iResolution.x*v_texCoord.x,gl_FragCoord.y/iResolution.y*v_texCoord.y) );  
	float uvx = gl_FragCoord.y/iResolution.y;
	float uvy = gl_FragCoord.x/iResolution.x;
	vec4 color2 = texture2D(u_texture1,vec2(uvy,1- uvx) );  
	//genType step (genType edge, genType x)，genType step (float edge, genType x)
	//genType smoothstep (genType edge0,genType edge1,genType x)
	//0 --> 1-width:是线段外围， 1-width --> 1 是线段中心
	//float v = 1.0 - smoothstep(0.0, width, d);
	
	//分段处理
	if(v <= width)
	{
		//线段外内容
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0,0.0,0.5,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}
	else if(v <= (1-width))
	{
		//除了中心外的内容
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0.5,0.0,0.0,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}else if(v > (1-width)) 
	{
		 //最中心那条线
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0,0.5,0.0,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}else
	{
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0,0.0,0.5,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}
	
	//gl_FragColor = vec4(v,v,v,1.0)*vec4(0.5,0.5,0,0.1);//正常情况

	
	//vec2 a = vec2(pos[0], pos[1]);  //iGlobalTime
    //vec2 b = vec2(pos[4], pos[5]);  //iGlobalTime
    //float d = PointLineSegDist2d(a, b - a, I);
	
	//float v = 1.0 - smoothstep(0.0, width, d);
	//gl_FragColor = vec4(v,v,v,1.0); // line    *vec4(0,0.0,0.5,0.1)
	
    //gl_FragColor = vec4(pow(1.0 - d, 17.0)); // glow
}
