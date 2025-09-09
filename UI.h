#pragma once
#include"world.h"
class UI{

	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	std::vector <Texture> textures;
public:

	UI();

	void toGPU(std::vector <Texture>& textures);
	void render(Shader& shader, Camera& camera);
};

