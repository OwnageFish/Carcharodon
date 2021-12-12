#version 330 core

in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D screenTexture;

void main()
{
	//FragColor.rg = vTexCoord;
	//FragColor.ba = vec2(1.0);
	FragColor.rgb = texture(screenTexture, vTexCoord).rgb;
	FragColor.a = 1.0;

} 