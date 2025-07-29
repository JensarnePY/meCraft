#pragma once

#include <algorithm>
#include "renderClasses/Mesh.h"
#include "FastNoiseLite.h"

struct chunkdata {

	glm::vec3 pos;

};

class world {
	float chunkSize = 32;

	//std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	std::vector<Texture> textures{
			Texture("res/grass_top.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_bottom.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_side.png", 2, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	std::vector <Vertex> world_vertices;
	std::vector <GLuint> world_indices;

	std::vector<chunkdata> chunk;
	Mesh mesh;
	FastNoiseLite Noise;
	void gen_chunkdata(glm::vec3 pos, std::vector <Vertex>* vertices, std::vector <GLuint>* indices);
public:
	void update(Camera& camera, int renderDistent);
	void render(Shader& shader, Camera& camera);
};