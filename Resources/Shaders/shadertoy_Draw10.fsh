//马赛克02  ,
//实现了2个强度参数，部分图片报错
#ifdef GL_ES
precision mediump float;
#endif


varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec4 v_pos;

uniform sampler2D u_texture1;
uniform float u_interpolate;
uniform vec2 u_texture1Size;
uniform int intensity;
uniform int intensity2;
//common
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


// "USE_TILE_BORDER" creates a border around each tile.
// "USE_ROUNDED_CORNERS" gives each tile a rounded effect.
// If neither are defined, it is a basic pixelization filter.

#define USE_TILE_BORDER

void main() 
{
	float time = pow(sin(iGlobalTime * 1), 2.0);
	int wcount = 1;
	int hcount = wcount;
	vec2 tileSize = vec2(int(u_texture1Size.x/wcount),int(u_texture1Size.y/hcount));
	int pixelCount = int(tileSize.x*tileSize.y);
	vec4 totalPixel = vec4(0,0,0,0);
	float scalex = gl_FragCoord.x/u_texture1Size.x;  //倍率
	float scaley = gl_FragCoord.y/u_texture1Size.y;  //倍率
	int precision1 = intensity; //分段(强度)
	int precision2 = intensity2;  //每段细分
	float step1x = 1.0/precision1;  //size
	float step1y = 1.0/precision1;
	float step2x = step1x/precision2; //循环里面的每个步骤+
	float step2y = step1y/precision2;
	int idxx = int(v_texCoord.x/step1x);
	int idxy = int(v_texCoord.y/step1y);
	int pcount = 0;
	for (float y = idxy*step1y; y < (idxy + 1)*step1y; y+=step2y)
	{
		for (float x = idxx*step1x; x < (idxx + 1)*step1x; x+=step2x)
		{
			
			if(y > 1)
			{
				y = 0.9;
			}
			if(x > 1)
			{
				x = 0.9;
			}
			vec4 color2 = texture2D(u_texture1,vec2(x,y) );
			totalPixel.x = totalPixel.x + color2.x;
			totalPixel.y = totalPixel.y + color2.y;
			totalPixel.z = totalPixel.z + color2.z;
			totalPixel.w = totalPixel.w + color2.w;
			pcount++;
	   }
	}	
	int tilePixelCount = pcount;
	vec4 averagePixel = vec4(totalPixel.x/tilePixelCount,totalPixel.y/tilePixelCount,totalPixel.z/tilePixelCount,totalPixel.w/tilePixelCount);
	vec4 color1 = texture2D(CC_Texture0, v_texCoord);
	
	if(step1x>0)
	{
		gl_FragColor = averagePixel;
	}else
	{
		gl_FragColor = averagePixel;
	}
	//float uvx = mod(gl_FragCoord.y - v_pos.y,u_texture1Size.y)/u_texture1Size.y;
	//float uvy = mod(gl_FragCoord.x - v_pos.x,u_texture1Size.x)/u_texture1Size.x;
	//float uvx =  gl_FragCoord.y/u_texture1Size.y;
	//float uvy =  gl_FragCoord.x/u_texture1Size.x;
	//vec4 color3 = texture2D(u_texture1,vec2(uvy,1- uvx) ); 
	//gl_FragColor = color3;
	//gl_FragColor = averagePixel;
}