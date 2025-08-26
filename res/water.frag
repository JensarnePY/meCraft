#version 460 core

out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;
in float face;


uniform sampler2D tex0;

uniform vec3 camPos;
uniform float time;


void main()
{

	//ambient lighting
	const float ambient = 0.4f;

	
	// diffuse lighting
	vec3 Normal = normalize(normal);
	const vec3 lightDirection = normalize(vec3(1.5f, 1.0f, 1.25f));
	const float diffuse = max(dot(Normal, lightDirection), 0.0f);
	

	FragColor = texture(tex0, texCoord) * ambient + diffuse;
	
}