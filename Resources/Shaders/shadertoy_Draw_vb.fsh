//glVertexBuffer
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
	//CC_Texture0 u_texture1
	vec4 color2 = v_fragmentColor*texture2D(u_texture1, v_texCoord);  //vec2(iResolution.x*0.5,iResolution.y*0.8)
	//color2.w = 1.0;
	//genType step (genType edge, genType x)，genType step (float edge, genType x)
	//genType smoothstep (genType edge0,genType edge1,genType x)
	//0 --> 1-width:是线段外围， 1-width --> 1 是线段中心
	//float v = 1.0 - smoothstep(0.0, width, d);
	/*
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
	*/
	gl_FragColor = vec4(v,v,v,1.0)*vec4(0.5,0.5,0,0.1);//正常情况
	//gl_FragColor = vec4(v,v,v,1.0)*color2;//正常情况   color2  vec4(0,0,0,0.1)
	//gl_FragColor.xyz = color2.rgb;
	//gl_FragColor.w = 1.0;
	
	//vec2 a = vec2(pos[0], pos[1]);  //iGlobalTime
    //vec2 b = vec2(pos[4], pos[5]);  //iGlobalTime
    //float d = PointLineSegDist2d(a, b - a, I);
	
	//float v = 1.0 - smoothstep(0.0, width, d);
	//gl_FragColor = vec4(v,v,v,1.0); // line    *vec4(0,0.0,0.5,0.1)
	
    //gl_FragColor = vec4(pow(1.0 - d, 17.0)); // glow
}
