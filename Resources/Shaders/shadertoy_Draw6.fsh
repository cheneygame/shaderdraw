//纹理测试专用
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_texture1;
uniform float u_interpolate;

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


void main() {
    vec4 color1 = texture2D(CC_Texture0, v_texCoord); //v_texCoord:0-1
    vec4 color2 = texture2D(u_texture1, v_texCoord);
	//vec2(gl_FragCoord.x/iResolution.x,gl_FragCoord.y/iResolution.y)
	//vec2(gl_FragCoord.x/iResolution.x - 0.5,gl_FragCoord.y/iResolution.y - 0.5)
	//vec4 color2 = texture2D(u_texture1, vec2(gl_FragCoord.x/iResolution.x - 0.5,gl_FragCoord.y/iResolution.y - 0.5));
    //gl_FragColor = v_fragmentColor * mix( color1, color2, 0.5);
	//gl_FragColor =  v_fragmentColor*color2*color1;
	if(v_texCoord.x > 1)
	{
		gl_FragColor =  color1;
	}else
	{
		gl_FragColor =  color2;
	}
	//gl_FragColor =  mix( color1, color2, 0.5);
	//gl_FragColor = color2*vec4(0.6,0.6,0.6,0.5);
}

