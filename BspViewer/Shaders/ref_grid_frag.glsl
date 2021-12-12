#version 330 core

#define PI 3.1415926538

in vec3 Pos;
out vec4 outColor;

float near = 0.1; 
float far  = 100.0; 

uniform float width;
uniform float invUn;
float rDist = 16;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec2 polynom ( vec2 x ) { // f = 4x^2 - 4x + 1
	vec2 y = 4 * x;
	y = y * x - y + 1;
	return y;
}

vec4 gridify ( vec2 inCoord, vec3 defaultColor, float depth ) {
	//depth += 0.0; // adjust line scaling 
	//float sDeriv = 1;
	//return vec4 (0.3);

	float sDeriv = 1 - dot ( fwidth( inCoord / rDist ), vec2(1.0) );
	if ( sDeriv <= 0.0 )
		discard;

	float thresh = -( depth * width * invUn * sDeriv );

	vec2 linInd = fract ( inCoord * invUn );
	vec2 linPol = polynom ( linInd );

	vec2 linDec = step( thresh + 1, linPol);
	
	float lineMax = max(linDec.x, linDec.y);
	if ( lineMax == 0 )
		discard;

	vec2 colInd = clamp (abs(inCoord * invUn), -0.5, 0.5);
	vec2 colPol = polynom ( colInd );

	vec2 colDec = step( thresh + 1, colPol);

	return vec4( mix ( 0.5, 1.0, colDec.x), 0.5, mix ( 0.5, 1.0, colDec.y), lineMax );
}

void main()
{

	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	vec3 defaultColor = vec3( 0.333333 );
	vec4 line = gridify(Pos.xz, defaultColor, depth );
	outColor = line;
	outColor.w *= 1 - depth;
}

 