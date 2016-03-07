uniform vec2 center;
uniform vec2 resolution;
uniform vec2 mouse;

vec2   iResolution = resolution;           // viewport resolution (in pixels)
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)

vec4   iMouse = vec4(mouse.x,mouse.y,0,0);                // mouse pixel coords. xy: current (if MLB down), zw: click

float sdfCircle( const vec2 c, const float r, const vec2 coord )
{
    vec2 offset = coord - c;
    
    return sqrt((offset.x * offset.x) + (offset.y * offset.y)) - r;
}

float sdfUnion( const float a, const float b )
{
    return min(a, b);
}

float sdfDifference( const float a, const float b)
{
    return max(a, -b);
}

float sdfIntersection( const float a, const float b )
{
    return max(a, b);
}

float signedDistance( const vec2 coord )
{
    float a = sdfCircle(vec2(0.4, 0.5), 0.5, coord);
    float b = sdfCircle(vec2(0.6, 0.5), 0.4, coord);
    float c = sdfIntersection(a, b);
    float d = sdfCircle(vec2(0.9, 0.5), 0.2, coord);
    float e = sdfUnion(c, d);
    float f = sdfCircle(vec2(0.9, 0.5), 0.1, coord);
    float g = sdfDifference(e, f);
    return g;   
}

vec4 render( const float distance, const float pixSize, const float strokeWidth, const float gamma, const vec3 color)
{
    float halfStroke = strokeWidth * 0.5;
    if (distance < -halfStroke + pixSize) {
        float factor = smoothstep(-halfStroke - pixSize, -halfStroke + pixSize, distance);
		return vec4(mix(color, vec3(0.0), factor * gamma), 1.0);
    } else if (distance <= halfStroke - pixSize) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float factor = smoothstep(halfStroke - pixSize, halfStroke + pixSize, distance);
        return vec4(vec3(factor * gamma), 1.0);
    }    
}

//void mainImage( out vec4 fragColor, in vec2 gl_FragCoord )
//iMouse.xy;
void main(void)
{
    float size = min(iResolution.x, iResolution.y);
    float pixSize = 1.0 / size;
	vec2 uv = gl_FragCoord.xy / size;
    
    float d = signedDistance(uv);
    
    gl_FragColor = render(d, pixSize, 0.010, 1.1, vec3(0.3, 0.5, 0.2));
    
}

