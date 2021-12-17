#version 330 core

in vec3 pos;

out vec4 color;


float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main(){
    
    float sDeriv = 1 - dot ( fwidth( pos.xz / 16 ), vec2(1.0) );
	color = vec4(0.0, sDeriv ,LinearizeDepth(gl_FragCoord.z) / far, 0.8);
}