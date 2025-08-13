#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in float aface;
layout (location = 3) in float ablockID;


out vec3 crntPos;
out vec2 texCoord;
out float face;
out float blockID;

uniform mat4 camMatrix;
uniform mat4 model;


void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));
	texCoord = aTex;
	face = aface;
	blockID = ablockID;
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}