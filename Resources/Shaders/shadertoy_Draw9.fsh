//马赛克
#ifdef GL_ES
precision mediump float;
#endif


varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec4 v_pos;

uniform sampler2D u_texture1;
uniform float u_interpolate;
uniform vec2 u_texture1Size;

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
	/*
	const float minTileSize = 1.0;
	const float maxTileSize = 32.0;
	const float textureSamplesCount = 3.0;
	const float textureEdgeOffset = 0.005;
	const float borderSize = 1.0;
	const float speed = 1.0;
	
	float time = pow(sin(iGlobalTime * speed), 2.0);
	//float tileSize = minTileSize + floor(time * (maxTileSize - minTileSize));
	//tileSize += mod(tileSize, 2.0);
	//vec2 tileNumber = vec2(floor(gl_FragCoord.x / tileSize),floor(gl_FragCoord.y / tileSize));
	float tileSize = 20;
	vec2 tileNumber = vec2(10,10);
	vec4 accumulator = vec4(0.0);
	for (float y = 0.0; y < textureSamplesCount; ++y)
	{
		for (float x = 0.0; x < textureSamplesCount; ++x)
		{
			vec2 textureCoordinates = (tileNumber + vec2((x + 0.5)/textureSamplesCount, (y + 0.5)/textureSamplesCount)) * tileSize / iResolution.xy;
			textureCoordinates.y = 1.0 - textureCoordinates.y;
			textureCoordinates = clamp(textureCoordinates, 0.0 + textureEdgeOffset, 1.0 - textureEdgeOffset);
			accumulator += texture2D(CC_Texture0, textureCoordinates);
	   }
	}
	
	gl_FragColor = accumulator / vec4(textureSamplesCount * textureSamplesCount);
	*/
	float time = pow(sin(iGlobalTime * 1), 2.0);
	int wcount = 60;
	int hcount = wcount;
	vec2 tileSize = vec2(int(u_texture1Size.x/wcount),int(u_texture1Size.y/hcount));
	int pixelCount = int(tileSize.x*tileSize.y);
	vec4 totalPixel = vec4(0,0,0,0);
	float scalex = gl_FragCoord.x/u_texture1Size.x;  //倍率
	float scaley = gl_FragCoord.y/u_texture1Size.y;  //倍率
	vec2 idxv = vec2(int(mod(gl_FragCoord.x,u_texture1Size.x)/ tileSize.x),int(mod(gl_FragCoord.y,u_texture1Size.y)/ tileSize.y));  //当前索引  0-wcount,hcount ,tileSize,
	for (float y = 1; y < tileSize.y; ++y)
	{
		for (float x = 1; x < tileSize.x; ++x)
		{
			float uvx = mod(y*idxv.y,u_texture1Size.y)/u_texture1Size.y;
			float uvy = mod(x*idxv.x,u_texture1Size.x)/u_texture1Size.x;
			vec4 color2 = texture2D(u_texture1,vec2(uvy,1- uvx) );  
			//vec4 color2 = texture2D(u_texture1,vec2(0.01*x,0.01*y) );  
			totalPixel.x = totalPixel.x + color2.x;
			totalPixel.y = totalPixel.y + color2.y;
			totalPixel.z = totalPixel.z + color2.z;
			totalPixel.w = totalPixel.w + color2.w;
	   }
	}	
	int tilePixelCount = pixelCount;
	vec4 averagePixel = vec4(totalPixel.x/tilePixelCount,totalPixel.y/tilePixelCount,totalPixel.z/tilePixelCount,totalPixel.w/tilePixelCount);
	vec4 color1 = texture2D(CC_Texture0, v_texCoord);
	
	if(totalPixel.x < 100 && totalPixel.y < 100 && totalPixel.z < 100)
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