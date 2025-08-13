#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec2 texCoord;
in float face;
in float blockID;


uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 camPos;


void main()
{

	// ambient lighting
	//float ambient = 0.40f;
	//
	//// diffuse lighting
	//vec3 normal = normalize(Normal);
	//vec3 lightDirection = normalize(vec3(0.5f, 0.75f, 0.25f));
	//float diffuse = max(dot(normal, lightDirection), 0.5f);
	//
	float addon = 1;

	if(blockID == 0){
		if(face == 0) FragColor = texture(tex0, texCoord) * addon;
		else if(face == 1) FragColor = texture(tex1, texCoord) * addon;
		else FragColor = texture(tex2, texCoord) * addon;
	}if(blockID == 1){
		FragColor = texture(tex1, texCoord) * addon;
	}
	//if(dir == 2.0f){
	//	FragColor = texture(tex2, texCoord) * addon;
	//}
	//FragColor = texture(tex0, texCoord) * addon;
}