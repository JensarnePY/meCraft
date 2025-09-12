#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
	gl_Position = projection * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
	TexCoord = aTexCoords;
}