#version 330 core

layout (std140) uniform spaceTransforms {
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
};

layout (location = 0) in vec3 position;

out vec3 Pos;

void main()
{
	Pos = position;
	gl_Position = projection * view * model * vec4(position, 1.0);
}