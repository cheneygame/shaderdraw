
uniform vec2 center;
uniform vec2 resolution;

vec2   icenter = center;
vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
vec4      iMouse = vec4(0,0,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click
//uniform sampler2D iChannel0;          // input channel. XX = 2D/Cube
//#define Use_Linear
//#define Use_Cosine
//#define Use_Smoothstep
//#define Use_Cubic
//#define Use_ThirdOrderSpline
#define Use_Catmull_Rom


//--------------------------------------------------------------------------------
float Hash( float n )
{
    return fract(sin(n)*43758.5453123);
}

//--------------------------------------------------------------------------------
float Cubic(float x, float v0,float v1, float v2,float v3) 
{
	float p = (v3 - v2) - (v0 - v1);
	return p*(x*x*x) + ((v0 - v1) - p)*(x*x) + (v2 - v0)*x + v1;
}

//--------------------------------------------------------------------------------
float Catmull_Rom(float x, float v0,float v1, float v2,float v3) 
{
	float c2 = -.5 * v0	+ 0.5*v2;
	float c3 = v0		+ -2.5*v1 + 2.0*v2 + -.5*v3;
	float c4 = -.5 * v0	+ 1.5*v1 + -1.5*v2 + 0.5*v3;
	return(((c4 * x + c3) * x + c2) * x + v1);
	
//	Or, the same result with...
//	float x2 = x  * x;
//	float x3 = x2 * x;
//	return 0.5 * ( ( 2.0 * v1) + (-v0 + v2) * x +
//                  (2.0 * v0 - 5.0 *v1 + 4.0 * v2 - v3) * x2 +
//                  (-v0 + 3.0*v1 - 3.0 *v2 + v3) * x3);

	
}

//--------------------------------------------------------------------------------
float ThirdOrderSpline(float x, float L1,float L0, float H0,float H1) 
{
	return 		  L0 +.5 *
			x * ( H0-L1 +
			x * ( H0 + L0 * -2.0 +  L1 +
			x * ((H0 - L0)* 9.0	 + (L1 - H1)*3.0 +
			x * ((L0 - H0)* 15.0 + (H1 - L1)*5.0 +
			x * ((H0 - L0)* 6.0	 + (L1 - H1)*2.0 )))));
}

//--------------------------------------------------------------------------------
float Cosine(float x, float v0, float v1) 
{
	x = (1.0-cos(x*3.1415927)) * .5;
	return (v1-v0)*x + v0;
}

//--------------------------------------------------------------------------------
float Linear(float x, float v0, float v1) 
{
	return (v1-v0)*x + v0;
}

//--------------------------------------------------------------------------------
float Smoothstep(float x, float v0, float v1) 
{
	x = x*x*(3.0-2.0*x);
	return (v1-v0)*x + v0;
}

//================================================================================
//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	uv.x *= iResolution.x/iResolution.y;
	
	float pos = (iGlobalTime*.5+uv.x) * 4.0;
	float x  = fract(pos);
	float v0 = Hash(floor(pos));
	float v1 = Hash(floor(pos)+1.0);
	float v2 = Hash(floor(pos)+2.0);
	float v3 = Hash(floor(pos)+3.0);
	float f;
	
#ifdef Use_Linear
	f = Linear(x, v1, v2);
#elif defined Use_Cosine
	f = Cosine(x, v1, v2);
#elif defined Use_Smoothstep
	f = Smoothstep(x, v1, v2);
#elif defined Use_Cubic
	f = Cubic(x, v0, v1, v2, v3);
#elif defined Use_Catmull_Rom
	f = Catmull_Rom(x, v0, v1, v2, v3);
#elif defined Use_ThirdOrderSpline
	f = ThirdOrderSpline(x, v0, v1, v2, v3);
#endif

	// Blobs...
	f = .02 / abs(f-uv.y);
	float d = .03/length((vec2(((uv.x)/9.0*.25), uv.y)-vec2(x+.03, v1)) * vec2(.25,1.0));
	f = max(f, d*d);
	d = .03/length((vec2(((uv.x)/9.0*.25), uv.y)-vec2(x-.97, v2)) * vec2(.25,1.0));
	f = max(f, d*d);

	gl_FragColor = vec4(vec3(1.0,.2, .05) * f, 1.0);
}