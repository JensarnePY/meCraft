#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
//in vec3 color;
in vec2 texCoord;
in float dir;


uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 camPos;


void main()
{
	if(dir == 0.0f){
		FragColor = texture(tex0, texCoord);
	}if(dir == 1.0f){
		FragColor = texture(tex1, texCoord);
	}if(dir == 2.0f){
		FragColor = texture(tex2, texCoord);
	}
}