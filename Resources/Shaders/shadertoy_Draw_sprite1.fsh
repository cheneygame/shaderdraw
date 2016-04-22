//白色区域转化为透明
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
	vec4 color2 = texture2D(CC_Texture0, v_texCoord);
	if(color2.x + color2.y + color2.z > (1 - 0.3))  //白色赛选为透明度
	{
		color2.w = 0;
	}
	gl_FragColor = color2;


}