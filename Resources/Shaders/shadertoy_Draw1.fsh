uniform vec2 center;
uniform vec2 resolution;
uniform vec2 mouse;
uniform float pos[];// = {100,60.0, 130, 150, 130};

vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)

vec4   iMouse = vec4(mouse.x,mouse.y,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click


float det(vec2 a, vec2 b) { return a.x*b.y-b.x*a.y; }

vec2 closestPointInSegment( vec2 a, vec2 b )
{
  vec2 ba = b - a;
  return a + ba*clamp( -dot(a,ba)/dot(ba,ba), 0.0, 1.0 );
}

// From: http://research.microsoft.com/en-us/um/people/hoppe/ravg.pdf
vec2 get_distance_vector(vec2 b0, vec2 b1, vec2 b2) {
	
  float a=det(b0,b2), b=2.0*det(b1,b0), d=2.0*det(b2,b1); // 饾浖,饾浗,饾浛(饾憹)
  
  if( abs(2.0*a+b+d) < 1000.0 ) return closestPointInSegment(b0,b2);
	
  float f=b*d-a*a; // 饾憮(饾憹)
  vec2 d21=b2-b1, d10=b1-b0, d20=b2-b0;
  vec2 gf=2.0*(b*d21+d*d10+a*d20);
  gf=vec2(gf.y,-gf.x); // 鈭囸潙?饾憹)
  vec2 pp=-f*gf/dot(gf,gf); // 饾憹鈥?  
  vec2 d0p=b0-pp; // 饾憹鈥?to origin
  float ap=det(d0p,d20), bp=2.0*det(d10,d0p); // 饾浖,饾浗(饾憹鈥?
  // (note that 2*ap+bp+dp=2*a+b+d=4*area(b0,b1,b2))
  float t=clamp((ap+bp)/(2.0*a+b+d), 0.0 ,1.0); // 饾憽虆
  return mix(mix(b0,b1,t),mix(b1,b2,t),t); // 饾懀饾憱= 饾憦(饾憽虆)

}

float approx_distance(vec2 p, vec2 b0, vec2 b1, vec2 b2) {
  return length(get_distance_vector(b0-p, b1-p, b2-p));
}

//void mainImage( out vec4 fragColor, in vec2 fragCoord )
//iMouse.xy;
void main(void)
{
	vec2 xy = gl_FragCoord.xy ;  // - center.xy - iResolution.xy
	
	//vec2 b0 = vec2(0.25, .5) * iResolution.xy;
	//vec2 b1 = iMouse.xy;
	//vec2 b2 = vec2(.75, .5) * iResolution.xy;
	
	vec2 b0 = vec2(pos[1],pos[2]) ;
	vec2 b1 = iMouse.xy;
	vec2 b2 = vec2(pos[3],pos[4]) ;
	
	vec2 mid = .5*(b0+b2) + vec2(0.0,0.01);
	
	float d = approx_distance(xy, b0, b1, b2);
	float thickness = 1.0;
	
	float a;
	
	if(d < thickness) {
	  a = 1.0;
	} else {
	  // Anti-alias the edge.
	  a = 1.0 - smoothstep(d, thickness, thickness+0.6);
	}
	
	//fragColor = vec4(a,1.0,1.0, 1.0);
	
	
	gl_FragColor = vec4(a,a,a, 1.0);  //vec4(a,a,a, 1.0);
}