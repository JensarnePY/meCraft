#version 460 core

// Outputs colors RGBA
out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;
in float face;
in float blockID;


uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform vec3 camPos;
uniform float time;


void main()
{

	//ambient lighting
	float ambient = 0.4f;

	
	// diffuse lighting
	vec3 Normal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.5f, 1.0f, 1.25f));
	float diffuse = max(dot(Normal, lightDirection), 0.0f);
	
	float addon = ambient + diffuse;

	if(blockID == 1){

		if(face == 0) FragColor = texture(tex0, texCoord) * addon;
		else if(face == 1) FragColor = texture(tex1, texCoord) * addon;
		else FragColor = texture(tex2, texCoord) * addon;

	}if(blockID == 2){

		FragColor = texture(tex1, texCoord) * addon;

	}if(blockID == 3){

		FragColor = texture(tex3, texCoord) * addon;
	}if(blockID == 4){

		FragColor = texture(tex4, texCoord) * addon;
	}if(blockID == 5){

		FragColor = texture(tex5, texCoord) * addon;
	}if(blockID == 6){

		FragColor = texture(tex6, texCoord) * addon;
	}if(blockID == 7){

		FragColor = texture(tex7, texCoord) * addon;
	}
}