//addition to shadertoy
uniform vec2 center;
uniform vec2 resolution;
uniform vec2 mouse;

vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)
vec4   iMouse = vec4(mouse.x,mouse.y,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click

//气球
#define amplitude 0.2  //风力，扁的程度
#define BackgroundColor vec4(0.957, 0.925, 0.773, 1.0)
#define EdgeColor vec4(0.2, 0.2, 0.2, 1.0)
#define BlueColor vec4(0.384, 0.667, 0.655, 1.0)
#define PurpleColor vec4(0.761, 0.706, 0.835, 1.0)
#define YellowColor vec4(0.961, 0.753, 0.196, 1.0)
#define GreenColor vec4(0.624, 0.796, 0.361, 1.0)
#define OrangeColor vec4(0.953, 0.482, 0.318, 1.0)
#define RedColor vec4(0.886, 0.557, 0.616, 1.0)

float noise2d(vec2 p) {
	float t = texture2D(CC_Texture0, p).x;   //CC_Texture0  iChannel0
	t += 0.5 * texture2D(CC_Texture0, p * 2.0).x;
	t += 0.25 * texture2D(CC_Texture0, p * 4.0).x;
	return t / 1.75;
}

float line(vec2 p, vec2 p0, vec2 p1, float width) {
    vec2 dir0 = p1 - p0;
    vec2 dir1 = p - p0;
    float h = clamp(dot(dir1, dir0)/dot(dir0, dir0), 0.0, 1.0);
    float d = (length(dir1 - dir0 * h) - width * 0.5);
    return d;
}

vec4 drawline(vec2 p, vec2 p0, vec2 p1, float width) {   		
    float d = line(p, p0, p1, width);
    d += noise2d(p * vec2(0.2)) * 0.005;
    float w = fwidth(d) * 1.0;
    
    return vec4(EdgeColor.rgb, 1.-smoothstep(-w, w, d));
}

float metaball(vec2 p, float r) {
	return r / dot(p, p);
}

//color球体颜色
vec4 balloon(vec2 pos, vec2 start, vec2 end, float radius, vec4 color) {
    // Draw line
    vec2 linePos = pos;
    linePos.x *= (1.0 + sin(noise2d(pos * 0.005) * pos.y * 8.) * 0.05);
    vec4 line = drawline(linePos, 
                         start * (1.0 + vec2(cos(iGlobalTime * 1.4), sin(iGlobalTime * 2.4)) * 0.4 * amplitude), 
                         end, 0.015);
    
    vec2 c0 = start * (1.0 + vec2(cos(iGlobalTime * 1.4), sin(iGlobalTime * 2.4)) * 0.57 * amplitude);
    vec2 c1 = start * (1.0 + vec2(cos(iGlobalTime * 1.4), sin(iGlobalTime * 1.9)) * 0.49 * amplitude);
    vec2 c2 = start * (1.0 + vec2(sin(iGlobalTime * 1.9), cos(iGlobalTime * 2.4)) * 0.46 * amplitude);
			
	float r = metaball(pos - c0, radius*1.1) *
		 		metaball(pos - c1, radius*1.3) *
				metaball(pos - c2, radius*0.9);
    
    vec2 boundary = vec2(0.4, 0.5);
	vec4 c = vec4(0);
			
	vec4 egdeColor = EdgeColor;
	vec4 blobColor = color;
    
	r += noise2d(pos * vec2(0.05)) * 0.15;
    
    if (r < boundary.x) {
		c = egdeColor;
		c.a = 0.0;
	} else if (r < boundary.y) {
		c = egdeColor;
		c.a = 1.0;
    } else {
        c = blobColor;
        //c = mix(blobColor,texture2D(iChannel1,.5+(pos-c2)*3.),.5+.5*cos(10.*start.x+iGlobalTime));
        c.a = 1.0;
    }
    
    // Blur the edges
    float w = 0.05;
	if (r > boundary.x - w && r < boundary.x) {
		c = mix(line, egdeColor, smoothstep(-w, 0.0, r - boundary.x));
        c.a = mix(0.0, 1.0, smoothstep(-w, 0.0, r - boundary.x));
	}
	if (r > boundary.y - w && r < boundary.y + w) {
		c.rgb = mix(egdeColor.rgb, blobColor.rgb, smoothstep(-w, w, r - boundary.y));
	}
    
    c.rgb += noise2d(pos * 0.1) * 0.1;
    
    c.rgb = mix(line.rgb, c.rgb, c.a);
    c.a = max(line.a, c.a);
    
    return c;
}

//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{
	vec2 uv = gl_FragCoord.xy / iResolution.yy;
    vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/min(iResolution.y,iResolution.x); 
    
    if (iMouse.z > 0.0) {
        uv.y -= (iMouse.y/iResolution.y - 0.5)*0.3;
    }
	
    float width = iResolution.x/iResolution.y; 

    gl_FragColor = BackgroundColor;
    
    float end = -0.1;
    
    // First level
    vec4 c = balloon(uv, vec2(width * 0.5, 0.9), vec2(width*0.5, end), 0.013, BlueColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    // Second level
    c = balloon(uv, vec2(width*0.4, 0.75), vec2(width*0.5, end), 0.015, PurpleColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    c = balloon(uv, vec2(width*0.6, 0.7), vec2(width*0.5, end), 0.015, YellowColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    // Third level
    c = balloon(uv, vec2(width*0.25, 0.67), vec2(width*0.5, end), 0.013, GreenColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    c = balloon(uv, vec2(width*0.7, 0.63), vec2(width*0.5, end), 0.013, OrangeColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    // Fourth level
    c = balloon(uv, vec2(width*0.38, 0.35), vec2(width*0.5, end), 0.011, RedColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    c = balloon(uv, vec2(width * 0.2, 0.45), vec2(width*0.5, end), 0.014, BlueColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    c = balloon(uv, vec2(width*0.73, 0.32), vec2(width*0.5, end), 0.012, YellowColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    c = balloon(uv, vec2(width*0.55, 0.4), vec2(width*0.5, end), 0.013, GreenColor);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, c.rgb, c.a);
    
    gl_FragColor.rgb = gl_FragColor.rgb*(1.0-0.15*length(p));
    gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(1.0/1.8));
}


