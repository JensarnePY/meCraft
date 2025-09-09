#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, TexCoord);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}