#pragma once
#include"world.h"
class UI{

	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	Texture tex();
public:

	UI() = default;


	void toGPU();
	void render(Shader& shader);
};

