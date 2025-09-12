#pragma once
#include"world.h"
class UI{
private:
	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	std::vector <Texture> textures;

	int win_width = 0;
	int win_height = 0;

	glm::mat4 projection = glm::mat4(0.0f);
	glm::mat4 model = glm::mat4(1.0f);
public:

	glm::vec2 pos = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	float rotate = 0.0f;

	UI();

	void toGPU(std::vector <Texture>& textures);
	void update(int win_width, int win_height, glm::vec2 pos, glm::vec2 size, float rotate);
	void render(Shader& shader);
};

