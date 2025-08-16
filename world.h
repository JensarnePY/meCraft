#pragma once

#include <future>
#include <thread>
#include "renderClasses/Mesh.h"
#include "FastNoiseLite.h"



struct chunkdata {
	glm::vec3 pos;
	Mesh mesh;
	bool gen = false;
	bool render = false;
	bool loaded_to_gpu = false;
	std::vector <Vertex> vertices;
	std::vector <bool> noiselist;
	std::vector <float> blockIdList;
	const float chunkSize = 32;

	chunkdata(glm::vec3 position) : pos(position) {}
	int getBlockIDPos(float x, float y, float z) {return x + z * chunkSize + y * chunkSize * chunkSize;}
	int getBlockIDPos(glm::vec3 pos) {return pos.x + pos.z * chunkSize + pos.y * chunkSize * chunkSize;}
	int getpos(int x, int y, int z) {
		const int size = chunkSize + 2;
		x += 1;
		y += 1;
		z += 1;
		return x + z * size + y * size * size;
	}
	void reload();
};

class world {
public:
	const float chunkSize = 32;
	int threads_gen = 0;
	int MAX_threads_gen = 5;

	std::vector<Texture> textures{
			Texture("res/grass_top.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_bottom.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/grass_side.png", 2, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("res/stone.png", 3, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	FastNoiseLite Noise;
	bool thr_ready = true;
	std::vector<chunkdata> chunk;
	world();
	int getpos(int x, int y, int z);
	chunkdata* getchunk(int x, int y, int z);
	void pre_load_chunk(glm::vec3 pos, int renderDistent);
	void gen_chunkdata(chunkdata* chunk, const FastNoiseLite* Noise);
	void update(Camera& camera, int renderDistent);
	void render(Shader& shader, Camera& camera);
};