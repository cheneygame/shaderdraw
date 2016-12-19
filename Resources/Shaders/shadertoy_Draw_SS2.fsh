
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

#define RAngle radians(0.0f) 
#define RectHW 0.1f
#define RectHH 0.2f

#define minw 0.003
#define maxw 0.03
#define subwspeed 0.0005
#define blurw 0.01f

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

    return length(p - (a + n * PointLineAlong2d(a, n, p)));
}

float PointLineSegDist2d(vec2 a, vec2 n, vec2 p)
{
    float q = PointLineAlong2d(a, n, p);
    return length(p - (a + n * clamp(q, 0., 1.)));
}


float rand(vec2 co){
 return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


bool inRect(vec2 origin,vec2 npos)
{
	npos = (npos.xy/iResolution.xy-0.5)/0.5;
	vec2 npos_offset = npos - origin;  
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); 
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
	vec2 npos_offset = npos - origin; 
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); 
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
	vec2 npos_offset = npos - origin;  
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); 
	npos_offset = npos_offset*m22;
	npos = npos_offset + origin;
	

	float w = u_texture1Size.x/iResolution.x;
	float h = u_texture1Size.y/iResolution.y;
	float hw = u_texture1Size.x/iResolution.x/2; 
	float hh = u_texture1Size.y/iResolution.y/2;  
	vec2 minp = vec2(origin.x - hw,origin.y - hh);
	vec2 maxp = vec2(origin.x + hw,origin.y + hh);
	float ret = 0.0f;
	
	if(npos.x >= minp.x && npos.y >= minp.y && npos.x <= maxp.x && npos.y <= maxp.y)
	{
		float offsetx = (npos.x - minp.x);
		float offsety = (npos.y - minp.y);
		vec2 coord = vec2(offsetx/w,offsety/h);
		vec4 color2 = texture2D(u_texture1,coord );
		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y); 
		if(color2.x + color2.y + color2.z > 2.99)  
		{
			color2 = texture2D(u_texture2,nCoord);
		}
		else if(color2.w == 0.0)  
		{
			color2 = texture2D(u_texture2,nCoord);
		}
		else{
			color2 = vec4(scolor.x,scolor.y,scolor.z,scolor.w); 
		}
		return color2;
	}else{

		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y); 
		vec4 color3 = texture2D(u_texture2,nCoord);
		if(color3.x > 0.0 || color3.y > 0.0 || color3.z > 0.0) 
		{
			return color3;			
		}
		return vec4(0,0,0,0);
	}
	return vec4(0,0,0,0);
}

vec4 getpixelInRect2(vec2 origin,vec2 npos)
{
	vec2 nCoord = vec2(npos.x,npos.y);
	npos = (npos.xy/iResolution.xy-0.5)/0.5;
	vec2 npos_offset = npos - origin;  
	mat2 m22 = mat2(cos(RAngle),sin(RAngle),-sin(RAngle),cos(RAngle)); 
	npos_offset = npos_offset*m22;
	npos = npos_offset + origin;
	
	
	float w = u_texture1Size.x/iResolution.x;
	float h = u_texture1Size.y/iResolution.y;
	float hw = u_texture1Size.x/iResolution.x/2;  
	float hh = u_texture1Size.y/iResolution.y/2;  
	vec2 minp = vec2(origin.x - hw,origin.y - hh);
	vec2 maxp = vec2(origin.x + hw,origin.y + hh);
	float ret = 0.0f;
	
	if(npos.x >= minp.x && npos.y >= minp.y && npos.x <= maxp.x && npos.y <= maxp.y)
	{
		float offsetx = (npos.x - minp.x);
		float offsety = (npos.y - minp.y);
		vec2 coord = vec2(offsetx/w,offsety/h);
		vec4 color2 = texture2D(u_texture1,coord );
		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y);  
		return color2;
	}else{

		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y); 
		vec4 color3 = texture2D(u_texture2,nCoord);
		return color3;
	}
	return vec4(0,0,0,0);
}

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
	float hw = u_texture1Size.x/iResolution.x/2; 
	float hh = u_texture1Size.y/iResolution.y/2; 
	vec2 minp = vec2(origin.x - hw,origin.y - hh);
	vec2 maxp = vec2(origin.x + hw,origin.y + hh);
	float ret = 0.0f;
	
	if(npos.x >= minp.x && npos.y >= minp.y && npos.x <= maxp.x && npos.y <= maxp.y)
	{
		float offsetx = (npos.x - minp.x);
		float offsety = (npos.y - minp.y);
		vec2 coord = vec2(offsetx/w,offsety/h);
		vec4 color2 = texture2D(u_texture1,coord );
		nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y);  
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
		vec2 nCoord = vec2(gl_FragCoord.x/iResolution.x,1- gl_FragCoord.y/iResolution.y); 
		vec4 pixel = texture2D(u_texture2,nCoord); 
		float pixelA = pixel.a;
		bool isGetPencilPixel = false;
		for(int i = 0;i < posnum;i++)  
		{
			
			width = maxw - subwspeed*i;
			width = max(width,minw);
			int idx1 = i*2;
			vec2 a = vec2(pos[idx1], pos[idx1 + 1]);

			vec4 temp = getpixelInRect3(a,npos);
			if(temp.w > 0)
			{
				isGetPencilPixel = true;
				vec4 mixColor = vec4(mix(pixel.rgb,temp.rgb,1-pixel.a),temp.a); 

				if(pixel.a == 0.0)
				{
					mixColor = vec4(mix(pixel.rgb,temp.rgb,1),temp.a);
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

	vec2 speed0 = vec2(0.0432, 0.0123);
    vec2 speed1 = vec2(0.0257, 0.0332);

	vec4 pcolor = getPicPixel();
	gl_FragColor = vec4(pcolor.rgb,v_fragmentColor.a*pcolor.a);
	
}
