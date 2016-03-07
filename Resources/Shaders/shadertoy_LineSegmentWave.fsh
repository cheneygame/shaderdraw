
uniform vec2 center;
uniform vec2 resolution;

vec2   icenter = center;
vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
vec4      iMouse = vec4(0,0,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click


//void mainImage( out vec4 o, in vec2 I )
//void mainImage( out vec4 gl_FragColor, in vec2 gl_FragCoord )
void main(void)
{
	vec4 o = gl_FragColor;
	vec2 I = vec2(gl_FragCoord.x,gl_FragCoord.y);
    I = 2. * I / iResolution.xy - 1.;
    gl_FragColor -= gl_FragColor;
    vec4 Q = 2. * abs(2. * fract(iGlobalTime * vec4(.0432, .0123, .0257, .0332)) - 1.) - 1.;
    Q.wz -= Q.xy;
    gl_FragColor += cos(length(I - (Q.xy + Q.wz * clamp(dot(I - Q.xy, Q.wz) / dot(Q.wz, Q.wz), 0., 1.)))*25.);
	//gl_FragColor = vec4(1.0 - smoothstep(0.0, 0.03, d)); // line
    //gl_FragColor = vec4(pow(1.0 - d, 17.0)); // glow
}
