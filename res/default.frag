#version 460 core

// Outputs colors RGBA
out vec4 FragColor;

in vec3 pos;
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
uniform vec3 camDir;
uniform float time;

//	grass_top = 0
//	grass_block = 1
//	dirt_block = 2
//	defalt_stone = 3
//	wood_log = 4
//	leaves = 5
//	water = 6
//	sand = 7
//

float CalcExpFogFactor(vec3 worldpos)
{
	float fogend = 40 * 32;

    float CameraToPixelDist = length(worldpos - camPos);
    float DistRatio = 4.0 * CameraToPixelDist / fogend;
    float FogFactor = 0.0;
	float fogdensity = 0.2f;

    FogFactor = max(exp(-DistRatio * fogdensity * DistRatio * fogdensity), 0.5);
    return FogFactor;
}

void main()
{

	//ambient lighting
	float ambient = 0.4f;

	
	// diffuse lighting
	vec3 Normal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.5f, 1.0f, 1.25f));
	float diffuse = max(dot(Normal, lightDirection), 0.0f);
	
	float addon = ambient + diffuse;
	vec4 tempcolor;

	if(blockID == 1){

		if(face == 0) tempcolor = texture(tex0, texCoord) * addon;
		else if(face == 1) tempcolor = texture(tex1, texCoord) * addon;
		else tempcolor = texture(tex2, texCoord) * addon;

	}if(blockID == 2){

		tempcolor = texture(tex1, texCoord) * addon;
	}if(blockID == 3){

		tempcolor = texture(tex3, texCoord) * addon;
	}if(blockID == 4){

		tempcolor = texture(tex4, texCoord) * addon;
	}if(blockID == 5){

		tempcolor = texture(tex5, texCoord) * addon;
	}if(blockID == 6){

		tempcolor = texture(tex6, texCoord) * addon;
	}if(blockID == 7){

		tempcolor = texture(tex7, texCoord) * 0.6f * addon;
	}

	float fogfactor = CalcExpFogFactor(pos);

	tempcolor = mix(vec4(vec3(0.5f), 1.0f), tempcolor, fogfactor);

	FragColor = tempcolor;
}