#pragma once
#include"world.h"
class UI{

	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	Texture tex;
	Shader shader;
public:

	UI();

	void toGPU();
	void render();
};

