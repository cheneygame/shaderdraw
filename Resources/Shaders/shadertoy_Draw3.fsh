
uniform vec2 center;
uniform vec2 resolution;
uniform int poslen;
uniform float pos[1024];

uniform int posidx;  //第几个点
uniform vec2 newPos; //坐标

vec2   curpos = vec2(0,0);

vec2   icenter = center;
vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
vec4      iMouse = vec4(0,0,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click


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
	float width = 0.03;
	vec2 I = vec2(gl_FragCoord.x,gl_FragCoord.y);
    vec2 speed0 = vec2(0.0432, 0.0123);
    vec2 speed1 = vec2(0.0257, 0.0332);
    I = 2. * I / iResolution.xy - 1.;
    //vec2 a = mirror(30 * speed0);  //iGlobalTime
    //vec2 b = mirror(30 * speed1);  //iGlobalTime
	float v = 0.0;
	if(posidx < 2)
	{
		curpos.xy = newPos.xy;
	}else
	{
		vec2 a = vec2(curpos.x, curpos.y);
		vec2 b = vec2(newPos.x,newPos.y);
		float d = PointLineSegDist2d(a, b - a, I);
		v = 1.0 - smoothstep(0.0, width, d);
		curpos.xy = newPos.xy;
	}
	
	//0 --> 1-width:是线段外围， 1-width --> 1 是线段中心
	//float v = 1.0 - smoothstep(0.0, width, d);
	if(v <= width)
	{
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0.5,0.5,0.0,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}
	else if(v <= (1-width))
	{
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0.5,0.0,0.0,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}else if(v > (1-width))
	{
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0,0.5,0,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}else
	{
		gl_FragColor = vec4(v,v,v,1.0)*vec4(0,0.0,0.5,0.1); // line    *vec4(0,0.0,0.5,0.1)
	}
	//gl_FragColor = vec4(v,v,v,1.0)*vec4(0,0.5,0,0.1); // line    *vec4(0,0.0,0.5,0.1)
	
	//vec2 a = vec2(pos[0], pos[1]);  //iGlobalTime
    //vec2 b = vec2(pos[4], pos[5]);  //iGlobalTime
    //float d = PointLineSegDist2d(a, b - a, I);
	
	//float v = 1.0 - smoothstep(0.0, width, d);
	//gl_FragColor = vec4(v,v,v,1.0); // line    *vec4(0,0.0,0.5,0.1)
	
    //gl_FragColor = vec4(pow(1.0 - d, 17.0)); // glow
}
