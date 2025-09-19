#pragma once
#include"world.h"
#include"camera.h"

class renUI;

class UI {
private:
	std::vector<renUI> m_render_l;
public:
	UI(GLFWwindow* window);

	void update();
	void render(Shader& shader);
};

class renUI{
private:
	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	Texture textures;

	int win_width = 0;
	int win_height = 0;

	glm::mat4 projection = glm::mat4(0.0f);
	glm::mat4 model = glm::mat4(1.0f);
public:

	glm::vec2 pos = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	float rotate = 0.0f;

	renUI();

	void toGPU(Texture& textures);
	void update(int win_width, int win_height, glm::vec2 pos, glm::vec2 size, float rotate);
	void render(Shader& shader);
};

