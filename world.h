#pragma once

#include <algorithm>
#include <future>
#include <thread>
#include "renderClasses/Mesh.h"
#include "FastNoiseLite.h"


struct chunkdata {
	glm::vec3 pos;
	Mesh mesh;
	bool gen = false;
	bool render = false;
	std::vector <GLuint> indices;
	std::vector <Vertex> vertices;
	std::vector <bool> noiselist;

	chunkdata(glm::vec3 position) : pos(position) {}
};

class world {
public:
	const float chunkSize = 32;
	int threads_gen = 0;
	int MAX_threads_gen = 0;

	std::vector<Texture> textures{
			Texture("res/grass_top.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_bottom.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_side.png", 2, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	FastNoiseLite Noise;
	bool thr_ready = true;
	std::vector<chunkdata> chunk;
	world();
	int getpos(int x, int y, int z);
	chunkdata* getchunk(int x, int y, int z);
	void pre_load_chunk(glm::vec3 pos, int renderDistent);
	void gen_chunkdata(glm::vec3 pos, std::vector <Vertex>* vertices, std::vector <GLuint>* indices, std::vector <bool>* pitnoiselist, float chunkSize, const FastNoiseLite* Noise, bool* ready, bool noiseready);
	void update(Camera& camera, int renderDistent);
	void render(Shader& shader, Camera& camera);
};