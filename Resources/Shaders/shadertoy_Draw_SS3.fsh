
uniform vec2 center;
uniform vec2 resolution;
uniform int poslen;
uniform float pos[256];  


uniform vec2 mouse;
uniform sampler2D u_texture1;
uniform vec2 u_texture1Size;  
uniform sampler2D u_texture2; 
uniform vec4 scolor;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

vec2   icenter = center;
vec2   iResolution = resolution;           
float  iGlobalTime = CC_Time[1];       
vec4   iMouse = vec4(mouse.x,mouse.y,0,0);  

#define RAngle radians(0.0) 
#define RectHW 0.1
#define RectHH 0.2

#define minw 0.003
#define maxw 0.03
#define subwspeed 0.0005
#define blurw 0.01



vec4 getpixelInRect3(vec2 origin,vec2 npos)
{
	vec2 nCoord = vec2(npos.x,npos.y);
	npos = (npos.xy/iResolution.xy-0.5)/0.5;
	vec2 npos_offset = npos - origin;  
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); 
	npos_offset = npos_offset*m22;
	npos = npos_offset + origin;
	
	float w = u_texture1Size.x/iResolution.x;
	float h = u_texture1Size.y/iResolution.y;
	float hw = u_texture1Size.x/iResolution.x/2.0; 
	float hh = u_texture1Size.y/iResolution.y/2.0; 
	vec2 minp = vec2(origin.x - hw,origin.y - hh);
	vec2 maxp = vec2(origin.x + hw,origin.y + hh);
	
	if(npos.x >= minp.x && npos.y >= minp.y && npos.x <= maxp.x && npos.y <= maxp.y)
	{
		float offsetx = (npos.x - minp.x);
		float offsety = (npos.y - minp.y);
		vec2 coord = vec2(offsetx/w,offsety/h);
		vec4 color2 = texture2D(u_texture1,coord );
		nCoord = vec2(gl_FragCoord.x/iResolution.x,1.0 - gl_FragCoord.y/iResolution.y);  
		color2 = vec4(scolor.x,scolor.y,scolor.z,scolor.w);
		return color2;
	}
	return vec4(0,0,0,0);
}



vec4 getPicPixel()
{
	float width = maxw;
	vec2 I = vec2(gl_FragCoord.x,gl_FragCoord.y);
	vec2 npos = vec2(gl_FragCoord.x,gl_FragCoord.y);
    I = 2. * I / iResolution.xy - 1.;
	int posnum = poslen/2; 
	float v = 0.0;
	bool found = false;
	if(posnum > 0)
	{
		vec2 nCoord = vec2(gl_FragCoord.x/iResolution.x,1.0 - gl_FragCoord.y/iResolution.y); 
		vec4 pixel = texture2D(u_texture2,nCoord); 
		for(int i = 0;i < posnum;i++)  
		{
			
			int idx1 = i*2;
			vec2 a = vec2(pos[idx1], pos[idx1 + 1]);

			vec4 temp = getpixelInRect3(a,npos);
			if(temp.w > 0.0)
			{
				vec4 mixColor = vec4(mix(pixel.rgb,temp.rgb,1.0-pixel.a),temp.a); 

				if(pixel.a == 0.0)
				{
					mixColor = vec4(mix(pixel.rgb,temp.rgb,1.0),temp.a);
				}else
				{
					mixColor = vec4(mix(pixel.rgb,temp.rgb,0.5),temp.a);
				}
				pixel = vec4(temp.rgb,0.5); //mixColor
				
				break;
			}
		}
		return pixel;
	}
	return vec4(0,0,0,0);
}



void main(void)
{

	vec4 pcolor = getPicPixel();
	gl_FragColor = vec4(pcolor.rgb,v_fragmentColor.a*pcolor.a);
	
}
